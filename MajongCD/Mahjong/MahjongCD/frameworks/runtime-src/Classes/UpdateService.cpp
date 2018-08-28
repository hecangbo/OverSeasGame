#include "UpdateService.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "base/CCScheduler.h"

#ifdef WIN32
#else
#include <unistd.h>
#endif
#include "scripting/lua-bindings/manual/CCLuaEngine.h"

#include "CFileUtil.h"
#include "UpdateHelper.h"
#include "sha1.h"
#include "Utility.h"

UpdateService::UpdateService(void)
:m_Stop(false)
, m_Handler(0)
, m_BigVersion(-1)
, m_Version(-1)
, m_DownloadFileIndex(0)
, m_DownloadCheckListIndex(0)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	timeval _time;
	gettimeofday(&_time, NULL);
	char mainName[128];
	char subName[128];
	sprintf(mainName, "hcbgame-main:%u-%ld", _time.tv_usec, _time.tv_sec);
	sprintf(subName, "hcbame-sub:%u-%ld", _time.tv_usec, _time.tv_sec);
	m_pMain = sem_open(mainName, O_CREAT, 0644, 0);
	m_pSub = sem_open(subName, O_CREAT, 0644, 0);
#else
	m_pMain = new sem_t();
	m_pSub = new sem_t();
	sem_init(m_pMain, 0, 0);
	sem_init(m_pSub, 0, 0);
#endif

	downloader.reset(new network::Downloader());
}


UpdateService::~UpdateService(void)
{
}


void UpdateService::Start(string info, int aHandler)
{
	rapidjson::Document dj;
	dj.Parse<0>(info.c_str());
	m_UpdateURLInfo.VersionUrl = dj["Version"].GetString();
	m_UpdateURLInfo.UpdateUrl = dj["Game"].GetString();
	SetScriptHandler(aHandler);
	m_Path = CCFileUtils::sharedFileUtils()->getWritablePath();
	////CCLOG("数据目录:%s", m_Path.c_str());
	CFileUtil::MakeDir(m_Path.c_str());
	Director::getInstance()->getScheduler()->scheduleUpdateForTarget(this, 0, false);
	
	//std::thread t1(&UpdateService::Run, this);
	//t1.join();

	Run();
}


void UpdateService::Stop()
{
	m_Stop = true;
	SendCommand(Exit, 0);
	Thread::Stop();
}
void UpdateService::update(float dt)
{	
	//////CCLOG("update");
	updateDispatch();
}

void UpdateService::SendCommand(UpdateCommand aCommand, void* apData)
{
	m_Operation.Command = aCommand;
	m_Operation.pData = apData;
	sem_post(m_pMain);
}
void UpdateService::SendCommand(UpdateCommand aCommand, std::string& aAddress, std::string& aSavePath)
{
	m_Operation.Command = aCommand;
	m_Operation.m_Address = aAddress;
	m_Operation.m_SavePath = aSavePath;
	sem_post(m_pMain);
}

void UpdateService::PushMsg(UpdateMsg& aMsg)
{
	MutexLock::AutoLock lLock(m_Lock);
	m_MessagesList.push_back(aMsg);
	Director::getInstance()->getScheduler()->resumeTarget(this);
}
void UpdateService::checkVersion()
{
	SendCommand(CheckVersion, 0);
}

//整包更新
void UpdateService::checkDownloadFull(string sAddress,string path)
{
	m_UpdateURLInfo.VersionUrl = sAddress;
	m_UpdateURLInfo.pathSave = path;
	SendCommand(DownloadFullVersion, 0);
}

void UpdateService::RunTask()
{
	//CCLOG("........................RunTask");
	while (!m_Stop)
	{
		//CCLOG("RunTask2222\n");
		sem_wait(m_pMain);
		switch (m_Operation.Command)
		{
		case CheckVersion:
			CheckUpdateVersion();
			break;
		case DownloadCheckList:
			DownLoadCheckList();		//小版本
			break;
		case DownloadFile:		
			break;
		case DownloadFullVersion:
			DownloadUpdateFullVersion();
			break;
		case Exit:
		default:
			break;
		}
	}
	Director::getInstance()->getScheduler()->unscheduleUpdateForTarget(this);
}

