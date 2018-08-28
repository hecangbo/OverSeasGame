#ifndef TCP_H
#define TCP_H

#ifdef WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#endif

#include <errno.h>

class Tcp
{
public:
    Tcp();

    ~Tcp();

	enum
	{
		SHUT_RECEIVE,
		SHUT_SEND,
		SHUT_BOTH
	};

	bool SetBlocking();

    bool SetNonblocking();

    int Connect(char* apAddr, int aPort);

    void Close();

    int Send(char* apBuffer, int aLength);

    int Recv(char* apBuffer, int aLength);

    int SendFull(char* apBuffer, int aLength);

    int RecvFull(char *apBuffer, int aLength);

    int GetSocket(){return m_Socket;}

    bool Create();

private:
    int m_Socket;
};

#endif // TCP_H
