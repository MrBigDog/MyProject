//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_observer_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : observer_base class
//	Reference : 
//
//////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_observer_base.h"
#include <usCore/us_render_device.h>

namespace uniscope_globe
{
	observer_base::observer_base(void)
	{
		m_state = observer_state::IDLE;

		m_spherical_transform = new spherical_transform();

		m_ortho_width = 1;
		m_ortho_height = 1;

		m_enable_ortho_mode = false;

		m_position_local = vector3<double>::s_zero;
		m_direction_local = vector3<double>::s_unit_y;
		m_aim_local = vector3<double>::s_zero;
		m_view_center_mat_tnb = matrix4<double>::s_identity;
		m_view_center_mat_tnb_invs = matrix4<double>::s_identity;
		m_clip_matrix = matrix4<double>::s_identity;

		m_intersect_model = false;

		m_custom_near_far_mode = false;

		m_clip_x = -1;
		m_clip_y = -1;
		m_clip_width = 2;
		m_clip_height = 2;
	}

	observer_base::observer_base(observer_base& observer)
	{
		copy(observer);
	}

	observer_base::~observer_base(void)
	{
	}

	void observer_base::copy(observer_base& observer)
	{
		m_state = observer.m_state;

		m_spherical_transform->m_position_geo = observer.m_spherical_transform->m_position_geo;
		m_spherical_transform->m_target_geo = observer.m_spherical_transform->m_target_geo;
		m_aim_geo = observer.m_aim_geo;
		m_origin_geo = observer.m_origin_geo;
		m_spherical_transform->m_direction_geo = observer.m_spherical_transform->m_direction_geo;
		m_spherical_transform->m_up_geo = observer.m_spherical_transform->m_up_geo;
		m_up_normal_geo = observer.m_up_normal_geo;

		m_spherical_transform->m_position_sphr = observer.m_spherical_transform->m_position_sphr;
		m_spherical_transform->m_target_sphr = observer.m_spherical_transform->m_target_sphr;
		m_aim_sphr = observer.m_aim_sphr;
		m_origin_sphr = observer.m_origin_sphr;

		m_fov = observer.m_fov;
		m_aspect = observer.m_aspect;
		m_near_plane = observer.m_near_plane;
		m_far_plane = observer.m_far_plane;

		m_max_level = observer.m_max_level;
		m_tile_width = observer.m_tile_width;

		m_view_matrix_geo = observer.m_view_matrix_geo;
		m_view_matrix_rp = observer.m_view_matrix_rp;
		m_projection_matrix = observer.m_projection_matrix;
		m_frustum = observer.m_frustum;
		m_view_port = observer.m_view_port;
		m_current_dem_height = observer.m_current_dem_height;
		// 		m_lookat_pose = observer.m_lookat_pose;
		// 		m_observer_pose = observer.m_observer_pose;

		m_spherical_transform = observer.m_spherical_transform;

		m_speed = observer.m_speed;

		m_position_local = observer.m_position_local;
		m_direction_local = observer.m_direction_local;
		m_aim_local = observer.m_aim_local;
		m_view_center_mat_tnb = observer.m_view_center_mat_tnb;

		m_enable_ortho_mode = observer.m_enable_ortho_mode;

		m_intersect_model = observer.m_intersect_model;

		m_clip_x = observer.m_clip_x;
		m_clip_y = observer.m_clip_y;
		m_clip_width = observer.m_clip_width;
		m_clip_height = observer.m_clip_height;

		m_custom_near_far_mode = observer.m_custom_near_far_mode;
		;
	}

	// x = -1, y = -1, w = 2, h = 2;
	void observer_base::set_clip_rect(float x, float y, float w, float h)
	{
		m_clip_x = x;
		m_clip_y = y;
		m_clip_width = w;
		m_clip_height = h;
	}

