#include "UpdateHelper.h"
#include "ClientRequestDataPackage.h"
#include <memory>
#include <string>
#include <stdlib.h>

bool UpdateHelper::ParseVersionData(char *inData, unsigned long inLen, ReqVersion& aVersion)
{

	tagPackHeader *pHead = (tagPackHeader *)inData;
	unsigned short	wDataLen = pHead->wDataLength + sizeof(tagPackHeader);

	//CRC校验，如果校验CRC校验码不一致，断开这个连接
	if (pHead->CheckCRC32(inData, wDataLen, pHead->wPackType, 0) == 0)
	{
		//收到数据的校验码不一致直接退出
		return false;
	}
	if (wDataLen >= sizeof(ReqVersion))
	{
		aVersion = *(ReqVersion*)(inData + sizeof(tagPackHeader));
		return true;
	}
	return true;
}

int	UpdateHelper::fetchNumber(ReqVersion const& version)
{
	char  versionChars[10];
	memset(versionChars, 0, sizeof versionChars);
	sprintf(versionChars, "%d", version.lVersion);
	std::string	versionStr = versionChars;
	std::string	fetchVersion(versionStr.end() - 2, versionStr.end());
	return atoi(fetchVersion.data());
}

std::string UpdateHelper::getZipFileNameWithVersion(const char *format, int aVersion)
{
	char zipFileName[512];
	sprintf(zipFileName, format, aVersion);
	return zipFileName;
}

unsigned char UpdateHelper::toHex(const unsigned char &x)
{
	return x > 9 ? x - 10 + 'A' : x + '0';
}

unsigned char UpdateHelper::fromHex(const unsigned char &x)
{
	return isdigit(x) ? x - '0' : x - 'A' + 10;
}

std::string UpdateHelper::makeURL(std::string decUrl)
{
	return URLEncode(decUrl.c_str());
}

std::string UpdateHelper::URLEncode(const char *sIn)
{
	std::string sOut;
	//string head = "http://";

	for (size_t ix = 0; ix < strlen(sIn); ix++)
	{
		unsigned char buf[4];
		memset(buf, 0, 4);
		if (isascii((unsigned char)sIn[ix]))
		{
			if (sIn[ix] == ' ')
				strncpy((char*)buf, "%20", sizeof buf);
			else
				buf[0] = sIn[ix];
		}
		else
		{
			buf[0] = '%';
			buf[1] = toHex((unsigned char)sIn[ix] >> 4);
			buf[2] = toHex((unsigned char)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	//head += sOut;
	std::string decsr;
	decsr = URLDecode(sOut.c_str());
	return sOut;
}

std::string UpdateHelper::URLDecode(const char *sIn)
{
	std::string sOut;
	for (size_t ix = 0; ix < strlen(sIn); ix++)
	{
		unsigned char ch = 0;
		if (sIn[ix] == '%')
		{
			ch = (fromHex(sIn[ix + 1]) << 4);
			ch |= fromHex(sIn[ix + 2]);
			ix += 2;
		}
		else if (sIn[ix] == '+')
		{
			ch = ' ';
		}
		else
		{
			ch = sIn[ix];
		}
		sOut += (char)ch;
	}
	return sOut;
}

bool UpdateHelper::reverseStringWithEachTwoChars(const char *org, char *out, unsigned int orgStrLen)
{
	if (org == 0 || out == 0 || orgStrLen % 2 != 0)
		return false;
	int off = orgStrLen - 2;
	int finishCnt = 0, start = 0;
	while (off >= 0)
	{
		for (int index = off; off < orgStrLen - finishCnt; index++)
		{
			*(out + start) = *(org + off);
			off++;
			start++;
		}
		finishCnt += 2;
		off = orgStrLen - finishCnt - 2;
	}
	return 1;
}

long UpdateHelper::hexStrToDecValue(const char * hexStr, unsigned int len)
{
	long  result = 0;
	for (int n = len - 1; n > 0; n--)
	{
		if (hexStr[n] >= 'A' && hexStr[n] <= 'Z')
			result += (10 + hexStr[n] - 'A')*power(16, len - 1 - n);
		else if (hexStr[n] >= 'a'&& hexStr[n] <= 'z')
			result += (10 + hexStr[n] - 'a')*power(16, len - 1 - n);
		else if (hexStr[n] >= '0' && hexStr[n] <= '9')
			result += (hexStr[n] - '0')*power(16, len - 1 - n);
	}
	return result;
}

void UpdateHelper::replaceChar(std::string &str, char oldChar, char newChar)
{
	int pos = std::string::npos;
	int start = 0;
	while ((pos = str.find(oldChar, start)) != std::string::npos)
	{
		str[pos] = newChar;
		start = pos + 1;
		if (start >= str.length())
			break;
	}
}

unsigned long UpdateHelper::power(unsigned long value, int exp)
{
	if (exp == 0)
		return 1;
	return power(value, exp - 1)*value;
}
