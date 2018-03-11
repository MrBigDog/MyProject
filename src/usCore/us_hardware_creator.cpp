///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_creator.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_creator class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_hardware_creator.h"

namespace uniscope_globe
{
	hardware_creator::hardware_creator( void )
	{

	}

	hardware_creator::~hardware_creator( void )
	{
		clear();
	}

	void hardware_creator::insert_mesh ( hardware_base* hardware )
	{
		US_LOCK_AUTO_MUTEX

			if ( hardware && m_mesh_create_array.size() < 20000 )
			{
				m_mesh_create_array.push_back( hardware );
			}
	}

	void hardware_creator::insert_texture ( hardware_base* hardware )
	{
		US_LOCK_AUTO_MUTEX

			if ( hardware && m_text_create_array.size() < 20000 )
			{
				m_text_create_array.push_back( hardware );
			}
	}

	void hardware_creator::update( render_device* device, ulong v_time )
	{
		US_LOCK_AUTO_MUTEX
		
#ifdef _PERFORMANCE_TEST_

		long v_type = (long)v_time;
		if ( v_type > 0 )
		{
			ulong start_time = timeGetTime();
			ulong last_time = 0;

			int mesh_num = 0, text_num = 0;
			// mesh
			ulong avail_time = ulong( v_time * 0.5 );
			int ni = 0;
			for (  ;( ni < m_mesh_create_array.size() ) && ( last_time < avail_time ); ni++ )
			{
				hardware_base* hardware = m_mesh_create_array[ni];
				assert( NULL != hardware );
				hardware->create( device );

				last_time = timeGetTime() - start_time;

				mesh_num++;
			}
			if (ni < m_mesh_create_array.size())
			{
				int bbbb=1;
			}
			US_RENDER_TRACE3(  L"mesh create array count : %10d, vTime:%10d, LostCreate:%10d\n", m_mesh_create_array.size(),v_type,m_mesh_create_array.size()-ni);
			m_mesh_create_array.clear();

			// texture
			avail_time = v_time;
			for ( ni = 0; ( ni < m_text_create_array.size() ) && ( last_time < avail_time ); ni++ )
			{
				hardware_base* hardware = m_text_create_array[ni];
				assert( NULL != hardware );
				hardware->create( device );

				last_time = timeGetTime() - start_time;

				text_num++;
			}
			if (ni < m_text_create_array.size())
			{
				int bbbb=1;
			}
			US_RENDER_TRACE2(  L"texture create array count : %10d, LostCreate:%10d\n", m_text_create_array.size(),m_text_create_array.size()-ni);
			m_text_create_array.clear();


			US_RENDER_TRACE2( L"time : %10d, cost : %10d \n", v_time, last_time );
			US_RENDER_TRACE2( L"mesh : %10d, text : %10d \n", mesh_num, text_num );
		}
		else
		{

			// mesh
			int cur_create_num = system_environment::s_create_mesh_num;
			int create_num = (int)m_mesh_create_array.size();
			cur_create_num = min( create_num, cur_create_num );

			int cur_create_size = 0;
			int i = 0;
			for ( ; (i < cur_create_num) && (cur_create_size < system_environment::s_create_mesh_size) ; i++ )
			{
				hardware_base* hardware = m_mesh_create_array[i] ;
				assert( NULL != hardware );
				cur_create_size += hardware->create( device );
			}
			if (i<m_mesh_create_array.size())
			{
				int nSize = system_environment::s_create_mesh_size;
				nSize = 1;
			}
			
			//US_RENDER_TRACE1( L"mesh create array count : %d\n", m_mesh_create_array.size());
			US_RENDER_TRACE3(  L"mesh create array count : %10d, vTime:%10d,LostNum:%10d\n", m_mesh_create_array.size(),v_type,m_mesh_create_array.size()-i);
			m_mesh_create_array.clear();


			// texture
			cur_create_num = system_environment::s_create_texture_num;
			create_num = (int)m_text_create_array.size();
			cur_create_num = min( create_num, cur_create_num );

			cur_create_size = 0;
			for ( i = 0; ( i < cur_create_num ) && ( cur_create_size < system_environment::s_create_texture_size ); i++ )
			{
				hardware_base* hardware = m_text_create_array[i];
				assert( NULL != hardware );
				cur_create_size += hardware->create( device );
			}
			if (i<m_text_create_array.size())
			{
				int nSize = system_environment::s_create_texture_size ;
				nSize = 0;
			}
			US_RENDER_TRACE2( L"texture create array count : %10d,LostNum:%10d\n", m_text_create_array.size(),m_text_create_array.size()-i);
			m_text_create_array.clear();

			US_RENDER_TRACE0( L"time :\n" );
			US_RENDER_TRACE0( L"mesh :\n" );
			/*			US_RENDER_TRACE1( L"mesh_num : %10d \n", cur_create_num );		
			US_RENDER_TRACE1( L"text_num : %10d \n", cur_create_num );	*/		
		}
#else
		long v_type = (long)v_time;
		if ( v_type > 0 )
		{
			ulong start_time = timeGetTime();
			ulong last_time = 0;

			int mesh_num = 0, text_num = 0;
			// mesh
			ulong avail_time = ulong( v_time * 0.5 );
			for ( int ni = 0; ( ni < m_mesh_create_array.size() ) && ( last_time < avail_time ); ni++ )
			{
				hardware_base* hardware = m_mesh_create_array[ni];
				assert( NULL != hardware );
				hardware->create( device );

				last_time = timeGetTime() - start_time;

				mesh_num++;
			}
			m_mesh_create_array.clear();

			// texture
			avail_time = v_time;
			for ( int ni = 0; ( ni < m_text_create_array.size() ) && ( last_time < avail_time ); ni++ )
			{
				hardware_base* hardware = m_text_create_array[ni];
				assert( NULL != hardware );
				hardware->create( device );

				last_time = timeGetTime() - start_time;

				text_num++;
			}
			m_text_create_array.clear();
		}
		else
		{

			// mesh
			int cur_create_num = system_environment::s_create_mesh_num;
			int create_num = (int)m_mesh_create_array.size();
			cur_create_num = min( create_num, cur_create_num );

			int cur_create_size = 0;
			for ( int i = 0; (i < cur_create_num) && (cur_create_size < system_environment::s_create_mesh_size) ; i++ )
			{
				hardware_base* hardware = m_mesh_create_array[i] ;
				assert( NULL != hardware );
				cur_create_size += hardware->create( device );
			}

			m_mesh_create_array.clear();


			// texture
			cur_create_num = system_environment::s_create_texture_num;
			create_num = (int)m_text_create_array.size();
			cur_create_num = min( create_num, cur_create_num );

			cur_create_size = 0;
			for ( int i = 0; ( i < cur_create_num ) && ( cur_create_size < system_environment::s_create_texture_size ); i++ )
			{
				hardware_base* hardware = m_text_create_array[i];
				assert( NULL != hardware );
				cur_create_size += hardware->create( device );
			}
			m_text_create_array.clear();		
		}
#endif
	}

	void hardware_creator::clear( void )
	{
		US_LOCK_AUTO_MUTEX

		m_mesh_create_array.clear();

		m_text_create_array.clear();
	}
	
}