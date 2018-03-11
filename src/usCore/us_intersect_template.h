///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_intersect_template.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : intersect_template class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_INTERSECT_TEMPLATE_H_
#define _US_INTERSECT_TEMPLATE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	struct part_sphere
	{
		double v_radian;
		double v_fov;
		vector_3d v_src_point;
		vector_3d v_des_point;
		lon_lat_rect m_rect;
	};

	struct bldg_part_sphere
	{
		double heading;
		double fov;
		double start_side;
		double end_side;
		double v_radian;
		vector_3d v_center;
	};

	int USCORE_EXPORT point_in_part_sphere(vector_3d& v_point, part_sphere& v_ps);

	bool USCORE_EXPORT ray_intersect_box_2d(double* minB, double* maxB, double* origin, double* dir, double* coord);

	long USCORE_EXPORT aabb2d_intersect_part_sphere(axis_aligned_box_d& v_aabb, part_sphere& v_ps);
}

#endif // _US_INTERSECT_TEMPLATE_H_ 