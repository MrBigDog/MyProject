///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_manager.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "us_d3d9_effect_manager.h"

namespace uniscope_globe
{
	d3d9_effect_manager::d3d9_effect_manager( render_device* rd )
	{
		m_effect_array.resize( US_EFFECT_COUNT );

		m_effect_array[US_EFFECT_VERTEX_BLEND] = new d3d9_effect_vertex_blend(rd, IDR_SHADER_VERTEX_BLEND );
		
		m_effect_array[US_EFFECT_SKY_SPHERE] = new d3d9_effect_sky_sphere(rd);
		
		m_effect_array[US_EFFECT_TERRAIN_RENDER] = new d3d9_effect_terrain_blend(rd);
		
		m_effect_array[US_EFFECT_REFLECTION_SURFACE] = new d3d9_effect_reflection_surface(rd);

		m_effect_array[US_EFFECT_REFRACTION_SURFACE] = new d3d9_effect_reflection_surface_ex(rd);
		
		//static d3d9_effect_flow_direction dir(rd);
		m_effect_array[US_EFFECT_FLOW_DIR] = new d3d9_effect_flow_direction(rd);

		m_effect_array[US_EFFECT_FLOOD_ANALYSIS] = new d3d9_effect_flood(rd);
		
		m_effect_array[US_EFFECT_VERTEX_BLEND_EX] = new d3d9_effect_vertex_blend(rd, IDR_SHADER_VERTEX_BLEND_EX );
		
		m_effect_array[US_EFFECT_SHADOW_VOLUME] = new d3d9_effect_shadow_volume(rd);

		m_effect_array[US_EFFECT_COMMON_MESH_FP] = new d3d9_effect_common_mesh_fp(rd);
		
		m_effect_array[US_EFFECT_GEOMETRY_LINE_FLAKE] = new d3d9_effect_render_object_line_flake(rd);
		
		m_effect_array[US_EFFECT_GEOMETRY_LINE_VOLUME] = new d3d9_effect_render_object_line_volume(rd);
		
		m_effect_array[US_EFFECT_PP_COMBINE_HDR] = new d3d9_effect_pp_combine_hdr(rd);

		m_effect_array[US_EFFECT_PP_DOWN_FILTER_4X] = new d3d9_effect_pp_down_filter(rd);

		m_effect_array[US_EFFECT_PP_UP_FILTER_4X] = new d3d9_effect_pp_up_filter(rd);

		m_effect_array[US_EFFECT_PP_GAUSSIAN_BLUR_H] = new d3d9_effect_pp_gaussian_blur_h(rd);

		m_effect_array[US_EFFECT_PP_GAUSSIAN_BLUR_V] = new d3d9_effect_pp_gaussian_blur_v(rd);

		m_effect_array[US_EFFECT_PP_COMBINE_DOF] = new d3d9_effect_pp_combine_dof(rd);	

		m_effect_array[US_EFFECT_PP_TONE_MAP] = new d3d9_effect_pp_tone_map(rd);	

		m_effect_array[US_EFFECT_PP_COMBINE_HSL] = new d3d9_effect_pp_combine_hsl(rd);


		m_effect_array[US_EFFECT_COMMON_MESH_GPU] = new d3d9_effect_common_mesh_gpu(rd);

		
		//file_buffer f_buf;
		//f_buf.file_open( L"c:\\effect_init.txt", L"wt" );
		//for ( int ni = 0; ni < m_effect_array.size(); ni++ )
		//{


		//	wchar_t tmp[256];
		//	memset( tmp, 0, 256 );
		//	wsprintf( tmp, L"%d_%d \n", ni, m_effect_array[ni] );
		//	f_buf.write( wstring(tmp) );
		//}
		//f_buf.file_close();
	}


	d3d9_effect_manager::~d3d9_effect_manager( void )
	{
		render_effect_array::iterator itr = m_effect_array.begin();
		for( ; itr != m_effect_array.end(); itr++ )
		{
			AUTO_DELETE( *itr );
		}
		m_effect_array.clear();
	}

	ULONG_PTR d3d9_effect_manager::get_effect( long effect_handle )
	{
		return (ULONG_PTR)(m_effect_array[effect_handle]);
	}

	long d3d9_effect_manager::on_lost_device( void )
	{
		render_effect_array::iterator itr = m_effect_array.begin();
		for( ; itr != m_effect_array.end(); itr++ )
		{
			(*itr)->on_lost_device();
		}
		return S_OK;
	}

	long d3d9_effect_manager::on_reset_device( void )
	{
		render_effect_array::iterator itr = m_effect_array.begin();
		for( ; itr != m_effect_array.end(); itr++ )
		{
			(*itr)->on_reset_device();
		}
		return S_OK;
	}
}