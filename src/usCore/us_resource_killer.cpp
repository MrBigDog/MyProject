///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_resource_killer.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : resource_killer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_resource_killer.h"
#include <usCore/us_resource_base.h>

namespace uniscope_globe
{
	// hardware
	//resource_killer* singleton_resource_killer::s_ins = NULL;
	DWORD tls_singleton_resource_killer::m_tls_index = TlsAlloc();

	resource_killer::resource_killer(void)
	{

	}

	resource_killer::~resource_killer(void)
	{
		//clear();
		while (m_list.size() > 0)
		{
			resource_base* v_res = (resource_base*)m_list.front();
			m_list.pop_front();
			AUTO_RELEASE_SHARED_DATA(v_res);
		}
	}

	void resource_killer::push(resource_base* v_ptr)
	{
		US_LOCK_AUTO_MUTEX

			//m_array.push_back( v_ptr );
			m_list.push_back(v_ptr);
	}

	void resource_killer::clear(ulong v_time)
	{
		US_LOCK_AUTO_MUTEX

			//for ( int i = 0; i < (int)m_array.size(); i++ )
			//{
			//	AUTO_RELEASE( m_array[i] );
			//}
			//m_array.clear();

			ulong vtt = v_time;
		if (long(vtt) < 1)
		{
			vtt = 3;
		}

		ulong start_time = timeGetTime();
		ulong last_time = 0;

		int kill_num = 0;
		while (m_list.size() > 0 && last_time < vtt)
		{
			resource_base* v_res = (resource_base*)m_list.front();
			m_list.pop_front();
			AUTO_RELEASE_SHARED_DATA(v_res);
			kill_num++;

			last_time = timeGetTime() - start_time;
		}

		//US_RENDER_TRACE2( L" kill num : %d, cost : %d \n", kill_num, last_time );

		//int v_num = 0;
		//int v_size = 0;
		//int v_range = 1<<23;
		//while ( m_list.size() > 0 && v_num < 50 && v_size < v_range )
		//{
		//	resource_base* v_res = (resource_base*)m_list.front();
		//	v_size += v_res->get_mem_size();
		//	m_list.pop_front();
		//	AUTO_RELEASE(v_res);
		//	v_num++;
		//}
	}



	// memory
	//memory_ptr_destroyer* singleton_memory_ptr_destroyer::s_ins = NULL;
	DWORD tls_singleton_memory_ptr_destroyer::m_tls_index = TlsAlloc();

	memory_ptr_destroyer::memory_ptr_destroyer(void)
	{

	}

	memory_ptr_destroyer::~memory_ptr_destroyer(void)
	{
		clear();
	}

	void memory_ptr_destroyer::push(object_base* v_ptr)
	{
		//US_LOCK_AUTO_MUTEX

		m_array.push_back(v_ptr);
	}

	void memory_ptr_destroyer::clear()
	{
		//US_LOCK_AUTO_MUTEX

		for (int i = 0; i < (int)m_array.size(); i++)
		{
			AUTO_DELETE(m_array[i]);
		}
		m_array.clear();
	}



}