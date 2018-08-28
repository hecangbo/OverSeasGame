#pragma once

#include "EscapeCode.h"

class FileEncryptor
{
private:
	FileEncryptor(){};
	~FileEncryptor(){};

public:
	enum EncryptType
	{
		None,
		CEncrypt,
		XXTea,
	};

	static bool Encode(char const* apFile);

	static unsigned char* Decode(char const* apFile, int& aSize);

	static bool DecodeToFile(char const* apFile);


	static unsigned char* xxteaEncrypt(const unsigned char* apPlainBuffer, int size, int&inOutSize);
	////xxtea
	static unsigned char* Encrypt(unsigned char* apBuffer, int aSize, int& aOutSize);

	static bool EncryptFile(char const* apFile);

	static bool EncryptToFile(char const* apFile, unsigned char* apBuffer, int aSize);

	static unsigned char* DecryptFromFile(char const* apFile, int& aSize);

	static unsigned char* Decrypt(unsigned char* apBuffer, int aSize, int& aOutSize);

	static unsigned char* DecryptWithHeader(unsigned char* apBuffer, int aSize, int& aOutSize);

	static bool DecryptToFile(char const* apFile);

	static EncryptType GetEncryptType(unsigned char const* apBuffer, int aSize);

	static int GetHeaderLength();

private:
	static unsigned char* LoadFile(char const* apFile, int& aSize);

	static unsigned char* SkipHeader(unsigned char* apBuffer, int& aSize);

	static void WriteHeader(FILE* apFile, int aSize, EncryptType aType);

	static bool HasHeader(unsigned char const* apBuffer, int aSize);

	static int Stricmp(char const* str1, char const* str2)
	{
#ifdef WIN32
		return _stricmp(str1, str2);
#else
		return strcasecmp(str1, str2);
#endif
	}

};