#include "FileEncryptor.h"

#ifndef WIN32

#include "cocos2d.h"
using namespace cocos2d;

#endif

#include "xxtea/xxtea.h"

static const char s_Key[] = "9pqL<MNABCRSFGHIJKOPTUVWXYlm>Zabcdhijkno8rstuvwxyz01253<467+/=DEQ";
static char s_Name[]      = "Encryptor";

//xxtea
static unsigned char s_xxteaKey[] = "Yuo cnanto ckacr adn my tiypgn is rhitg!^_^";
static char s_xxteaName[] = "EncryptXX";

struct EncryptHeader
{
	char Name[10];
	int Size;
};


bool FileEncryptor::Encode(char const* apFile)
{
	int lSize = 0;
	unsigned char* lpBuffer = LoadFile(apFile, lSize);
	bool lRet = false;
	
	if (lSize > 0)
	{
		if (GetEncryptType(lpBuffer, lSize) != None)
		{
			lRet = true;
		}
		else
		{
			CEncrypt::instance().Encrypt(lpBuffer, lSize, (BYTE*)s_Key, sizeof(s_Key));
			FILE *lpFile = fopen(apFile, "w+b");
			if (NULL != lpFile)
			{
				WriteHeader(lpFile, lSize, FileEncryptor::CEncrypt);
				fwrite(lpBuffer, 1, lSize, lpFile);
				fclose(lpFile);
				lRet = true;
			}
		}
		free(lpBuffer);
	}
	return lRet;
}

void FileEncryptor::WriteHeader(FILE* apFile, int aSize, EncryptType aType)
{
	char* lName[] = { "", s_Name, s_xxteaName };

	switch (aType)
	{
	case FileEncryptor::CEncrypt:
	case FileEncryptor::XXTea:
		{
		EncryptHeader lHeader;
		strncpy(lHeader.Name, lName[aType], sizeof(lHeader.Name));
		lHeader.Size = aSize;
		fwrite((char*)&lHeader, sizeof(lHeader), 1, apFile);
		}
		break;
	default:
		break;
	}
}

unsigned char* FileEncryptor::Decode(char const* apFile, int& aSize)
{
	int lSize = 0;
	unsigned char* lpBuffer = LoadFile(apFile, lSize);
	if (lSize > 0)
	{
		if (GetEncryptType(lpBuffer, lSize) == FileEncryptor::CEncrypt)
		{
			lpBuffer = SkipHeader(lpBuffer, lSize);
			CEncrypt::instance().Decode(lpBuffer, lSize, (BYTE*)s_Key, sizeof(s_Key));
		}
		aSize = (int)lSize;
	}
	return lpBuffer;
}

bool FileEncryptor::DecodeToFile(char const* apFile)
{
	int lSize = 0;
	bool lRet = false;
	unsigned char* lpBuffer = Decode(apFile, lSize);
	if (lSize > 0)
	{
		FILE *lpFile = fopen(apFile, "w+b");
		if (NULL != lpFile)
		{
			fwrite(lpBuffer, 1, lSize, lpFile);
			fclose(lpFile);
			lRet = true;
		}
		free(lpBuffer);
	}
	return lRet;
}

unsigned char* FileEncryptor::LoadFile(char const* apFile, int& aSize)
{
	unsigned char* lpBuffer = NULL;

#ifndef WIN32
	ssize_t lSize = 0;
	std::string filePah = apFile;
	lpBuffer = CCFileUtils::sharedFileUtils()->getFileData(filePah, "rb", &lSize);
	aSize = lSize;
#else
	FILE *lpFile = fopen(apFile, "rb");
	if (NULL != lpFile)
	{
		int  lLen = 0;
		fseek(lpFile, 0, SEEK_END);
		lLen = ftell(lpFile);
		rewind(lpFile);

		lpBuffer = (unsigned char*)malloc(lLen);
		fread(lpBuffer, sizeof(unsigned char), lLen, lpFile);
		fclose(lpFile);
		aSize = lLen;
	}
#endif
	return lpBuffer;
}

FileEncryptor::EncryptType FileEncryptor::GetEncryptType(unsigned char const* apBuffer, int aSize)
{
	FileEncryptor::EncryptType lType = FileEncryptor::None;
	EncryptHeader const* lpHeader = (EncryptHeader const*)apBuffer;
	if ((aSize - sizeof(EncryptHeader) == lpHeader->Size))
	{
		if (Stricmp(lpHeader->Name, s_Name) == 0)
		{
			lType = FileEncryptor::CEncrypt;
		}
		else if (Stricmp(lpHeader->Name, s_xxteaName) == 0)
		{ 
			lType = FileEncryptor::XXTea;
		}
	}

	return lType;
}

