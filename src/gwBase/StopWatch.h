/************************************************************************/
/* �߾���ʱ��                                                            */
/************************************************************************/

#ifndef GWBASE_STOPWATCH_H
#define GWBASE_STOPWATCH_H

#include <compiler.h>

#ifndef __cplusplus
# error This library requires C++
#endif

#if defined(__linux__) && ! defined(HAVE_GETRUSAGE)
#  define HAVE_GETRUSAGE
#endif

#if defined( WIN32 ) && defined( HAVE_GETRUSAGE )
#  undef HAVE_GETRUSAGE
#endif // WIN32

#if defined( HAVE_GETRUSAGE )
#  if defined( __FreeBSD__ )
#    include <sys/types.h>
#  endif 
#  include <sys/time.h>
#  include <sys/resource.h>
#  include <unistd.h>
#elif defined( WIN32 )
#  include <windows.h>
#else
#  include <time.h>
#endif

namespace gwBase
{
	class StopWatch
	{
	public:
		StopWatch()
		{
			// state_ = uninitialized; 
		}

		/** Start counting time */
		void start()
		{
			// state_ = running;
			t1_ = systemTime();
		}

		/** Stop counting time */
		void stop()
		{
			t2_ = systemTime();
			// BZPRECONDITION(state_ == running);
			// state_ = stopped;
		}

		/** @return the elapsed time between start and stop */
		double elapsedSeconds()
		{
			// BZPRECONDITION(state_ == stopped);
			return t2_ - t1_;
		}

	private:
		StopWatch(StopWatch&) { }
		void operator=(StopWatch&) { }

		double systemTime()
		{
#if defined( HAVE_GETRUSAGE )
			getrusage(RUSAGE_SELF, &resourceUsage_);
			double seconds = resourceUsage_.ru_utime.tv_sec
				+ resourceUsage_.ru_stime.tv_sec;
			double micros = resourceUsage_.ru_utime.tv_usec
				+ resourceUsage_.ru_stime.tv_usec;
			return seconds + micros / 1.0e6;
#elif defined( WIN32 )
			return double(GetTickCount()) * double(1e-3);
#else
			return clock() / (double)CLOCKS_PER_SEC;
#endif
	}

		//enum { uninitialized, running, stopped } state_;

#if defined( HAVE_GETRUSAGE )
		struct rusage resourceUsage_;
#endif

		double t1_, t2_;
	};
}

#endif // _STOPWATCH_HXX
