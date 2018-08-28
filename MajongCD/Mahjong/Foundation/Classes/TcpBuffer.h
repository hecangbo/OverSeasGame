#ifndef _TCP_BUFFER_H
#define _TCP_BUFFER_H

struct TcpBuffer
{
	char* m_pBuffer;
	int m_Size;

	bool IsAvalibe(){return m_pBuffer != 0;}

	void Set(char* apBuffer, int aSize){m_pBuffer = apBuffer; m_Size = aSize;}

	void RetSet(){m_pBuffer = 0; m_Size = 0;}

	char* Detach(){char* lp = m_pBuffer; m_pBuffer = 0; return lp;}
};

#endif