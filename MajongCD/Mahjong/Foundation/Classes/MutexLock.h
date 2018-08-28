#ifndef _MUTEX_LOCK_H_
#define  _MUTEX_LOCK_H_

#include "pthread.h"

class MutexLock
{
public:
	MutexLock();
	~MutexLock();

	void Lock();

	void Unlock();

public:
	class AutoLock
	{
	private:
		MutexLock & sp_;

		AutoLock( MutexLock const & );
		AutoLock & operator=( MutexLock const & );
	public:
		explicit AutoLock( MutexLock & sp ): sp_( sp )
		{
			sp.Lock();
		}

		~AutoLock()
		{
			sp_.Unlock();
		}
	};

private:
	pthread_mutex_t m_Lock;
};


#endif
