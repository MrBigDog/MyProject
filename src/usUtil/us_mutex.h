//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mutex.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : radian template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_MUTEX_H_
#define _US_MUTEX_H_

#include <usUtil\Export.h>

#include <list>
#include <wtypes.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT win_mutex
	{
	public:
		win_mutex(void);
		~win_mutex(void);

	public:
		void lock();
		void unlock();

	private:
		CRITICAL_SECTION critical_section;
	};

	///add by felix 
	class USUTIL_EXPORT GWWaitCondition
	{
	public:
		/**
		* Create a new condition variable.
		*/
		GWWaitCondition() {}

		/**
		* Destroy the condition object.
		*/
		~GWWaitCondition();

		/**
		* Wait for this condition variable to be signaled.
		*
		* @param mutex Reference to a locked mutex.
		*/
		void wait(win_mutex& mutex);

		/**
		* Wait for this condition variable to be signaled for at most \a 'msec'
		* milliseconds.
		*
		* @param mutex Reference to a locked mutex.
		* @param msec  Milliseconds to wait for a signal.
		*
		* @return
		*/
		bool wait(win_mutex& mutex, unsigned msec);

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

		GWWaitCondition(const GWWaitCondition&);
		GWWaitCondition& operator=(const GWWaitCondition&);

	private:
		win_mutex _mutex;

		std::list<HANDLE> _waiters;
		std::list<HANDLE> _pool;
	};

	class USUTIL_EXPORT unique_lock
	{
	private:
		unique_lock(void) { }

	public:
		unique_lock(win_mutex& mutex);

		unique_lock(win_mutex* mutex);

		~unique_lock(void);

	protected:

		// lock & unlock
		void lock();
		void unlock();

		// be locked
		bool owns_lock() const;

	private:
		win_mutex*	mutex_pointer;

		bool		is_locked;
	};



#define US_AUTO_MUTEX_NAME mutex

#ifdef WIN32

#define US_AUTO_MUTEX win_mutex US_AUTO_MUTEX_NAME;
#define US_LOCK_AUTO_MUTEX unique_lock auto_mutex_lock( US_AUTO_MUTEX_NAME );

#define US_MUTEX(name) mutable win_mutex name;
#define US_LOCK_MUTEX(name) unique_lock named_mutex_lock( name );

#define US_AUTO_SHARED_MUTEX mutable win_mutex* US_AUTO_MUTEX_NAME;
#define US_NEW_AUTO_SHARED_MUTEX assert(!US_AUTO_MUTEX_NAME); US_AUTO_MUTEX_NAME = new win_mutex;
#define US_DEL_AUTO_SHARED_MUTEX assert(US_AUTO_MUTEX_NAME); delete US_AUTO_MUTEX_NAME;
#define US_LOCK_AUTO_SHARED_MUTEX assert(US_AUTO_MUTEX_NAME); unique_lock shared_mutex_lock(*US_AUTO_MUTEX_NAME);

#define US_COPY_AUTO_SHARED_MUTEX(from) assert(!US_AUTO_MUTEX_NAME); US_AUTO_MUTEX_NAME = from;
#define US_SET_AUTO_SHARED_MUTEX_NULL US_AUTO_MUTEX_NAME = 0;
#define US_MUTEX_CONDITIONAL(mutex) if ( mutex )

#endif

}

#endif // _US_RADIAN_H_