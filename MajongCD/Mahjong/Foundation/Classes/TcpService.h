#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include "Thread.h"
#include "Tcp.h"
#include "ITcpHandler.h"
#include "TcpBuffer.h"
#include <queue>
#include "MemoryPool.h"
#include "MutexLock.h"

class TcpService : public Thread
{
public:
	enum ErrorCode
	{
		FAILED,
		SUCCESS
	};

	TcpService(ITcpHandler& aMessageHandler);

	virtual ~TcpService();

	void Start(const char* apAddress, int aPort);

	void AsyncRecv(char* apBuffer, int aSize);

	void AsyncSend(char* apBuffer, int aSize);

	int Send(char* apBuffer, int aSize);

	void ClearSendData();

	int GetSendDataCount(){return m_SendQueue.size();}

	MutexLock& GetLock(){ return m_Lock; }

private:
	bool Connect();

	void Communication();

	void HandleError();

protected:
	virtual void RunTask();

private:
	Tcp m_Tcp;
	char m_Address[128];
	int m_Port;
	TcpBuffer m_RecvBuf;
	ITcpHandler& m_TcpHandler;
	std::queue<TcpBuffer*> m_SendQueue;
	MemoryPool m_MemoryPool;
	MutexLock m_Lock;
};

#endif

