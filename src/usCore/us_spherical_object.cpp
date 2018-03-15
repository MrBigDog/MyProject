///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_spherical_object.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : spherical_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_spherical_object.h"
#include <usUtil/us_spherical_coords.h>
#include <usCore/us_document_base.h>
#include <usCore/us_observer_base.h>

namespace uniscope_globe
{
	spherical_object::spherical_object(void)
	{
		//m_texture_enable = true;

		//m_selectable = true;

		//m_max_height = 6000.0;

		//m_min_height = -6000.0;

		m_state = spherical_object_state::SPHERICAL_OBJECT_IDLE;

		m_spherical_transform = NULL;

		m_spatial_reference = NULL;

		m_min_visible_range = 0;

		m_max_visible_range = 4.5;
	}

	spherical_object::spherical_object(document_base* in_doc)
		: spatial_object(in_doc)
	{
		//m_texture_enable = true;

		//m_selectable = true;

		//m_max_height = 6000.0;

		//m_min_height = -6000.0;

		m_state = spherical_object_state::SPHERICAL_OBJECT_IDLE;

		m_spherical_transform = NULL;

		m_min_visible_range = 0;

		m_max_visible_range = 4.5;

	}


	spherical_object::~spherical_object(void)
	{

	}

	void spherical_object::finalize(void)
	{
		release_rigid_body();

		spatial_object::finalize();

		AUTO_DELETE(m_spherical_transform);

		AUTO_DELETE(m_spatial_reference);
	}

	// rigid body
	void spherical_object::update_motion(time_value in_frame_time)
	{
		update_position(in_frame_time);

		update_rotation(in_frame_time);
	}

	void spherical_object::update_position(time_value in_elapsed_time)
	{
		if (m_velocity == vector3<float>::s_zero) return;

		//if(get_clip_tree_root() 
		//axis_aligned_box<double> v_aabb = get_clip_tree_root()->get_node_aabb();

		axis_aligned_box<double> v_aabb = m_aabb;

		double height = v_aabb.get_max().y - v_aabb.get_min().y;

		vector3<double> v_pos1 = vector3<double>(0.0, height * 0.5, 0.0) * m_spherical_transform->m_mat_tnb;
		vector3<double> v_pos2 = vector3<double>(m_velocity.x * in_elapsed_time, m_velocity.y * in_elapsed_time + height * 0.5, m_velocity.z * in_elapsed_time) * m_spherical_transform->m_mat_tnb;

		vector3<double> v_pos3 = vector3<double>(0.0, height * 0.8, 0.0) * m_spherical_transform->m_mat_tnb;
		vector3<double> v_pos4 = vector3<double>(m_velocity.x * in_elapsed_time, m_velocity.y * in_elapsed_time + height * 0.8, m_velocity.z * in_elapsed_time) * m_spherical_transform->m_mat_tnb;

		ray<double> v_ray1;
		v_ray1.ray_origin = v_pos1;
		v_ray1.ray_direction = vector3<double>::normalize(v_pos2 - v_pos1);

		ray<double> v_ray2;
		v_ray2.ray_origin = v_pos3;
		v_ray2.ray_direction = vector3<double>::normalize(v_pos4 - v_pos3);

		double v_range = (v_pos4 - v_pos3).length();

		vector3<double> v_intersection;
		if (m_document->pick_vector(v_ray1, v_range, v_intersection))
		{
			double length = (v_intersection - v_ray1.ray_origin).length();
			if (length < 0.0005)
				return;
		}

		if (m_document->pick_vector(v_ray2, v_range, v_intersection))
		{
			double length = (v_intersection - v_ray2.ray_origin).length();
			if (length < 0.0005)
				return;
		}

		ray<double> v_ray;
		v_ray.ray_origin = vector3<double>(m_velocity.x * in_elapsed_time, m_velocity.y * in_elapsed_time/* + height *5*/, m_velocity.z * in_elapsed_time) * m_spherical_transform->m_mat_tnb;
		v_ray.ray_direction = -vector3<double>::normalize(v_ray.ray_origin);
		v_ray.ray_origin = v_ray.ray_origin - v_ray.ray_direction * height * 0.001;

		//ulong v_time = timeGetTime();

		//vector3<double> test_pos1 = m_spherical_transform->m_position_geo;

		if (m_document->pick_vector(v_ray, spherical_coords::s_semi_major + height * 0.001, v_intersection))
		{
			m_spherical_transform->create_from_cart(v_intersection, m_spherical_transform->m_direction_cart, m_spherical_transform->m_up_cart);
		}
		//v_intersection = vector3<double>( m_velocity.x * in_elapsed_time, m_velocity.y * in_elapsed_time, m_velocity.z * in_elapsed_time ) * m_spherical_transform->m_mat_tnb;
		//m_spherical_transform->create_from_cart( v_intersection, m_spherical_transform->m_direction_cart, m_spherical_transform->m_up_cart );

		//double test_length = (test_pos1 -  m_spherical_transform->m_position_geo).length();

		//US_RENDER_TRACE1( L"test_length: %lf\n", test_length * 1000 );	
		//US_RENDER_TRACE1( L"time: %lf\n", in_elapsed_time );
		//US_RENDER_TRACE1( L"speed: %lf\n", test_length / in_elapsed_time * 1000 );
	}

