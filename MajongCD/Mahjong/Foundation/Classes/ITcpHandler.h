#ifndef _I_TCP_HANDLER_H_
#define _I_TCP_HANDLER_H_

struct ITcpHandler
{
	virtual void HandleConnect(bool aConnected) = 0;

	virtual void HandleRecv(char* apBuffer, int aSize) = 0;

	virtual void HandleSend(char* apBuffer, int aSize) = 0;

	virtual void HandleError() = 0;
};

#endif