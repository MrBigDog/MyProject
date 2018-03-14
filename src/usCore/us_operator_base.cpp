///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_operator_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : operator_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_operator_base.h"
#include <usCore/us_operator_executant.h>

namespace uniscope_globe
{
	operator_base::operator_base(void)
	{
		m_target_object = NULL;

		m_executant = NULL;

		m_is_end = false;
	}

	operator_base::~operator_base(void)
	{

	}

	void operator_base::on_begin(time_value current_time)
	{
		m_begin_time = current_time;
	}

	void operator_base::on_execute(time_value current_time)
	{

	}

	void operator_base::on_draw(render_device* device)
	{

	}

	void operator_base::on_end(time_value current_time)
	{

	}

	void operator_base::unregister_operator(void)
	{
		m_executant->unregister_operator(this);
	}

	void operator_base::set_target_object(object_base* object)
	{
		m_target_object = object;
	}

	object_base* operator_base::get_target_object(void)
	{
		return m_target_object;
	}

}