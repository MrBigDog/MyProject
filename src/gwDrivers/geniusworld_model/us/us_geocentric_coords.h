//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geocentric_coords.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geocentric coordinate
//	Reference : http://mathworld.wolfram.com/GeocentricLatitude.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOCENTRIC_COORDS_H_
#define _US_GEOCENTRIC_COORDS_H_

#include "us_vector3.h"
#include "us_matrix4.h"

namespace uniscope_globe
{
	class geocentric_coords
	{
	public:
		geocentric_coords(void);
		~geocentric_coords(void);

	public:
		static vector3<double> from_spherical_d(double longitude, double latitude, double altitude);
		static vector3<double> from_spherical_d(const vector3<double>& src_position);
		static vector3<double> from_cartesian_d(const vector3<double>& src_position);
		
		static vector3<double> from_cartesian_d(const vector3<float>& src_position);
		static matrix4<double> transform_from_cartesian_d(const vector3<double>& origin_position);
		static matrix4<double> transform_from_cartesian_d(double lon, double lat, double alt);
		static matrix4<double> transform_from_cartesian_scale_d(double lon, double lat, double alt);
	};
}



#endif // _US_GEOCENTRIC_COORDS_H_
