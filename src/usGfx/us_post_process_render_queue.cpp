
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_post_process_render_queue.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : post_process_render_queue class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_post_process_render_queue.h"
#include <usGfx/us_d3d9_effect_manager.h>
#include <usGfx/us_d3d9_render_target_manager.h>
#include <usGfx/us_d3d9_effect_pp_combine_hsl.h>
#include <usGfx/us_d3d9_vertex_declear.h>
#include <usGfx/us_d3d9_render_target.h>
#include <usCore/us_render_argument.h>
#include <usCore/us_render_device.h>
#include <usCore/us_system_environment.h>

namespace uniscope_globe
{
	post_process_render_queue::post_process_render_queue(render_device* device)
	{
		//m_render_effect = new d3d9_effect_pp_hdr(device);
		//m_render_target_chain = new d3d9_render_target_chain(device);
		//m_render_target_chain->create()


		m_pp_effect_index_array.push_back(US_EFFECT_PP_TONE_MAP);
		//m_pp_effect_index_array.push_back(US_EFFECT_PP_DOWN_FILTER_4X);
		//m_pp_effect_index_array.push_back(US_EFFECT_PP_GAUSSIAN_BLUR_H);
		//m_pp_effect_index_array.push_back(US_EFFECT_PP_GAUSSIAN_BLUR_H);
		//m_pp_effect_index_array.push_back(US_EFFECT_PP_GAUSSIAN_BLUR_V);
		//m_pp_effect_index_array.push_back(US_EFFECT_PP_GAUSSIAN_BLUR_V);
		//m_pp_effect_index_array.push_back(US_EFFECT_PP_UP_FILTER_4X);


		m_pp_render_target = NULL;
	}

	post_process_render_queue::~post_process_render_queue(void)
	{
		AUTO_DELETE(m_pp_render_target)
	}

	//*
	void post_process_render_queue::draw(render_argument* args)
	{
		if (system_environment::hsl_adjustment_enable)
		{
			draw_hsl(args);
		}
		else
		{
			render_node::draw(args);
		}

	}
	//*/

