///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shadow_render.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shadow_render class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_shadow_render.h"

namespace uniscope_globe
{

	shadow_render::shadow_render( void )
	{
		m_current_render_map = new material_map;

		m_background_render_map = new material_map;

		m_clipper_render = NULL;
	}

	shadow_render::~shadow_render( void )
	{
		clear();
	}

	void shadow_render::set_clipper_render( shadow_clipper_render* v_render )
	{
		m_clipper_render = v_render;
	}

	shadow_clipper_render* shadow_render::get_clipper_render()
	{
		return m_clipper_render;
	}

	void shadow_render::push( render_object* in_object )
	{
		ulong v_shadow_color = 0;
		int v_shadow_render_type = 0;

		shadow_volume_creator* v_shadow_cast = dynamic_cast<shadow_volume_creator*>(in_object);
		if ( v_shadow_cast != NULL )
		{
			v_shadow_color =  in_object->get_shadow_color();
			v_shadow_render_type = 0;
		}
		else
		{
			v_shadow_render_type = 1;
			v_shadow_color = 0xaa000000;
		}

		ulong v_draw_order = in_object->get_draw_order();
		shadow_render_queue* v_render_queue = NULL;
		material_key_pair v_key = std::make_pair(v_draw_order, v_shadow_color);

		material_map::iterator itr = m_background_render_map->find( v_key );
		if( itr != m_background_render_map->end() )
		{
			v_render_queue = itr->second;
		}
		else
		{
			v_render_queue = new shadow_render_queue;

			(*m_background_render_map)[v_key] = v_render_queue;
		}
		v_render_queue->push(in_object);
	}

	void shadow_render::flush( void )
	{
		// begin flip render_object
		{
			US_LOCK_AUTO_MUTEX

				material_map* temp_map = m_current_render_map;

			m_current_render_map = m_background_render_map;

			m_background_render_map = temp_map;
		}
		clear_background();

		if ( m_clipper_render != NULL )
		{
			m_clipper_render->flush();
		}

		render_node::flush();
	}

	int shadow_render::get_count( void ) 
	{ 
		return (int)m_current_render_map->size();
	}

	void shadow_render::draw( render_argument* args )
	{
		US_LOCK_AUTO_MUTEX

			if ( m_current_render_map->size() == 0 )
			{
				if ( m_clipper_render == NULL )
				{
					draw_normal_scene( args );

					return;
				}
				
				if ( m_clipper_render->get_status() == 0 )
				{
					draw_normal_scene( args );

					return;
				}
			}
			
			// save current projection matrix
			matrix4<float> mat_proj_old = args->m_device->get_projection_matrix();

			double v_fov = args->m_observer->get_fov();
			double v_aspect = args->m_observer->get_aspect();
			double v_near_plane = args->m_observer->get_near_plane();
			double v_far_plane = args->m_observer->get_far_plane();
			// set projection matrix

			//matrix4<double> mat_proj = matrix4<double>::perspective( v_fov, v_aspect, v_near_plane, args->m_observer->get_position_geo().length() );
			//args->m_device->set_projection_matrix( matrix4<float>(mat_proj.m) );

			// draw scene pass one
			draw_scene_pass_one( args );

			// draw shadow volume
			draw_shadow_volume( args );

			// draw clipper 
			draw_shadow_clipper_volume( args );

			//device->set_projection_matrix( mat_proj_old );

			//device->clear_zbuffer();

			// draw scene pass two
			//draw_scene_pass_two( args->m_observer, args->m_device );

			
			
	}

	void shadow_render::clear_background( void )
	{
		material_map::iterator itr = m_background_render_map->begin();
		for( ; itr != m_background_render_map->end(); itr++ )
		{
			AUTO_DELETE( itr->second );
		}

		m_background_render_map->clear();
	}

	void shadow_render::clear( void )
	{
		material_map::iterator itr = m_current_render_map->begin();
		for( ; itr != m_current_render_map->end(); itr ++ )
		{
			AUTO_DELETE( itr->second );
		}
		AUTO_DELETE( m_current_render_map );

		itr = m_background_render_map->begin();
		for( ; itr != m_background_render_map->end(); itr ++ )
		{
			AUTO_DELETE( itr->second  );
		}

		AUTO_DELETE( m_background_render_map );

		remove_all();
	}

