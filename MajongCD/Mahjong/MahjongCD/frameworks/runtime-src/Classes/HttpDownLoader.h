/*
*下载更新
*/
#pragma once
#include "cocos2d.h"
#include <string>
#include "cocos-ext.h"
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
USING_NS_CC;
using namespace std;
using namespace cocos2d::network;

#define     SHA1_SIZE                                        20
#define     ZIP_UPDATELIST_NAME_FORMAT			  "CheckList_Star.ini_%d00.zip"
#define     UNZIP_UPDATELIST_NAME                     "CheckList_Star.ini"
#define     UPDATE_CHECKLIST_HEAD                    "[masters]\r\n"
#define     UPDATE_CHECKLIST_TAIL                      "[version]\r\n"

struct CheckItem
{
	char filename[512];
	unsigned long zipsize;
	unsigned long unzipsize;
	unsigned long version;
	bool bUpdata;
	char sha1[SHA1_SIZE * 2 + 1];
};

class HttpDownLoader:public Ref
{
public:
	HttpDownLoader();
	~HttpDownLoader();

public:
	//void sendRequest(string url, int type);
	//void onResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	void sendRequest(string url);
	void onResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	//小版本更新
	void DownLoadCheckList(string lAddress, int aVersion);
	void onResponseDownLoadCheckList(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	bool ParseUpdateDataInCheckList(char const* apFile);
	bool checkLocalFileWithCheckItem(CheckItem *pItem);

	

	bool DownloadFiles(int aIndex);			//小版本下载文件
	void onResponseDownloadFiles(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
private:
	std::vector<CheckItem> m_vUpdateItem;
	//std::vector<std::string> m_vecFilesNeedUpdate;

	string m_Path;
	long m_DownloadFileIndex;


	CC_SYNTHESIZE(int, aVersion, Version);
	CC_SYNTHESIZE(string, lAddress, Address);
};

