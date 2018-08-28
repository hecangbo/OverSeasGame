#ifndef __LUA_JAVA_H__
#define __LUA_JAVA_H__

#include <jni.h>

#ifdef __cplusplus
extern "C"
{
#endif


void luaJava_setJavaVM(JavaVM* apJavaVM);


#ifdef __cplusplus
};
#endif

#endif