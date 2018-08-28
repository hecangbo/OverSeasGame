#include <string.h>
#include "ClientRequestDataPackage.h"
unsigned long tagPackHeader::EnCrypt(BYTE * InOutText,WORD wLen,BYTE & bFlag,BYTE * bRandKey)
{
	if(InOutText== NULL||wLen<1||wLen>= MAX_S_BUFF_LEN) return  0;
	BYTE bSkey[33] = {"B8AFA801FB5A234F79DA67804FD371FB"};
	int nKeyLen = 0;
	if(bRandKey!=NULL)
	{
		nKeyLen = strlen((char *)bRandKey)%32;
		for(int i = 0 ;i<nKeyLen;i++) bSkey[i] +=bRandKey[i];
	}
	//执行密钥变更
	//this->bRandFlag= ::GetTickCount()%256;
	bFlag = this->bRandFlag;
	for(int i = 0 ;i<32;i++)bSkey[i] += this->bRandFlag;

	//第一步 输入数据进行映射
	BYTE bRoundFrom = bSkey[0] +bSkey[10] +bSkey[29]+ iSbox[wLen%256]+this->bRandFlag;
	byte bTemp = 0;
	byte bTemp2 = 0;
	byte t;
	ulCRCCode  = 0x00000000; //初值为0
	for(int i =0; i < wLen; i++)
	{
		bRoundFrom+=i;
		bTemp = InOutText[i];

		t= (ulCRCCode >> 24) & 0xFF; //要移出的字节的值 	
		ulCRCCode= (ulCRCCode << 8) | bTemp; //将新移进来的字节值添在寄存器末字节中 
		ulCRCCode=ulCRCCode^CRC_32_tab[t]; //将寄存器与查出的值进行xor运算 


		InOutText[i] += iSbox[i%256]+bSkey[i%32];
		InOutText[i] ^= ByteMap[ByteMap2[bRoundFrom]];
		InOutText[i] += iSbox[bRoundFrom]+iSbox[bTemp2];
		bTemp2 = bTemp;
	}
	//第二步 用密钥和数据长度组合进行换位加密
	byte bISkey = 0;
	int nOffx = 0;
	for(int i = 0;i<wLen;i++)
	{
		bTemp =  InOutText[i];
		bISkey = bSkey[i%32] +ByteMap2[bRoundFrom];
		nOffx = (i +bISkey)%wLen;
		InOutText[i] = InOutText[nOffx];
		InOutText[nOffx] = bTemp;
	}
	return ulCRCCode;
}
bool tagPackHeader::DecCrypt(BYTE * InOutText,WORD wLen,BYTE * bRandKey )
{
	if(InOutText==NULL||wLen<1||wLen>=MAX_S_BUFF_LEN) return  false;
	BYTE bSkey[33] = {"B8AFA801FB5A234F79DA67804FD371FB"};
	int nKeyLen = 0;
	if(bRandKey!=NULL)
	{
		nKeyLen = strlen((char *)bRandKey)%32;
		for(int i = 0 ;i<nKeyLen;i++) bSkey[i] +=bRandKey[i];
	}
	//执行密钥变更
	for(int i = 0 ;i<32;i++)bSkey[i] += this->bRandFlag;

	//第一步  用密钥和数据长度组合进行换位解密
	BYTE bRoundFrom = bSkey[0] +bSkey[10] +bSkey[29]+iSbox[wLen%256]+this->bRandFlag;
	for(int i =0; i < wLen; i++)
		bRoundFrom+=i;

	byte bTemp = 0;
	byte bISkey = 0;
	int nOffx = 0;
	for(int i = wLen-1;i>-1;i--)
	{
		bTemp =  InOutText[i];
		bISkey = bSkey[i%32] +ByteMap2[bRoundFrom];
		nOffx = (i +bISkey)%wLen;
		InOutText[i] = InOutText[nOffx];
		InOutText[nOffx] = bTemp;
	}

	//第二步 输入数据进行映射逆转换
	bRoundFrom = bSkey[0] +bSkey[10] +bSkey[29]+iSbox[wLen%256]+this->bRandFlag;
	bTemp = 0;
	byte t;
	unsigned long ulCRCC32  = 0x00000000; //初值为0
	for(int i= 0; i < wLen; i++)
	{
		bRoundFrom +=i;
		InOutText[i] -= iSbox[bRoundFrom]+iSbox[bTemp];
		InOutText[i] ^= ByteMap[ByteMap2[bRoundFrom]];
		InOutText[i] -= iSbox[i%256]+bSkey[i%32];
		bTemp = InOutText[i];

		t= (ulCRCC32 >> 24) & 0xFF; //要移出的字节的值 	
		ulCRCC32= (ulCRCC32 << 8) | bTemp; //将新移进来的字节值添在寄存器末字节中 
		ulCRCC32=ulCRCC32^CRC_32_tab[t]; //将寄存器与查出的值进行xor运算 
	}
	return ulCRCC32==ulCRCCode;
}
unsigned long tagPackHeader::GenerateSendPack(char  * szDataBuf,WORD &wLen,BYTE *bSkey,WORD wPType )
{
	WORD wHeadLen = sizeof(tagPackHeader);
	if(szDataBuf==NULL||wLen<=wHeadLen||MAX_S_BUFF_LEN<wHeadLen+wLen) return 0;
	tagPackHeader *pHeader = (tagPackHeader *)szDataBuf;
	pHeader->wDataLength =wLen-wHeadLen;
	pHeader->wPackType = wPType;
	pHeader->ulCRCCode = pHeader->EnCrypt((BYTE *)(szDataBuf + wHeadLen),pHeader->wDataLength,pHeader->bRandFlag,bSkey);
	wLen = pHeader->wDataLength+wHeadLen;
	return pHeader->ulCRCCode;
}
bool tagPackHeader::CheckCRC32(char  * szDataBuf,WORD &wLen,WORD &wType,BYTE *bSkey)
{
	WORD wHeadLen = sizeof(tagPackHeader);
	if(szDataBuf==NULL||wLen<=wHeadLen||MAX_S_BUFF_LEN<wLen) return 0;
	tagPackHeader *pHeader = (tagPackHeader *)szDataBuf;
	wType =pHeader->wPackType;
	if(pHeader->DecCrypt((BYTE *)(szDataBuf + wHeadLen),pHeader->wDataLength,bSkey))
	{
		wLen = pHeader->wDataLength;
		return true;
	}
	return false;
}
