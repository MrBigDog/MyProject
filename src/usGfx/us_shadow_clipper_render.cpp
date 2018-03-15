///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shadow_clipper_render.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shadow_clipper_render class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_shadow_clipper_render.h"

namespace uniscope_globe
{
	shadow_clipper_render::shadow_clipper_render( void )
	{
		m_current_render_map = new material_map;

		m_background_render_map = new material_map;

		m_clipper_ro = NULL;
	}

	shadow_clipper_render::~shadow_clipper_render( void )
	{
		clear();
	}

	void shadow_clipper_render::push( render_object* in_object)
	{
		ulong v_shadow_color = 0;
		int v_shadow_clipper_render_type = 0;

		shadow_volume_creator* v_shadow_cast = dynamic_cast<shadow_volume_creator*>(in_object);
		if ( v_shadow_cast != NULL )
		{
			v_shadow_color =  in_object->get_shadow_color();
			v_shadow_clipper_render_type = 0;
		}
		else
		{
			v_shadow_clipper_render_type = 1;
			v_shadow_color = 0xaa000000;
		}

		shadow_render_queue* v_render_queue = NULL;
		material_map::iterator itr = m_background_render_map->find( v_shadow_color );
		if( itr != m_background_render_map->end() )
		{
			v_render_queue = itr->second;
		}
		else
		{
			v_render_queue = new shadow_render_queue;

			(*m_background_render_map)[v_shadow_color] = v_render_queue;
		}
		v_render_queue->push(in_object);
	}

	void shadow_clipper_render::flush( void )
	{
		// begin flip render_object
		{
			US_LOCK_AUTO_MUTEX

			material_map* temp_map = m_current_render_map;

			m_current_render_map = m_background_render_map;

			m_background_render_map = temp_map;
		}
		clear_background();

		render_node::flush();
	}

	void shadow_clipper_render::clear_background( void )
	{
		material_map::iterator itr = m_background_render_map->begin();
		for( ; itr != m_background_render_map->end(); itr++ )
		{
			AUTO_DELETE( itr->second );
		}

		m_background_render_map->clear();
	}

