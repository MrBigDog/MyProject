///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_thread_data.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose :
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_thread_data.h"
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	thread_data::thread_data(void)
	{
		thread_id = 0;
		thread_priority = 0;
		thread_stack_size = 0;

		thread_handle = 0;
		thread_event = 0;
		thread_event_terminate = 0;
		thread_event_on_terminate = 0;


		thread_created = false;

		m_canceling = false;
	}

	thread_data::~thread_data(void)
	{
		destroy();
	}

	void thread_data::destroy(void)
	{
		AUTO_CLOSE_HANDLE(thread_handle);
		AUTO_CLOSE_HANDLE(thread_event);
		AUTO_CLOSE_HANDLE(thread_event_terminate);
		AUTO_CLOSE_HANDLE(thread_event_on_terminate);

		thread_id = 0;
		thread_priority = 0;
		thread_stack_size = 0;

		thread_created = false;
	}


}