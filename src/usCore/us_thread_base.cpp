///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_thread_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : thread_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_thread_base.h"
#include <wtypes.h>

namespace uniscope_globe
{
	thread_base::thread_base(void)
	{
	}

	thread_base::~thread_base(void)
	{
		destroy();
	}

	bool thread_base::check_valid(bool re_create)
	{
		DWORD code = 0;
		GetExitCodeThread(thread_private_info.thread_handle, &code);

		if (code != STILL_ACTIVE)
		{
			TerminateThread(thread_private_info.thread_handle, code);
			thread_private_info.destroy();

			if (re_create)
			{
				create();
			}
			return false;
		}

		return true;
	}

	void thread_base::create()
	{
		if (!thread_private_info.thread_created)
		{
			thread_private_info.thread_event_terminate = ::CreateEvent(NULL, TRUE, FALSE, NULL);

			thread_private_info.thread_event_on_terminate = ::CreateEvent(NULL, TRUE, FALSE, NULL);

			thread_private_info.thread_event = CreateEvent(NULL, TRUE, FALSE, NULL);

			thread_private_info.thread_handle = CreateThread(NULL, 0, thread_fun, this, CREATE_SUSPENDED, &thread_private_info.thread_id);

			thread_private_info.thread_created = true;

			SetThreadPriority(thread_private_info.thread_handle, THREAD_PRIORITY_NORMAL);

			ResumeThread(thread_private_info.thread_handle);
		}
	}

	void thread_base::destroy()
	{
		//DWORD code = 0;
		//GetExitCodeThread( thread_private_info.thread_handle, &code );
		//TerminateThread( thread_private_info.thread_handle, code );

		//thread_private_info.destroy();

		//return;
		thread_private_info.m_canceling = true;

		DWORD code = 0;
		GetExitCodeThread(thread_private_info.thread_handle, &code);
		if (code != STILL_ACTIVE)
		{
			TerminateThread(thread_private_info.thread_handle, code);
			thread_private_info.destroy();
			return;
		}

		// signal to terminate
		SetEvent(thread_private_info.thread_event_terminate);

		Sleep(1);

		// wait for response
		while (WAIT_TIMEOUT == WaitForSingleObject(thread_private_info.thread_event_on_terminate, 50))
		{
			Sleep(1);
		}

		thread_private_info.destroy();

		//thread_private_info.m_canceling = true;
		//	
		//// signal to terminate
		//SetEvent( thread_private_info.thread_event_terminate );
		//Sleep(1);

		//// wait for response
		//DWORD ret = WaitForSingleObject( thread_private_info.thread_event_on_terminate, 2000 );
		//if( WAIT_TIMEOUT == ret )
		//{	
		//	DWORD code = 0;
		//	GetExitCodeThread( thread_private_info.thread_handle, &code );
		//	TerminateThread( thread_private_info.thread_handle, code );
		//}

		//thread_private_info.destroy();
	}

	void thread_base::thread_run(void)
	{
		if (thread_private_info.thread_event)
		{
			SetEvent(thread_private_info.thread_event);
		}
	}

	void thread_base::thread_stop(void)
	{
		if (thread_private_info.thread_event)
		{
			ResetEvent(thread_private_info.thread_event);
		}
	}

	uint thread_base::get_thread_id()
	{
		return thread_private_info.thread_id;
	}

	uint thread_base::set_thread_priority(uint priority)
	{
		return 0;
	}

	void thread_base::set_thread_stack(uint stack_size)
	{
		thread_private_info.thread_stack_size = stack_size;
	}

	uint thread_base::get_thread_stack(void)
	{
		return thread_private_info.thread_stack_size;
	}

	void thread_base::on_start(void* param)
	{
	}

	void thread_base::on_stop(void)
	{
	}

	bool thread_base::on_waiting(void)
	{
		if (thread_private_info.m_canceling)
			return true;

		//HANDLE multi_handle[2];
		//multi_handle[0] = thread_private_info.thread_event_terminate;
		//multi_handle[1] = thread_private_info.thread_event;
		//DWORD ret = WaitForMultipleObjects( 2, multi_handle, FALSE, INFINITE );
		//if( ( ret - WAIT_OBJECT_0 ) == 0 )
		//	return true; // terminate

		HANDLE multi_handle[2];
		multi_handle[0] = thread_private_info.thread_event_terminate;
		multi_handle[1] = thread_private_info.thread_event;
		DWORD ret = 0;
		while (WAIT_TIMEOUT == (ret = WaitForMultipleObjects(2, multi_handle, FALSE, 50)))
		{
			if (thread_private_info.m_canceling)
				return true;
		}

		if ((ret - WAIT_OBJECT_0) == 0)
			return true; // terminate

		if (thread_private_info.m_canceling)
			return true;

		return false;
	}

	void thread_base::on_running(void)
	{
	}

	void thread_base::on_terminate(void)
	{
		if (thread_private_info.thread_event_on_terminate)
			::SetEvent(thread_private_info.thread_event_on_terminate);
	}

	unsigned long  thread_base::thread_fun(void *param)
	{
		thread_base *thread = (thread_base*)param;

		if (thread->on_waiting())
		{
			thread->on_terminate();

			return 0;
		}

		thread->on_start(param);

		while (thread)
		{
			if (thread->on_waiting())
				break;

			thread->on_running();

			if (thread->thread_private_info.m_canceling)
				break;
		}

		thread->on_terminate();

		return 0;
	}

}