	void shadow_render::draw_normal_scene( render_argument* args )
	{
		draw_scene( args );
	}

	//// draw all shadow volume
	//void shadow_render::draw_shadow_volume( render_argument* args )
	//{
	//	//*
	//	args->m_device->set_texture( 0, NULL );
	//	material_map::iterator itr = m_current_render_map->begin();
	//	int v_count = 2;

	//	matrix4<float> v_mat_view = args->m_device->get_view_matrix();
	//	matrix4<float> v_mat_proj = args->m_device->get_projection_matrix();
	//	float v_far_plane = args->m_observer->get_position_geo().length();
	//	int v_create_count = 0;

	//	vector4<double> v_light;
	//	v_light.x = -math<double>::cos_( args->m_shadow_param.m_elevation_angle ) * math<double>::sin_( args->m_shadow_param.m_azimuth_angle );
	//	v_light.y = -math<double>::sin_( args->m_shadow_param.m_elevation_angle );
	//	v_light.z = -math<double>::cos_( args->m_shadow_param.m_elevation_angle ) * math<double>::cos_( args->m_shadow_param.m_azimuth_angle );
	//	v_light.w = 0;

	//	args->m_shadow_param.m_light = v_light;

	//	spherical_transform st;

	//	d3d9_effect_shadow_volume* v_render = (d3d9_effect_shadow_volume*)args->m_device->get_effect(US_EFFECT_SHADOW_VOLUME);
	//	args->m_render = v_render;	

	//	v_render->set_technique();

	//	for( ; itr != m_current_render_map->end(); itr ++ )
	//	{
	//		uint n_pass = 0;
	//		v_render->begin( &n_pass, 0 );

	//		v_render->set_view_matrix( v_mat_view );
	//		v_render->set_projection_matrix( v_mat_proj );

	//		float v_far_plane = args->m_observer->get_position_geo().length();
	//		v_render->set_far_plane( v_far_plane );

	//		shadow_render_queue* v_render_queue = itr->second;
	//		
	//		shadow_render_queue::render_object_array::iterator ro_itr = v_render_queue->m_render_object_queue.begin();
	//		for( ; ro_itr != v_render_queue->m_render_object_queue.end(); ro_itr++ )
	//		{
	//			render_object* v_ro = (*ro_itr);

	//			matrix4<double> v_mat = v_ro->get_world_matrix();
	//			vector3<double> v_pos_sphr = spherical_coords::from_geocentric_d( vector3<double>::s_zero * v_mat );

	//			spherical_transform st;
	//			st.create_from_sphr(	v_pos_sphr.x, v_pos_sphr.y, v_pos_sphr.z, args->m_shadow_param.m_azimuth_angle + PI,  args->m_shadow_param.m_elevation_angle, 0.0 );

	//			matrix4<double> v_light_mat = matrix4<double>::translation( -cartesian_coords::s_reference_position_geo.x, 
	//				-cartesian_coords::s_reference_position_geo.y,
	//				-cartesian_coords::s_reference_position_geo.z ) * matrix4<double>( v_mat_view.m );

	//			vector3<double> v_light_view = matrix4<double>::rotate_normal( st.m_direction_geo, v_light_mat ); 
	//			v_light_view.normalize();

	//			v_render->set_light_dir( vector3<float>( &(v_light_view.x) ));

	//			// º∆À„ Transform
	//			shadow_volume_creator* v_shadow_cast = dynamic_cast<shadow_volume_creator*>(v_ro);
	//			if(v_shadow_cast)
	//			{
	//				if( v_create_count < 100 && v_shadow_cast->create_shadow_object( args ) )
	//				{
	//					v_create_count++;
	//				}

