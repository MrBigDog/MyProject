//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_cartesian_coords.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : cartesian coordinate
//	Reference : http://mathworld.wolfram.com/CartesianCoordinates.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef US_CARTESIAN_COORDS_H
#define US_CARTESIAN_COORDS_H 1

#include <usUtil\Export.h>
//#include <usUtil\us_vector3.h>
#include <usUtil\us_matrix4.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT cartesian_coords
	{
	public:
		cartesian_coords(void);
		~cartesian_coords(void);

	public:
		static vector3<float> from_spherical_f( const vector3<double>& src_position );

		static vector3<double> from_spherical_d( const vector3<double>& src_position );

		static vector3<float> from_geocentric_f( const vector3<double>& src_position );

		static vector3<double> from_geocentric_d( const vector3<double>& src_position );

		static matrix4<float> transform_from_geocentric_f( double lon, double lat, double alt );

		static matrix4<double> transform_from_geocentric_d( const vector3<double>& origin_position_geo );

		static matrix4<double> transform_from_geocentric_d( double lon, double lat, double alt );

	public:
		static vector3<double> s_reference_position_geo;

		static vector3<double> s_reference_position_sphr;
	};
}

#endif // _US_CARTESIAN_COORDS_H_
