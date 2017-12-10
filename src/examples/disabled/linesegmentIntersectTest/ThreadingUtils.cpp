#include "ThreadingUtils.h"

#ifdef _WIN32
extern "C" unsigned long __stdcall GetCurrentThreadId();
#elif defined(__APPLE__) || defined(__LINUX__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
#   include <unistd.h>
#   include <sys/syscall.h>
#else
#   include <pthread.h>
#endif


//------------------------------------------------------------------------
unsigned getCurrentThreadId()
{
	/*   OpenThreads::Thread* t = OpenThreads::Thread::CurrentThread();
	 return t ? t->getThreadId() : 0u;*/

#ifdef _WIN32
	return (unsigned)::GetCurrentThreadId();
#elif __APPLE__
	return ::syscall(SYS_thread_selfid);
#elif __ANDROID__
	return gettid();
#elif __LINUX__
	return (unsigned)::syscall(SYS_gettid);
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	long  tid;
	syscall(SYS_thr_self, &tid);
	return (unsigned)tid;
#else
	 /* :XXX: this truncates to 32 bits, but better than nothing */
	return (unsigned)pthread_self();
#endif
}
