//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_gaussian_coords.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : gaussian coordinate
//	Reference : http://mathworld.wolfram.com/GaussianCoordinateSystem.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_GAUSSIAN_COORDS_H_
#define _US_GAUSSIAN_COORDS_H_

#include <usUtil\Export.h>
#include <usUtil\us_vector3.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT gaussian_coords
	{
	public:
		gaussian_coords(void);
		~gaussian_coords(void);

	public:
		static vector3<double> from_spherical(vector3<double> src_position);

		static vector3<double> from_geocentric(vector3<double> src_position);
	};
}

#endif _US_GAUSSIAN_COORDS_H_
