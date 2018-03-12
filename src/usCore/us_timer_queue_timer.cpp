///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_timer_queue_timer.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : timer_queue_timer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_timer_queue_timer.h"

namespace uniscope_globe
{
	timer_queue_timer* singleton_timer_queue_timer::s_ins = NULL;

	timer_queue_timer::timer_queue_timer( void )
	{
		m_timer_queue = ::CreateTimerQueue();
	}

	timer_queue_timer::~timer_queue_timer( void )
	{
		if( m_timer_queue )
		{
			::DeleteTimerQueueEx( m_timer_queue, NULL );
			m_timer_queue = NULL;
		}
	}

	void timer_queue_timer::register_timer( HANDLE& out_new_timer,WAITORTIMERCALLBACK callback_fun, PVOID params, DWORD due_time, DWORD period, ULONG flags )
	{
		::CreateTimerQueueTimer(	&out_new_timer, 
									m_timer_queue, 
									callback_fun, 
									params,
									due_time,
									period,
									flags );
	}

	void timer_queue_timer::un_register_timer( HANDLE in_timer )
	{
		::DeleteTimerQueueTimer( m_timer_queue, in_timer, NULL );				
	}
}