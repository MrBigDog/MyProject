///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_reclaimer_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : reclaim_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_reclaim_base.h"
#include <usCore/us_document_base.h>
//#include <usCore/us_resource_reclaimer.h>
#include <usCore/us_frame_counter.h>

namespace uniscope_globe
{
	reclaim_base::reclaim_base(document_base* in_doc)
	{
		m_can_reclaim = true;
		m_size = 0;
		m_param = 1.0;
		m_last_call_time = 0;
		m_update_count = 0;
		m_document = in_doc;
		m_resource_reclaimer = m_document->get_resource_reclaimer();
	}

	reclaim_base::~reclaim_base(void)
	{
	}

	void reclaim_base::update_size(ULONG_PTR v_size)
	{
		ULONG_PTR v_sub = v_size - m_size;
		m_size = v_size;
		//m_last_call_time = system_environment::s_frame_time;

		m_resource_reclaimer->increase(v_sub);
	}

	void reclaim_base::refresh(ULONG_PTR v_size)
	{
		update_size(v_size);

		m_last_call_time = m_document->get_frame_counter()->m_logic_time;

		m_resource_reclaimer->push_front(this);
	}

	// remove from reclaim list
	void reclaim_base::destroy(void)
	{
		//if ( __use_count__ == 0 )
		if (__use_count__ == 1)
		{
			m_resource_reclaimer->erase(this);
			m_resource_reclaimer->decrease(m_size);
			release();
		}
		else
		{
			m_resource_reclaimer->move_to_head(this);
			//m_resource_reclaimer->erase( this );
			m_resource_reclaimer->decrease(m_size);
			m_size = 0;
		}
	}

	void reclaim_base::enable_reclaim(bool v_enable)
	{
		m_can_reclaim = v_enable;
	}

	bool reclaim_base::can_reclaim(int v_life_time)
	{
		if (!m_can_reclaim)
			return m_can_reclaim;

		//time_value v_interval = system_environment::s_frame_time - m_last_call_time;
		time_value v_interval = m_document->get_frame_counter()->m_logic_time - m_last_call_time;
		double v_lt = v_life_time * m_param;
		//v_lt = max( 3.0, v_lt );
		if (v_interval < v_lt)
		{
			return false;
		}

		return true;
	}

	void reclaim_base::set_reclaim_param(double v_param)
	{
		m_param = v_param;
	}

	double reclaim_base::get_reclaim_param(void)
	{
		return m_param;
	}

}