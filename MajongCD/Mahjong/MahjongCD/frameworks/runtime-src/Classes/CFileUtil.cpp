#include "CFileUtil.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "cocos2d.h"
//#include "cocos-ext.h"

using namespace std;
vector<string>  CFileUtil::extrFilter;

bool		CFileUtil::isFileExist(const char* psFilePath)
{
	if( !psFilePath ) 
		return false;
#ifdef WIN32
	if (_access(psFilePath, 0) == 0)
	{
		return true;
	}
#else
	if(access (psFilePath, F_OK) == 0)
		return true;
	//else
	//	printErrorOnFileOperation();
#endif
	return false;
}
void     CFileUtil::addExtrFilter(string filter)
{
	extrFilter.push_back(filter);
}
bool		CFileUtil::makeDataIntoFile(const char *pDataBuf,  unsigned long dataLen,  const char *pSaveFileFullPath)
{
	FILE	*fp = fopen(pSaveFileFullPath, "wb+"); 
	if (!fp)
		return false;
	if (1 ==  fwrite(pDataBuf, dataLen, 1, fp))
	{
		//log("write %s success.", pSaveFileFullPath);
	}
	else
	{
		//log("write %s fail..", pSaveFileFullPath);
	}
	fclose(fp); 
	return true;
}
bool      CFileUtil::locateFileInZipFile(unzFile file)
{
	return true;
}
bool      CFileUtil::extractFilesWithoutDirFromZip(const char *szZipFilePath,const char *szUnZipFileSavePath, unsigned long * pSize, bool deleteZip)
{
	int    err;
	bool bResult = false;
	unzFile punzFile = NULL;
	unsigned char * pBuffer = NULL;
	*pSize = 0;

	//log("get File From Zip");
	//log("zipfile name = %s", szZipFilePath);

	if (!szZipFilePath || strlen(szZipFilePath) == 0)
		goto end;
	do 
	{
		if (!(punzFile = unzOpen64(szZipFilePath)))
		{
			log("open zipfile fail");
			goto end;
		}
		if (UNZ_OK != unzGoToFirstFile(punzFile))goto end;

		do 
		{
			char szFilePathA[260];
			unz_file_info64 FileInfo;
			char szCurrentFileName[UNZ_MAXFILENAMEINZIP+1];

			if (UNZ_OK != unzGetCurrentFileInfo64(punzFile, &FileInfo, szCurrentFileName, sizeof(szCurrentFileName) - 1, NULL, 0, NULL, 0))goto end;
			//if (UNZ_OK != unzGetCurrentFileInfo(punzFile, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0))	goto end;
			if (UNZ_OK != unzOpenCurrentFile(punzFile))goto end;

			pBuffer = new unsigned char[FileInfo.uncompressed_size];
			int nSize = 0;

			nSize = unzReadCurrentFile(punzFile, pBuffer, FileInfo.uncompressed_size);
			if (nSize == 0 || nSize != (int)FileInfo.uncompressed_size)
				goto end;
			(*pSize) += FileInfo.uncompressed_size;

			//log("unzip file name = %s", szCurrentFileName);

			if (!makedir(szUnZipFileSavePath))
				goto end ;

			string unzipFileSavaPath = szUnZipFileSavePath;
			unzipFileSavaPath += szCurrentFileName;

			FILE *pfile = fopen(unzipFileSavaPath.c_str(), "wb");
			if (!pfile)
			{
				log("create unzip file  %s fail ", unzipFileSavaPath.c_str());
				goto end;
			}
			fwrite(pBuffer, FileInfo.uncompressed_size, 1,  pfile);
			fclose(pfile);
			free(pBuffer);

			//log("get unzip file [%s] success. ", szCurrentFileName);

			unzCloseCurrentFile(punzFile);
			err = unzGoToNextFile(punzFile);

		} while (UNZ_END_OF_LIST_OF_FILE != err);
		//
	} while (0);

	bResult  = true;
end:
	if (punzFile)
		unzClose(punzFile);
	//É¾³ýÔ­ÓÐzip°ü
	if (deleteZip && bResult)
	{
		remove(szZipFilePath);
		//log("delete zipFile [%s] success. ", szZipFilePath);
	}
	return bResult;
}
bool		CFileUtil::unCompress(const char *zipfilename,   const char *unzipsavepath, int opt_extract_without_path, int opt_overwrite)
{
	uLong	 i;
	unz_global_info64	 gi;
	int		err;
	FILE	* fout=NULL;
	unzFile  uf = unzOpen64(zipfilename);
	if (!uf)
	{
		log("open zip file %s fail.\n", zipfilename);
		return false;
	}
	err = unzGetGlobalInfo64(uf, &gi);
	if (err != UNZ_OK)
	{
		log("error %d with zipfile in unzGetGlobalInfo \n", err);
		return false;
	}

	for (i=0;i<gi.number_entry;i++)
	{
		char outpath[256] = {0};
		strncpy(outpath, unzipsavepath, sizeof outpath);
		if (do_extract_currentfile(uf, outpath) != UNZ_OK)
		{
			log("extract  current  file fail.");
			return false;
		}
		if ((i+1)<gi.number_entry)
		{
			err = unzGoToNextFile(uf);
			if (err!=UNZ_OK)
			{
				log("error %d with zipfile in unzGoToNextFile\n", err);
				return false;
			}
		}
	}
	unzClose(uf);
	return true;
}
 int		CFileUtil::compareFileSize(const char*file1Path, const char* file2Path)
 {
	 if (isFileExist(file1Path) && !isFileExist(file2Path))
		 return 1;
	 if (!isFileExist(file1Path) && isFileExist(file2Path))
		 return -1;
	 if (isFileExist(file1Path) && isFileExist(file2Path))
	 {
		 if (getFileSize(file1Path) > getFileSize(file2Path))
			 return 1;
		 else if (getFileSize(file1Path) > getFileSize(file2Path))
			 return -1;
		 else
			 return 0;
	 }
	 else
		 return 0;
 }
