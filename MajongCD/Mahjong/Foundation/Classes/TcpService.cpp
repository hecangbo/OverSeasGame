#include "TcpService.h"
#include <string.h>
#ifndef WIN32
#include <netdb.h>
#endif

TcpService::TcpService(ITcpHandler& aTcpHandler)
	: m_TcpHandler(aTcpHandler),
	  m_MemoryPool(sizeof(TcpBuffer), 8)
{
	m_RecvBuf.RetSet();
}

TcpService::~TcpService(void)
{
	
}

void TcpService::RunTask()
{
	m_Tcp.Create();
	m_Tcp.SetNonblocking();
	int i = 1;
	if(Connect())
	{
		//m_Tcp.SetNonblocking();
		Communication();
	}
}

void TcpService::Start(const char* apAddress, int aPort )
{
	strncpy(m_Address, apAddress, sizeof(m_Address));
	m_Port = aPort;
	Run();
}

void TcpService::AsyncRecv(char* apBuffer, int aSize)
{
	m_RecvBuf.Set(apBuffer, aSize);
}

void TcpService::AsyncSend(char* apBuffer, int aSize)
{
	m_Lock.Lock();

	TcpBuffer* lpBuffer = (TcpBuffer*)m_MemoryPool.Alloc();
	lpBuffer->Set(apBuffer, aSize);
	m_SendQueue.push(lpBuffer);

	m_Lock.Unlock();
}

bool TcpService::Connect()
{
	char lIP[32] = {0};
#ifdef WIN32
	HOSTENT* lpHost;
#else
	hostent* lpHost;
#endif
	lpHost = gethostbyname(m_Address);
	if(0 == lpHost)
	{
		m_TcpHandler.HandleConnect(FAILED);
		return false;
	}

	sprintf(lIP, "%d.%d.%d.%d",
		(lpHost->h_addr_list[0][0]&0x00ff),
		(lpHost->h_addr_list[0][1]&0x00ff),
		(lpHost->h_addr_list[0][2]&0x00ff),
		(lpHost->h_addr_list[0][3]&0x00ff));
	int lRet = m_Tcp.Connect(lIP, m_Port);
	
	while(true)
	{
		lRet = 0;
		timeval lTimeVal; 
		lTimeVal.tv_sec = 8;
		lTimeVal.tv_usec = 0;
		fd_set lWriteSet;
		FD_ZERO(&lWriteSet);
		FD_SET(m_Tcp.GetSocket(), &lWriteSet);

		if(select(m_Tcp.GetSocket() + 1, 0, &lWriteSet, 0, &lTimeVal) <= 0)
		{
			lRet = -1;
			break;
		}

		if(!FD_ISSET(m_Tcp.GetSocket(), &lWriteSet))
		{
			lRet = -1;
			break;
		}

		int lError;
		unsigned int lLen = sizeof(lError);
#ifdef WIN32
		if(getsockopt(m_Tcp.GetSocket(), SOL_SOCKET, SO_ERROR, (char *)&lError, (int*)&lLen) < 0)
#elif defined(ANDROID)
		if(getsockopt(m_Tcp.GetSocket(), SOL_SOCKET, SO_ERROR, &lError, (int*)&lLen) < 0)
#else
		if(getsockopt(m_Tcp.GetSocket(), SOL_SOCKET, SO_ERROR, &lError, &lLen) < 0)
#endif
		{
			lRet = -1;
			break;
		}

		if(lError != 0) 
		{
			lRet = -1;
			break;
		}
		break;
	}
	
	if(lRet > -1)
	{
		m_TcpHandler.HandleConnect(SUCCESS);
		return true;
	}
	else
	{
		m_TcpHandler.HandleConnect(FAILED);
	}
	return false;
}

void TcpService::Communication()
{
	timeval lTimeVal = {10, 0}; 
	fd_set lWriteSet;
	fd_set lReadSet;
	int lSize = 0;
	int lRet = 0;

	while(!m_Stop)
	{
		FD_ZERO(&lWriteSet);
		FD_ZERO(&lReadSet);

		FD_SET(m_Tcp.GetSocket(), &lWriteSet);
		FD_SET(m_Tcp.GetSocket(), &lReadSet);

		lRet = select(m_Tcp.GetSocket() + 1, &lReadSet, &lWriteSet, 0, &lTimeVal);
		if(lRet > 0)
		{
			if (FD_ISSET(m_Tcp.GetSocket(), &lReadSet))
			{
				if(m_RecvBuf.IsAvalibe())
				{
					lSize = m_Tcp.RecvFull(m_RecvBuf.m_pBuffer, m_RecvBuf.m_Size);
					if(lSize == m_RecvBuf.m_Size)
					{
						m_TcpHandler.HandleRecv(m_RecvBuf.Detach(), m_RecvBuf.m_Size);
					}
					else
					{
						//HandleError();
						break;
					}
				}
			}

			if (FD_ISSET(m_Tcp.GetSocket(), &lWriteSet))
			{
				m_Lock.Lock();
				TcpBuffer* lpSendBuffer = 0;
				while (!m_SendQueue.empty())
				{
					lpSendBuffer = m_SendQueue.front();
					if(lpSendBuffer->IsAvalibe())
					{
						lSize = m_Tcp.SendFull(lpSendBuffer->m_pBuffer, lpSendBuffer->m_Size);
						if(lSize == lpSendBuffer->m_Size)
						{
							m_TcpHandler.HandleSend(lpSendBuffer->Detach(), lpSendBuffer->m_Size);
							m_SendQueue.pop();
							m_MemoryPool.Free(lpSendBuffer);
						}
						else
						{
							//HandleError();
							break;
						}
					}
				}
				m_Lock.Unlock();
			}
		}
		else if(lRet < 0 && errno != EINTR)
		{
			break;
		}
#ifdef WIN32
		Sleep(1);
#else
		usleep(1);
#endif
	} 

	HandleError();
}

void TcpService::HandleError()
{
	m_Stop = true;
	m_Tcp.Close();
	m_TcpHandler.HandleError();
}

int TcpService::Send( char* apBuffer, int aSize )
{
	return m_Tcp.Send(apBuffer, aSize);
}

void TcpService::ClearSendData()
{
	TcpBuffer* lpSendBuffer = 0;
	while(!m_SendQueue.empty())
	{
		lpSendBuffer = m_SendQueue.front();
		m_SendQueue.pop();
		m_MemoryPool.Free(lpSendBuffer);
	}
}

