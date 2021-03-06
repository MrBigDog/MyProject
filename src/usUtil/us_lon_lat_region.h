///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_lon_lat_region.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : lon_lat_region class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_LON_LAT_REGION_H_
#define _US_LON_LAT_REGION_H_

#include <usUtil\Export.h>
#include <usUtil\us_lon_lat_geometry.h>
//#include <usUtil\us_vector3.h>
#include <usUtil/us_lon_lat_rect.h>
#include <vector>

namespace uniscope_globe
{
	//class lon_lat_rect;

	class USUTIL_EXPORT lon_lat_region : public lon_lat_geometry
	{
	public:
		typedef std::vector< vector3<double> > vertex_array;

	public:
		lon_lat_region(void);

		virtual ~lon_lat_region(void);

		void push_back(vector3<double>& vert);

		void clear(void);

		ulong get_count(void) const;

		vector3<double> operator[] (ulong index) const;

	public:
		lon_lat_rect get_bound(void);

		//bool contain( lon_lat_rect& in_rect );

		bool contain(vector3<double> pos) const;

		bool intersect(const lon_lat_rect& in_rect);

	public:
		vertex_array m_vertex_array;

		lon_lat_rect m_bound;

	};
}

#endif // _US_LON_LAT_REGION_H_