///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_thread_valve.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : thread_valve class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_THREAD_VALVE_H_
#define _US_THREAD_VALVE_H_

#include <usCore\Export.h>

namespace uniscope_globe
{
	class USCORE_EXPORT thread_valve
	{
	public:
		thread_valve(void);

		virtual ~thread_valve(void);

	public:
		static int control_time(void);

	public:
		static double	last_time;
	};


	class USCORE_EXPORT singleton_thread_valve : public singleton<thread_valve>
	{
	public:
		singleton_thread_valve(void)
		{
		}

		virtual ~singleton_thread_valve(void)
		{
		}
	};
	//*/


#define TRY_SUSPEND_THREAD { int time_sleep = singleton_thread_valve::instance().control_time(); if ( time_sleep > 0 ) { Sleep( time_sleep ); } }
}

#endif // _US_THREAD_VALVE_H_