//查查版本
void UpdateService::CheckUpdateVersion()
{
	size_t lPos = m_UpdateURLInfo.VersionUrl.find_last_of(":");
	string lPort = m_UpdateURLInfo.VersionUrl.substr(lPos + 1);
	string lAddress = m_UpdateURLInfo.VersionUrl.substr(0, lPos);
	//CCLOG(lAddress.c_str());
	sendRequest(lAddress, 0);
}

void UpdateService::sendRequest(string url, int type)
{
	HttpRequest *request = new (std::nothrow)HttpRequest();
	request->setUrl(url);
	request->setRequestType(HttpRequest::Type::GET);  //(HttpRequest::Type)type
	request->setResponseCallback(CC_CALLBACK_2(UpdateService::onResponse, this));
	//request->setResponseCallback(callback);
	request->setTag("CDMahjong");
	HttpClient::getInstance()->send(request);
	request->release();
}

void UpdateService::onResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	if (!response)
	{
		return;
	}
	
	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}

	if (!response->isSucceed())
	{
		log("response failed, error buffer: %s", response->getErrorBuffer());
		return;
	}
	std::vector<char>* buffer = response->getResponseData();

	string res;
	res.insert(res.begin(), buffer->begin(), buffer->end());
	//CCLOG(res.c_str());
	OnCheckVersion(res);
}


void UpdateService::OnCheckVersion(string msg)
{
	if (msg.empty())
	{
		return;
	}
	rapidjson::Document dj;
	dj.Parse<0>(msg.c_str());
		
	string value = dj["type"].GetString();
	if (value == "update")
	{		
		UpdateMsg lMsg;
		lMsg.command = CheckVersion;
		lMsg.result = true;
		lMsg.vaule = dj["hotfix"].GetInt();
		lMsg.bigver = dj["bigversion"].GetInt();
		lMsg.versize = dj["size"].GetInt();
		lMsg.flag = 0;
		PushMsg(lMsg);
	}
}

void UpdateService::SendCheckVersion(UpdateMsg const& aMsg)
{
	LuaStack* lpStack = LuaEngine::getInstance()->getLuaStack();
	lpStack->pushString("CheckVersion");
	lpStack->pushBoolean(aMsg.result);
	lpStack->pushInt(aMsg.vaule);
	lpStack->pushInt(aMsg.flag);
	lpStack->pushInt(aMsg.bigver);
	lpStack->pushInt(aMsg.versize);
	lpStack->executeFunctionByHandler(m_Handler, 6);
	lpStack->clean();
}


