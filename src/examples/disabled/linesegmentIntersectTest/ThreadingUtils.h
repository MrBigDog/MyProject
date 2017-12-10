#ifndef ThreadingUtils_h__
#define ThreadingUtils_h__

#include <OpenThreads/Condition>
#include <OpenThreads/Mutex>
#include <OpenThreads/Thread>
#include <OpenThreads/ScopedLock>
#include <osg/ref_ptr>
#include <set>
#include <map>

#define USE_CUSTOM_READ_WRITE_LOCK 1
//#ifdef _DEBUG
//#  define TRACE_THREADS 1
//#endif


typedef OpenThreads::Mutex Mutex;
typedef OpenThreads::ScopedLock<OpenThreads::Mutex> ScopedMutexLock;
typedef OpenThreads::Thread Thread;

/**
 * Gets the unique ID of the running thread. Use this instead of
 * OpenThreads::Thread::CurrentThread, which only works reliably on
 * threads created with the OpenThreads framework
 */
extern unsigned getCurrentThreadId();

#ifdef USE_CUSTOM_READ_WRITE_LOCK

/**
 * Event with a toggled signal state.
 */
class Event
{
public:
	Event() : _set(false) { }
	~Event()
	{
		reset();
		for (int i = 0; i < 255; ++i) // workaround buggy broadcast
			_cond.signal();
	}

	inline bool wait()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_m);
		return _set ? true : (_cond.wait(&_m) == 0);
	}

	/** waits on a signal, and then automatically resets it before returning. */
	inline bool waitAndReset()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_m);
		if (_set)
		{
			_set = false;
			return true;
		}
		else
		{
			bool value = _cond.wait(&_m) == 0;
			_set = false;
			return value;
		}
	}

	inline void set()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_m);
		if (!_set)
		{
			_set = true;
			_cond.broadcast(); // possible deadlock before OSG r10457 on windows
			//_cond.signal();
		}
	}

	inline void reset()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_m);
		_set = false;
	}

	inline bool isSet() const { return _set; }

protected:
	OpenThreads::Mutex _m;
	OpenThreads::Condition _cond;
	bool _set;
};

/** Same as an Event, but waits on multiple notifications before releasing its wait. */
class MultiEvent
{
public:
	MultiEvent(int num = 1) : _set(num), _num(num) { }

	~MultiEvent()
	{
		reset();
		for (int i = 0; i < 255; ++i) // workaround buggy broadcast
			_cond.signal();
	}

	inline bool wait()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_m);
		while (_set > 0)
			if (_cond.wait(&_m) != 0)
				return false;
		return true;
	}

	/** waits on a signal, and then automatically resets it before returning. */
	inline bool waitAndReset()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_m);
		while (_set > 0)
			if (_cond.wait(&_m) != 0)
				return false;
		_set = _num;
		return true;
	}

	inline void notify()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_m);
		if (_set > 0)
			--_set;
		if (_set == 0)
			_cond.broadcast(); // possible deadlock before OSG r10457 on windows
		//_cond.signal();
	}

	inline void reset(int num = 0)
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_m);
		if (num > 0) _num = num;
		_set = _num;
	}

protected:
	OpenThreads::Mutex _m;
	OpenThreads::Condition _cond;
	int _set, _num;
};

/**
 * Custom read/write lock. The read/write lock in OSG can unlock mutexes from a different
 * thread than the one that locked them - this can hang the thread in Windows.
 *
 * Adapted from:
 * http://www.codeproject.com/KB/threads/ReadWriteLock.aspx
 */
class ReadWriteMutex
{
#ifdef TRACE_THREADS
	typedef std::set<unsigned> TracedThreads;
	TracedThreads _trace;
	OpenThreads::Mutex _traceMutex;
#endif

public:
	ReadWriteMutex() :
		_readerCount(0)
	{
		_noWriterEvent.set();
		_noReadersEvent.set();
	}

	void readLock()
	{

#ifdef TRACE_THREADS
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> ttLock(_traceMutex);
			if (_trace.find(getCurrentThreadId()) != _trace.end())
				OE_WARN << "TRACE: tried to double-lock" << std::endl;
		}
#endif
		for (; ; )
		{
			_noWriterEvent.wait();             // wait for a writer to quit if there is one
			incrementReaderCount();            // register this reader
			if (!_noWriterEvent.isSet())     // double lock check, in case a writer snuck in while inrementing
				decrementReaderCount();        // if it did, undo the registration and try again
			else
				break;                         // otherwise, we're in
		}

#ifdef TRACE_THREADS
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> ttLock(_traceMutex);
			_trace.insert(getCurrentThreadId());
		}
#endif
	}

	void readUnlock()
	{
		decrementReaderCount();                // unregister this reader

#ifdef TRACE_THREADS
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> ttLock(_traceMutex);
			_trace.erase(getCurrentThreadId());
		}
#endif
	}

	void writeLock()
	{
#ifdef TRACE_THREADS
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> ttLock(_traceMutex);
			if (_trace.find(getCurrentThreadId()) != _trace.end())
				OE_WARN << "TRACE: tried to double-lock" << std::endl;
		}
#endif
		for (;;)
		{
			_noReadersEvent.wait(); // wait for no readers

			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lockWriterMutex);
			_noWriterEvent.wait();  // wait for no writers
			_noWriterEvent.reset(); // signal that there is now a writer

			if (_noReadersEvent.isSet()) // still no readers? done.
				break;
			else
				_noWriterEvent.set(); // otherwise, a reader snuck in, so try again.
		}

#ifdef TRACE_THREADS
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> ttLock(_traceMutex);
			_trace.insert(getCurrentThreadId());
		}
#endif
	}

	void writeUnlock()
	{
		_noWriterEvent.set();

#ifdef TRACE_THREADS
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> ttLock(_traceMutex);
			_trace.erase(getCurrentThreadId());
		}
#endif
	}

protected:
	void incrementReaderCount()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_readerCountMutex);
		_readerCount++;            // add a reader
		_noReadersEvent.reset();   // there's at least one reader now so clear the flag
	}

	void decrementReaderCount()
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_readerCountMutex);
		_readerCount--;               // remove a reader
		if (_readerCount <= 0)      // if that was the last one, signal that writers are now allowed
			_noReadersEvent.set();
	}

private:
	int    _readerCount;
	Mutex  _lockWriterMutex;
	Mutex  _readerCountMutex;
	Event  _noWriterEvent;
	Event  _noReadersEvent;
};

struct ScopedWriteLock
{
	ScopedWriteLock(ReadWriteMutex& lock) : _lock(lock) { _lock.writeLock(); }
	~ScopedWriteLock() { _lock.writeUnlock(); }
protected:
	ReadWriteMutex& _lock;
};

struct ScopedReadLock
{
	ScopedReadLock(ReadWriteMutex& lock) : _lock(lock) { _lock.readLock(); }
	~ScopedReadLock() { _lock.readUnlock(); }
protected:
	ReadWriteMutex& _lock;
};

#else

typedef OpenThreads::ReadWriteMutex  ReadWriteMutex;
typedef OpenThreads::ScopedWriteLock ScopedWriteLock;
typedef OpenThreads::ScopedReadLock  ScopedReadLock;

#endif

#endif // ThreadingUtils_h__
