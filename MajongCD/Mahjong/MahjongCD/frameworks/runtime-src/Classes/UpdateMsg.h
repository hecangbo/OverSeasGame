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

//��ȡ�ķ���������
public:
	void SetReceiveMsg(string msg);
	string GetReciveMsg();
private:
	string m_receiveMsg;

//��ǰ״̬
public:
	void setCurStatus(UpdateCommand command);
	UpdateCommand getCurStatus();
private:
	UpdateCommand m_updatecommand;
};

