#include "HttpDownLoader.h"
//#include "UpdateMsg.h"
#include "UpdateHelper.h"
#include "CFileUtil.h"
#include "sha1.h"
#include "Utility.h"

HttpDownLoader::HttpDownLoader()
:m_DownloadFileIndex(0)
{
	m_Path = CCFileUtils::sharedFileUtils()->getWritablePath();
}


HttpDownLoader::~HttpDownLoader()
{
}


void HttpDownLoader::sendRequest(string url)
{
	HttpRequest *request = new (std::nothrow)HttpRequest();
	request->setUrl(url);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(HttpDownLoader::onResponse, this));
	//request->onProgress(1, 2, 4, 5);
	request->setTag("CDMahjong");
	HttpClient::getInstance()->send(request);
	request->release();
}
void HttpDownLoader::onResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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

	/*if (response->getHttpRequest()->getReferenceCount() != 2)
	{
	log("request ref count not 2, is %d", response->getHttpRequest()->getReferenceCount());
	}*/	
	//string res;
	//res.insert(res.begin(), buffer->begin(), buffer->end());

	//UpdateMsg::getInstance()->SetReceiveMsg(res);
	//std::string bufffff(buffer->begin(), buffer->end());
	std::string res;
	res.insert(res.begin(), buffer->begin(), buffer->end());
	//string path = FileUtils::sharedFileUtils()->getWritablePath();
	string path;
	if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		path = "f://mnt//sdcard//qp.game.cache//";
	else
		path = "//mnt//sdcard//qp.game.cache//";
	
	log("apk save path==%s", path.c_str());
	path += "mahjongCD.apk";
	FILE *fp = fopen(path.c_str(), "wb+");
	fwrite(res.c_str(), 1, res.size(), fp);
	fclose(fp);

	//UpdateMsg::getInstance()->setCurStatus(DownloadFullVersion);
}


void HttpDownLoader::DownLoadCheckList(string lAddress, int aVersion)
{
	setVersion(aVersion);
	setAddress(lAddress);
	/*string fileName = CCFileUtils::getInstance()->getWritablePath();
	fileName = fileName + UpdateHelper::getZipFileNameWithVersion(ZIP_UPDATELIST_NAME_FORMAT, aVersion).c_str();*/
	string fileName = lAddress + UpdateHelper::getZipFileNameWithVersion(ZIP_UPDATELIST_NAME_FORMAT, aVersion).c_str();
	HttpRequest *request = new (std::nothrow)HttpRequest();
	request->setUrl(fileName);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(HttpDownLoader::onResponseDownLoadCheckList, this));
	request->setTag("CheckList");
	HttpClient::getInstance()->send(request);
	request->release();
}

void HttpDownLoader::onResponseDownLoadCheckList(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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

	std::string res;
	res.insert(res.begin(), buffer->begin(), buffer->end());
	
	string m_Path = CCFileUtils::sharedFileUtils()->getWritablePath();
	string	zipFilePath = m_Path + UpdateHelper::getZipFileNameWithVersion(ZIP_UPDATELIST_NAME_FORMAT, getVersion());
	CCLOG("当前路径为===%s", zipFilePath.c_str());

	FILE *fp = fopen(zipFilePath.c_str(), "wb+");
	fwrite(res.c_str(), 1, res.size(), fp);
	fclose(fp);

	if (CFileUtil::isFileExist(zipFilePath.c_str()))
	{
		unsigned long unZipFileLen;
		if (!CFileUtil::extractFilesWithoutDirFromZip(zipFilePath.c_str(), m_Path.c_str(),&unZipFileLen))
		{
			CCLOG("提取文件失败");
			remove(zipFilePath.c_str());
			return ;
		}
	}

	//解析提取的文件；
	std::string lCheckFile = m_Path + UNZIP_UPDATELIST_NAME;
	ParseUpdateDataInCheckList(lCheckFile.c_str());
	DownloadFiles(0);
	return ;
}

bool HttpDownLoader::ParseUpdateDataInCheckList(char const* apFile)
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
	return true;
}

bool HttpDownLoader::checkLocalFileWithCheckItem(CheckItem *pItem)
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

bool HttpDownLoader::DownloadFiles(int aIndex)
{
	bool lRet = true;
	std::string lFileFaild;
	for (m_DownloadFileIndex = aIndex; m_DownloadFileIndex < m_vUpdateItem.size()/* && !m_Stop*/; ++m_DownloadFileIndex)
	{
		string fileName = m_Path + m_vUpdateItem[m_DownloadFileIndex].filename;
		CFileUtil::MakeDir(fileName.c_str());
		string url = getAddress() + m_vUpdateItem[m_DownloadFileIndex].filename;

		HttpRequest *request = new (std::nothrow)HttpRequest();
		request->setUrl(url);
		request->setRequestType(HttpRequest::Type::GET);
		request->setResponseCallback(CC_CALLBACK_2(HttpDownLoader::onResponseDownloadFiles, this));
		request->setTag("DownloadFiles");
		HttpClient::getInstance()->send(request);
		request->release();
	}
	return lRet;
}

void HttpDownLoader::onResponseDownloadFiles(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
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

	std::string res;
	res.insert(res.begin(), buffer->begin(), buffer->end());

	//string m_Path = CCFileUtils::sharedFileUtils()->getWritablePath();
	if (m_vUpdateItem.size() <= 0)
		return;
	string	zipFilePath = m_Path + m_vUpdateItem[m_DownloadFileIndex-1].filename;
	CCLOG("当前路径为===%s", zipFilePath.c_str());

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

	//UpdateMsg::getInstance()->setCurStatus(DownloadCheckList);
}