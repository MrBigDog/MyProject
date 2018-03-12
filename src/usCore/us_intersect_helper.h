
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_intersect_helper.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : intersect_helper class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_INTERSECT_HELPER_H_
#define _US_INTERSECT_HELPER_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT intersect_helper
	{
	public:
		intersect_helper(void) {}
		virtual ~intersect_helper(void) {}

	public:
		static bool intersect_projected_aabb_and_rect(observer_base* v_observer, axis_aligned_box_d& v_aabb_geo, const rect& v_rect);


		static bool intersect_projected_aabb_and_point(observer_base* v_observer, axis_aligned_box_d& v_aabb_geo, const point& v_select_point);

	};
}

#endif // _US_INTERSECT_HELPER_H_ 