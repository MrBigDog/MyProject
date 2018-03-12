///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_thread_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//	Reference : http://openthreads.sourceforge.net/
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_THREAD_BASE_H_
#define _US_THREAD_BASE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{

	//__declspec(thread) float s_priority = 0.0;

	class USCORE_EXPORT thread_base
	{
	public:
		thread_base(void);

		virtual ~thread_base(void);

	public:
		// create / destroy thread
		void create();
		virtual void destroy();

		// check thread
		virtual bool check_valid(bool re_crated);

		// thread valve
		void thread_run(void);
		void thread_stop(void);

		// thread id
		uint get_thread_id();
		uint set_thread_priority(uint priority);

		// set / get thread stack , must set before thread create
		void set_thread_stack(uint stack_size);
		uint get_thread_stack(void);

		// work function
		virtual void on_start(void* param);
		virtual void on_stop(void);
		virtual bool on_waiting(void);
		virtual void on_running(void);


		// thread function
		static unsigned long __stdcall thread_fun(void *param);

	protected:
		virtual void on_terminate(void);

	protected:
		thread_data		thread_private_info;
	};

}

#endif // _US_THREAD_BASE_H_