	//				render_object* v_shadow_object = (render_object*)v_shadow_cast->get_shadow_object();
	//				//render_object* v_shadow_object = (render_object*)(v_shadow_cast);
	//				if( v_shadow_object != NULL )
	//				{
	//					// pass 1
	//					v_render->begin_pass(v_shadow_cast->get_pass1());
	//					v_shadow_object->draw( args );
	//					v_render->end_pass();

	//					// pass 2
	//					v_render->begin_pass(v_shadow_cast->get_pass2());
	//					v_shadow_object->draw( args );
	//					v_render->end_pass();				
	//				}
	//			}
	//			else
	//			{


	//				// pass 1
	//				v_render->begin_pass(0);
	//				v_ro->draw( args );
	//				v_render->end_pass();

	//				// pass 2
	//				v_render->begin_pass(1);
	//				v_ro->draw( args );
	//				v_render->end_pass();

	//			}
	//		}

	//		// Pass 4£¨ Fill Color
	//		v_render->begin_pass(4);
	//		{
	//			view_port v_view_port = args->m_device->get_view_port();
	//			rect v_rect( 0, 0, v_view_port.m_width, v_view_port.m_height );
	//			vector_3d vec[4];
	//			vec[0].x = v_rect.m_left,	vec[0].y = v_rect.m_bottom, vec[0].z = 0;
	//			vec[1].x = v_rect.m_left,	vec[1].y = v_rect.m_top,    vec[1].z = 0;
	//			vec[2].x = v_rect.m_right,	vec[2].y = v_rect.m_bottom, vec[2].z = 0;
	//			vec[3].x = v_rect.m_right,	vec[3].y = v_rect.m_top,    vec[3].z = 0;

	//			transformed_color vertex_array[4];
	//			for( ulong ni = 0; ni < 4; ni++ )
	//			{
	//				vertex_array[ni].x = (float)vec[ni].x;
	//				vertex_array[ni].y = (float)vec[ni].y;
	//				vertex_array[ni].z = 0.0f;
	//				vertex_array[ni].rhw = 1.0f;
	//				vertex_array[ni].color = itr->first;
	//			}

	//			// draw rect
	//			args->m_device->set_vertex_declaration( transformed_color::fvf );
	//			args->m_device->draw_triangle_strip( (void*)vertex_array, 4, sizeof(transformed_color) );

	//		}
	//		v_render->end_pass();
	//		v_render->end();
	//		v_count ++;
	//	}

	//	//*/

	//	// test
	//	/*
	//	render_state v_render_state( args->m_device );
	//	v_render_state.set_state( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	//	v_render_state.set_state( D3DRS_CULLMODE, D3DCULL_CCW);
	//	v_render_state.set_state( D3DRS_ZENABLE, FALSE );

	//	args->m_device->set_texture( 0, NULL );
	//	material_map::iterator itr = m_current_render_map->begin();
	//	int v_count = 2;
	//	for( ; itr != m_current_render_map->end(); itr ++ )
	//	{
	//		render_object_array* v_render_queue = itr->second;
	//		render_object_array::iterator ro_itr = v_render_queue->begin();
	//		for( ; ro_itr != v_render_queue->end(); ro_itr++ )
	//		{
	//			(*ro_itr)->draw( args );
	//		}
	//	}
	//	//*/
	//}