	void shadow_clipper_render::clear( void )
	{
		AUTO_RELEASE_SHARED_DATA( m_clipper_ro );

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

	void shadow_clipper_render::draw( render_argument* args )
	{
		US_LOCK_AUTO_MUTEX

		draw_clip_mesh( args );

		draw_shadow_volume( args );
	}

	long shadow_clipper_render::get_status( void )
	{
		if ( m_current_render_map->size() > 0 )
			return 1;

		return 0;
	}

	void shadow_clipper_render::begin_clipper( render_object* v_ro )
	{
		m_clipper_ro = v_ro;
	}

	void shadow_clipper_render::end_clipper( void )
	{
		m_clipper_ro = NULL;
	}

	// draw all shadow volume
	void shadow_clipper_render::draw_shadow_volume( render_argument* args )
	{
		args->m_device->set_texture( 0, NULL );
		material_map::iterator itr = m_current_render_map->begin();
		int v_count = 2;

		matrix4<float> v_mat_view = args->m_device->get_view_matrix();
		matrix4<float> v_mat_proj = args->m_device->get_projection_matrix();
		float v_far_plane = args->m_observer->get_position_geo().length();
		int v_create_count = 0;

		vector4<double> v_light;
		v_light.x = -math<double>::cos_( args->m_shadow_param.m_elevation_angle ) * math<double>::sin_( args->m_shadow_param.m_azimuth_angle );
		v_light.y = -math<double>::sin_( args->m_shadow_param.m_elevation_angle );
		v_light.z = -math<double>::cos_( args->m_shadow_param.m_elevation_angle ) * math<double>::cos_( args->m_shadow_param.m_azimuth_angle );
		v_light.w = 0;

		args->m_shadow_param.m_light = v_light;

		spherical_transform st;

		d3d9_effect_shadow_volume* v_render = (d3d9_effect_shadow_volume*)args->m_device->get_effect(US_EFFECT_SHADOW_VOLUME);
		args->m_render = v_render;	

		v_render->set_technique();

		for( ; itr != m_current_render_map->end(); itr ++ )
		{
			//draw_clip_mesh( args );

			uint n_pass = 0;
			v_render->begin( &n_pass, 0 );

			v_render->set_view_matrix( v_mat_view );
			v_render->set_projection_matrix( v_mat_proj );

			float v_far_plane = args->m_observer->get_position_geo().length();
			v_render->set_far_plane( v_far_plane );

			shadow_render_queue* v_render_queue = itr->second;

			shadow_render_queue::render_object_array::iterator ro_itr = v_render_queue->m_render_object_queue.begin();
			for( ; ro_itr != v_render_queue->m_render_object_queue.end(); ro_itr++ )
			{
				render_object* v_ro = (*ro_itr);

				matrix4<double> v_mat = v_ro->get_world_matrix();
				vector3<double> v_pos_sphr = spherical_coords::from_geocentric_d( vector3<double>::s_zero * v_mat );

				spherical_transform st;
				st.create_from_sphr(	v_pos_sphr.x, v_pos_sphr.y, v_pos_sphr.z, args->m_shadow_param.m_azimuth_angle + PI,  args->m_shadow_param.m_elevation_angle, 0.0 );

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
					if( v_create_count < 100 && v_shadow_cast->create_shadow_object( args ) )
					{
						v_create_count++;
					}

					render_object* v_shadow_object = (render_object*)v_shadow_cast->get_shadow_object();
					//render_object* v_shadow_object = (render_object*)(v_shadow_cast);
					if( v_shadow_object != NULL )
					{
						// pass 1
						v_render->begin_pass(v_shadow_cast->get_pass1());
						v_shadow_object->draw( args );
						v_render->end_pass();

						// pass 2
						v_render->begin_pass(v_shadow_cast->get_pass2());
						v_shadow_object->draw( args );
						v_render->end_pass();				
					}
				}
				else
				{
					// pass 1
					v_render->begin_pass(0);
					v_ro->draw( args );
					v_render->end_pass();

					// pass 2
					v_render->begin_pass(1);
					v_ro->draw( args );
					v_render->end_pass();
				}
			}

			// Pass 4£¨ Fill Color
			v_render->begin_pass(4);
			{
				view_port v_view_port = args->m_device->get_view_port();
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
					vertex_array[ni].color = itr->first;
				}

				// draw rect
				args->m_device->set_vertex_declaration( transformed_color::fvf );
				args->m_device->draw_triangle_strip( (void*)vertex_array, 4, sizeof(transformed_color) );

			}
			v_render->end_pass();
			v_render->end();
			v_count ++;
		}
	}

	void shadow_clipper_render::draw_clip_mesh( render_argument* args )
	{
		if ( m_clipper_ro == NULL )
			return;

		args->m_device->clear_stencil( 2 );

		matrix4<float> v_mat_view = args->m_device->get_view_matrix();
		matrix4<float> v_mat_proj = args->m_device->get_projection_matrix();
		float v_far_plane = args->m_observer->get_position_geo().length();

		d3d9_effect_shadow_volume* v_render = (d3d9_effect_shadow_volume*)args->m_render;
		v_render->set_view_matrix( v_mat_view );
		v_render->set_projection_matrix( v_mat_proj );
		v_render->set_far_plane( v_far_plane );

		uint n_pass = 0;
		v_render->begin( &n_pass, 0 );

		// º∆À„ Transform
		shadow_volume_creator* v_shadow_cast = dynamic_cast<shadow_volume_creator*>(m_clipper_ro);

		render_object* v_shadow_object = (render_object*)v_shadow_cast->get_shadow_object();
		if( v_shadow_object != NULL )
		{
			// pass 1
			v_render->begin_pass(2);
			v_shadow_object->draw( args );
			v_render->end_pass();

			// pass 2
			v_render->begin_pass(3);
			v_shadow_object->draw( args );
			v_render->end_pass();				
		}
		
		v_render->end();
	}

}