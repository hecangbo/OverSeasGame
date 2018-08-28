#ifndef _CFILE_UTIL_H
#define _CFILE_UTIL_H

#include <ctype.h>
#include "unzip/ioapi.h"
#include "unzip/unzip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
//#include "CCPlatformDefine.h"
//#include "CCPlatformConfig.h"
#include <vector>
#include <string>

#ifdef WIN32
#include <io.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>
#endif

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

#ifdef _WIN32
#define USEWIN32IOAPI
//#include <iowin32.h>
#endif

#ifndef     UNZ_MAXFILENAMEINZIP
#define    UNZ_MAXFILENAMEINZIP (256)
#endif
using namespace  std;
using namespace cocos2d;
class CFileUtil
{
public:
	CFileUtil(){};
private:
	CFileUtil(const CFileUtil &other);
	CFileUtil & operator = (const CFileUtil& other);
public:
	static bool     m_bInterruptOpt;
	static vector<string>   extrFilter;
	static int		compareFileSize(const char*file1, const char* file2);
	static unsigned long	getFileSize(const char* pFileName);   
	static bool     locateFileInZipFile(unzFile file);
	static bool		isFileExist(const char* pFileName);
	static bool		makeDataIntoFile(const char *pDataBuf,  unsigned long dataLen,  const char *pSaveFileFullPath);
	static bool		extractFilesWithoutDirFromZip(const char *psZipFile, const char *psunZipFileSavePath, unsigned long * pSize, bool=true);
	static bool		unCompress(const char *zipfilename,   const char *unzipsavepath, int opt_extract_without_path=0, int opt_overwrite=0);
	static  int      makedir (const char *newdir);
	static  int      mymkdir(const char* dirname);
	static  int      do_extract_currentfile( unzFile uf,  const char *fileoutpath,   int extract_without_path = 0, int popt_overwrite=0);
	static void     change_file_date(const char *filename, uLong dosdate, tm_unz tmu_date);
	static void     replaceChar(char *str, char oldChar, char newChar);
	static void     addExtrFilter(string filter);
	static void     printErrorOnFileOperation();
	static void MakeDir(char const* apFile);

	static std::string ExtrFilePath(char const* apFile);

};
#endif