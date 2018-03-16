
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shadow_map_render.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shadow_map_render class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_shadow_map_render.h"

#define  SHADOW_MAP_SIZE 1024

namespace uniscope_globe
{
	shadow_map_render::shadow_map_render( render_device* v_device )
	{
		//LPDIRECT3DDEVICE9 v_d3d9_device = (LPDIRECT3DDEVICE9)v_device->get_device();
		//if( FAILED( v_d3d9_device->CreateTexture(	SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 1,
		//											D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		//											D3DPOOL_DEFAULT, &m_pShadowMap,
		//											NULL ) ) )
		//{
		//	return;
		//}

		//// Grab the texture's surface
		//m_pShadowMap->GetSurfaceLevel( 0, &g_pShadowSurf );


		//if( FAILED( v_d3d9_device->CreateTexture(	SCREEN_WIDTH, SCREEN_HEIGHT, 1,
		//											D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pScreenMap, NULL ) ) )
		//{
		//	return;
		//}
		//// Grab the texture's surface
		//m_pScreenMap->GetSurfaceLevel( 0, & m_pScreenSurf );
		//d3d9_render_target* v_render_target = new d3d9_render_target(v_device);
		//v_render_target->create(v_device->m_d3d_present_param.BackBufferWidth, 
		//						v_device->m_d3d_present_param.BackBufferHeight,
		//						D3DFMT_R32F,
		//						v_device->m_d3d_present_param.MultiSampleType,
		//						v_device->m_d3d_present_param.MultiSampleQuality);




	}

	shadow_map_render::~shadow_map_render( void )
	{

	}

	void shadow_map_render::draw( render_argument* args )
	{
		d3d9_effect_common_mesh_with_shadow_map* v_effect = (d3d9_effect_common_mesh_with_shadow_map*)args->m_device->get_effect(0);

		////////////////////////////////////////////////////////////////
		// 设置矩阵
		///////////////////////////////////////////////////////////////
		{
			//
			// Light space matrices
			//
			render_argument v_args = *args;
			matrix4<float> v_mat_view = v_args.m_device->get_view_matrix();
			matrix4<float> v_mat_proj = v_args.m_device->get_projection_matrix();
			vector3<double> v_light_target_geo = v_args.m_observer->get_position_geo();
			vector3<double> v_pos_sphr = v_args.m_observer->get_position_sphr();
			float v_far_plane = v_pos_sphr.length();
			int v_create_count = 0;

			vector3<double> v_light_dir;
			v_light_dir.x = -math<double>::cos_( v_args.m_shadow_param.m_elevation_angle ) * math<double>::sin_( v_args.m_shadow_param.m_azimuth_angle );
			v_light_dir.y = -math<double>::sin_( v_args.m_shadow_param.m_elevation_angle );
			v_light_dir.z = -math<double>::cos_( v_args.m_shadow_param.m_elevation_angle ) * math<double>::cos_( v_args.m_shadow_param.m_azimuth_angle );
	

			spherical_transform st;
			st.create_from_sphr( v_pos_sphr.x, v_pos_sphr.y, v_pos_sphr.z, 0,  0, 0.0 );

			vector3<double> v_light_dir_geo = matrix4<double>::rotate_normal( v_light_dir, st.m_mat_tnb ); 
			v_light_dir_geo.normalize();

			vector3<double> v_light_pos_geo = -v_light_dir_geo * 1024 + v_light_target_geo;
			vector3<double> v_light_up_normal_geo = vector_3d::normalize(v_light_target_geo);
			vector3<double> v_light_right_geo = vector_3d::cross( v_light_up_normal_geo, -v_light_dir);
			v_light_dir_geo.normalize();

			vector3<double> v_light_up_geo = vector_3d::cross( v_light_right_geo, -v_light_dir);
			v_light_up_geo.normalize();

			matrix4<double> mat_light_view = matrix4<double>::look_at(v_light_pos_geo, v_light_target_geo, v_light_up_geo );

			// Compute the projection matrix
			matrix4<double> mat_light_projection = matrix4<double>::orthographic(45.0f, 45.0f, 1.0f, 1024.0f);
			//D3DXMatrixOrthoLH( &matProj, 45.0f, 45.0f, 1.0f, 1024.0f );

			// Compute the light-view-projection matrix
			matrix4<double> mat_light_view_projection = mat_light_view * mat_light_projection;

			// Compute the texture matrix
			float tex_offset = 0.5 + (0.5 / (float)SHADOW_MAP_SIZE);
			matrix4<double> mat_tex_adjustment(	0.5f,		0.0f,	0.0f,	0.0f,
												0.0f,	   -0.5f,	0.0f,	0.0f,
												0.0f,		0.0f,	1.0f,	0.0f,
												tex_offset, tex_offset,  0.0f, 1.0f );

			matrix4<double> matTexture = mat_light_view_projection * mat_tex_adjustment;


			v_effect->set_view_matrix(matrix4<float>((double*)&v_mat_view));
			v_effect->set_projection_matrix(matrix4<float>((double*)&v_mat_proj));
			v_effect->set_light_view_projection_matrix(matrix4<float>((double*)&mat_light_view_projection));

			v_effect->set_light_position(vector_3f((double*)&v_light_pos_geo));
		}

		////////////////////////////////////////////////////////////////
		// 绘制阴影
		////////////////////////////////////////////////////////////////
		render_target* v_render_target = (render_target*)args->m_device->get_render_target(0);
		v_render_target->begin();
		{
			v_effect->set_shadow_map_technique();

			//v_effect->set_world_matrix();

			

			uint v_pass_count = 0;
			v_effect->begin(&v_pass_count, 0);
			for(int ni = 0; ni < v_pass_count; ni++ )
			{
				v_effect->begin_pass( ni );
				draw_scene( args );
				v_effect->end_pass();
			}
			v_effect->end();
		}
		v_render_target->end();

		////////////////////////////////////////////////////////////////
		// 绘制场景
		////////////////////////////////////////////////////////////////
		{			
			v_effect->set_scene_technique();
			uint v_pass_count = 0;
			v_effect->begin(&v_pass_count, 0);
			for(int ni = 0; ni < v_pass_count; ni++ )
			{
				v_effect->begin_pass( ni );
				v_effect->set_shadow_map_texture( 0, v_render_target->get_texture() );
				draw_scene( args );
				v_effect->end_pass();
			}
			v_effect->end();
		}
	}

	void shadow_map_render::draw_scene( render_argument* args )
	{
		render_node::draw( args );
	}
	
}