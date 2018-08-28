#pragma once
#include "cocos2d.h"
#include <string>
#include <vector>
#include "cocos-ext.h"
#include "CDHttpClient.h"
#include "HttpDownLoader.h"

#include "Thread.h"
#include "TcpService.h"
#include <semaphore.h>

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;

enum UpdateCommand
{
	CheckVersion,
	DownloadCheckList,			// 小版本
	DownloadFile,
	DownloadFullVersion,		//大版本
	Exit,

	Downloading,
	CanStartGame,
	DownloadDone,

	DownloadingFull,

	CommandMax
};

struct UpdateMsg
{
	UpdateCommand command;
	bool result;
	int vaule;
	int bigver;
	int versize;
	int writeSize;
	int dFileSize;
	int dNow;
	int dTotal;
	int flag;
};

struct UpdateOperation
{
	UpdateOperation()
	{
		Command = CommandMax;
		pData = 0;
	}
	UpdateCommand Command;
	void* pData;
	std::string m_Address;
	std::string m_SavePath;
};


struct UpdateURLInfo
{
	string VersionUrl;		//version地址
	string UpdateUrl;		//小版本更新地址
	string pathSave;
};
class UpdateService
	:public Ref				//public CCNode
	, public Thread
{
public:
	UpdateService(void);
	virtual ~UpdateService(void);
public:
	virtual void RunTask();

public:
	void Start(string info, int aHandler);
	virtual void Stop();
	void update(float dt);   //使用了schedule函数
	void SetScriptHandler(long aHandler){ m_Handler = aHandler; }
private:
	UpdateURLInfo m_UpdateURLInfo;
	long m_Handler;

public:
	void SendCommand(UpdateCommand aCommand, void* apData);
	void SendCommand(UpdateCommand aCommand, std::string& aAddress, std::string& aSavePath);
	void checkVersion();
private:
	UpdateOperation m_Operation;
	sem_t* m_pMain;//信号量
	sem_t* m_pSub;
	
	bool m_Stop;		//下载是否结束标记

public:
	void CheckUpdateVersion();		//得到服务器的版本号
	void sendRequest(string url, int type);
	void onResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);


public:
	void updateDispatch();


public:
	void OnCheckVersion(string msg);			//检查版本号
	void checkDownload(int aVersion);

public:
	void PushMsg(UpdateMsg& aMsg);
private:
	std::list<UpdateMsg> m_MessagesList;
	MutexLock m_Lock;

public:
	void SendCheckVersion(UpdateMsg const& aMsg);

private:
	string m_Path;			//游戏数据存储目录
//大版本
public:
	void checkDownloadFull(string sAddress, string path);		//大版本更新
	void DownloadUpdateFullVersion();
	void sendRequestDownloadUpdateFullVersion(string url);
	void onResponseDownloadUpdateFullVersion(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	void OnDownloadProgress(string url);			//下载进度（大小版本都用这个）
	void OnDownloadFullVersion(bool aRet);
	void SendDownloadVersionDone(UpdateMsg const& aMsg);		//整包下载完成
	void SendDownloadProgress(UpdateMsg const& aMsg);			//下载进度
private:
	int m_BigVersion;
//小版本
public:
	void DownLoadCheckList();
	void sendRequestDownloadCheckList(string rul);
	void onResponseDownloadCheckList(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	bool ParseUpdateDataInCheckList(char const* apFile);
	bool checkLocalFileWithCheckItem(CheckItem *pItem);
	bool DownloadFiles(int aIndex);
	void onResponseDownloadFiles(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	void OnDownloadCheckList(bool aRet);
	void SendDownloadCheckList(UpdateMsg const& aMsg);		//小版本更新下载完成
private:
	int m_Version;
	std::vector<CheckItem> m_vUpdateItem;
	long m_DownloadFileIndex;
	long m_DownloadCheckListIndex;		//当前实际下载个数
	CC_SYNTHESIZE(int, aVersion, Version);

	std::unique_ptr<network::Downloader> downloader;
};