	// draw all shadow volume
	void shadow_render::draw_shadow_volume( render_argument* e )
	{
		//*
		render_argument v_args = *e;
		v_args.m_device->set_texture( 0, NULL );
		material_map::iterator itr = m_current_render_map->begin();
		int v_count = 2;

		matrix4<float> v_mat_view = v_args.m_device->get_view_matrix();
		matrix4<float> v_mat_proj = v_args.m_device->get_projection_matrix();
		float v_far_plane = v_args.m_observer->get_position_geo().length();
		int v_create_count = 0;

		vector4<double> v_light;
		v_light.x = -math<double>::cos_( v_args.m_shadow_param.m_elevation_angle ) * math<double>::sin_( v_args.m_shadow_param.m_azimuth_angle );
		v_light.y = -math<double>::sin_( v_args.m_shadow_param.m_elevation_angle );
		v_light.z = -math<double>::cos_( v_args.m_shadow_param.m_elevation_angle ) * math<double>::cos_( v_args.m_shadow_param.m_azimuth_angle );
		v_light.w = 0;

		v_args.m_shadow_param.m_light = v_light;

		spherical_transform st;

	
		d3d9_effect_shadow_volume* v_render = (d3d9_effect_shadow_volume*)v_args.m_device->get_effect(US_EFFECT_SHADOW_VOLUME);
		v_args.m_render = v_render;	

		v_render->set_technique();

		for( ; itr != m_current_render_map->end(); itr ++ )
		{
			uint n_pass = 0;
			v_render->begin( &n_pass, 0 );

			v_render->set_view_matrix( v_mat_view );
			v_render->set_projection_matrix( v_mat_proj );

			float v_far_plane = v_args.m_observer->get_position_geo().length();
			v_render->set_far_plane( v_far_plane );

			shadow_render_queue* v_render_queue = itr->second;

			shadow_render_queue::render_object_array::iterator ro_itr = v_render_queue->m_render_object_queue.begin();
			for( ; ro_itr != v_render_queue->m_render_object_queue.end(); ro_itr++ )
			{
				render_object* v_ro = (*ro_itr);

				matrix4<double> v_mat = v_ro->get_world_matrix();
				vector3<double> v_pos_sphr = spherical_coords::from_geocentric_d( vector3<double>::s_zero * v_mat );

				spherical_transform st;
				st.create_from_sphr(	v_pos_sphr.x, v_pos_sphr.y, v_pos_sphr.z, v_args.m_shadow_param.m_azimuth_angle + PI,  v_args.m_shadow_param.m_elevation_angle, 0.0 );

				matrix4<double> v_light_mat = matrix4<double>::translation( -cartesian_coords::s_reference_position_geo.x, 
					-cartesian_coords::s_reference_position_geo.y,
					-cartesian_coords::s_reference_position_geo.z ) * matrix4<double>( v_mat_view.m );

				vector3<double> v_light_view = matrix4<double>::rotate_normal( st.m_direction_geo, v_light_mat ); 
				v_light_view.normalize();

				v_render->set_light_dir( vector3<float>( &(v_light_view.x) ));

				// º∆À„ Transform
				shadow_volume_creator* v_shadow_cast = dynamic_cast<shadow_volume_creator*>(v_ro);
				if(v_shadow_cast)
				{
					if( v_create_count < 100 && v_shadow_cast->create_shadow_object( &v_args ) )
					{
						v_create_count++;
					}

					render_object* v_shadow_object = (render_object*)v_shadow_cast->get_shadow_object();
					//render_object* v_shadow_object = (render_object*)(v_shadow_cast);
					if( v_shadow_object != NULL )
					{
						// pass 1
						v_render->begin_pass(v_shadow_cast->get_pass1());
						v_shadow_object->draw( &v_args );
						v_render->end_pass();

						// pass 2
						v_render->begin_pass(v_shadow_cast->get_pass2());
						v_shadow_object->draw( &v_args );
						v_render->end_pass();				
					}
				}
				else
				{
					// pass 1
					v_render->begin_pass(0);
					v_ro->draw( &v_args );
					v_render->end_pass();

					// pass 2
					v_render->begin_pass(1);
					v_ro->draw( &v_args );
					v_render->end_pass();
				}
			}

			// Pass 4£¨ Fill Color
			v_render->begin_pass(4);
			{
				view_port v_view_port = v_args.m_device->get_view_port();
				rect v_rect( 0, 0, v_view_port.m_width, v_view_port.m_height );
				vector_3d vec[4];
				vec[0].x = v_rect.m_left,	vec[0].y = v_rect.m_bottom, vec[0].z = 0;
				vec[1].x = v_rect.m_left,	vec[1].y = v_rect.m_top,    vec[1].z = 0;
				vec[2].x = v_rect.m_right,	vec[2].y = v_rect.m_bottom, vec[2].z = 0;
				vec[3].x = v_rect.m_right,	vec[3].y = v_rect.m_top,    vec[3].z = 0;

				transformed_color vertex_array[4];
				for( ulong ni = 0; ni < 4; ni++ )
				{
					vertex_array[ni].x = (float)vec[ni].x;
					vertex_array[ni].y = (float)vec[ni].y;
					vertex_array[ni].z = 0.0f;
					vertex_array[ni].rhw = 1.0f;
					vertex_array[ni].color = itr->first.second;
				}

				// draw rect
				v_args.m_device->set_vertex_declaration( transformed_color::fvf );
				v_args.m_device->draw_triangle_strip( (void*)vertex_array, 4, sizeof(transformed_color) );

			}
			v_render->end_pass();
			v_render->end();
			v_count ++;
		}
	}