/*大版本更新begin********************/
void UpdateService::DownloadUpdateFullVersion()
{
	
	//CCLOG(m_UpdateURLInfo.VersionUrl.c_str());
	sendRequestDownloadUpdateFullVersion(m_UpdateURLInfo.VersionUrl.c_str());
}
void UpdateService::sendRequestDownloadUpdateFullVersion(string url)
{
	
	downloader->createDownloadDataTask(url);
	HttpRequest *request = new (std::nothrow)HttpRequest();
	request->setUrl(url);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(UpdateService::onResponseDownloadUpdateFullVersion, this));
	request->setTag("CDMahjong");
	HttpClient::getInstance()->send(request);
	request->release();
	OnDownloadProgress(url);
	
}
void UpdateService::OnDownloadProgress(string url)
{
	downloader->onTaskProgress = [this](const network::DownloadTask& task,
		int64_t bytesReceived,
		int64_t totalBytesReceived,
		int64_t totalBytesExpected)
	{
		////CCLOG("=======%lld, %lld, %lld", bytesReceived, totalBytesReceived, totalBytesExpected);
		//下载进度		
		UpdateMsg lMsg;
		lMsg.command = Downloading;
		lMsg.result = true;
		lMsg.dFileSize = bytesReceived;
		lMsg.dNow = totalBytesReceived;
		lMsg.dTotal = totalBytesExpected;
		PushMsg(lMsg);

	};

	downloader->onDataTaskSuccess = [this](const cocos2d::network::DownloadTask& task,
		std::vector<unsigned char>& data)
	{
		//下载完成
		////CCLOG("down is sucess!");
	};
	
	downloader->onFileTaskSuccess = [this](const cocos2d::network::DownloadTask& task)
	{
		////CCLOG("...........const cocos2d::network::DownloadTask& task\n\n");
	};

	downloader->onTaskError = [this](const cocos2d::network::DownloadTask& task,
		int errorCode,
		int errorCodeInternal,
		const std::string& errorStr)
	{
		log("Failed to download : %s, identifier(%s) error code(%d), internal error code(%d) desc(%s)"
			, task.requestURL.c_str()
			, task.identifier.c_str()
			, errorCode
			, errorCodeInternal
			, errorStr.c_str());
	};
	
}
void UpdateService::onResponseDownloadUpdateFullVersion(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	
	bool lRet = true;
	if (!response)
	{
		return;
	}

	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}

	if (!response->isSucceed())
	{
		//下载失败
		log("response failed, error buffer: %s", response->getErrorBuffer());
		lRet = false;
		//return;
		
	}
	if (lRet)
	{
		std::vector<char>* buffer = response->getResponseData();

		/*std::string res;
		res.insert(res.begin(), buffer->begin(), buffer->end());
		string bufffff(buffer->begin(), buffer->end());*/

		char* res = (char*)malloc(buffer->size());
		std::copy(buffer->begin(), buffer->end(), res);
		string path;

		path = m_UpdateURLInfo.pathSave;
		path += "mahjongCD.apk";
		FILE *fp = fopen(path.c_str(), "wb+");
		//fwrite(res.c_str(), 1, res.size(), fp);
		fwrite(res, 1, buffer->size(), fp);
		fclose(fp);
	}
	OnDownloadFullVersion(lRet);
}

void UpdateService::OnDownloadFullVersion(bool aRet)
{
	UpdateMsg lMsg;
	lMsg.command = DownloadFullVersion;
	lMsg.result = aRet;
	PushMsg(lMsg);
}


void UpdateService::SendDownloadVersionDone(UpdateMsg const& aMsg)
{
	LuaStack* lpStack = LuaEngine::getInstance()->getLuaStack();
	lpStack->pushString("DownloadFullVersionresult");
	lpStack->pushBoolean(aMsg.result);
	lpStack->executeFunctionByHandler(m_Handler, 2);
	lpStack->clean();
}


void UpdateService::SendDownloadProgress(UpdateMsg const& aMsg)
{
	LuaStack* lpStack = LuaEngine::getInstance()->getLuaStack();
	lpStack->pushString("Downloading");
	lpStack->pushBoolean(aMsg.result);
	lpStack->pushLong(aMsg.dFileSize);
	lpStack->pushLong(aMsg.dNow);
	lpStack->pushLong(aMsg.dTotal);
	lpStack->executeFunctionByHandler(m_Handler, 5);
	lpStack->clean();
}
/********************大版本更新end*/



void UpdateService::updateDispatch()
{	
	//UpdateCommand m_status = UpdateMsg::getInstance()->getCurStatus();
//	////CCLOG(".......当前状态", m_status);
	MutexLock::AutoLock lLock(m_Lock);
	std::list<UpdateMsg>::iterator lItor;
	for (lItor = m_MessagesList.begin(); lItor != m_MessagesList.end(); ++lItor)
	{
		UpdateMsg const& lMsg = *lItor;
		switch (lMsg.command)
		{
		case CheckVersion:
			SendCheckVersion(lMsg);
			break;
		case DownloadCheckList:
			SendDownloadCheckList(lMsg);
			break;
		case DownloadFile:
			break;
		case DownloadFullVersion:
			SendDownloadVersionDone(lMsg);		//整包下载完成
			break;
		case Downloading:
			SendDownloadProgress(lMsg);
			break;
		case CanStartGame:
			break;
		case DownloadDone:
			//小版本更新完成
			break;
		case DownloadingFull:
			break;
		default:
			break;
		}
	}
	m_MessagesList.clear();
	Director::getInstance()->getScheduler()->pauseTarget(this);
}





