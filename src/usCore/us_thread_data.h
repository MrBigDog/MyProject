///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_thread_data.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose :
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_THREAD_DATA_H_
#define _US_THREAD_DATA_H_

#include <usCore\Export.h>

namespace uniscope_globe
{
	class USCORE_EXPORT thread_data
	{
	public:
		thread_data(void);
		~thread_data(void);

		typedef void* lpvoid;

	public:
		void destroy(void);

	public:
		unsigned long	thread_id;
		unsigned long	thread_stack_size;
		int		thread_priority;

		lpvoid	thread_handle;
		lpvoid	thread_event;
		lpvoid  thread_event_terminate;
		lpvoid  thread_event_on_terminate;

		// true --> created
		// false --> uncreated
		bool	thread_created;

		bool m_canceling;

	};

}


#endif
