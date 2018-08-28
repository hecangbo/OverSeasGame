//2010-08-04 ��ӡ���2���ļ��У�ר�����ڷ��ü���/���ܵ���ͺ���
//��Ҫ��ԭ��������ȫ�������ڵ�������ȥʵ�֣�����̫���ˡ�

#ifndef _ESCAPE_CODE_H_
#define _ESCAPE_CODE_H_ 
//#include "include/cmdid.h"

//------------------------------------
//CCharCode
//����Ǵӷ���˿�������
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