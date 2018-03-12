///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_download_mission.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : download_mission class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "us_download_mission.h"

namespace uniscope_globe
{

	download_mission::download_mission( const LPCTSTR str_url, downloader* v_downloader  )
	{
		m_request_state = US_DEFAULT;
		m_mission_executant = NULL;
		m_index = 0;
		m_error_count = 0;
		m_request_url = str_url;
		m_downloader = v_downloader;
		m_dispose = false;
	}

	download_mission::~download_mission( void )
	{
		
	}

	void download_mission::set_executant( mission_executant* v_executant )
	{
		m_mission_executant = v_executant;
	}

	void download_mission::set_index( interior_index v_id )
	{
		m_index = v_id;
	}

	void download_mission::set_url( const LPCTSTR str_url )
	{
		m_request_url = str_url;
	}

	bool download_mission::update()
	{
		switch( m_request_state )
		{
		case US_DEFAULT:
			case_default();
			break;
		case US_QUEUE:
			case_queue();
			break;
		case US_WAITING:
			case_waiting();
			break;
		case US_LOADING:
			case_loading();
			break;
		case US_LOADED:
			case_loaded();
			break;
		case US_ERROR:
			return true;
			break;
		}

		return m_finished;
	}

	void download_mission::dispose()
	{
		if ( m_request_state == US_WAITING || m_request_state == US_LOADING )
		{
			m_dispose = true;
			return;
		}

		delete this;
	}

	// 加入downloader队列
	void download_mission::case_default()
	{
		//event_download_start( NULL );

		m_request_state = US_QUEUE;

		if( singleton_priority_stack::ptr() )
			m_priority = singleton_priority_stack::instance().get_priority();	
		else
			m_priority = 0xffffffff;

		//m_mission_executant->receive_mission( (mission_base*)this );
		m_mission_executant->push_back_buffer( this );
	}

	// 如果在downloader队列中，即被多次update
	void download_mission::case_queue()
	{
		if( singleton_priority_stack::ptr() )
			m_priority = singleton_priority_stack::instance().get_priority();	
		else
			m_priority = 0xffffffff;
	}

	void download_mission::case_waiting()
	{
		m_mission_executant->update_mission( (mission_base*)this );
	}

	void download_mission::case_loading()
	{
		// do nothing
	}

	void download_mission::case_loaded()
	{
		// do nothing
	}

	void download_mission::case_error( void )
	{
		// do nothing
	}

	//[Thread Download]
	void download_mission::execute( void )
	{
		m_request_state = US_LOADING;
		
		common_loader loader( m_request_url.c_str(), m_downloader );

		try
		{
			int v_size = 0;
			byte* v_buf = NULL;

			int  load_res = loader.load( v_buf, v_size );
			if ( load_res == LOAD_RESULT_SUC )
			{
				on_succeed( v_buf, v_size );

				AUTO_DELETE( v_buf );
			}
			else
			{
				on_failed( load_res );
			}
		}
		catch (...)
		{
			on_failed( LOAD_RESULT_ERR );
		}

		if ( m_dispose )
		{
			delete this;
		}
	}

	void download_mission::reset( void )
	{
		if ( m_request_state != US_LOADED && m_request_state != US_QUEUE )
			return;

		if ( m_request_state == US_ERROR )
			return;
		
		m_request_state = US_DEFAULT;
		m_finished = false;
	}

	void download_mission::startup( void )
	{
		event_download_start( NULL );

		m_request_state = US_WAITING;
	}

	void download_mission::terminate( void )
	{
		download_argument dl_arg( NULL, US_DOWNLOAD_RESET, m_index );
		event_download_finish( &dl_arg );

		m_request_state = US_DEFAULT;
		m_finished = false;
 
		if ( m_dispose )
		{
			delete this;
		}
	}

	void download_mission::on_succeed( byte* v_buf, int v_size )
	{
		raw_buffer r_buf( v_buf, v_size );
		download_argument dl_arg( &r_buf, US_DOWNLOAD_SUCCEED, m_index );
		dl_arg.m_str_url = m_request_url.c_str();
		event_download_finish( &dl_arg );

		m_finished = true;

		m_request_state = US_LOADED;
	}

	void download_mission::on_failed( int load_result )
	{
		if ( load_result == LOAD_RESULT_NOF )
		{
			download_argument dl_arg( NULL, US_DOWNLOAD_NOFILE, m_index );
			event_download_finish( &dl_arg );

			m_request_state = US_ERROR;

			return;
		}

		m_error_count++;
		if ( m_error_count < 3 )
		{
			download_argument dl_arg( NULL, US_DOWNLOAD_FAILED, m_index );
			event_download_finish( &dl_arg );

			m_request_state = US_DEFAULT;
		}
		else
		{
			download_argument dl_arg( NULL, US_DOWNLOAD_ERROR, m_index );
			event_download_finish( &dl_arg );

			m_request_state = US_ERROR;
		}

	}
	
}