int			CFileUtil::makedir (const char *newdir)
{
	char *buffer ;
	char *p;
	int  len = (int)strlen(newdir);

	if (len <= 0)
		return 0;

	buffer = (char*)malloc(len+1);
	if (buffer==NULL)
	{
		printf("Error allocating memory\n");
		return UNZ_INTERNALERROR;
	}
	strcpy(buffer,newdir);

	if (buffer[len-1] == '/') {
		buffer[len-1] = '\0';
	}
	if (mymkdir(buffer) == 0)
	{
		free(buffer);
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
			printf("couldn't create directory %s\n",buffer);
			free(buffer);
			return 0;
		}
		if (hold == 0)
			break;
		*p++ = hold;
	}
	free(buffer);
	return 1;
}
int			CFileUtil::mymkdir(const char* dirname)
{
	int ret=0;
#ifdef WIN32
	ret = _mkdir(dirname);
#else
	ret = mkdir (dirname,0775);
	if (ret == -1)
	{
		//printf("create dir : %s fail.",dirname);
		//printErrorOnFileOperation();
	}

#endif
	return ret;
}

int GetDir(char const* apPath, int& aSrcOff, char aDir[260], int& aDstOff)
{
	int lLen = 0;
	while(aSrcOff < strlen(apPath))
	{
		aDir[aDstOff] = apPath[aSrcOff];
		++lLen;
		++aDstOff;

		if(apPath[aSrcOff] == '/' || apPath[aSrcOff] == '\\')
		{
			++aSrcOff;
			break;
		}
		++aSrcOff;
	}
	return lLen;
}

bool IsExistDir(char const* apDir)
{
#ifdef WIN32
	return (_access(apDir, 0) == 0);
#else
	return (access(apDir, 0) == 0);
#endif
}

