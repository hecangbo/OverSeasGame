#include "Thread.h"

Thread::Thread()
	: m_Stop(true)
{
#ifdef WIN32
	m_ThreadID.p = 0;
	m_ThreadID.x = 0;
#else
	m_ThreadID = 0;
#endif
}

Thread::~Thread()
{
}

void Thread::Run()
{
	m_Stop = false;
    pthread_create(&m_ThreadID, 0, &Thread::Run, (void*)this);
}

void Thread::Stop()
{
	m_Stop = true;
    pthread_join(m_ThreadID, 0);
#ifdef WIN32
	m_ThreadID.p = 0;
	m_ThreadID.x = 0;
#else
	m_ThreadID = 0;
#endif

}

void* Thread::Run(void *apParam)
{
    Thread* lpThread = (Thread*)apParam;
    if(0 != lpThread)
    {
        lpThread->RunTask();
    }
    return 0;
}

bool Thread::IsValid()
{
#ifdef WIN32
	return m_ThreadID.p != 0;
#else
	return m_ThreadID != 0;
#endif
}

void Thread::InValid()
{
#ifdef WIN32
	m_ThreadID.p = 0;
#else
	m_ThreadID = 0;
#endif
}
