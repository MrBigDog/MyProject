///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_timer_queue_timer.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : timer_queue_timer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TIMER_QUEUE_TIMER_H_
#define _US_TIMER_QUEUE_TIMER_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT timer_queue_timer
	{
	public:
		timer_queue_timer( void );

		virtual ~timer_queue_timer( void );

	public:
		void register_timer( HANDLE& out_new_timer,WAITORTIMERCALLBACK callback_fun, PVOID params, DWORD due_time, DWORD period, ULONG flags );

		void un_register_timer( HANDLE in_timer );

	protected:
		HANDLE m_timer_queue;
	};

	class singleton_timer_queue_timer
		: public singleton<timer_queue_timer>
	{

	public:
		singleton_timer_queue_timer( void )
		{
		}

		virtual ~singleton_timer_queue_timer( void )
		{
		}
	};
}

#endif // _US_TIMER_QUEUE_TIMER_H_
