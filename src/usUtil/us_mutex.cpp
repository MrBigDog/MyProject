//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mutex.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : radian template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_mutex.h"
#include <assert.h>

namespace uniscope_globe
{
	//
	// win_mutex
	//
	win_mutex::win_mutex(void)
	{
		InitializeCriticalSection(&critical_section);
	}

	win_mutex::~win_mutex(void)
	{
		DeleteCriticalSection(&critical_section);
	}

	void win_mutex::lock()
	{
		EnterCriticalSection(&critical_section);
	}

	void win_mutex::unlock()
	{
		LeaveCriticalSection(&critical_section);
	}


	//
	// lock
	//
	unique_lock::unique_lock(win_mutex& mutex)
		: mutex_pointer(&mutex)
		, is_locked(false)
	{
		lock();
	}

	unique_lock::unique_lock(win_mutex* mutex)
		: mutex_pointer(mutex)
		, is_locked(false)
	{
		lock();
	}

	unique_lock::~unique_lock(void)
	{
		unlock();
	}


	void unique_lock::lock()
	{
		if (owns_lock())
		{
			assert(false);
			return;
		}
		mutex_pointer->lock();
		is_locked = true;
	}

	void unique_lock::unlock()
	{
		if (!owns_lock())
		{
			assert(false);
			return;
		}
		mutex_pointer->unlock();
		is_locked = false;
	}

	bool unique_lock::owns_lock() const
	{
		return is_locked;
	}

	//////////////////////////////////////////////////add by felix///////////////////////////////////////////////////////////
	GWWaitCondition::~GWWaitCondition(void)
	{
		// The waiters list should be empty anyway
		_mutex.lock();
		while (!_pool.empty()) {
			CloseHandle(_pool.front());
			_pool.pop_front();
		}
		_mutex.unlock();
	}

	void GWWaitCondition::signal(void)
	{
		_mutex.lock();
		if (!_waiters.empty())
			SetEvent(_waiters.back());
		_mutex.unlock();
	}

	void GWWaitCondition::broadcast(void)
	{
		_mutex.lock();
		for (std::list<HANDLE>::iterator i = _waiters.begin(); i != _waiters.end(); ++i)
			SetEvent(*i);
		_mutex.unlock();
	}

	bool GWWaitCondition::wait(win_mutex& mutex, unsigned msec)
	{
		_mutex.lock();
		if (_pool.empty())
			_waiters.push_front(CreateEvent(NULL, FALSE, FALSE, NULL));
		else
			_waiters.splice(_waiters.begin(), _pool, _pool.begin());
		std::list<HANDLE>::iterator i = _waiters.begin();
		_mutex.unlock();

		mutex.unlock();

		DWORD result = WaitForSingleObject(*i, msec);

		mutex.lock();

		_mutex.lock();
		if (result != WAIT_OBJECT_0)
			result = WaitForSingleObject(*i, 0);
		_pool.splice(_pool.begin(), _waiters, i);
		_mutex.unlock();

		return result == WAIT_OBJECT_0;
	}

	void GWWaitCondition::wait(win_mutex& mutex)
	{
		wait(mutex, INFINITE);
	}


}