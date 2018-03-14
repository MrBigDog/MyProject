
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_observer_helper.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : observer_helper class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_OBSERVER_HELPER_H_
#define _US_OBSERVER_HELPER_H_

#include <usCore\Export.h>
#include <usUtil\us_vector3.h>
#include <usUtil\us_region.h>

namespace uniscope_globe
{
	class observer_base;

	class USCORE_EXPORT observer_helper
	{
	public:
		observer_helper(void) {}

		virtual ~observer_helper(void) {}

	public:
		static vector_3d projection_goecentric(observer_base* in_observer, const vector_3d& vector_geo);

		static vector_3d projection_world(observer_base* in_observer, const vector_3d& vector_world);

		static vector_3f projection_world_f(observer_base* in_observer, const vector_3d& vector_world);

		static void project_line_sphr(observer_base* in_observer, coordinates_array_f& out_vec_array_scrn, coordinates_array_d& in_vec_array_sphr);

		static void project_line_geo(observer_base* in_observer, coordinates_array_f& out_vec_array_scrn, coordinates_array_d& in_vec_array_geo);

		static void project_line_cart_strip_ex(observer_base* in_observer, coordinates_array& out_vec_array_scrn, coordinates_array_d& in_vec_array_cart, matrix_4d& tnb_matrix);

		static void project_region_sphr(observer_base* in_observer, region& out_region_scrn, coordinates_array_d& in_vec_array_sphr);

		static void project_region_geo(observer_base* in_observer, region& out_region_scrn, coordinates_array_d& in_vec_array_geo);

	public:
		static vector_3d unprojection_geocentric(observer_base* in_observer, const vector_3d& vector_on_viewport);

		static vector_3d unprojection_world(observer_base* in_observer, const vector_3d& vector_on_viewport);

	};
}

#endif // _US_OBSERVER_HELPER_H_ 