#include "EscapeCode.h"
#include <sstream>

#ifndef WIN32
#include <stdlib.h>
#endif
//------------------------------------
//Simo_ShortToBit
//这个是从服务端拷过来的
//------------------------------------

class Simo_ShortToBit
{
public:
	DWORD low;
	Simo_ShortToBit(){Init();}
	Simo_ShortToBit(int b){ low=b; }
	void Init() {low=0;}
	int GetBit(int pos,int count){
		int temp=0;
		for(int i=0;i<count;i++)
			temp=temp | (1<<i);
		return (low>>pos) & temp;
	}
	bool GetBit(int pos){
		return (low>>pos)&0x01;
	}
	void SetBit(int pos,bool IsSet){
		if(IsSet)
			low=low | (1<<pos);
		else
			low=low & ~(1<<pos);
	}
	void SetBit(int pos,int count,int result){
		for(int i=0;i<count;i++)
			SetBit(pos+i,(result>>i)&0x01);
	}
	Simo_ShortToBit& operator = (Simo_ShortToBit &x){
		low=x.low;
	}
	int operator = (int x){
		low=x;
	}
	bool operator == (Simo_ShortToBit &x){
		return low==x.low;
	}
	bool operator != (Simo_ShortToBit &x){
		return !(*this==x);
	}
	void TurnFromString(string &s)
	{
		int length = s.size();
		if(length==0){
			Init();
			return;
		}
		if(length<=16){
			low=atoi(s.c_str());
			return;
		}
	}
	string TurnToString(){
		string s;
		stringstream ss;
		ss<<low;
		return ss.str();
	}
};


//------------------------------------
//CCharCode
//------------------------------------

CCharCode& CCharCode::instance()
{
	static CCharCode _instance;
	return _instance;
}

string CCharCode::CharToBase64(const char *sText)
{
	string s;
	int iLen=strlen(sText);
	int iStep=iLen/3;
	int iRest=iLen%3;

	for(int i=0;i<iStep;i++)
		s+=CCharCode::TransferChar(sText+i*3,3,BASE64CHAR);

	if(iRest>0)
		s+=CCharCode::TransferChar(sText+(iLen-iRest),iRest,BASE64CHAR);

	return s;
}

string CCharCode::TransferChar(const char *sText,int iLen,const char *sCode)
{
	string s;
	Simo_ShortToBit bit;
	int i;

	for(i=0;i<iLen;i++)
		bit.SetBit(24-(i+1)*8,8,sText[i]);

	for(i=0;i<4;i++){
		if(i>iLen)
			s+='=';
		else
			s+=CCharCode::CodeToChar(bit.GetBit(24-(i+1)*6,6),sCode);
	}

	return s;
}

char CCharCode::CodeToChar(int iCode,const char *sCode)
{	
	return sCode[iCode];
}


string CCharCode::Base64ToChar(const char *sText)
{
	string s;
	int iLen=strlen(sText);
	int iStep=iLen/4;//一定是4的倍数

	for(int i=0;i<iStep;i++){
		s+=UntransferChar(sText+i*4,4,BASE64CHAR);
	}

	return s;
}

string CCharCode::UntransferChar(const char *sText,int iLen,const char *sCode)
{
	string s;
	Simo_ShortToBit bit;
	int iChars=0;
	int i;

	for(i=0;i<iLen;i++){
		char c=sText[i];
		if(c!='='){
			iChars++;
			bit.SetBit(24-(i+1)*6,6,CharToCode(c,sCode));
		}
	}

	iChars--;
	for(i=0;i<iChars;i++){
		s+=(char)bit.GetBit(24-(i+1)*8,8);
	}

	return s;
}

int CCharCode::CharToCode(int iCode,const char *sCode)
{
	int len=strlen(sCode);
	for(int i=0;i<len;i++){
		if(iCode==sCode[i])
			return i;
	}

	return 0;
}

//------------------------------------
//CEncrypt
//------------------------------------
#define NUM_CONFUSED_BYTE 62

static BYTE	g_confusion[NUM_CONFUSED_BYTE] =	
{
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
	'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 
	'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', 'U', 'V', 'W', 'X', 'Y', 'Z', 
};

CEncrypt& CEncrypt::instance()
{
	static CEncrypt _cencrypt;
	return _cencrypt;
}

//举个例子，字符'a'需要根据BYTE *key加密。
// 1) 首先去g_confusion查找有木有字符'a',有的话记住它的索引值，这里是0。没有的话就保持原文不加密。
// 2) 根据BYTE *key产生一个int值，可以是每个BYTE的和与g_confusion的长度的余。
// 3) 返回g_confusion里索引是这个int的字符作为密文。
//
//解密的过程以此类似，反过来就可以了。
void CEncrypt::Encrypt(BYTE *io, WORD ioLen, BYTE *key, WORD keyLen)
{
	for(int ii = 0; ii < ioLen; ii++)
	{
		io[ii] = io[ii] ^ key[ii % keyLen];
	}
}

void CEncrypt::Decode(BYTE *io, WORD ioLen, BYTE *key, WORD keyLen)
{
	for(int ii = 0; ii < ioLen; ii++)
	{
		io[ii] = io[ii] ^ key[ii % keyLen];
	}
}

int CEncrypt::Find(BYTE key)
{
	//TODO:根据字符值范围查找更快点。。。

	for (int ii = 0; ii < NUM_CONFUSED_BYTE; ii++)
	{
		if (key == g_confusion[ii])
		{
			return ii;
		}
	}

	return -1;
}

int CEncrypt::Generate(BYTE *key, WORD keyLen)
{
	int num_confuse = 0;
	for (int ii = 0; ii < keyLen; ii++)
	{
		num_confuse += (int)(key[ii]);
	}

	return num_confuse % NUM_CONFUSED_BYTE;
}

