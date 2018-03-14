///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_filter_manager.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : filter_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_filter_manager.h"
#include <usUtil/us_object_base.h>
#include <usUtil/us_raw_buffer.h>
#include <usCore/us_filter_base.h>

namespace uniscope_globe
{
	filter_manager* singleton_filter_manager::s_ins = NULL;

	filter_manager::filter_manager(void)
	{

	}

	filter_manager::~filter_manager(void)
	{
		clear();
	}

	object_base* filter_manager::parse(raw_buffer* in_buffer)
	{
		ulong four_cc = *((ulong*)in_buffer->get_buffer());

		filter_base* filter = get_filter(four_cc);
		if (filter != NULL)
		{
			return filter->parse(in_buffer);
		}

		return NULL;
	}

	filter_base* filter_manager::get_filter(ulong data_type)
	{
		return m_filter_map[data_type];
	}


	void filter_manager::register_filter(ulong data_type, filter_base* filter)
	{
		m_filter_map[data_type] = filter;
		filter->set_manager(this);
	}

	void filter_manager::clear(void)
	{
		filter_map::iterator itr = m_filter_map.begin();
		for (; itr != m_filter_map.end(); itr++)
		{
			AUTO_DELETE(itr->second);
		}

		m_filter_map.clear();
	}
}