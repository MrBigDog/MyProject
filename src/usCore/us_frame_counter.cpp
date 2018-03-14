///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_frame_counter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : frame_counter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_frame_counter.h"
#include <wtypes.h>

namespace uniscope_globe
{
	DWORD tls_singleton_frame_counter::m_tls_index = TlsAlloc();

	frame_counter::frame_counter(void)
	{
		m_frame_time = 0.0;
		m_last_frame_time = 0.0;
		m_last_frame_tick_count = 0.0;
		m_frame_time_tick = 0.0;

		m_update_count = 0;
		m_update_frame_count = 0;
		m_update_logic_count = 0;
		m_frame_per_sec = 0.0;
		m_logic_per_sec = 0.0;

		m_logic_time = 0.0;
		m_last_logic_time = 0.0;
		m_last_logic_tick_count = 0.0;
		m_logic_time_tick = 0.0;
		m_frame_count = 0;

		LARGE_INTEGER v_tick_per_sec;
		::QueryPerformanceFrequency(&v_tick_per_sec);
		m_ticks_per_sec = v_tick_per_sec.QuadPart;
	}

	frame_counter::~frame_counter(void)
	{

	}

	void frame_counter::update_frame_time(void)
	{
		m_update_frame_count++;
		m_frame_count++;
		m_frame_time = get_current_time();
		//m_frame_time = (double)timeGetTime() * 0.001;
		m_last_frame_tick_count = m_frame_time - m_last_frame_time;
		m_last_frame_time = m_frame_time;

		// ´óÓÚ1Ãë£¬¼ÆËãFPS
		if ((m_frame_time - m_frame_time_tick) > 1.0)
		{
			m_frame_per_sec = m_update_frame_count / (m_frame_time - m_frame_time_tick);
			m_update_frame_count = 0;
			m_frame_time_tick = m_frame_time;
		}
	}

	void frame_counter::update_logic_time(void)
	{
		m_update_count++;
		m_update_logic_count++;
		//m_logic_time = (double)timeGetTime() * 0.001;
		m_logic_time = get_current_time();
		m_last_logic_tick_count = m_logic_time - m_last_logic_time;
		m_last_logic_time = m_logic_time;

		if ((m_logic_time - m_logic_time_tick) > 1.0)
		{
			m_logic_per_sec = m_update_logic_count / (m_logic_time - m_logic_time_tick);
			m_update_logic_count = 0;
			m_logic_time_tick = m_logic_time;
		}
	}

	double frame_counter::get_current_time(void)
	{
		LARGE_INTEGER v_time;
		QueryPerformanceCounter(&v_time);
		return v_time.QuadPart / (double)m_ticks_per_sec;
	}

	double frame_counter::get_elapsed_frame_time(void)
	{
		return this->get_current_time() - this->m_frame_time;
	}

}