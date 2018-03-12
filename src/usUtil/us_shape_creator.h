///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shape_creator.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shape_creator class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SHAPE_CREATOR_H_
#define _US_SHAPE_CREATOR_H_

#include <usUtil\Export.h>
#include <usUtil\us_vector3.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT shape_creator
	{
	public:
		shape_creator(void);
		~shape_creator(void);

		// for sphere
	public:
		static void create_circle(/*OUT*/ vector3<double>* out_circle_points, /*IN*/ vector3<double>& in_center_point, /*IN*/ double in_radius_radians, /*IN*/ int in_segment);
		static void create_circle_not_clamp(/*OUT*/ vector3<double>* out_circle_points, /*IN*/ vector3<double>& in_center_point, /*IN*/ double in_radius_radians, /*IN*/ int in_segment);

		// for flat
	public:
		static void create_circle_on_plane(/*OUT*/ vector3<double>* out_circle_points, /*IN*/ vector3<double>& in_center_point, /*IN*/ double in_radius_radians, /*IN*/ int in_segment);

		static void create_circle_normal_on_plane(/*OUT*/ vector3<double>* out_circle_normal, /*IN*/ int in_segment);
	};
}

#endif // _US_SHAPE_CREATOR_H_