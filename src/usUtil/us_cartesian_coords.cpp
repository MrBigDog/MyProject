//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_cartesian_coords.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : cartesian coordinate
//	Reference : http://mathworld.wolfram.com/CartesianCoordinates.html
//
//////////////////////////////////////////////////////////////////////////

#include "us_cartesian_coords.h"
#include "us_geocentric_coords.h"

namespace uniscope_globe
{
	vector3<double> cartesian_coords::s_reference_position_geo = vector3<double>::s_zero;

	vector3<double> cartesian_coords::s_reference_position_sphr = vector3<double>::s_zero;

	cartesian_coords::cartesian_coords(void)
	{

	}

	cartesian_coords::~cartesian_coords(void)
	{

	}

	vector_3f cartesian_coords::from_spherical_f(const vector3<double>& src_position)
	{
		vector3<double> v_pos = geocentric_coords::from_spherical_d(src_position) - s_reference_position_geo;
		return vector_3f((float)v_pos.x, (float)v_pos.y, (float)v_pos.z);
	}

	vector3<double> cartesian_coords::from_spherical_d(const vector3<double>& src_position)
	{
		return (geocentric_coords::from_spherical_d(src_position) - s_reference_position_geo);
	}

	vector3<float> cartesian_coords::from_geocentric_f(const vector3<double>& src_position)
	{
		return vector3<float>(float(src_position.x - s_reference_position_geo.x),
			float(src_position.y - s_reference_position_geo.y),
			float(src_position.z - s_reference_position_geo.z));
	}

	vector3<double> cartesian_coords::from_geocentric_d(const vector3<double>& src_position)
	{
		return src_position - s_reference_position_geo;
	}

	matrix4<float> cartesian_coords::transform_from_geocentric_f(double lon, double lat, double alt)
	{
		vector3<double> geocentric_pos = geocentric_coords::from_spherical_d(lon, lat, alt);

		vector3<double> vec_t, vec_n, vec_b;

		vec_n = geocentric_pos;
		vec_n.normalize();

		vec_t = vector3<double>::cross(vec_n, vector3<double>::s_unit_y);
		vec_t.normalize();

		vec_b = vector3<double>::cross(vec_t, vec_n);
		vec_b.normalize();

		matrix4<double> mat_ret = matrix4<double>::from_tnbp(vec_t, vec_n, vec_b, geocentric_pos);
		mat_ret.inverse();

		return matrix4<float>((double*)mat_ret.m);
	}

	matrix4<double> cartesian_coords::transform_from_geocentric_d(const vector3<double>& origin_position_geo)
	{
		//vector3<double> geocentric_pos = geocentric_coords::from_spherical_d( origin_position );

		vector3<double> vec_t, vec_n, vec_b;

		vec_n = origin_position_geo;
		vec_n.normalize();

		vec_t = vector3<double>::cross(vec_n, vector3<double>::s_unit_y);
		vec_t.normalize();

		vec_b = vector3<double>::cross(vec_t, vec_n);
		vec_b.normalize();

		matrix4<double> mat_ret = matrix4<double>::from_tnbp(vec_t, vec_n, vec_b, origin_position_geo);
		return mat_ret.inverse();
	}

	matrix4<double> cartesian_coords::transform_from_geocentric_d(double lon, double lat, double alt)
	{
		vector3<double> geocentric_pos = geocentric_coords::from_spherical_d(lon, lat, alt);

		vector3<double> vec_t, vec_n, vec_b;

		vec_n = geocentric_pos;
		vec_n.normalize();

		vec_t = vector3<double>::cross(vec_n, vector3<double>::s_unit_y);
		vec_t.normalize();

		vec_b = vector3<double>::cross(vec_t, vec_n);
		vec_b.normalize();

		matrix4<double> mat_ret = matrix4<double>::from_tnbp(vec_t, vec_n, vec_b, geocentric_pos);
		return mat_ret.inverse();
	}


}