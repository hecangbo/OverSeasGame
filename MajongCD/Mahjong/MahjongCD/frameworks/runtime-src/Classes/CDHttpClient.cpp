#include "CDHttpClient.h"

using namespace cocos2d::network;
//#include "UpdateMsg.h"
CDHttpClient::CDHttpClient()
{
	//m_updateservice = new UpdateService();
}


CDHttpClient::~CDHttpClient()
{
}

//void CDHttpClient::sendRequest(string url, int type, const ccHttpRequestCallback& callback)
//获取服务器升级版本信息
void CDHttpClient::sendRequest(string url, int type)
{
	HttpRequest *request = new (std::nothrow)HttpRequest();
	request->setUrl(url);
	request->setRequestType(HttpRequest::Type::GET);  //(HttpRequest::Type)type
	request->setResponseCallback(CC_CALLBACK_2(CDHttpClient::onResponse, this));
	//request->setResponseCallback(callback);
	request->setTag("CDMahjong");	
	HttpClient::getInstance()->send(request);
	request->release();
}
void CDHttpClient::onResponse(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	if (!response)
	{
		return;
	}

	//if ((response->getHttpRequest()->getTag()))
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
	string res;
	res.insert(res.begin(), buffer->begin(), buffer->end());

	/*res = "";
	char* s = &(*buffer->begin());
	res = string(s, buffer->size());*/
	
	//UpdateMsg::getInstance()->SetReceiveMsg(res);
	//m_updateservice->OnCheckVersion(res);
}


void CDHttpClient::SetBuffer(vector<char>* buffer)
{
	m_buffer = buffer;
}

string CDHttpClient::Getbuffer(vector<char>* buffer)
{
	string res;
	for (unsigned int i = 0; i < m_buffer->size(); i++)
		res+= (*m_buffer)[i];
	//UpdateMsg::getInstance()->SetReceiveMsg(res);
	return res;
}