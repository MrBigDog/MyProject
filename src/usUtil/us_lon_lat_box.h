//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_lon_lat_box.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : angle template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_LON_LAT_BOX_H_
#define _US_LON_LAT_BOX_H_

#include <usUtil\Export.h>
#include <usUtil\us_vector3.h>
#include <usUtil\us_matrix4.h>
#include <usUtil\us_lon_lat_rect.h>
#include <usUtil\us_lon_lat_geometry.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT lon_lat_box : public lon_lat_geometry
	{
	public:
		vector_3d corner_value[8];

	public:
		// default construct
		lon_lat_box(void);

		lon_lat_box(lon_lat_rect& rect);

		lon_lat_box(lon_lat_rect& rect, double val1, double val2);

		lon_lat_box(double north, double south, double east, double west, double val1, double val2);

		~lon_lat_box(void);

	public:
		void set_extent(lon_lat_rect& rect);

		void set_extent(lon_lat_rect& rect, double val1, double val2);

		void set_extent(double north, double south, double east, double west, double val1, double val2);

		vector_3d get_center(void);

		axis_aligned_box_d get_aabb(void);

		axis_aligned_box_d get_aabb(matrix_4d& mat);

	}; //lon_lat_box

} // uniscope_globe

#endif // #define _US_LON_LAT_BOX_H_
