#include <compiler.h>
#include <gwBase/Thread.h>

#ifdef _WIN32

/////////////////////////////////////////////////////////////////////////////
/// win32 threads
/////////////////////////////////////////////////////////////////////////////
#include <list>
#include <windows.h>

struct GWThread::PrivateData
{
	PrivateData() : _handle(INVALID_HANDLE_VALUE)
	{
	}
	~PrivateData()
	{
		if (_handle == INVALID_HANDLE_VALUE)
			return;
		CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;
	}

	static DWORD WINAPI start_routine(LPVOID data)
	{
		GWThread* thread = reinterpret_cast<GWThread*>(data);
		thread->run();
		return 0;
	}

	bool start(GWThread& thread)
	{
		if (_handle != INVALID_HANDLE_VALUE)
			return false;
		_handle = CreateThread(0, 0, start_routine, &thread, 0, 0);
		if (_handle == INVALID_HANDLE_VALUE)
			return false;
		return true;
	}

	void join()
	{
		if (_handle == INVALID_HANDLE_VALUE)
			return;
		DWORD ret = WaitForSingleObject(_handle, INFINITE);
		if (ret != WAIT_OBJECT_0)
			return;
		CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;
	}

	HANDLE _handle;
};

long GWThread::current(void) {
	return (long)GetCurrentThreadId();
}

struct GWMutex::PrivateData 
{
	PrivateData()
	{
		InitializeCriticalSection((LPCRITICAL_SECTION)&_criticalSection);
	}

	~PrivateData()
	{
		DeleteCriticalSection((LPCRITICAL_SECTION)&_criticalSection);
	}

	void lock(void)
	{
		EnterCriticalSection((LPCRITICAL_SECTION)&_criticalSection);
	}

	void unlock(void)
	{
		LeaveCriticalSection((LPCRITICAL_SECTION)&_criticalSection);
	}

	CRITICAL_SECTION _criticalSection;
};

struct GWWaitCondition::PrivateData 
{
	~PrivateData(void)
	{
		// The waiters list should be empty anyway
		_mutex.lock();
		while (!_pool.empty())
		{
			CloseHandle(_pool.front());
			_pool.pop_front();
		}
		_mutex.unlock();
	}

	void signal(void)
	{
		_mutex.lock();
		if (!_waiters.empty())
			SetEvent(_waiters.back());
		_mutex.unlock();
	}

	void broadcast(void)
	{
		_mutex.lock();
		for (std::list<HANDLE>::iterator i = _waiters.begin(); i != _waiters.end(); ++i)
			SetEvent(*i);
		_mutex.unlock();
	}

	bool wait(GWMutex::PrivateData& externalMutex, DWORD msec)
	{
		_mutex.lock();
		if (_pool.empty())
			_waiters.push_front(CreateEvent(NULL, FALSE, FALSE, NULL));
		else
			_waiters.splice(_waiters.begin(), _pool, _pool.begin());
		std::list<HANDLE>::iterator i = _waiters.begin();
		_mutex.unlock();

		externalMutex.unlock();

		DWORD result = WaitForSingleObject(*i, msec);

		externalMutex.lock();

		_mutex.lock();
		if (result != WAIT_OBJECT_0)
			result = WaitForSingleObject(*i, 0);
		_pool.splice(_pool.begin(), _waiters, i);
		_mutex.unlock();

		return result == WAIT_OBJECT_0;
	}

	void wait(GWMutex::PrivateData& externalMutex)
	{
		wait(externalMutex, INFINITE);
	}

	// Protect the list of waiters
	GWMutex::PrivateData _mutex;

	std::list<HANDLE> _waiters;
	std::list<HANDLE> _pool;
};

#else
/////////////////////////////////////////////////////////////////////////////
/// posix threads
/////////////////////////////////////////////////////////////////////////////

#include <pthread.h>
#include <cassert>
#include <cerrno>
#include <sys/time.h>

struct GWThread::PrivateData 
{
	PrivateData() :
		_started(false)
	{
	}
	~PrivateData()
	{
		// If we are still having a started thread and nobody waited,
		// now detach ...
		if (!_started)
			return;
		pthread_detach(_thread);
	}