bool FileEncryptor::HasHeader( unsigned char const* apBuffer, int aSize )
{
	EncryptHeader const* lpHeader = (EncryptHeader const*)apBuffer;
	if ((aSize - sizeof(EncryptHeader) == lpHeader->Size))
	{
		if (Stricmp(lpHeader->Name, s_Name) == 0 || Stricmp(lpHeader->Name, s_xxteaName))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

unsigned char* FileEncryptor::SkipHeader(unsigned char* apBuffer, int& aSize)
{
	unsigned char* lpBuffer = NULL;
	EncryptHeader const* lpHeader = (EncryptHeader const*)apBuffer;
	if (lpHeader->Size > 0)
	{
		lpBuffer = (unsigned char*)malloc(lpHeader->Size);
		memcpy(lpBuffer, apBuffer + sizeof(EncryptHeader), lpHeader->Size);
		aSize = lpHeader->Size;
	}
	else
	{
		aSize = 0;
	}
	free(apBuffer);
	return lpBuffer;
	
}

unsigned char* FileEncryptor::Encrypt(unsigned char* apBuffer, int aSize, int& aOutSize)
{
	unsigned char* lpData = NULL;
	if (aSize > 0 && apBuffer != NULL)
	{
		lpData = xxtea_encrypt(apBuffer, aSize, s_xxteaKey, strlen((char*)s_xxteaKey), (xxtea_long*)&aOutSize);
	}
	return lpData;
}

bool FileEncryptor::EncryptFile(char const* apFile)
{
	int lSize = 0;
	unsigned char* lpBuffer = LoadFile(apFile, lSize);
	bool lRet = false;

	if (lSize > 0)
	{
		if (GetEncryptType(lpBuffer, lSize) != FileEncryptor::None)
		{
			lRet = true;
		}
		else
		{
			/*unsigned char* lpRes = Encrypt(lpBuffer, lSize, lSize);
			FILE *lpFile = fopen(apFile, "w+b");
			if (NULL != lpFile)
			{
				WriteHeader(lpFile, lSize, FileEncryptor::XXTea);
				fwrite(lpRes, 1, lSize, lpFile);
				fclose(lpFile);
				lRet = true;
			}
			free(lpRes);*/
			lRet = EncryptToFile(apFile, lpBuffer, lSize);
		}
		free(lpBuffer);
	}
	return lRet;
}

bool FileEncryptor::EncryptToFile( char const* apFile, unsigned char* apBuffer, int aSize )
{
	bool lRet = false;
	int lOutSize = 0;
	unsigned char* lpRes = Encrypt(apBuffer, aSize, aSize);
	FILE *lpFile = fopen(apFile, "w+b");
	if (NULL != lpFile)
	{
		WriteHeader(lpFile, aSize, FileEncryptor::XXTea);
		fwrite(lpRes, 1, aSize, lpFile);
		fclose(lpFile);
		lRet = true;
	}
	free(lpRes);
	return lRet;
}

unsigned char* FileEncryptor::Decrypt(unsigned char* apBuffer, int aSize, int& aOutSize)
{
	unsigned char* lpData = NULL;
	if (aSize > 0 && apBuffer != NULL)
	{
		lpData = xxtea_decrypt(apBuffer, aSize, s_xxteaKey, strlen((char*)s_xxteaKey), (xxtea_long*)&aOutSize);
	}
	return lpData;
}

unsigned char* FileEncryptor::DecryptWithHeader(unsigned char* apBuffer, int aSize, int& aOutSize)
{
	unsigned char* lpData = NULL;
	if (aSize > 0 && apBuffer != NULL)
	{
		int len = GetHeaderLength();
		lpData = Decrypt(apBuffer + len, aSize - len, aOutSize);
	}
	return lpData;
}

unsigned char* FileEncryptor::DecryptFromFile(char const* apFile, int& aSize)
{
	int lSize = 0;
	
	unsigned char* lpBuffer = LoadFile(apFile, lSize);
	if (lSize > 0)
	{
		aSize = lSize;
		if (GetEncryptType(lpBuffer, lSize) == FileEncryptor::XXTea)
		{
			unsigned char* lpRes = DecryptWithHeader(lpBuffer , lSize, lSize);
			free(lpBuffer);
			aSize = lSize;
			return lpRes;
		}
	}
	return lpBuffer;
}

bool FileEncryptor::DecryptToFile(char const* apFile)
{
	int lSize = 0;
	bool lRet = false;
	unsigned char* lpBuffer = DecryptFromFile(apFile, lSize);
	if (lSize > 0 && lpBuffer != NULL)
	{
		FILE *lpFile = fopen(apFile, "w+b");
		if (NULL != lpFile)
		{
			fwrite(lpBuffer, 1, lSize, lpFile);
			fclose(lpFile);
			lRet = true;
		}
		free(lpBuffer);
	}
	return lRet;
}

int FileEncryptor::GetHeaderLength()
{
	return sizeof(EncryptHeader);
}

unsigned char* FileEncryptor::xxteaEncrypt(const unsigned char* apPlainBuffer, int size, int&inOutSize)
{
	bool lRet = false;
	unsigned char* lpRes = Encrypt((unsigned char*)apPlainBuffer, size, inOutSize);
	return lpRes;
}
