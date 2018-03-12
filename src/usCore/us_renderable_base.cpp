
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_renderable_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : renderable_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_renderable_base.h"

namespace uniscope_globe
{

	renderable_base::renderable_base( void )
	{
		m_blend_color  = 0x00000000;
		m_alpha_blend = true;
		m_depth_enable = true;
		m_depth_bias = 0.0f;
		m_draw_order = 0;

		// 0 表示普通渲染， 1 表示高亮渲染
		m_render_type_state = 0;

		m_last_time = 0;
		m_interval_time = 0;
		m_high_light_color = 0xffff0000;

		m_flash_start_time = 0.0;
		m_light_enable = 0;
	}

	renderable_base::~renderable_base( void )
	{

	}

	void renderable_base::copy_from( renderable_base* v_src_data )
	{
		m_blend_color = v_src_data->m_blend_color;

		m_depth_enable = v_src_data->m_depth_enable;

		m_alpha_blend = v_src_data->m_alpha_blend;

		m_depth_bias = v_src_data->m_depth_bias;

		m_draw_order = v_src_data->m_draw_order;

		m_light_enable = v_src_data->m_light_enable;
	}

	void renderable_base::show_high_light( double v_last_time, double v_interval_time, ulong v_color )
	{
		m_render_type_state = 1;
		m_last_time = v_last_time;
		m_interval_time = v_interval_time;
		//if ( m_interval_time == 0 )
		//{
		//	m_interval_time = 0.05;
		//}

		m_high_light_color = v_color;
	}

	void renderable_base::stop_high_light()
	{
		m_render_type_state = 0;

		m_last_time = 0;
		m_interval_time = 0;
		m_high_light_color = 0xffff0000;

		m_flash_start_time = 0.0;
	}

	bool renderable_base::draw_high_light( render_argument* v_args )
	{
		if ( m_render_type_state == 0 )
			return true;
		
		if ( m_flash_start_time == 0.0 )
		{
			m_flash_start_time = v_args->m_frame_counter->m_frame_time;
		}

		time_value time_segment = ( v_args->m_frame_counter->m_frame_time - m_flash_start_time );
		if ( time_segment > m_last_time )
		{
			m_render_type_state = 0;

			m_last_time = 0;
			m_interval_time = 0;
			m_high_light_color = 0xffff0000;
			m_flash_start_time = 0;

			return true;
		}
		
		if( m_interval_time == 0.0)
		{
			return true;
		}
		else
		{
			int v_v = time_segment / m_interval_time;
			if ( v_v%2 == 0 )
				return true;
		}


		return false;
	}

	
}