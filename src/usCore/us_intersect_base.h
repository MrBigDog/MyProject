///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_intersect_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : intersect_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_INTERSECT_BASE_H_
#define _US_INTERSECT_BASE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class observer_base;
	class USCORE_EXPORT intersect_base
	{
	public:
		intersect_base(void) { }

		virtual ~intersect_base(void) { }

	public:
		//virtual bool pick( const ray<double>& in_ray, intersect_result& out_result ) { return false; }

		virtual bool intersect(const lon_lat_rect& in_rect, intersect_result& out_result) { return false; }

		//virtual bool intersect( const rect& in_rect, intersect_result& out_result ) { return false; }

		virtual bool intersect(observer_base* v_observer, const rect& in_rect, intersect_result& in_result) { return false; }

		virtual bool intersect(observer_base* v_observer, const point& pt, intersect_result& out_result) { return false; }

		virtual bool intersect(const ray<double>& a_ray, intersect_result& out_result) { return false; }

		virtual bool intersect(const ray<double>& a_ray, double in_range, intersect_result& out_result) { return false; }

		virtual bool intersect(const axis_aligned_box<double>& box) { return false; }

		virtual bool intersect(const circle& v_circle, intersect_result& out_result) { return false; }

		virtual bool intersect(const lon_lat_region& v_region, intersect_result& out_result) { return false; }

	public:
		virtual bool intersect(part_sphere& pt_sphr, intersect_result& out_result) { return false; }

	public:
		virtual bool intersect_material(const ray<double>& a_ray, intersect_result& result) { return false; }

	};
}

#endif // _US_INTERSECT_BASE_H_