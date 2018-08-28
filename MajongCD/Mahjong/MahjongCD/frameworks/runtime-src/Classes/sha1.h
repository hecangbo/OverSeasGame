#ifndef _SHA1_H_
#define _SHA1_H_

//#include "cstdint.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef _SHA_enum_
#define _SHA_enum_
enum
{
	shaParamError = -1,/*��������*/
    shaSuccess = 0,
    shaNull,                   /* ��ָʾ���� */
    shaInputTooLong,    /* ��������̫����ʾ */
    shaStateError          /* called Input after Result --������������֮ */
};
#endif
#define SHA1HashSize 20

/*
 *  �������ֽṹ���������������Ϣ for the SHA-1
 *  hashing operation
 */
typedef struct SHA1Context
{
    unsigned  int Intermediate_Hash[SHA1HashSize/4]; /* Message Digest  */

    unsigned  int Length_Low;            /* Message length in bits      */
    unsigned  int Length_High;           /* Message length in bits      */

                               /* Index into message block array   */
    unsigned   short  Message_Block_Index;
    unsigned   char Message_Block[64];      /* 512-bit message blocks      */

    int Computed;               /* Is the digest computed?         */
    int Corrupted;             /* Is the message digest corrupted? */
} SHA1Context;

/*
 *  ����ԭ��
 */
int SHA1Reset(  SHA1Context *);
int SHA1Input(  SHA1Context *, const unsigned   char *,unsigned int);
int SHA1Result( SHA1Context *, unsigned   char Message_Digest[SHA1HashSize]);
int GetFileSHA1(FILE *pf,  unsigned   char (&hash_code)[SHA1HashSize]);
#endif /*_SHA1_H_*/
