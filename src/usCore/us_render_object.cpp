///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_render_object.h"

namespace uniscope_globe
{
	render_object::render_object(void)
	{
		m_render_device = NULL;		
		m_view_distance = 0.0;
		m_render_priority = 5000.0;
		m_shadow_color = 0xaa000000;
		m_draw_order = 0x7fffffff;
	}

	render_object::~render_object( void )
	{

	}

	int render_object::compare_fun(const void* ptr1, const void* ptr2)
	{
		render_object** ro1 = (render_object**) ptr1;
		render_object** ro2 = (render_object**) ptr2;

		if( (*ro1)->m_view_distance <= (*ro2)->m_view_distance ) 
			return 1;
		else 
			return -1;
	}

	void render_object::copy_from( render_object* v_src_data )
	{
		m_render_aabb = v_src_data->m_render_aabb;

		m_time_stamp = v_src_data->m_time_stamp;

		m_shadow_color = v_src_data->m_shadow_color;

		m_shadow_type = v_src_data->m_shadow_type;

		renderable_base::copy_from(v_src_data);
	}
}