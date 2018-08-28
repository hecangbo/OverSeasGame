/*
*获取版本服务器信息
*/
#pragma once
#include "cocos2d.h"
#include <string>
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "extensions/cocos-ext.h"
#include <vector>
//#include "UpdateService.h"
using namespace std;
USING_NS_CC;

class CDHttpClient:public Ref
{
public:
	CDHttpClient();
	~CDHttpClient();
	//void sendRequest(string url, int type, const ccHttpRequestCallback& callback);  //HttpRequest::Type 
	//服务器升级版本信息
	void sendRequest(string url, int type);
	void onResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

public:
	void SetBuffer(vector<char>* buffer);
	string Getbuffer(vector<char>* buffer);
private:
	vector<char>* m_buffer;
	//UpdateService *m_updateservice;
};