	void observer_base::submit_to_device(render_device* device)
	{
		// calculate view matrix;
		m_view_matrix_geo = matrix4<double>::look_at(m_spherical_transform->m_position_geo, m_spherical_transform->m_target_geo, m_spherical_transform->m_up_geo);
		//m_view_matrix = m_mat_tnb_no_scale_inverse;

		// calculate device matrix;
		m_view_matrix_rp = matrix4<double>::look_at(m_spherical_transform->m_position_geo - cartesian_coords::s_reference_position_geo,
			m_spherical_transform->m_target_geo - cartesian_coords::s_reference_position_geo, m_spherical_transform->m_up_geo);

		// calculate projection matrix;
		m_aspect = (double)m_view_port.m_width / m_view_port.m_height;

		if (!m_enable_ortho_mode)
		{
			m_projection_matrix = matrix4<double>::perspective(m_fov, m_aspect, m_near_plane, m_far_plane);
		}
		else
		{
			m_projection_matrix = matrix4<double>::orthographic(m_ortho_width, m_ortho_height, m_near_plane, m_far_plane);
		}




		matrix4<float> v_view_matrix((double*)&m_view_matrix_rp);
		matrix4<float> v_projection_matrix((double*)&m_projection_matrix);
		device->set_near_plane(m_near_plane);
		device->set_far_plane(m_far_plane);
		device->set_view_matrix(v_view_matrix);
		device->set_projection_matrix(v_projection_matrix);



		// calculate m_frustum;
		if (get_ortho_mode() == false)
		{
			m_frustum.update(m_fov, m_aspect, m_near_plane, m_far_plane, m_view_matrix_geo);
		}
		else
		{
			m_frustum.update_ex(this->get_ortho_width(), this->get_ortho_height(), this->m_clip_x, this->m_clip_y, this->m_clip_width, this->m_clip_height, this->get_near_plane(), this->get_far_plane(), m_view_matrix_geo);
		}


		compute_aim_point();
		// calculate terrain level;
		double v_distance = (m_spherical_transform->m_position_geo - m_aim_geo).length();
		double v_relative_height = m_spherical_transform->m_position_sphr.z - get_dem_height();

		double v_temp_scale = tan(m_fov * 0.5) / ((double)m_view_port.m_height * 0.5);
		m_geo_to_pixel_scale_by_aim = v_distance / v_temp_scale;
		m_pixel_to_geo_scale_by_aim = v_distance * v_temp_scale;

		m_geo_to_pixel_scale_by_height = v_relative_height / v_temp_scale;
		m_pixel_to_geo_scale_by_height = v_relative_height * v_temp_scale;

		m_tile_width = 256.0 * m_pixel_to_geo_scale_by_aim;
		m_max_level = int(log(PI * spherical_coords::s_datum_plane / m_tile_width) / log(2.0));
		m_max_level += 2;
		//if(m_enable_ortho_mode)
		//{
		//	m_max_level += 2;
		//}


		m_max_level = m_max_level < 24 ? m_max_level : 23;
		m_tile_width = interior_index_factory::get_bound_size(m_max_level);

		//return in_pixel_size * tan( m_fov * 0.5 ) / ((double)m_view_port.m_height * 0.5) * (m_spherical_transform->m_position_geo - m_aim_geo).length();


		//m_geo_to_pixel_scale = tan( m_fov * 0.5 ) / ((double)m_view_port.m_height * 0.5) * (m_spherical_transform->m_position_sphr.z - get_dem_height());



		//{
		//	double tile_width1 = PI * spherical_coords::s_semi_major / math<double>::pow_( 2.0, m_max_level );
		//	double tile_width2 = PI * spherical_coords::s_semi_major / math<double>::pow_( 2.0, m_max_level + 1 );

		//	double height1 = tile_width1 * ((double)m_view_port.m_height * 0.5) / ( v_pixel_size * tan( m_fov * 0.5 ) );
		//	double height2 = tile_width2 * ((double)m_view_port.m_height * 0.5) / ( v_pixel_size * tan( m_fov * 0.5 ) );

		//	double kkk = 0;
		//}

		calculate_local_parameter();
	}

	double observer_base::get_geo_to_pixel_scale_by_distance(double v_distance)
	{
		double v_temp_scale = tan(m_fov * 0.5) / ((double)m_view_port.m_height * 0.5);

		return v_distance / v_temp_scale;
	}