//小版本
void UpdateService::checkDownload(int aVersion)
{	
	setVersion(aVersion);
	//m_UpdateURLInfo.UpdateUrl = UpdateHelper::makeURL(m_UpdateURLInfo.UpdateUrl + UpdateHelper::getZipFileNameWithVersion(ZIP_UPDATELIST_NAME_FORMAT, aVersion)).c_str();
	SendCommand(DownloadCheckList, 0);	
}
void UpdateService::DownLoadCheckList()
{
	string lAddress = UpdateHelper::makeURL(m_UpdateURLInfo.UpdateUrl + UpdateHelper::getZipFileNameWithVersion(ZIP_UPDATELIST_NAME_FORMAT, aVersion)).c_str();
	sendRequestDownloadCheckList(lAddress);
}
void UpdateService::sendRequestDownloadCheckList(string url)
{
	downloader->createDownloadDataTask(url);
	HttpRequest *request = new (std::nothrow)HttpRequest();
	request->setUrl(url);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(UpdateService::onResponseDownloadCheckList, this));
	request->setTag("CDMahjong");
	HttpClient::getInstance()->send(request);
	request->release();

	OnDownloadProgress(url);
}


void UpdateService::onResponseDownloadCheckList(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	bool lRet = true;
	if (!response)
	{
		return;
	}

	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}

	if (!response->isSucceed())
	{
		log("response failed, error buffer: %s", response->getErrorBuffer());
		lRet = false;
		//return;
	}
	if (true == lRet)
	{
		std::vector<char>* buffer = response->getResponseData();

		string res;
		res.insert(res.begin(), buffer->begin(), buffer->end());

		string zipFilePath = m_Path + UpdateHelper::getZipFileNameWithVersion(ZIP_UPDATELIST_NAME_FORMAT, getVersion());
		////CCLOG("当前路径为===%s", zipFilePath.c_str());
		FILE *fp = fopen(zipFilePath.c_str(), "wb+");
		fwrite(res.c_str(), 1, res.size(), fp);
		fclose(fp);

		if (CFileUtil::isFileExist(zipFilePath.c_str()))
		{
			unsigned long unZipFileLen;
			if (!CFileUtil::extractFilesWithoutDirFromZip(zipFilePath.c_str(), m_Path.c_str(), &unZipFileLen))
			{
				////CCLOG("提取文件失败");
				remove(zipFilePath.c_str());
				return;
			}
		}

		//解析提取的文件；
		std::string lCheckFile = m_Path + UNZIP_UPDATELIST_NAME;
		ParseUpdateDataInCheckList(lCheckFile.c_str());
	}

	if (false == lRet)
	{
		OnDownloadCheckList(lRet);
	}
}


