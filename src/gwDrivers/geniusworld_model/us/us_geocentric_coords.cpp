//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geocentric_coords.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geocentric coordinate
//	Reference : http://mathworld.wolfram.com/GeocentricLatitude.html
//
//////////////////////////////////////////////////////////////////////////

#include "us_math.h"
#include "us_cartesian_coords.h"
#include "us_geocentric_coords.h"

namespace uniscope_globe
{
	geocentric_coords::geocentric_coords(void)
	{}

	geocentric_coords::~geocentric_coords(void)
	{}

	vector3<double> geocentric_coords::from_spherical_d(double longitude, double latitude, double altitude)
	{
		double v_radius = altitude;
		double v_cos_lat = v_radius * math<double>::cos_(latitude);
		return vector3<double>(v_cos_lat * math<double>::cos_(longitude), v_radius * math<double>::sin_(latitude), v_cos_lat * math<double>::sin_(longitude));
	}

	vector3<double> geocentric_coords::from_spherical_d(const vector3<double>& src_position)
	{
		double v_radius = src_position.z;
		double v_cos_lat = v_radius * math<double>::cos_(src_position.y);
		return vector3<double>(v_cos_lat * math<double>::cos_(src_position.x), v_radius * math<double>::sin_(src_position.y), v_cos_lat * math<double>::sin_(src_position.x));
	}

	vector3<double> geocentric_coords::from_cartesian_d(const vector3<double>& src_position)
	{
		return src_position + cartesian_coords::s_reference_position_geo;
	}

	vector3<double> geocentric_coords::from_cartesian_d(const vector3<float>& src_position)
	{
		vector3<double> ret_val(src_position.x + cartesian_coords::s_reference_position_geo.x,
			src_position.y + cartesian_coords::s_reference_position_geo.y,
			src_position.z + cartesian_coords::s_reference_position_geo.z);
		return ret_val;
	}

	matrix4<double> geocentric_coords::transform_from_cartesian_d(const vector3<double>& origin_position)
	{
		vector3<double> vec_t, vec_n, vec_b;

		vec_n = origin_position;
		vec_n.normalize();

		vec_t = vector3<double>::cross(vec_n, vector3<double>::s_unit_y);
		vec_t.normalize();

		vec_b = vector3<double>::cross(vec_t, vec_n);
		vec_b.normalize();

		return matrix4<double>::from_tnbp(vec_t, vec_n, vec_b, origin_position);
	}

	matrix4<double> geocentric_coords::transform_from_cartesian_d(double lon, double lat, double alt)
	{
		vector3<double> geocentric_pos = geocentric_coords::from_spherical_d(lon, lat, alt);

		vector3<double> vec_t, vec_n, vec_b;

		vec_n = geocentric_pos;
		vec_n.normalize();

		vec_t = vector3<double>::cross(vec_n, vector3<double>::s_unit_y);
		vec_t.normalize();

		vec_b = vector3<double>::cross(vec_t, vec_n);
		vec_b.normalize();

		return matrix4<double>::from_tnbp(vec_t, vec_n, vec_b, geocentric_pos);
	}

	matrix4<double> geocentric_coords::transform_from_cartesian_scale_d(double lon, double lat, double alt)
	{
		vector3<double> geocentric_pos = geocentric_coords::from_spherical_d(lon, lat, alt);

		vector3<double> vec_t, vec_n, vec_b;

		vec_n = geocentric_pos;
		vec_n.normalize();

		vec_t = vector3<double>::cross(vec_n, vector3<double>::s_unit_y);
		vec_t.normalize();

		vec_b = vector3<double>::cross(vec_t, vec_n);
		vec_b.normalize();

		matrix4<double> rotate_mat = matrix_4d::from_tnbp(vec_t, vec_n, vec_b, vector_3d(0.0, 0.0, 0.0));
		matrix4<double> scale_mat = matrix_4d::scaling(0.001, 0.001, 0.001);
		matrix4<double> transform_mat = matrix_4d::translation(geocentric_pos.x, geocentric_pos.y, geocentric_pos.z);

		return scale_mat *rotate_mat * transform_mat;

	}
}