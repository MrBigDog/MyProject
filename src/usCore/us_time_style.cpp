///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_time_style.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : time_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_time_style.h"
#include <usUtil/us_math.h>

namespace uniscope_globe
{
	time_style::time_style()
	{
		m_start_time = 0.0;
		m_duration = 0.0;
		m_interval = 0.0;
		m_cursor = 0;
	}

	time_style::time_style(time_style& st)
	{
		m_start_time = 0.0;
		m_duration = st.m_duration;
		m_interval = st.m_interval;
		m_cursor = 0;
	}

	time_style::~time_style(void)
	{
	}

	void time_style::start(double v_start)
	{
		m_start_time = v_start;
	}

	void time_style::set_parameter(double v_duration, double v_interval)
	{
		m_duration = v_duration;
		m_interval = v_interval;
	}

	bool time_style::valid_cursor(double v_cur_time)
	{
		if ((m_duration - m_start_time) < math_d::s_zero_tolerance || m_interval < math_d::s_zero_tolerance)
			return true;

		int v_cursor = (v_cur_time - m_start_time) / m_interval;

		if (m_cursor == v_cursor)
			return false;

		m_cursor = v_cursor;

		return true;
	}


}