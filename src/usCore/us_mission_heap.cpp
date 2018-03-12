///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mission_heap.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : request_heap class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "us_mission_heap.h"

namespace uniscope_globe
{
	mission_heap::mission_heap( void )
	{
		m_max_heap_size = 0;
		m_lastest_time = 0;
	}

	mission_heap::mission_heap( int executant_num, int max_heap_size )
	{
		m_max_heap_size = max_heap_size;
		m_lastest_time = 0;

		m_executant_array.resize( executant_num );
		m_leisure_executeants.resize( executant_num );
		for ( int i = 0 ; i < executant_num; i++ )
		{
			mission_executant_async* v_executant = new mission_executant_async;
			v_executant->set_parent( this );
			m_executant_array[i] = v_executant;
			m_leisure_executeants[i] = v_executant;
		}
	}

	mission_heap::~mission_heap( void )
	{
		for ( int i = 0; i < (int)m_executant_array.size(); i++ )
		{
			AUTO_DELETE( m_executant_array[i] );
		}
		m_executant_array.clear();
		m_leisure_executeants.clear();
	}

	void mission_heap::terminate_mission( void )
	{
		{
			US_LOCK_AUTO_MUTEX

			for ( int i = 0 ; i < (int)m_missions.size(); i++ )
			{
				mission_base *tmp_mission = (mission_base *)m_missions.extract();
				tmp_mission->terminate();
			}
			m_missions.reset();

			m_leisure_executeants.clear();
		}
				
		for ( int i = 0; i < (int)m_executant_array.size(); i++ )
		{
			m_executant_array[i]->terminate_mission();
		}
	}

	void mission_heap::receive_mission( mission_base* mission )
	{
		US_LOCK_AUTO_MUTEX

		attenuate_mission();

		m_missions.insert( mission, mission->m_priority );


#ifdef _PERFORMANCE_TEST_
		system_environment::s_request_download_mission_count ++;
		system_environment::s_update_download_mission_heap_size = m_missions.size();
#endif

		int heap_size = m_missions.size();
		if ( heap_size > m_max_heap_size )
		{
			mission_base *tmp_mission = (mission_base*)m_missions[heap_size - 1];

			if ( NULL != tmp_mission )
			{				
				tmp_mission->terminate();
			}

			m_missions.remove_inorder( heap_size - 1 );
		}

		distribute_mission();
	}

	void mission_heap::on_mission_complete( mission_executant* executant )
	{
		US_LOCK_AUTO_MUTEX

		m_leisure_executeants.push_back( executant );

		// sleep
		//::Sleep( 100 );

		distribute_mission();
	}

	void mission_heap::update_mission( mission_base* mission )
	{
		US_LOCK_AUTO_MUTEX

#ifdef _PERFORMANCE_TEST_
		system_environment::s_update_download_mission_count ++;
#endif
		mission->heap_key( mission->m_priority );

		m_missions.update( mission );
	}

	void mission_heap::distribute_mission( void )
	{
		if ( m_missions.size() == 0 )
			return;

		if ( m_leisure_executeants.size() == 0 )
			return;

		// get executant
		mission_executant* v_leisure_executant = m_leisure_executeants.front();
		m_leisure_executeants.pop_front();
		
		// get mission
		mission_base* v_mission = (mission_base*)m_missions[0];
		m_missions.extract();

		v_leisure_executant->receive_mission( v_mission );
	}

	void mission_heap::attenuate_mission( void )
	{
		double v_interval = tls_singleton_frame_counter::instance().m_logic_time - m_lastest_time;
		if ( v_interval > system_environment::s_attenuate_interval )
		{
			mission_base* v_mission = NULL;
			int heap_size = m_missions.size();
			for ( int i = 0; i < heap_size; i++ )
			{
				v_mission = (mission_base*)m_missions[i];

				v_mission->heap_key( v_mission->m_priority>>system_environment::s_attenuate_speed  );
			}
			m_lastest_time = tls_singleton_frame_counter::instance().m_logic_time;
		}

		if ( v_interval > system_environment::s_download_check_interval )
		{
			for ( int ni = 0; ni < m_executant_array.size(); ni++ )
			{
				thread_base* v_thread = dynamic_cast<thread_base*>(m_executant_array[ni]);
				v_thread->check_valid( true );
			}
		}

	}

	void mission_heap::push_back_buffer( mission_base* v_mission )
	{
		m_mission_array.push_back( v_mission );
	}

	void mission_heap::clear_back_buffer()
	{
		// ≈≈–Ú
		std::sort( m_mission_array.begin(), m_mission_array.end(), compare_mission_priority );

		int v_size = min( m_mission_array.size(), m_max_heap_size );

		US_LOCK_AUTO_MUTEX

		attenuate_mission();

		for ( int ni = 0; ni < v_size; ni++ )
		{
			m_missions.insert( m_mission_array[ni], m_mission_array[ni]->m_priority );

			m_mission_array[ni]->startup();
		}

		for ( int ni = v_size; ni < (int)m_mission_array.size(); ni++ )
		{
			m_mission_array[ni]->reset();
		}

		m_mission_array.clear();

		int heap_size = m_missions.size();
		while ( heap_size > m_max_heap_size )
		{
			mission_base *tmp_mission = (mission_base*)m_missions[heap_size - 1];

			if ( NULL != tmp_mission )
			{				
				tmp_mission->terminate();
			}

			m_missions.remove_inorder( heap_size - 1 );

			heap_size = m_missions.size();
		}
		
		distribute_mission();
	}

}