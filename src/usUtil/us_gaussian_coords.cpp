//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_gaussian_coords.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : gaussian coordinate
//	Reference : http://mathworld.wolfram.com/GaussianCoordinateSystem.html
//
//////////////////////////////////////////////////////////////////////////

#include "us_gaussian_coords.h"

namespace uniscope_globe
{
	gaussian_coords::gaussian_coords( void )
	{

	}

	gaussian_coords::~gaussian_coords( void )
	{

	}

	vector3<double> gaussian_coords::from_spherical( vector3<double> src_position )
	{
		return vector3<double>::s_zero;
	}

	vector3<double> gaussian_coords::from_geocentric( vector3<double> src_position )
	{
		return vector3<double>::s_zero;
	}
}