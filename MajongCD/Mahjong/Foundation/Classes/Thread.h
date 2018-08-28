#ifndef THREAD_H
#define THREAD_H

#include "pthread.h"

class Thread
{
public:
    Thread();
    virtual ~Thread();

    void Run();

    virtual void Stop();

	bool IsValid();

	void InValid();

protected:
    virtual void RunTask() = 0;

private:
    static void* Run(void* apParam);

protected:
	bool m_Stop;

private:
    pthread_t m_ThreadID;
};

#endif // THREAD_H
