#include "Tcp.h"

Tcp::Tcp()
    : m_Socket(-1)
{
    
}

Tcp::~Tcp()
{
    Close();
}

bool Tcp::Create()
{
    Close();
    m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(-1 != m_Socket)
    {
        return true;
    }
    return false;
}


bool Tcp::SetBlocking()
{
#ifdef WIN32
	unsigned long ul = 0;
	return (ioctlsocket(m_Socket, FIONBIO, (unsigned long*)&ul) != SOCKET_ERROR);
#else
	int lOptions;
	lOptions = fcntl(m_Socket, F_GETFL);
	if(lOptions >= 0 && (lOptions & O_NONBLOCK))
	{
		lOptions ^= O_NONBLOCK;
		if(fcntl(m_Socket, F_SETFL, lOptions) >= 0)
		{
			return 1;
		}
	}
	return 0;
#endif
}

bool Tcp::SetNonblocking()
{
#ifdef WIN32
	unsigned long ul = 1;
	return (ioctlsocket(m_Socket, FIONBIO, (unsigned long*)&ul) != SOCKET_ERROR);
#else
	int lOptions;
	lOptions = fcntl(m_Socket, F_GETFL);
	if(lOptions >= 0)
	{
		lOptions |= O_NONBLOCK;
		if(fcntl(m_Socket, F_SETFL, lOptions) >= 0)
		{
			return 1;
		}
	}
	return 0;
#endif
}

int Tcp::Connect(char *apAddr, int aPort)
{
    struct sockaddr_in lAddr;
    lAddr.sin_family = AF_INET;
    lAddr.sin_addr.s_addr = inet_addr(apAddr);
    lAddr.sin_port = htons(aPort);
    return connect(m_Socket, (sockaddr*)&lAddr, sizeof(lAddr));
}

void Tcp::Close()
{
    if(-1 != m_Socket)
    {
        shutdown(m_Socket, SHUT_BOTH);
#ifdef WIN32
		closesocket(m_Socket);
#else
        close(m_Socket);
#endif
        m_Socket = -1;
    }
}

int Tcp::Send(char *apBuf, int aLength)
{
    return send(m_Socket, apBuf, aLength, 0);
}

int Tcp::Recv(char *apBuf, int aLength)
{
    return recv(m_Socket, apBuf, aLength, 0);
}

int Tcp::SendFull(char *apBuffer, int aLength)
{
    int lTotLength = 0;
    int lLength = 0;
    while(true)
    {
        lLength = Send(apBuffer + lTotLength, aLength - lTotLength);
        if(lLength > 0)
        {
            lTotLength += lLength;
            if(lTotLength == aLength)
            {
               break;
            }
        }
        else
        {
			if(errno == EINTR)
			{
				continue;
			}
			else if(errno == EAGAIN)
			{
#ifdef WIN32
				Sleep(1);
#else
				usleep(1);
#endif
				continue;
			}
			else
			{
				break;
			}
        }
    }
    return lTotLength;
}

int Tcp::RecvFull(char *apBuffer, int aLength)
{
    int lTotLength = 0;
    int lLength = 0;
    while(true)
    {
        lLength = Recv(apBuffer + lTotLength, aLength - lTotLength);
        if(lLength > 0)
        {
            lTotLength += lLength;
            if(lTotLength == aLength)
            {
               break;
            }
        }
		else if(lLength < 0)
		{
			if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
			{
				continue;
			}
			else
			{
				int i = errno;
				break;
			}

		}
        else
        {
            break;
        }
    }
    return lTotLength;
}