	double observer_base::get_pixel_to_geo_scale_by_distance(double v_distance)
	{
		double v_temp_scale = tan(m_fov * 0.5) / ((double)m_view_port.m_height * 0.5);

		return v_temp_scale * v_distance;
	}

	void observer_base::compute_aim_point()
	{
		vector3<double> v_position_inverse = vector3<double>::normalize(-m_spherical_transform->m_position_geo);

		double angle_diff = math<double>::acos_(v_position_inverse.dot(m_spherical_transform->m_direction_geo));

		if (angle_diff > (HALF_PI/* + m_frustum.m_fov * 0.45*/))
		{
			m_aim_sphr = m_origin_sphr;
			m_aim_sphr.z = m_current_dem_height + spherical_coords::s_semi_major;
			m_aim_geo = geocentric_coords::from_spherical_d(m_aim_sphr);
			return;
		}
		else if (angle_diff < PI * 0.25)
		{
			m_aim_sphr = m_spherical_transform->m_target_sphr;
			m_aim_geo = m_spherical_transform->m_target_geo;
			return;
		}

		if (get_ortho_mode() == true)
		{
			m_aim_sphr = m_spherical_transform->m_target_sphr;
			m_aim_geo = m_spherical_transform->m_target_geo;
			return;
		}

		double v_up_side_down = m_spherical_transform->m_up_geo.dot(v_position_inverse);

		vector3<double> test_direction;
		if (v_up_side_down < 0.0)
		{
			vector3<double> little_negative = m_frustum.m_negative_y + (m_frustum.m_near_plane_center - m_frustum.m_negative_y) / 3;
			vector3<double> test_vec = m_frustum.m_near_plane_center + (angle_diff - PI* 0.25) / (PI * 0.25) * (little_negative - m_frustum.m_near_plane_center);
			test_direction = vector3<double>::normalize(test_vec - m_spherical_transform->m_position_geo);
		}
		else
		{
			vector3<double> little_positive = m_frustum.m_positive_y + (m_frustum.m_near_plane_center - m_frustum.m_positive_y) / 3;

			vector3<double> test_vec = m_frustum.m_near_plane_center + (angle_diff - PI* 0.25) / (PI * 0.25) * (little_positive - m_frustum.m_near_plane_center);
			test_direction = vector3<double>::normalize(test_vec - m_spherical_transform->m_position_geo);
		}

		// intersect
		// From "3D Math Primer for Graphics and Game Development" Page 286
		double a = test_direction.dot(-m_spherical_transform->m_position_geo);
		double e = m_spherical_transform->m_position_geo.length();
		double r = (m_current_dem_height + spherical_coords::s_semi_major);
		double f = sqrt(r*r - e*e + a*a);
		double t = a - f;

		m_aim_geo = m_spherical_transform->m_position_geo + test_direction * t;
		m_aim_sphr = spherical_coords::from_geocentric_d(m_aim_geo);
	}

	//double observer_base::pixel_to_width( double in_pixel_size )
	//{
	//	return in_pixel_size * tan( m_fov * 0.5 ) / ((double)m_view_port.m_height * 0.5) * (m_spherical_transform->m_position_geo - m_aim_geo).length();
	//}

	void observer_base::resize_viewport(int cx, int cy)
	{
		m_view_port.m_width = cx;
		m_view_port.m_height = cy;
	}

	void observer_base::calculate_local_parameter(void)
	{
		// 刷新相机局部空间参数
		vector_3d pos_shpr = get_position_sphr();
		spherical_transform v_transform;
		v_transform.create_from_sphr(pos_shpr.x, pos_shpr.y, pos_shpr.z, 0.0, 0.0, 0.0);
		m_view_center_mat_tnb = v_transform.m_mat_tnb;
		m_view_center_mat_tnb_invs = v_transform.m_mat_tnb_inverse;

		m_aim_local = get_aim_geo() * v_transform.m_mat_tnb_inverse;
		m_direction_local = matrix_4d::transform_normal(get_direction_geo(), v_transform.m_mat_tnb_inverse);
		m_direction_local.normalize();
	}
}

