#pragma once

#include <string>

#pragma pack(push)
#pragma pack(1)

struct CustomPackHeader
{
	unsigned short wDataLength;			//数据长度
};
struct ReqVersion
{
	int nGameID;
	int lVersion;
	int lUpdateFlag;
};

struct ReqFullVersion
{
	int nError;
	int nGameID;
	int lVersion;
	int lPlatform;
	int lUpdateFlag;
	int lBigVersion;
	int lVersionSize;
	int lReserved1;
};

#pragma pack(pop)

class UpdateHelper
{
public:
	static bool ParseVersionData(char *inData, unsigned long inLen, ReqVersion& aVersion);

	static int fetchNumber(ReqVersion const& version);

	static std::string getZipFileNameWithVersion(const char *format, int aVersion);

	static	unsigned char toHex(const unsigned char &x);

	static	unsigned char fromHex(const unsigned char &x);

	static std::string makeURL(std::string decUrl);

	static std::string URLEncode(const char *sIn);

	static std::string URLDecode(const char *sIn);

	static bool reverseStringWithEachTwoChars(const char *org, char *out, unsigned int orgStrLen);

	static long  hexStrToDecValue(const char * hexStr, unsigned int len);

	static void replaceChar(std::string &str, char oldChar, char newChar);

	static unsigned long power(unsigned long value, int exp);
};

