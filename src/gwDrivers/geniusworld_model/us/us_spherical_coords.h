//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_spherical_coords.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : spherical coordinate
//	Reference : http://mathworld.wolfram.com/SphericalCoordinates.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_SPHERICAL_COORDS_H_
#define _US_SPHERICAL_COORDS_H_

#include "us_vector3.h"

namespace uniscope_globe
{
	class spherical_coords
	{
	public:
		spherical_coords(void);
		~spherical_coords(void);

	public:
		static vector_3d from_geocentric_d(const vector_3d&  src_position);
		static vector_3d from_cartesian_d(const vector_3d&  src_position);
		static vector_3d from_cartesian_d(const vector_3f&  src_position);

		static double geodetic_circle_radians(double lon1, double lat1, double lon2, double lat2);
		static double geodetic_parallel_length(double width, double lat, double radius);

	public:
		static double s_semi_major;
		static double s_semi_minor;
		static double s_datum_plane;
		static double s_semi_major_meter;
		static double s_semi_minor_meter;
	};
}

#endif // _US_SPHERICAL_COORDS_H_


