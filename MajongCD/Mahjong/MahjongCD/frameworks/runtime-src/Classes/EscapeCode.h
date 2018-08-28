//2010-08-04 添加。这2个文件中，专门用于放置加密/解密的类和函数
//不要像原来那样，全部都放在调用类中去实现，那样太乱了。

#ifndef _ESCAPE_CODE_H_
#define _ESCAPE_CODE_H_ 
//#include "include/cmdid.h"

//------------------------------------
//CCharCode
//这个是从服务端拷过来的
//------------------------------------


#include "DataType.h"
#include <string>
using namespace std;

const char BASE64CHAR[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

class CCharCode
{
public:
	static CCharCode& instance();
public:
	string CharToBase64(const char *sText);
	string Base64ToChar(const char *sText);
	
	string TransferChar(const char *sText,int iLen,const char *sCode);
	char CodeToChar(int iCode,const char *sCode);
	
	string UntransferChar(const char *sText,int iLen,const char *sCode);
	int CharToCode(int iCode,const char *sCode);
};

//------------------------------------
//CEncrypt
//------------------------------------
class CEncrypt 
{
public:
	static CEncrypt& instance();

public:
	void Encrypt(BYTE *io, WORD ioLen, BYTE *key, WORD keyLen);
	void Decode(BYTE *io, WORD ioLen, BYTE *key, WORD keyLen);

private:
	int Find(BYTE key);
	int Generate(BYTE *key, WORD keyLen);
};

#endif