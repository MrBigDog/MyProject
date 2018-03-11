///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_lon_lat_rect.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : lon_lat_rect class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_LON_LAT_RECT_H_
#define _US_LON_LAT_RECT_H_

#include <usUtil\Export.h>
#include "us_circle.h"
#include "us_vector3.h"
#include "us_axis_aligned_box.h"

namespace uniscope_globe
{
	class lon_lat_region;

	class USUTIL_EXPORT lon_lat_rect  //: public lon_lat_geometry
	{
	public:
		lon_lat_rect(void);
		lon_lat_rect(double north, double south, double east, double west);
		~lon_lat_rect(void);

		void make_invalid(void);

	public:
		void combine(const vector3<double>& vertex);

		void combine(const lon_lat_rect& v_rect);

		double get_width(void);

		double get_height(void);

		vector3<double> get_center(void) const;

		vector3<double> get_conner(int type) const;

		void get_corners(vector3<double>* vec) const;

		bool operator == (const lon_lat_rect& vec) const;

		bool operator != (const lon_lat_rect& vec) const;

	public:
		bool contain(vector3<double>& in_vector) const;

		bool intersect(const lon_lat_rect& in_rect) const;

		//bool intersect( std::vector< vector3<double> >& vec_array );

		bool intersect(const lon_lat_region& in_region) const;

		bool intersect(const axis_aligned_box_d& aabb) const;

		bool intersect(const circle& v_circle) const;

	public:
		vector_3d nearest_point_sphr(vector_3d& vec_sphr);

	public:
		static lon_lat_rect from_aabb(vector3<double>& in_pos_sphr, axis_aligned_box_d& in_aabb);

		static vector_3d rect_nearest_point(const lon_lat_rect* in_rect, const vector3<double>& in_pt);

	public:
		double m_north;
		double m_south;
		double m_east;
		double m_west;
	};
}

#endif // _US_LON_LAT_RECT_H_