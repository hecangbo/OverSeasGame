#include "UpdateMsg.h"

UpdateMsg * UpdateMsg::m_pInstance = nullptr;
UpdateMsg::UpdateMsg()
{
}


UpdateMsg::~UpdateMsg()
{
}

UpdateMsg *UpdateMsg::getInstance()
{
	if (m_pInstance == nullptr)
		m_pInstance = new UpdateMsg();
	return m_pInstance;

}

void UpdateMsg::SetReceiveMsg(string msg)
{
	m_receiveMsg = msg;
}
string UpdateMsg::GetReciveMsg()
{
	return m_receiveMsg;

}

void UpdateMsg::setCurStatus(UpdateCommand command)
{
	m_updatecommand = command;
}
UpdateCommand UpdateMsg::getCurStatus()
{
	return m_updatecommand;
}