void CFileUtil::MakeDir(char const* apFile)
{
	char const* lpPath = strrchr(apFile, '/');
	if(0 != lpPath)
	{
		char temp[260] = {0};
		char lDir[260] = {0};
		strncpy(temp, apFile, lpPath - apFile);
		lpPath = temp;
		
		if(IsExistDir(temp))
		{
			return;
		}

		int lSrcOff = 0;
		int lDstOff = 0;
		while(true) 
		{
			if(GetDir(temp, lSrcOff, lDir, lDstOff) > 0)
			{
				if(IsExistDir(lDir))
				{
					continue;
				}
#ifdef WIN32
				_mkdir(lDir);
#else
				int ret = mkdir(lDir, 0770);
				log("Create dir %s:%s ", lDir, ret == 0 ? "success" : "faild");
#endif
			}
			else
			{
				break;
			}
		}
	}
}

int			CFileUtil::do_extract_currentfile( unzFile uf,  const char *fileoutpath,   int extract_without_path, int popt_overwrite)
{
	char		filename_inzip[256];
	char*	filename_withoutpath;
	char*	p;
	int			err=UNZ_OK;
	FILE		*fout=NULL;
	void		*buf;
	uInt		size_buf;

	unz_file_info64			file_info;
	uLong		ratio=0;
	err = unzGetCurrentFileInfo64(uf, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
	//if(/*strcasecmp(filename_inzip, "AndroidManifest.xml") == 0 || 
	//	strcasecmp(filename_inzip, "CopyrightDeclaration.xml") == 0 ||*/
	//	strstr(filename_inzip, "META-INF/") != NULL || 
	//	strstr(filename_inzip, "res/") != NULL ||
	//	/*strcasecmp(filename_inzip, "VERSION") == 0 ||
	//	strcasecmp(filename_inzip, "resources.arsc") == 0 ||
	//	strcasecmp(filename_inzip, "classes.dex") == 0 ||*/
	//	strstr(filename_inzip, "lib/") != NULL || 
	//	strstr(filename_inzip, "assets/") != NULL)
	//{
	//	log("Ignore zip file %s", filename_inzip);
	//	return UNZ_OK;
	//}

	if (err!=UNZ_OK)
	{
		log("error %d with zipfile in unzGetCurrentFileInfo\n",err);
		return err;
	}
	//
	//if (!isFileExist(fileoutpath))
	//	makedir(fileoutpath);
	//
	size_buf = WRITEBUFFERSIZE;
	buf = (void*)malloc(size_buf);
	if (buf==NULL)
	{
		log("Error allocating memory\n");
		return UNZ_INTERNALERROR;
	}

	p = filename_withoutpath = filename_inzip;
	while ((*p) != '\0')
	{
		if (((*p)=='/') || ((*p)=='\\'))
			filename_withoutpath = p+1;
		p++;
	}


	/*if ((*filename_withoutpath)=='\0')
	{
	char outpath[256] = {0};
	strcpy(outpath, fileoutpath);
	strcat(outpath, filename_inzip);
	if (extract_without_path ==0)
	{
	log("creating directory: %s\n",outpath);
	mymkdir(outpath);
	}
	}
	else*/
	{
		const char* write_filename;
		if (extract_without_path == 0)
			write_filename = filename_inzip;
		else
			write_filename = filename_withoutpath;
		err = unzOpenCurrentFilePassword(uf,NULL);
		if (err!=UNZ_OK)
			log("error %d with zipfile in unzOpenCurrentFilePassword\n",err);

		if ( err==UNZ_OK)
		{
			char outpath0[256] = {0};
			strcpy(outpath0, fileoutpath);
			strcat(outpath0, write_filename);

			CFileUtil::MakeDir(outpath0);
			fout = fopen(outpath0,"wb");
			//if (fout == NULL)
			//	printErrorOnFileOperation();
			
			/* some zipfile don't contain directory alone before file */
			if (fout == NULL && extract_without_path == 0 && filename_withoutpath != (char*)filename_inzip) 
			{
				
				char outpath[256] = {0};
				char c = *(filename_withoutpath-1);
				strncpy(outpath ,fileoutpath, sizeof outpath);
				*(filename_withoutpath-1) = '\0';
				strcat(outpath, write_filename);
				makedir(outpath);
				*(filename_withoutpath-1) = c;
				strcat(outpath, "\\");
				strcat(outpath, filename_withoutpath);
				int pos = string::npos;
				int start = 0;
				string path = outpath;
				while((pos =path.find('/', start))!= string::npos)
				{
					path[pos] = '\\';
					start = pos + 1;
					if (start >= path.length())
						break;
				}
				
				fout = fopen(outpath0/*path.c_str()*/,"wb");
				if (fout == NULL)
				{
					//log("error opening %s\n",outpath0/*write_filename*/);
					//printErrorOnFileOperation();
				}
			}
			
		}	
		if (fout!=NULL)
		{
			//log("extracting: %s\n",write_filename);
			do
			{
				err = unzReadCurrentFile(uf,buf,size_buf);
				if (err<0)
				{
					log("error %d with zipfile in unzReadCurrentFile\n",err);
					break;
				}
				if (err>0)
				{
					if (fwrite(buf,err,1,fout)!=1)
					{
						log("error in writing extracted file\n");
						err=UNZ_ERRNO;
						break;
					}
				}
			}while (err>0);
			if (fout)
				fclose(fout);
		}

		if (err==UNZ_OK)
		{
			err = unzCloseCurrentFile (uf);
			if (err!=UNZ_OK)
				log("error %d with zipfile in unzCloseCurrentFile\n",err);
		}
		else
			unzCloseCurrentFile(uf); /* don't lose the error */
	}
	free(buf);
	return err;
}
unsigned long	CFileUtil::getFileSize(const char* psFilePath)
{
	if (!isFileExist(psFilePath))
		return 0;
	unsigned long len;
	FILE *pf = fopen(psFilePath , "r");
	fseek(pf, 0, SEEK_SET);
	fseek(pf, 0, SEEK_END);
	len = ftell(pf);
	fseek(pf, 0, SEEK_SET); 
	fclose(pf);
	return len;
}
void		CFileUtil::printErrorOnFileOperation()
{
	log("errno=%d\n",errno); 
	char * mesg = strerror(errno);
	log("Mesg:%s\n",mesg); 
}
void		CFileUtil::replaceChar(char *str, char oldChar, char newChar)
{
	//int pos = string::npos;
	int start = 0;
	char *next = str;
	while((next = strchr(next ,oldChar)) != NULL)
	{
		*next = newChar;
		next = next + 1;
		if (next == str + strlen(str))
			break;
	}
}
void		CFileUtil::change_file_date(const char *filename, uLong dosdate, tm_unz tmu_date)
{
#ifdef WIN32
	HANDLE hFile;
	FILETIME ftm,ftLocal,ftCreate,ftLastAcc,ftLastWrite;
	hFile = CreateFileA(filename,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
	GetFileTime(hFile, &ftCreate, &ftLastAcc, &ftLastWrite);
	DosDateTimeToFileTime((WORD)(dosdate>>16),(WORD)dosdate,&ftLocal);
	LocalFileTimeToFileTime(&ftLocal,&ftm);
	SetFileTime(hFile,&ftm,&ftLastAcc,&ftm);
	CloseHandle(hFile);
#else
	struct utimbuf ut;
	struct tm newdate;
	newdate.tm_sec = tmu_date.tm_sec;
	newdate.tm_min=tmu_date.tm_min;
	newdate.tm_hour=tmu_date.tm_hour;
	newdate.tm_mday=tmu_date.tm_mday;
	newdate.tm_mon=tmu_date.tm_mon;
	if (tmu_date.tm_year > 1900)
		newdate.tm_year=tmu_date.tm_year - 1900;
	else
		newdate.tm_year=tmu_date.tm_year ;
	newdate.tm_isdst=-1;

	ut.actime=ut.modtime=mktime(&newdate);
	utime(filename,&ut);
#endif
}

std::string CFileUtil::ExtrFilePath(char const* apFile)
{
	char const* p = strrchr(apFile, '/');
	if (p != NULL)
	{
		std::string lPath;
		lPath.assign(apFile, p - apFile + 1);
		return lPath;
	}
	return apFile;
}
