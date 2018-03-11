///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_tangent_coords_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : tangent_coords_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_tangent_coords_base.h"
#include <usUtil/us_spherical_coords.h>
#include <usUtil/us_geocentric_coords.h>

namespace uniscope_globe
{
	tangent_coords_base::tangent_coords_base(void)
	{
	}

	tangent_coords_base::~tangent_coords_base(void)
	{
	}

	tangent_coords_base::tangent_coords_base(double in_longitude, double in_latitude)
	{
		m_sampling_interval = 1.0;

		m_origin_sphr.x = in_longitude;
		m_origin_sphr.y = in_latitude;
		m_origin_sphr.z = spherical_coords::s_semi_major;

		m_origin_geo = geocentric_coords::from_spherical_d(m_origin_sphr);

		//
		// 建立切平面坐标系
		//

		// 坐标轴
		y_axis = vector3<double>::normalize(m_origin_geo);
		x_axis = vector3<double>::cross(y_axis, vector3<double>::s_unit_y);
		x_axis.normalize();
		z_axis = vector3<double>::cross(x_axis, y_axis);
		z_axis.normalize();

		// 变换阵
		m_mat_to_geo = matrix4<double>::from_tnbp(x_axis, y_axis, z_axis, m_origin_geo);
		m_mat_to_tangent_plane = m_mat_to_geo.inverse();
	}

	tangent_coords_base::tangent_coords_base(vector3<double> in_origin_sphr)
	{
		m_sampling_interval = 1.0;

		m_origin_sphr = in_origin_sphr;

		m_origin_geo = geocentric_coords::from_spherical_d(m_origin_sphr);

		//
		// 建立切平面坐标系
		//

		// 坐标轴
		y_axis = vector3<double>::normalize(m_origin_geo);
		x_axis = vector3<double>::cross(y_axis, vector3<double>::s_unit_y);
		x_axis.normalize();
		z_axis = vector3<double>::cross(x_axis, y_axis);
		z_axis.normalize();

		// 变换阵
		m_mat_to_geo = matrix4<double>::from_tnbp(x_axis, y_axis, z_axis, m_origin_geo);
		m_mat_to_tangent_plane = m_mat_to_geo.inverse();
	}

}