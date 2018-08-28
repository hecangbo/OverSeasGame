#pragma once

#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h> 
#include <errno.h>
#include <string.h>
#endif

#include "cocos2d.h"
#include "FileEncryptor.h"
#include "tinyxml2/tinyxml2.h"

struct Utility
{
	static int MakeDir(char const* lpDir, int len)
	{
		char *buffer ;
		char *p;

		if (len <= 0)
			return 0;

		buffer = new char[len+1];
		memset(buffer, 0, len + 1);

		strncpy(buffer,lpDir, len);

		if (buffer[len-1] == '/') {
			buffer[len-1] = '\0';
		}
		if (mymkdir(buffer) == 0)
		{
			delete[] buffer;
			return 1;
		}

		p = buffer+1;
		while (1)
		{
			char hold;

			while(*p && *p != '\\' && *p != '/')
				p++;
			hold = *p;
			*p = 0;
			if ((mymkdir(buffer) == -1) && (errno == ENOENT))
			{
				delete[] buffer;
				return 0;
			}
			if (hold == 0)
				break;
			*p++ = hold;
		}
		delete[] buffer;
		return 1;
	}

	static int mymkdir(const char* dirname)
	{
		int ret=0;
#ifdef WIN32
		ret = _mkdir(dirname);
#else
		ret = mkdir (dirname,0775);
#endif
		return ret;
	}

	static int Stricmp(char const* str1, char const* str2)
	{
#ifdef WIN32
		return stricmp(str1, str2);
#else
		return strcasecmp(str1, str2);
#endif
	}

	static bool GetXMLFileData(char const* asFileName, tinyxml2::XMLDocument* apDoc)
	{
		unsigned long bufferSize = 0;
		unsigned char* lpData = NULL;
		int lSize = 0;
		lpData = FileEncryptor::DecryptFromFile(asFileName, lSize);
		/*cocos2d::log("%s\n%s", asFileName, lpData);*/
		bufferSize = lSize;
		
		/*{
			lpData = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(asFileName, "r", &bufferSize);
		}*/
		apDoc->Parse((char*)lpData, bufferSize);
		delete[] lpData;
		return true;
	}

};