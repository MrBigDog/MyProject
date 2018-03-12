///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mission_executant_async.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mission_executant_async class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_mission_executant_async.h"

namespace uniscope_globe
{
	mission_executant_async::mission_executant_async( void )
	{
		create();		
	}

	mission_executant_async::~mission_executant_async( void )
	{
		//destroy();
	}

	void mission_executant_async::receive_mission( mission_base* mission_value )
	{
		m_current_mission = mission_value;
		m_busy = true;
	

		//if ( !thread_private_info.thread_created )
		//{
		//	create();
		//}

		thread_run();
	}

	void mission_executant_async::terminate_mission( void )
	{
		thread_base::destroy();
	}

	bool mission_executant_async::on_waiting( void )
	{
		/**@add by Felix ÈÝ´í»úÖÆ*/
		AUTO_DECLEAR_STRUCT_EXCEPTION
		//TRY_SUSPEND_THREAD
		{
			int time_sleep = singleton_thread_valve::instance().control_time();
			time_sleep = max( 0, time_sleep );
			time_sleep = min( 200, time_sleep );
			time_sleep = 0;
			
			if ( !thread_private_info.m_canceling )
			{
				Sleep( time_sleep ); 
			} 
		}

		return thread_base::on_waiting();
	}

	void mission_executant_async::on_running( void )
	{
		try
		{
			m_current_mission->execute();

			m_current_mission = 0;
			m_busy = false;

			thread_stop();

			if ( m_parent )
			{
				m_parent->on_mission_complete( this );
			}
		}
		catch (gw_exception::exception_base& e)
		{
			GWLOG_ERROR(e.what());
		}
		catch (...)
		{
			GWLOG_ERROR("unknown error!");
		}
		
	}

}