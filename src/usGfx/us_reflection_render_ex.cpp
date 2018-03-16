///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_reflection_render_ex.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : reflection_render_ex class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_reflection_render_ex.h"
#include <usGfx/us_d3d9_effect_common_mesh.h>
#include <usGfx/us_d3d9_render_target_manager.h>
#include <usGfx/us_d3d9_effect_reflection_surface_ex.h>
#include <usGfx/us_d3d9_effect_manager.h>

#include <usCore/us_render_argument.h>
#include <usCore/us_render_device.h>
#include <usCore/us_render_target.h>
#include <usCore/us_system_environment.h>
#include <usCore/us_observer_base.h>
#include <usCore/us_render_state.h>
#include <usCore/us_frame_counter.h>

#include <usUtil/us_spherical_coords.h>
#include <usUtil/us_cartesian_coords.h>

namespace uniscope_globe
{
	reflection_render_ex::reflection_render_ex(void)
	{
		m_current_render_array = new render_object_array;

		m_background_render_array = new render_object_array;
	}

	reflection_render_ex::~reflection_render_ex(void)
	{
		clear();
	}

	void reflection_render_ex::push(render_object* in_object)
	{
		in_object->add_ref();
		m_background_render_array->push_back(in_object);
	}

	void reflection_render_ex::flush(void)
	{
		// begin flip render_object
		{
			US_LOCK_AUTO_MUTEX

				render_object_array* temp_map = m_current_render_array;
			m_current_render_array = m_background_render_array;
			m_background_render_array = temp_map;
		}
		clear_background();

		render_node::flush();
	}

	int reflection_render_ex::get_count(void)
	{
		return m_current_render_array->size();
	}


	void reflection_render_ex::add_reflection_plane(double height)
	{
		m_reflection_plane_array.push_back(height);
	}

