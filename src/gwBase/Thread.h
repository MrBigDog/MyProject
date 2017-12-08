#ifndef GWUTIL_THREAD_H
#define GWUTIL_THREAD_H 1

#include <compiler.h>
#include <gwBase/export.h>

/**
 * Encapsulate generic threading methods.
 * Users derive a class from GWThread and implement the run() member function.
 */
class GW_BASE_EXPORT GWThread
{
public:
	/**
	 * Create a new thread object.
	 * When a GWThread object is created it does not begin execution
	 * immediately.  It is started by calling the start() member function.
	 */
	GWThread();

	/**
	 * Start the underlying thread of execution.
	 * @return Pthread error code if execution fails, otherwise returns 0.
	 */
	bool start();

	/**
	 * Suspends the exection of the calling thread until this thread
	 * terminates.
	 */
	void join();

	/**
	 *Retreive the current thread id.
	 */
	static long current(void);

protected:
	/**
	 * Destroy a thread object.
	 * This is protected so that its illegal to simply delete a thread
	 * - it must return from its run() function.
	 */
	virtual ~GWThread();

	/**
	 * All threads execute by deriving the run() method of GWThread.
	 * If this function terminates then the thread also terminates.
	 */
	virtual void run() = 0;

private:
	// Disable copying.
	GWThread(const GWThread&);
	GWThread& operator=(const GWThread&);

	struct PrivateData;
	PrivateData* _privateData;

	friend struct PrivateData;
};

class GWWaitCondition;

/**
 * A mutex is used to protect a section of code such that at any time
 * only a single thread can execute the code.
 */
class GWMutex
{
public:
	/**
	 * Create a new mutex.
	 * Under Linux this is a 'fast' mutex.
	 */
	GWMutex();

	/**
	 * Destroy a mutex object.
	 * Note: it is the responsibility of the caller to ensure the mutex is
	 * unlocked before destruction occurs.
	 */
	~GWMutex();

	/**
	 * Lock this mutex.
	 * If the mutex is currently unlocked, it becomes locked and owned by
	 * the calling thread.  If the mutex is already locked by another thread,
	 * the calling thread is suspended until the mutex is unlocked.  If the
	 * mutex is already locked and owned by the calling thread, the calling
	 * thread is suspended until the mutex is unlocked, effectively causing
	 * the calling thread to deadlock.
	 */
	void lock();

	/**
	 * Unlock this mutex.
	 * It is assumed that the mutex is locked and owned by the calling thread.
	 */
	void unlock();

private:
	struct PrivateData;
	PrivateData* _privateData;

	friend class GWWaitCondition;
};

/**
 * A condition variable is a synchronization device that allows threads to
 * suspend execution until some predicate on shared data is satisfied.
 * A condition variable is always associated with a mutex to avoid race
 * conditions.
 */
class GWWaitCondition
{
public:
	/*** Create a new condition variable.*/
	GWWaitCondition();

	/**
	 * Destroy the condition object.
	 */
	~GWWaitCondition();

	/**
	 * Wait for this condition variable to be signaled.
	 *
	 * @param mutex Reference to a locked mutex.
	 */
	void wait(GWMutex& mutex);

	/**
	 * Wait for this condition variable to be signaled for at most \a 'msec'
	 * milliseconds.
	 *
	 * @param mutex Reference to a locked mutex.
	 * @param msec  Milliseconds to wait for a signal.
	 *
	 * @return
	 */
	bool wait(GWMutex& mutex, unsigned msec);

	/**
	 * Wake one thread waiting on this condition variable.
	 * Nothing happens if no threads are waiting.
	 * If several threads are waiting exactly one thread is restarted.  It
	 * is not specified which.
	 */
	void signal();

	/**
	 * Wake all threads waiting on this condition variable.
	 * Nothing happens if no threads are waiting.
	 */
	void broadcast();

private:
	// Disable copying.
	GWWaitCondition(const GWWaitCondition&);
	GWWaitCondition& operator=(const GWWaitCondition&);

	struct PrivateData;
	PrivateData* _privateData;
};

#endif /* GWUTIL_THREAD_H */
