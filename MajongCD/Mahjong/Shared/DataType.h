#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

#ifndef WIN32
typedef int __int32;
typedef long long __int64;
typedef bool BOOL;
#define FALSE false
#define TRUE true
#endif

typedef short               SHORT;
typedef signed short        INT16;
typedef unsigned short      USHORT;
typedef unsigned short      UINT16;
typedef int                 INT;
typedef signed int          INT32;
typedef unsigned int        UINT;
typedef unsigned int        UINT32;
typedef unsigned long       ULONG;

#ifdef WIN32
typedef int					BOOL;
#endif

#ifdef FALSE
#undef FALSE
#endif
#define FALSE				0

#ifdef TRUE
#undef TRUE
#endif
#define TRUE				1

typedef const void * LPCVOID;

#if WIN32
typedef unsigned long DWORD;
#else
typedef unsigned int DWORD;
#endif
typedef unsigned char BYTE;
typedef unsigned char byte;
typedef char CHAR;
//typedef long LONG;
//typedef int LONG;
typedef unsigned short      WORD;
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;

#endif