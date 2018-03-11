//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_coordinate_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : coordinate base class
//	Reference :
//
//////////////////////////////////////////////////////////////////////////

#include "us_coordinate_base.h"
#include <usUtil/us_geocentric_coords.h>
#include <usUtil/us_spherical_coords.h>

namespace uniscope_globe
{
	coordinate_base::coordinate_base(void)
	{
		m_sampling_interval = 1.0;
	}

	coordinate_base::~coordinate_base(void)
	{

	}

	coordinate_base::coordinate_base(vector3<double> in_origin_geo, vector3<double> in_direction, vector3<double> in_up)
	{
		m_sampling_interval = 1.0;

		m_origin_geo = in_origin_geo;

		//
		// 建立笛卡尔坐标系
		//

		// 坐标轴
		z_axis = vector3<double>::normalize(in_direction);
		x_axis = vector3<double>::normalize(vector3<double>::cross(in_up, z_axis));
		y_axis = vector3<double>::cross(z_axis, x_axis);
		y_axis.normalize();

		// 变换阵
		m_mat_to_geo = matrix4<double>::from_tnbp(x_axis, y_axis, z_axis, in_origin_geo);
		m_mat_to_tangent_plane = m_mat_to_geo.inverse();
	}

	vector3<double> coordinate_base::from_spherical(vector3<double> in_pos)
	{
		return geocentric_coords::from_spherical_d(in_pos) * m_mat_to_tangent_plane;
	}

	vector3<double> coordinate_base::from_spherical(double in_longitude, double in_latitude)
	{
		vector3<double> v_pos;
		v_pos.x = in_longitude;
		v_pos.y = in_latitude;
		v_pos.z = spherical_coords::s_semi_major;

		return geocentric_coords::from_spherical_d(v_pos) * m_mat_to_tangent_plane;
	}

	vector3<double> coordinate_base::to_spherical(vector3<double> in_pos_local)
	{
		return spherical_coords::from_geocentric_d(in_pos_local * m_mat_to_geo);
	}

	vector3<double> coordinate_base::from_geocentric(vector3<double> in_pos)
	{
		return in_pos * m_mat_to_tangent_plane;
	}

	vector3<double> coordinate_base::to_geocentric(vector3<double> in_pos)
	{
		return in_pos * m_mat_to_geo;
	}

	vector3<int> coordinate_base::from_geocentric_with_sampling(vector3<double> in_pos)
	{
		vector3<double> new_pos = in_pos * m_mat_to_tangent_plane;

		return vector3<int>(new_pos.x / m_sampling_interval, new_pos.y / m_sampling_interval, new_pos.z / m_sampling_interval);
	}

	vector3<double> coordinate_base::to_geocentric_with_sampling(vector3<int> in_pos)
	{
		vector3<double> ret_val;
		ret_val.x = in_pos.x * m_sampling_interval;
		ret_val.y = in_pos.y * m_sampling_interval;
		ret_val.z = in_pos.z * m_sampling_interval;

		return ret_val * m_mat_to_geo;
	}

	void coordinate_base::set_sampling_interval(double in_sampling_interval)
	{
		m_sampling_interval = in_sampling_interval;
	}

	double coordinate_base::get_sampling_interval(void)
	{
		return m_sampling_interval;
	}
}


