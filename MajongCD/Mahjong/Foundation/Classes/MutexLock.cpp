#include "MutexLock.h"
#include <pthread.h>

MutexLock::MutexLock(void)
//	: m_Lock(PTHREAD_RECURSIVE_MUTEX_INITIALIZER)
{
	/*pthread_mutex_init(&m_Lock, 0);*/
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_Lock, &attr);
}

MutexLock::~MutexLock(void)
{
	pthread_mutex_destroy(&m_Lock);
}

void MutexLock::Lock()
{
	pthread_mutex_lock(&m_Lock); 
}

void MutexLock::Unlock()
{
	pthread_mutex_unlock(&m_Lock); 
}