	void shadow_render::draw_shadow_clipper_volume( render_argument* args )
	{
		if ( m_clipper_render != NULL )
		{
			m_clipper_render->draw( args );
		}
	}

	void shadow_render::draw_scene_pass_one( render_argument* args )
	{
		// set render state
		render_state v_render_state( args->m_device );
		v_render_state.set_state( D3DRS_ALPHABLENDENABLE, TRUE );
		v_render_state.set_state( D3DRS_CLIPPLANEENABLE, TRUE );
		v_render_state.set_state( D3DRS_STENCILENABLE, TRUE );
		v_render_state.set_state( D3DRS_ZENABLE, TRUE );
		v_render_state.set_state( D3DRS_ZWRITEENABLE, TRUE );

		v_render_state.set_state( D3DRS_STENCILFUNC, D3DCMP_ALWAYS );
		v_render_state.set_state( D3DRS_STENCILREF, 1 );
		//v_render_state.set_state( D3DRS_STENCILREF, 2 );
		v_render_state.set_state( D3DRS_STENCILPASS,  D3DSTENCILOP_REPLACE);
		v_render_state.set_state( D3DRS_STENCILFAIL, D3DSTENCILOP_REPLACE);
		v_render_state.set_state( D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);

		//v_render_state.set_state( D3DRS_LIGHTING, TRUE);
		//v_render_state.set_state( D3DRS_LIGHT )


		//float slope_bias = 0.0f;
		//float depth_bias = 0.0f;
		//v_render_state.set_state( D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&slope_bias));
		//v_render_state.set_state( D3DRS_DEPTHBIAS, *((DWORD*)&depth_bias));

		// set clip plane
		float v_plane[4];
		vector3<double> v_dir = args->m_observer->get_direction_geo();
		v_plane[0] = -v_dir.x;
		v_plane[1] = -v_dir.y;
		v_plane[2] = -v_dir.z;
		v_plane[3] = args->m_observer->get_far_plane();
		args->m_device->set_clipping_plane( 0, v_plane );

		// draw scene
		draw_scene( args );
	}

	void shadow_render::draw_scene_pass_two( render_argument* args )
	{
		// set render state
		render_state v_render_state( args->m_device );
		v_render_state.set_state( D3DRS_ALPHABLENDENABLE, TRUE );
		v_render_state.set_state( D3DRS_SRCBLEND, D3DBLEND_ONE);
		v_render_state.set_state( D3DRS_DESTBLEND, D3DBLEND_ZERO );

		v_render_state.set_state( D3DRS_STENCILENABLE, TRUE );
		v_render_state.set_state( D3DRS_ZENABLE, TRUE );
		v_render_state.set_state( D3DRS_ZWRITEENABLE, TRUE );

		v_render_state.set_state( D3DRS_STENCILFUNC, D3DCMP_EQUAL );
		v_render_state.set_state( D3DRS_STENCILREF, 1 );
		v_render_state.set_state( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );
		v_render_state.set_state( D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		v_render_state.set_state( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

		draw_scene( args );
	}

	// draw all scene which shadow volume can influence
	void shadow_render::draw_scene( render_argument* args )
	{
		render_node_array::iterator itr = m_child_node_array.begin();
		for( ; itr != m_child_node_array.end(); itr++ )
		{
			(*itr)->draw( args );
		}
	}
}