bool UpdateService::ParseUpdateDataInCheckList(char const* apFile)
{
	bool  ret = false;
	FILE *pf;
	pf = fopen(apFile, "rb");
	if (!pf)
		return ret;
	int		lineSize = 4096;
	char	lineBuf[4096];
	memset(lineBuf, 0, sizeof lineBuf);

#define FILE_LEN 8

	m_vUpdateItem.clear();
	//m_vecFilesNeedUpdate.clear();
	CheckItem* pItem = new CheckItem();
	while (fgets(lineBuf, lineSize, pf))
	{
		char *pLine = lineBuf;
		if (Utility::Stricmp(lineBuf, UPDATE_CHECKLIST_HEAD) == 0)
			continue;
		else if (Utility::Stricmp(lineBuf, UPDATE_CHECKLIST_TAIL) == 0)
			break;
		{

			pItem->bUpdata = FALSE;
			//文件名：
			char pOffset = 0;
			memset(pItem->filename, 0, sizeof(pItem->filename));
			pOffset = strchr(pLine, '=') - pLine;
			strncpy(pItem->filename, pLine, pOffset);
			pLine = pLine + pOffset + 1;        //去除名字和等号 ：text.ini=
			//
			//压缩文件大小
			char dataSize[FILE_LEN + 1] = { 0 };
			UpdateHelper::reverseStringWithEachTwoChars(pLine, dataSize, FILE_LEN);
			unsigned long result = 0;
			result = UpdateHelper::hexStrToDecValue(dataSize, FILE_LEN);
			pItem->zipsize = result;
			pLine += sizeof(unsigned int)* 2;
			//原文件大小
			//sFileSizeAddFileSha sha_size;
			//memcpy(&sha_size, pLine, sizeof sha_size);
			//char dataSize[4*2];
			//memset(dataSize, 0, sizeof dataSize);
			UpdateHelper::reverseStringWithEachTwoChars(pLine, dataSize, FILE_LEN);
			//unsigned long result = 0;
			result = UpdateHelper::hexStrToDecValue(dataSize, FILE_LEN);
			pItem->unzipsize = result;
			pLine += sizeof(unsigned int)* 2;

#define   hasPieceLen  sizeof(unsigned int)*2
#define   bufempty(n) memset(has1_##n, 0, hasPieceLen)
			char has1_0[hasPieceLen], has1_1[hasPieceLen], has1_2[hasPieceLen], has1_3[hasPieceLen], has1_4[hasPieceLen];
			bufempty(0);
			bufempty(1);
			bufempty(2);
			bufempty(3);
			bufempty(4);
			for (int n = 0; n < 5; n++)
			{
				UpdateHelper::reverseStringWithEachTwoChars(pLine, pItem->sha1 + n*hasPieceLen, hasPieceLen);
				pLine += hasPieceLen;
			}
			pItem->sha1[2 * SHA1_SIZE] = '\0';
			char version[10] = { 0 };
			UpdateHelper::reverseStringWithEachTwoChars(pLine, version, sizeof(version)-2);//5*2
			pItem->version = UpdateHelper::hexStrToDecValue(version, sizeof(version)-2);
			//log("checklist item: name = %s, zipSize = %lu, unzipSize = %lu, sha = %s, version = %lu ", pItem->filename, pItem->zipsize, pItem->unzipsize, pItem->sha1, pItem->version);

			checkLocalFileWithCheckItem(pItem);
			if (pItem->bUpdata)
			{
				//log("pthread_join :%d", n);
				//整合版本号
				sprintf(pItem->filename, "%s_%d.zip", pItem->filename, (int)pItem->version);
				log("File: %s", pItem->filename);
				//m_TotalUpdateSize += pItem->zipsize;
				//m_vecFilesNeedUpdate.push_back(pItem->filename);
				m_vUpdateItem.push_back(*pItem);
			}
		}
	}
	delete pItem;
	fclose(pf);

	DownloadFiles(0);
	return true;
}

bool UpdateService::checkLocalFileWithCheckItem(CheckItem *pItem)
{
	//1.如果本地不存，则更新；

	string srcFilePath;
	srcFilePath.append(m_Path);
	srcFilePath.append(pItem->filename);
#ifndef WIN32
	UpdateHelper::replaceChar(srcFilePath, '\\', '/');
#endif
	if (!CFileUtil::isFileExist(srcFilePath.c_str()))
	{
		//CTLog("file :%s is noexist.", srcFilePath.c_str());
		pItem->bUpdata = TRUE;
	}
	//2.存在
	else
	{
		//2.1 先与同名文件比较大小
		if (CFileUtil::getFileSize(srcFilePath.c_str()) != pItem->unzipsize)
		{
			//CTLog("file :%s has different size.", srcFilePath.c_str());
			pItem->bUpdata = TRUE;
		}
		//2.2 比较sha1值
		else
		{
			uint8_t hash_value[SHA1_SIZE];
			char     hash_code[SHA1_SIZE * 2 + 1];
			memset(hash_value, 0, sizeof hash_value);
			memset(hash_code, 0, sizeof hash_code);
			FILE *pf = fopen(srcFilePath.c_str(), "rb"); //must be binary format;
			if (pf)
			{
				if (GetFileSHA1(pf, hash_value) == shaSuccess)
				{
					log("Compare file :%s ", srcFilePath.c_str());
					for (int n = 0; n < SHA1_SIZE; n++)
					{
						sprintf(hash_code + 2 * n, "%02X", hash_value[n]);
						//printf("%02X ", hash_value[n]);
					}
					//printf("\n");
					if (strncmp(hash_code, pItem->sha1, SHA1_SIZE) != 0)
					{
						//CTLog("file :%s has different sha value.", srcFilePath.c_str());
						pItem->bUpdata = TRUE;
					}
					else
						pItem->bUpdata = FALSE;
				}
				else
					pItem->bUpdata = FALSE;
			}
		}
	}
	return true;
}