	static void *start_routine(void* data)
	{
		GWThread* thread = reinterpret_cast<GWThread*>(data);
		thread->run();
		return 0;
	}

	bool start(GWThread& thread)
	{
		if (_started)
			return false;

		int ret = pthread_create(&_thread, 0, start_routine, &thread);
		if (0 != ret)
			return false;

		_started = true;
		return true;
	}

	void join()
	{
		if (!_started)
			return;

		pthread_join(_thread, 0);
		_started = false;
	}

	pthread_t _thread;
	bool _started;
};

long GWThread::current(void) 
{
	return (long)pthread_self();
}

struct GWMutex::PrivateData 
{
	PrivateData()
	{
		int err = pthread_mutex_init(&_mutex, 0);
		assert(err == 0);
		(void)err;
	}

	~PrivateData()
	{
		int err = pthread_mutex_destroy(&_mutex);
		assert(err == 0);
		(void)err;
	}

	void lock(void)
	{
		int err = pthread_mutex_lock(&_mutex);
		assert(err == 0);
		(void)err;
	}

	void unlock(void)
	{
		int err = pthread_mutex_unlock(&_mutex);
		assert(err == 0);
		(void)err;
	}

	pthread_mutex_t _mutex;
};

struct GWWaitCondition::PrivateData {
	PrivateData(void)
	{
		int err = pthread_cond_init(&_condition, NULL);
		assert(err == 0);
		(void)err;
	}
	~PrivateData(void)
	{
		int err = pthread_cond_destroy(&_condition);
		assert(err == 0);
		(void)err;
	}

	void signal(void)
	{
		int err = pthread_cond_signal(&_condition);
		assert(err == 0);
		(void)err;
	}

	void broadcast(void)
	{
		int err = pthread_cond_broadcast(&_condition);
		assert(err == 0);
		(void)err;
	}

	void wait(GWMutex::PrivateData& mutex)
	{
		int err = pthread_cond_wait(&_condition, &mutex._mutex);
		assert(err == 0);
		(void)err;
	}

	bool wait(GWMutex::PrivateData& mutex, unsigned msec)
	{
		struct timespec ts;
#ifdef HAVE_CLOCK_GETTIME
		if (0 != clock_gettime(CLOCK_REALTIME, &ts))
			return false;
#else
		struct timeval tv;
		if (0 != gettimeofday(&tv, NULL))
			return false;
		ts.tv_sec = tv.tv_sec;
		ts.tv_nsec = tv.tv_usec * 1000;
#endif

		ts.tv_nsec += 1000000 * (msec % 1000);
		if (1000000000 <= ts.tv_nsec) {
			ts.tv_nsec -= 1000000000;
			ts.tv_sec += 1;
		}
		ts.tv_sec += msec / 1000;

		int evalue = pthread_cond_timedwait(&_condition, &mutex._mutex, &ts);
		if (evalue == 0)
			return true;

		assert(evalue == ETIMEDOUT);
		return false;
	}

	pthread_cond_t _condition;
};

#endif

GWThread::GWThread() : _privateData(new PrivateData)
{
}

GWThread::~GWThread()
{
	delete _privateData;
	_privateData = 0;
}

bool GWThread::start()
{
	return _privateData->start(*this);
}

void GWThread::join()
{
	_privateData->join();
}

GWMutex::GWMutex() : _privateData(new PrivateData)
{
}

GWMutex::~GWMutex()
{
	delete _privateData;
	_privateData = 0;
}

void GWMutex::lock()
{
	_privateData->lock();
}

void GWMutex::unlock()
{
	_privateData->unlock();
}

GWWaitCondition::GWWaitCondition() :
	_privateData(new PrivateData)
{
}

GWWaitCondition::~GWWaitCondition()
{
	delete _privateData;
	_privateData = 0;
}

void GWWaitCondition::wait(GWMutex& mutex)
{
	_privateData->wait(*mutex._privateData);
}

bool GWWaitCondition::wait(GWMutex& mutex, unsigned msec)
{
	return _privateData->wait(*mutex._privateData, msec);
}

void GWWaitCondition::signal()
{
	_privateData->signal();
}

void GWWaitCondition::broadcast()
{
	_privateData->broadcast();
}
