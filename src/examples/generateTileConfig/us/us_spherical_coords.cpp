//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_spherical_coords.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : spherical coordinate
//	Reference : http://mathworld.wolfram.com/SphericalCoordinates.html
//
//////////////////////////////////////////////////////////////////////////
#include "us_math.h"
#include "us_spherical_coords.h"
#include "us_cartesian_coords.h"

namespace uniscope_globe
{
	// WGS84 parameter
	double spherical_coords::s_semi_major = 6378.137;

	double spherical_coords::s_semi_minor = 6356.7523142451793;

	double spherical_coords::s_semi_major_meter = 6378137.0;

	double spherical_coords::s_semi_minor_meter = 6356752.3142451793;

	double spherical_coords::s_datum_plane = 6378.137; // spherical_coords::s_semi_major - 0.1

	spherical_coords::spherical_coords(void)
	{

	}

	spherical_coords::~spherical_coords(void)
	{

	}

	vector_3d spherical_coords::from_geocentric_d(const vector_3d& src_position)
	{
		vector_3d v_ret_val;
		v_ret_val.z = math<double>::sqrt_(src_position.x * src_position.x + src_position.y * src_position.y + src_position.z * src_position.z);
		v_ret_val.x = math<double>::atan2_(src_position.z, src_position.x);
		v_ret_val.y = math<double>::asin_(src_position.y / v_ret_val.z);
		return v_ret_val;
	}

	vector_3d spherical_coords::from_cartesian_d(const vector_3d& src_position)
	{
		return spherical_coords::from_geocentric_d(src_position + cartesian_coords::s_reference_position_geo);
	}

	vector_3d spherical_coords::from_cartesian_d(const vector_3f& src_position)
	{
		return spherical_coords::from_geocentric_d(vector_3d((float*)&src_position) + cartesian_coords::s_reference_position_geo);;
	}

	double spherical_coords::geodetic_circle_radians(double lon1, double lat1, double lon2, double lat2)
	{
		//double cos_lat1 = cos(lat1);
		//double cos_lat2 = cos(lat2);
		//double cos_lon1_2 = cos(lon1-lon2);
		//double sin_lon1 = sin(lon1);
		//double sin_lat1 = sin(lat1);

		double a = math<double>::cos_(lat1) *
			math<double>::cos_(lat2) *
			math<double>::cos_(lon1 - lon2) +
			math<double>::sin_(lat1) *
			math<double>::sin_(lat2);

		return math<double>::fabs_(math<double>::acos_(a));

		//return fabs(acos(cos(latA)*cos(latB)*cos(lonA-lonB)+sin(latA)*sin(latB)));

		//return fabs(acos(cos(lat1)*cos(lat2)*cos(lon1-lon2)+sin(lon1)*sin(lat2)));
	}

	double spherical_coords::geodetic_parallel_length(double width, double lat, double radius)
	{
		return width * radius * math<double>::cos_(lat);
	}
}