///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_resource_reclaimer.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : resource_reclaimer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_resource_reclaimer.h"
#include <usCore/us_system_environment.h>
#include <usCore/us_reclaim_base.h>
#include <psapi.h>

namespace uniscope_globe
{
	DWORD singleton_resource_reclaimer::m_tls_index = TlsAlloc();

	resource_reclaimer::resource_reclaimer(void)
	{
		m_max_memory_size = 0L;
		m_last_cost_time = 0.0;
	}

	resource_reclaimer::~resource_reclaimer(void)
	{
		//reclaim_base* itr = dynamic_cast<reclaim_base*>( pop_tail() );
		//while ( itr )
		//{	
		//	AUTO_RELEASE( itr );
		//	
		//	//if( itr->m_use_count == 0 )
		//	//{
		//	//	AUTO_RELEASE( itr );
		//	//}
		//	//else
		//	//{
		//	//	push_head( itr );
		//	//}

		//	itr = dynamic_cast<reclaim_base*>( pop_tail() );
		//}
	}

	//void resource_reclaimer::update( void )
	//{
	//	US_LOCK_AUTO_MUTEX
	//			
	//	double v_reclaim_size = system_environment::s_reclaim_memory_size;
	//	double v_check_rate = m_max_memory_size / v_reclaim_size;

	//	int v_checked_num = 30;
	//	
	//	int v_life_time = system_environment::s_resource_life_time * ( 1.0 - v_check_rate * 2 );
	//	v_life_time = (v_life_time<3)?3:v_life_time;

	//	double v_cur_reclaim_size = 0.0;
	//	
	//	reclaim_base* v_tail = (reclaim_base*)chain_tail;
	//	while( v_checked_num>0  && v_tail != NULL &&  v_cur_reclaim_size < 3000000 )
	//	{
	//		if ( v_tail->can_reclaim( v_life_time ) )
	//		{
	//			//pop_tail();
	//			v_cur_reclaim_size += v_tail->get_size();

	//			v_tail->destroy();
	//		}
	//		else
	//		{
	//			move_to_head( v_tail );
	//		}
	//		
	//		v_tail = (reclaim_base*)chain_tail;

	//		v_checked_num--;
	//	}
	//}

	//**@add by felix 记录当前进程的内存使用情况*/
	ULONG_PTR resource_reclaimer::showMemoryInfo()
	{
		HANDLE handle = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
		return pmc.WorkingSetSize;
		//pmc.WorkingSetSize/1000 <<"K/"<<pmc.PeakWorkingSetSize/1000<<"K + "<<pmc.PagefileUsage/1000 <<"K/"<<pmc.PeakPagefileUsage/1000 ;
	}

	void resource_reclaimer::update(void)
	{
		US_LOCK_AUTO_MUTEX

			//double start_time = timeGetTime();

			double v_reclaim_size = system_environment::s_reclaim_memory_size;
		double v_check_rate = m_max_memory_size / v_reclaim_size;

		int v_checked_num = 30;

		int v_life_time = system_environment::s_resource_life_time * (1.0 - v_check_rate) * (1.0 - v_check_rate);
		v_life_time = (v_life_time < 3) ? 3 : v_life_time;

		system_environment::s_max_process_memory_size = showMemoryInfo();
		//modify begin by felix 此处注释掉，32位和64位使用相同算法，注意设置好最大内存
		v_life_time = (v_life_time > system_environment::s_resource_life_time) ? system_environment::s_resource_life_time : v_life_time;

		//modify end by felix
		v_checked_num = chain_size;// * v_check_rate * 1.5;
		//v_checked_num = max( v_checked_num, 128 );

		double v_cur_reclaim_size = 0.0;

		reclaim_base* v_tail = (reclaim_base*)chain_tail;

		//modify by felix
#ifdef WIN64
		while (v_checked_num > 0 && v_tail != NULL &&  v_cur_reclaim_size < system_environment::s_reclaim_memory_per_frame)
#else
		while (v_checked_num > 0 && v_tail != NULL &&  v_cur_reclaim_size < 30000000)
#endif
		{
			if (v_tail->can_reclaim(v_life_time))
			{
				//pop_tail();
				v_cur_reclaim_size += v_tail->get_size();

				v_tail->destroy();
			}
			else
			{
				move_to_head(v_tail);
			}

			v_tail = (reclaim_base*)chain_tail;

			v_checked_num--;
		}

		//double end_time = timeGetTime();
		//double v_time = end_time - start_time;

		//ustring str = ustring(L"time: ") + string_ext::wfrom_double( v_time )
		//			+ L" || rate: " + string_ext::wfrom_double( v_check_rate )
		//			+ L" || size: " + string_ext::wfrom_int( chain_size * 0.5 );
		//tls_singleton_downloader::instance().write( str.c_str() );
	}

	void resource_reclaimer::push_front(reclaim_base* reclaim)
	{
		US_LOCK_AUTO_MUTEX

			move_to_head(reclaim);
	}

	void resource_reclaimer::remove(reclaim_base* reclaim)
	{
		US_LOCK_AUTO_MUTEX

			erase(reclaim);

		m_max_memory_size -= reclaim->get_size();
	}

	void resource_reclaimer::increase(ULONG_PTR v_size)
	{
		m_max_memory_size += v_size;
	}

	void resource_reclaimer::decrease(ULONG_PTR v_size)
	{
		m_max_memory_size -= v_size;
	}

	ULONG_PTR resource_reclaimer::get_memory_size(void)
	{
		return m_max_memory_size;
	}


}