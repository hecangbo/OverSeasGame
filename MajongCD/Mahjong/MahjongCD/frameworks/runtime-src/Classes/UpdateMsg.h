#pragma once
#include <string>
#include "UpdateService.h"
using namespace std;
class UpdateMsg
{
public:
	UpdateMsg();
	~UpdateMsg();

public:
	static UpdateMsg*getInstance();
private:
	static UpdateMsg *m_pInstance;

//获取的服务器数据
public:
	void SetReceiveMsg(string msg);
	string GetReciveMsg();
private:
	string m_receiveMsg;

//当前状态
public:
	void setCurStatus(UpdateCommand command);
	UpdateCommand getCurStatus();
private:
	UpdateCommand m_updatecommand;
};

