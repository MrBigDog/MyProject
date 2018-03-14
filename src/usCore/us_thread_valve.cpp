///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_thread_valve.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : thread_valve class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_thread_valve.h"
#include <usCore/us_system_environment.h>

namespace uniscope_globe
{

	thread_valve* singleton_thread_valve::s_ins = NULL;

	double thread_valve::last_time = 0;

	thread_valve::thread_valve(void)
	{
		//last_time = 0;
	}

	thread_valve::~thread_valve(void)
	{

	}

	int thread_valve::control_time(void)
	{
		double cur_time = (double)timeGetTime();
		double internal_time = cur_time - last_time;

		if (internal_time > system_environment::s_download_sleep_interval)
		{
			return (int)system_environment::s_download_sleep_time;
		}

		return 0;
	}


}