	//*
	void post_process_render_queue::draw_hsl(render_argument* args)
	{
		render_device* device = args->m_device;
		observer_base* in_observer = args->m_observer;

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// vertext buffer
		view_port v_view_port = device->get_view_port();
		rect v_rect(0, 0, v_view_port.m_width, v_view_port.m_height);

		transformed_texture vertex_array[4];
		vertex_array[0].x = v_rect.m_left, vertex_array[0].y = v_rect.m_top, vertex_array[0].z = 0, vertex_array[0].rhw = 1.0f, vertex_array[0].tu1 = 0.0f, vertex_array[0].tv1 = 0.0f;
		vertex_array[1].x = v_rect.m_right, vertex_array[1].y = v_rect.m_top, vertex_array[1].z = 0, vertex_array[1].rhw = 1.0f, vertex_array[1].tu1 = 1.0f, vertex_array[1].tv1 = 0.0f;
		vertex_array[2].x = v_rect.m_left, vertex_array[2].y = v_rect.m_bottom, vertex_array[2].z = 0, vertex_array[2].rhw = 1.0f, vertex_array[2].tu1 = 0.0f, vertex_array[2].tv1 = 1.0f;
		vertex_array[3].x = v_rect.m_right, vertex_array[3].y = v_rect.m_bottom, vertex_array[3].z = 0, vertex_array[3].rhw = 1.0f, vertex_array[3].tu1 = 1.0f, vertex_array[3].tv1 = 1.0f;


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// render scene
		d3d9_render_target* v_scene_render_target = (d3d9_render_target*)device->get_render_target(US_RENDER_TARGET_COLOR_ADJUSTMENT);
		v_scene_render_target->begin();
		{
			device->clear(0x00000000, 1.0, 2);
			render_node_array::iterator itr = m_child_node_array.begin();
			for (; itr != m_child_node_array.end(); itr++)
			{
				(*itr)->draw(args);
			}
		}
		v_scene_render_target->end();

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//// do filter;
		//d3d9_render_target_chain* v_pp_render_target = (d3d9_render_target_chain*)device->get_render_target(US_RENDER_TARGET_POST_PROCESS);
		//v_pp_render_target->begin();
		//{
		//	float v_extent_x = 1.0f;
		//	float v_extent_y = 1.0f;
		//	for( int ni = 0; ni < m_pp_effect_index_array.size(); ni++ )
		//	{
		//		v_pp_render_target->set_render_target(0);

		//		d3d9_effect_pp* v_effect = (d3d9_effect_pp*)device->get_effect(m_pp_effect_index_array[ni]);
		//		v_effect->set_technique();
		//		v_effect->begin(NULL, NULL);
		//		{
		//			HRESULT hr = S_OK;
		//			// the Scene was save in m_pSceneSave, if is first do Post Process, Set src is m_pSceneSave
		//			if(ni == 0)
		//			{
		//				v_effect->set_src_texture(v_scene_render_target->get_texture());
		//			}
		//			else
		//			{
		//				v_effect->set_src_texture(v_pp_render_target->get_texture());
		//			}
		//			v_effect->commit_changes();

		//			float v_scale_x = v_effect->get_scale_x();
		//			float v_scale_y = v_effect->get_scale_y();

		//			if( vertex_array[1].tu1 != v_extent_x )
		//			{
		//				vertex_array[1].tu1 = vertex_array[3].tu1 = v_extent_x;
		//				vertex_array[2].tv1 = vertex_array[3].tv1 = v_extent_y;
		//			}
		//			if( v_scale_x != 1.0f )
		//			{
		//				vertex_array[1].x = (vertex_array[1].x + 0.5f * v_view_port.m_width) * v_scale_x - 0.5f * v_view_port.m_width;
		//				vertex_array[3].x = (vertex_array[3].x + 0.5f * v_view_port.m_width) * v_scale_x - 0.5f * v_view_port.m_width;
		//				vertex_array[2].y = (vertex_array[2].y + 0.5f * v_view_port.m_height) * v_scale_y - 0.5f * v_view_port.m_height;
		//				vertex_array[3].y = (vertex_array[3].y + 0.5f * v_view_port.m_height) * v_scale_y - 0.5f * v_view_port.m_height;
		//			}
		//			v_extent_x *= v_scale_x;
		//			v_extent_y *= v_scale_y;

		//			v_effect->begin_pass(0);
		//			v_effect->get_device()->set_vertex_declaration( transformed_texture::fvf );
		//			v_effect->get_device()->draw_triangle_strip( (void*)vertex_array, 4, transformed_texture::stride );
		//			v_effect->end_pass();

		//		}
		//		v_effect->end();
		//		v_pp_render_target->flip();
		//	}
		//}
		//v_pp_render_target->end();

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// final combine
		d3d9_effect_pp_combine_hsl* v_render_effect = (d3d9_effect_pp_combine_hsl*)device->get_effect(US_EFFECT_PP_COMBINE_HSL);
		v_render_effect->set_technique();
		v_render_effect->begin(0, 0);
		{
			v_render_effect->begin_pass(0);
			v_render_effect->set_saturation(system_environment::full_screen_saturation);
			v_render_effect->set_luminance(system_environment::full_screen_luminance);
			v_render_effect->set_contract(system_environment::full_screen_contract);
			v_render_effect->set_src_texture(v_scene_render_target->get_texture());
			//v_render_effect->set_dest_texture(v_pp_render_target->get_texture());
			v_render_effect->commit_changes();

			device->set_vertex_declaration(transformed_texture::fvf);
			device->draw_triangle_strip((void*)vertex_array, 4, transformed_texture::stride);

			v_render_effect->end_pass();
		}
		v_render_effect->end();
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//render_node::draw( args );
	}
	//*/
}