	void spherical_object::update_rotation(time_value in_elapsed_time)
	{
		if (m_rotation_velocity == 0.0f) return;

		double new_heading = m_spherical_transform->m_heading + m_rotation_velocity * in_elapsed_time * RADIAN;
		if (new_heading < 0) new_heading = 360.0 * RADIAN + new_heading;
		if (new_heading > 360 * RADIAN) new_heading -= 360 * RADIAN;

		m_spherical_transform->create_from_sphr(m_spherical_transform->m_longitude, m_spherical_transform->m_latitude, m_spherical_transform->m_altitude, new_heading, m_spherical_transform->m_tilt, m_spherical_transform->m_roll);
	}

	void spherical_object::set_spherical_transform(spherical_transform* v_transf)
	{
		//AUTO_DELETE( m_spherical_transform );

		m_spherical_transform = v_transf;
	}

	void spherical_object::set_spatial_reference(spatial_reference* v_ref)
	{
		m_spatial_reference = v_ref;
	}

	bool spherical_object::distance_clip(observer_base* v_observer)
	{
		if (!m_is_visible)	return false;
		if (m_max_visible_range < 0) return true;
		double v_distance = (v_observer->get_position_geo() - m_spherical_transform->m_position_geo).length();
		if (v_distance < m_min_visible_range || v_distance > m_max_visible_range) return false;
		return true;
	}

	bool spherical_object::frustum_clip(observer_base* v_observer)
	{
		if (!m_is_visible)	return false;
		if (m_max_visible_range < 0) return true;
		double v_distance = (v_observer->get_position_geo() - get_spherical_transform()->m_position_geo).length();
		if (v_distance < m_min_visible_range || v_distance > m_max_visible_range) return false;

		frustum<double> v_frustum;

		if (v_observer->get_ortho_mode())
		{
			v_frustum.update_ex(v_observer->get_ortho_width(), v_observer->get_ortho_height(),
				v_observer->get_near_plane(), v_observer->get_far_plane(), get_matrix_cart2geo() * v_observer->get_view_matrix_geo());
		}
		else
		{
			matrix4<double> v_transform_mat = get_matrix_cart2geo() * v_observer->get_view_matrix_geo();
			v_frustum.update(v_observer->get_fov(), v_observer->get_aspect(), v_observer->get_near_plane(), v_observer->get_far_plane(), v_transform_mat);
		}

		if (v_frustum.contain_box(m_aabb) > 0)
		{
			return true;
		}

		return false;
	}

	void spherical_object::translate_object(vector_3d& vec_sphr_offset)
	{
		spherical_transform* v_sphr_trans = this->get_spherical_transform();
		v_sphr_trans->create_from_sphr(v_sphr_trans->m_longitude + vec_sphr_offset.x, v_sphr_trans->m_latitude + vec_sphr_offset.y, v_sphr_trans->m_altitude + vec_sphr_offset.z, v_sphr_trans->m_heading, v_sphr_trans->m_tilt, v_sphr_trans->m_roll);
	}

	void spherical_object::rotate_object(quaternion_4d& quater)
	{
		spherical_transform* v_shpr_trans = get_spherical_transform();

		quaternion_4d quater_object = quaternion_4d::from_euler(v_shpr_trans->m_tilt, v_shpr_trans->m_heading, v_shpr_trans->m_roll) * quater;
		vector_3d euler_angle = quaternion_4d::to_euler(quater_object);

		v_shpr_trans->create_from_sphr(v_shpr_trans->m_longitude, v_shpr_trans->m_latitude, v_shpr_trans->m_altitude, euler_angle.y, euler_angle.x, euler_angle.z);
	}

	void spherical_object::scale_object(vector_3d& vec)
	{
		vector3<double> v_old_scale = get_spherical_transform()->get_scale();
		v_old_scale = v_old_scale * vec;
		//v_old_scale.x = max( 0.01, v_old_scale.x );
		//v_old_scale.y = max( 0.01, v_old_scale.y );
		//v_old_scale.z = max( 0.01, v_old_scale.z );

		get_spherical_transform()->set_scale(v_old_scale);
		//get_spherical_transform()->set_scale( vector3<double>(vec.x * v_old_scale.x,vec.y * v_old_scale.y,vec.z * v_old_scale.z) );
	}


	lon_lat_rect spherical_object::get_lon_lat_rect(void)
	{
		return lon_lat_rect::from_aabb(m_spherical_transform->m_position_sphr, m_aabb);
	}

	void spherical_object::copy_from(spatial_object* v_object)
	{
		spatial_object::copy_from(v_object);
		spherical_object* v_sphrical_object = dynamic_cast<spherical_object*>(v_object);
		if (v_sphrical_object == NULL) return;
		m_max_visible_range = v_sphrical_object->m_max_visible_range;
		m_min_visible_range = v_sphrical_object->m_min_visible_range;
		m_script = v_sphrical_object->m_script;
	}

}