	void reflection_render_ex::draw(render_argument* args)
	{
		US_LOCK_AUTO_MUTEX


			if (m_reflection_plane_array.size() > 0)
			{
				render_device* device = args->m_device;
				observer_base* in_observer = args->m_observer;

				//////////////////////////////////////////////////////////////////////////
				// ·´Éä
				render_target* v_reflect_render_target = (render_target*)device->get_render_target(US_RENDER_TARGET_REFLECT);
				v_reflect_render_target->begin();
				{
					device->clear(0x00000000, 1.0, 2);

					d3d9_effect_common_mesh* v_render = dynamic_cast<d3d9_effect_common_mesh*>(args->m_render);
					v_render->push_transform();
					double water_height = system_environment::s_water_height * 0.001;
					double v_height = -(in_observer->get_position_sphr().z - (water_height + spherical_coords::s_semi_major)) * 1000;

					if (system_environment::s_custom_water_suface)
					{
						vector_3d v_normal = system_environment::s_water_normal;
						vector_3d v_orig = system_environment::s_water_position - cartesian_coords::s_reference_position_geo;
						plane<float> v_plane(vector_3f((double*)&v_normal), vector_3f((double*)&v_orig));
						v_render->multiply_transform(matrix4<float>::reflection(vector_3f((double*)&(v_normal)), vector_3f((double*)&v_orig)));
					}
					else
					{
						spherical_transform st;
						st.create_from_sphr(in_observer->get_longitude(), in_observer->get_latitude(), in_observer->get_altitude(), 0, 0, 0);
						vector_3d v_normal = matrix4<double>::transform_normal(vector_3d(0, -1, 0), st.m_mat_tnb);
						v_normal.normalize();
						vector_3d v_orig = matrix4<double>::transform_coordinate(vector_3d(0, v_height, 0), st.m_mat_tnb) - cartesian_coords::s_reference_position_geo;
						plane<float> v_plane(vector_3f((double*)&v_normal), vector_3f((double*)&v_orig));
						v_render->multiply_transform(matrix4<float>::reflection(vector_3f((double*)&(v_normal)), vector_3f((double*)&v_orig)));
					}


					render_state v_render_state(device);
					v_render_state.set_state(D3DRS_CULLMODE, D3DCULL_CW);
					v_render_state.set_state(D3DRS_CLIPPLANEENABLE, TRUE);
					//v_render_state.set_state( D3DRS_ZWRITEENABLE, FALSE);


					args->m_render_flag = 0x1000;
					for (int ni = 0; ni < m_child_node_array.size(); ni++)
					{
						m_child_node_array[ni]->draw(args);
					}

					v_render->pop_transform();
				}
				v_reflect_render_target->end();

				//////////////////////////////////////////////////////////////////////////
				// ÕÛÉä
				render_target* v_refract_render_target = (render_target*)device->get_render_target(US_RENDER_TARGET_REFRACT);
				v_refract_render_target->begin();
				{
					device->clear(0x00000000, 1.0, 2);

					d3d9_effect_common_mesh* v_render = dynamic_cast<d3d9_effect_common_mesh*>(args->m_render);
					//v_render->push_transform();
					double water_height = system_environment::s_water_height * 0.001;
					double v_height = -(in_observer->get_position_sphr().z - (water_height + spherical_coords::s_semi_major)) * 1000;

					spherical_transform st;
					st.create_from_sphr(in_observer->get_longitude(), in_observer->get_latitude(), in_observer->get_altitude(), 0, 0, 0);
					vector_3d v_normal = matrix4<double>::transform_normal(vector_3d(0, -1, 0), st.m_mat_tnb);
					v_normal.normalize();
					vector_3d v_orig = matrix4<double>::transform_coordinate(vector_3d(0, v_height, 0), st.m_mat_tnb) - cartesian_coords::s_reference_position_geo;
					plane<float> v_plane(vector_3f((double*)&v_normal), vector_3f((double*)&v_orig));

					//plane<double> v_plane_d(vector_3d(0,-1,0), vector_3d(0,v_height,0));
					//v_plane_d.transform(st.m_mat_tnb);
					//plane<float> v_plane((double*)&v_plane_d);

					//args->m_device->set_clipping_plane( 0, (float*)&v_plane );

					//////////////////////////////////////////////////////////////////////////
					//v_render->multiply_transform(matrix4<float>::reflection(vector_3f((double*)&(v_normal)),vector_3f((double*)&v_orig)));

					//////////////////////////////////////////////////////////////////////////
					//v_orig.normalize();
					//v_orig = v_orig * 0.005;
					//v_render->multiply_transform(matrix4<float>::translation(v_orig.x, v_orig.y, v_orig.z ) );

					render_state v_render_state(device);
					//v_render_state.set_state( D3DRS_CULLMODE, D3DCULL_NONE );
					//v_render_state.set_state( D3DRS_CLIPPLANEENABLE, TRUE );
					//v_render_state.set_state( D3DRS_ZWRITEENABLE, FALSE);


					args->m_render_flag = 0x1000;
					for (int ni = 0; ni < m_child_node_array.size(); ni++)
					{
						m_child_node_array[ni]->draw(args);
					}

					//v_render->pop_transform();
				}
				v_refract_render_target->end();


				args->m_render_flag = 0;
				d3d9_effect_reflection_surface_ex* v_effect = (d3d9_effect_reflection_surface_ex*)args->m_device->get_effect(US_EFFECT_REFRACTION_SURFACE);
				v_effect->update_offset(args->m_frame_counter->m_last_frame_tick_count);
				draw_scene(args);
				m_reflection_plane_array.clear();
				args->m_render_flag = 0;
			}
			else
			{
				draw_scene(args);
				args->m_render_flag = 0;
			}
	}

	void reflection_render_ex::clear_background(void)
	{
		render_object_array::iterator itr = m_background_render_array->begin();
		for (; itr != m_background_render_array->end(); itr++)
		{
			AUTO_RELEASE_SHARED_DATA(*itr);
		}

		m_background_render_array->clear();
	}

	void reflection_render_ex::clear(void)
	{
		render_object_array::iterator itr = m_current_render_array->begin();
		for (; itr != m_current_render_array->end(); itr++)
		{
			AUTO_DELETE(*itr);
		}
		AUTO_DELETE(m_current_render_array);

		itr = m_background_render_array->begin();
		for (; itr != m_background_render_array->end(); itr++)
		{
			AUTO_DELETE(*itr);
		}

		AUTO_DELETE(m_background_render_array);

		remove_all();
	}

	void reflection_render_ex::draw_scene(render_argument* args)
	{
		render_node_array::iterator itr = m_child_node_array.begin();
		for (; itr != m_child_node_array.end(); itr++)
		{
			(*itr)->draw(args);
		}
	}

}