bool UpdateService::DownloadFiles(int aIndex)
{
	bool lRet = false;
	std::string lFileFaild;
	for (m_DownloadFileIndex = aIndex; m_DownloadFileIndex < m_vUpdateItem.size()/* && !m_Stop*/; ++m_DownloadFileIndex)
	{
		string fileName = m_Path + m_vUpdateItem[m_DownloadFileIndex].filename;
		CFileUtil::MakeDir(fileName.c_str());
		string url = m_UpdateURLInfo.UpdateUrl + m_vUpdateItem[m_DownloadFileIndex].filename;

		HttpRequest *request = new (std::nothrow)HttpRequest();
		request->setUrl(url);
		request->setRequestType(HttpRequest::Type::GET);
		request->setResponseCallback(CC_CALLBACK_2(UpdateService::onResponseDownloadFiles, this));
		request->setTag("DownloadFiles");
		HttpClient::getInstance()->send(request);
		request->release();
		lRet = true;
	}
	if (false == lRet)
	{
		OnDownloadCheckList(lRet);
	}
	return lRet;
}

void UpdateService::onResponseDownloadFiles(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	bool lRet = true;
	if (!response)
	{
		return;
	}

	if (0 != strlen(response->getHttpRequest()->getTag()))
	{
		log("%s completed", response->getHttpRequest()->getTag());
	}

	if (!response->isSucceed())
	{
		log("response failed, error buffer: %s", response->getErrorBuffer());
		lRet = false;
		//return;
	}
	if (true == lRet)
	{
		m_DownloadCheckListIndex++;

		std::vector<char>* buffer = response->getResponseData();

		std::string res;
		res.insert(res.begin(), buffer->begin(), buffer->end());

		//string m_Path = CCFileUtils::sharedFileUtils()->getWritablePath();
		if (m_vUpdateItem.size() <= 0)
			return;
		string	zipFilePath = m_Path + m_vUpdateItem[m_DownloadFileIndex - 1].filename;
		////CCLOG("当前路径为===%s", zipFilePath.c_str());

		FILE *fp = fopen(zipFilePath.c_str(), "wb+");
		fwrite(res.c_str(), 1, res.size(), fp);
		fclose(fp);


		if (CFileUtil::isFileExist(zipFilePath.c_str()))
		{
			unsigned long  unZipFileLen;
			//释放zip文件
			std::string lPath = CFileUtil::ExtrFilePath(zipFilePath.c_str());
			if (!CFileUtil::extractFilesWithoutDirFromZip(zipFilePath.c_str(), lPath.c_str(), &unZipFileLen))
			{
				return;
			}
		}
	}
	if (false == lRet || m_DownloadCheckListIndex == m_vUpdateItem.size())
	{
		////CCLOG("当前实际下载个数:%d", m_DownloadCheckListIndex);
		OnDownloadCheckList(lRet);
	}
}


void UpdateService::OnDownloadCheckList(bool aRet)
{
		UpdateMsg lMsg;
		lMsg.command = DownloadCheckList;
		lMsg.result = aRet;
		//lMsg.vaule = aSize;
		PushMsg(lMsg);
}

void UpdateService::SendDownloadCheckList(UpdateMsg const& aMsg)
{
	LuaStack* lpStack = LuaEngine::getInstance()->getLuaStack();
	lpStack->pushString("DownloadCheckList");
	lpStack->pushBoolean(aMsg.result);
	//lpStack->pushLong(aMsg.vaule);
	lpStack->executeFunctionByHandler(m_Handler, 2);
	lpStack->clean();
}