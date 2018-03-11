///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_region.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : region class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_REGION_H_
#define _US_REGION_H_

#include <usUtil\Export.h>
#include <usUtil\us_rect.h>
#include <usUtil\us_point.h>
#include <usUtil\us_common_file.h>
#include <vector>

namespace uniscope_globe
{
	class USUTIL_EXPORT region
	{
	public:
		typedef std::vector<point> point_array;

	public:
		region(void);

		virtual ~region(void);

	public:
		void add_point(point& pt);

		void clear(void);

		ulong get_size(void);

		point get_at(ulong index);

	public:
		rect get_bound(void);

		bool contain(const point& pt);

	public:
		bool intersect(const rect& in_rect);

	public:
		point_array m_point_array;

		rect		m_bound;
	};
}

#endif // _US_REGION_H_