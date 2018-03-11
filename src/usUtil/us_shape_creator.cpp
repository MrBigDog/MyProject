///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shape_creator.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shape_creator class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_shape_creator.h"
#include <usUtil/us_math.h>
#include <usUtil/us_matrix4.h>
#include <usUtil/us_geocentric_coords.h>

namespace uniscope_globe
{
	shape_creator::shape_creator(void)
	{

	}

	shape_creator::~shape_creator(void)
	{

	}

	void shape_creator::create_circle(/*OUT*/ vector3<double>* out_circle_points, /*IN*/ vector3<double>& in_center_point, /*IN*/ double in_radius_radians, /*IN*/ int in_segment)
	{
		vector3<double> v_center_point_geo = geocentric_coords::from_spherical_d(in_center_point);
		vector3<double> v_direction = vector3<double>::normalize(v_center_point_geo);
		vector3<double> v_rigth = v_direction.cross(vector3<double>::s_unit_y);
		matrix4<double> mat_rotate = matrix4<double>::rotate_axis_lh(vector3<double>::s_zero, vector3<double>::s_zero + v_rigth, in_radius_radians);
		vector3<double> v_north_geo = matrix4<double>::transform_coordinate(v_center_point_geo, mat_rotate);

		for (int ni = 0; ni < in_segment; ni++)
		{
			matrix4<double> mat_rotate = matrix4<double>::rotate_axis_lh(vector3<double>::s_zero, vector3<double>::s_zero + v_center_point_geo, math<double>::s_two_pi / in_segment * ni);
			out_circle_points[ni] = matrix4<double>::transform_coordinate(v_north_geo, mat_rotate);
		}
	}

	void shape_creator::create_circle_not_clamp(/*OUT*/ vector3<double>* out_circle_points, /*IN*/ vector3<double>& in_center_point, /*IN*/ double in_radius_radians, /*IN*/ int in_segment)
	{
		vector3<double> v_center_point_geo = geocentric_coords::from_spherical_d(in_center_point);
		vector3<double> v_direction = vector3<double>::normalize(v_center_point_geo);
		vector3<double> v_rigth = v_direction.cross(vector3<double>::s_unit_y);
		matrix4<double> mat_rotate = matrix4<double>::rotate_axis_lh(vector3<double>::s_zero, vector3<double>::s_zero + v_rigth, in_radius_radians);
		vector3<double> v_north_geo = matrix4<double>::transform_coordinate(v_center_point_geo, mat_rotate);
		v_north_geo *= 1 / math_d::cos_(in_radius_radians);

		for (int ni = 0; ni < in_segment; ni++)
		{
			matrix4<double> mat_rotate = matrix4<double>::rotate_axis_lh(vector3<double>::s_zero, vector3<double>::s_zero + v_center_point_geo, math<double>::s_two_pi / in_segment * ni);
			out_circle_points[ni] = matrix4<double>::transform_coordinate(v_north_geo, mat_rotate);
		}
	}

	void shape_creator::create_circle_on_plane(/*OUT*/ vector3<double>* out_circle_points, /*IN*/ vector3<double>& in_center_point, /*IN*/ double in_radius_radians, /*IN*/ int in_segment)
	{
		double v_x = 0.0, v_z = 0.0;
		double v_y = in_center_point.y;

		double v_angle = 0.0;
		double v_angle_step = math_d::s_two_pi / in_segment;
		for (int ni = 0; ni < in_segment; ni++)
		{
			v_x = in_center_point.x + math_d::sin_(v_angle) * in_radius_radians;
			v_z = in_center_point.z + math_d::cos_(v_angle) * in_radius_radians;
			out_circle_points[ni] = vector_3d(v_x, v_y, v_z);

			v_angle -= v_angle_step;
		}
	}

	void shape_creator::create_circle_normal_on_plane(/*OUT*/ vector3<double>* out_circle_normal, /*IN*/ int in_segment)
	{
		vector_3d vec(0.0, 0.0, 0.0);

		double v_angle = 0.0;
		double v_angle_step = math_d::s_two_pi / in_segment;
		for (int ni = 0; ni < in_segment; ni++)
		{
			vec.x = math_d::sin_(v_angle);
			vec.y = 0.0;
			vec.z = math_d::cos_(v_angle);
			out_circle_normal[ni] = vector_3d::normalize(vec);

			v_angle -= v_angle_step;
		}
	}

}