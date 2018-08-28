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
	DownloadCheckList,			// С�汾
	DownloadFile,
	DownloadFullVersion,		//��汾
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
	string VersionUrl;		//version��ַ
	string UpdateUrl;		//С�汾���µ�ַ
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
	void update(float dt);   //ʹ����schedule����
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
	sem_t* m_pMain;//�ź���
	sem_t* m_pSub;
	
	bool m_Stop;		//�����Ƿ�������

public:
	void CheckUpdateVersion();		//�õ��������İ汾��
	void sendRequest(string url, int type);
	void onResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);


public:
	void updateDispatch();


public:
	void OnCheckVersion(string msg);			//���汾��
	void checkDownload(int aVersion);

public:
	void PushMsg(UpdateMsg& aMsg);
private:
	std::list<UpdateMsg> m_MessagesList;
	MutexLock m_Lock;

public:
	void SendCheckVersion(UpdateMsg const& aMsg);

private:
	string m_Path;			//��Ϸ���ݴ洢Ŀ¼
//��汾
public:
	void checkDownloadFull(string sAddress, string path);		//��汾����
	void DownloadUpdateFullVersion();
	void sendRequestDownloadUpdateFullVersion(string url);
	void onResponseDownloadUpdateFullVersion(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	void OnDownloadProgress(string url);			//���ؽ��ȣ���С�汾���������
	void OnDownloadFullVersion(bool aRet);
	void SendDownloadVersionDone(UpdateMsg const& aMsg);		//�����������
	void SendDownloadProgress(UpdateMsg const& aMsg);			//���ؽ���
private:
	int m_BigVersion;
//С�汾
public:
	void DownLoadCheckList();
	void sendRequestDownloadCheckList(string rul);
	void onResponseDownloadCheckList(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	bool ParseUpdateDataInCheckList(char const* apFile);
	bool checkLocalFileWithCheckItem(CheckItem *pItem);
	bool DownloadFiles(int aIndex);
	void onResponseDownloadFiles(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	void OnDownloadCheckList(bool aRet);
	void SendDownloadCheckList(UpdateMsg const& aMsg);		//С�汾�����������
private:
	int m_Version;
	std::vector<CheckItem> m_vUpdateItem;
	long m_DownloadFileIndex;
	long m_DownloadCheckListIndex;		//��ǰʵ�����ظ���
	CC_SYNTHESIZE(int, aVersion, Version);

	std::unique_ptr<network::Downloader> downloader;
};

