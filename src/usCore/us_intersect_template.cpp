///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_intersect_template.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : intersect_template class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_intersect_template.h"

namespace uniscope_globe
{
	int point_in_part_sphere( vector_3d& v_point, part_sphere& v_ps )
	{
		if ( (v_point - v_ps.v_src_point).length() > v_ps.v_radian )
			return 1;

		vector_3d vec_dir = v_ps.v_des_point - v_ps.v_src_point;
		vec_dir.y = 0.0;
		vec_dir.normalize();

		vector_3d vec_ptos = v_point - v_ps.v_src_point;
		vec_ptos.y = 0.0;
		vec_ptos.normalize();

		double v_threshold = math_d::cos_( v_ps.v_fov * 0.5 );
		if ( vector_3d::dot(vec_dir, vec_ptos ) > v_threshold )
			return 0;

		return 1;
	}

	bool ray_intersect_box_2d( double* minB, double* maxB, double* origin, double* dir,double* coord )
	{
		bool inside = true;
		int quadrant[2];
		register int i;
		int whichPlane;
		double maxT[2];
		double candidatePlane[2];

		/* Find candidate planes; this loop can be avoided if
		rays cast all from the eye(assume perpsective view) */
		for (i=0; i<2; i++)
			if(origin[i] < minB[i]) {
				quadrant[i] = LEFT;
				candidatePlane[i] = minB[i];
				inside = false;
			}else if (origin[i] > maxB[i]) {
				quadrant[i] = RIGHT;
				candidatePlane[i] = maxB[i];
				inside = false;
			}else	{
				quadrant[i] = MIDDLE;
			}

			/* Ray origin inside bounding box */
			if(inside)	{
				coord = origin;
				return true;
			}


			/* Calculate T distances to candidate planes */
			for (i = 0; i < 2; i++)
				if (quadrant[i] != MIDDLE && math<double>::fabs_( (double)dir[i] ) > math<double>::s_zero_tolerance )
					maxT[i] = (candidatePlane[i]-origin[i]) / dir[i];
				else
					maxT[i] = -1.;

			/* Get largest of the maxT's for final choice of intersection */
			whichPlane = 0;
			for (i = 1; i < 2; i++)
				if (maxT[whichPlane] < maxT[i])
					whichPlane = i;

			/* Check final candidate actually inside box */
			if (maxT[whichPlane] < (double)0 ) return false;
			for (i = 0; i < 2; i++)
				if (whichPlane != i) {
					coord[i] = origin[i] + maxT[whichPlane] *dir[i];
					if (coord[i] < minB[i] || coord[i] > maxB[i])
						return false;
				} else {
					coord[i] = candidatePlane[i];
				}


				return true;				/* ray hits box */
	}


	long aabb2d_intersect_part_sphere( axis_aligned_box_d& v_aabb, part_sphere& v_ps )
	{
		long ret = -1;

		// calcularate nearest distance
		vector_3d vec_min = v_aabb.get_min();
		vector_3d vec_max = v_aabb.get_max();

		vector_3d v_nearest_pos = v_ps.v_src_point;

		v_nearest_pos.x = max( vec_min.x, v_nearest_pos.x );
		v_nearest_pos.x = min( vec_max.x, v_nearest_pos.x );

		v_nearest_pos.y = max( vec_min.y, v_nearest_pos.y );
		v_nearest_pos.y = min( vec_max.y, v_nearest_pos.y );

		v_nearest_pos.z = max( vec_min.z, v_nearest_pos.z );
		v_nearest_pos.z = min( vec_max.z, v_nearest_pos.z );

		if ( (v_nearest_pos-v_ps.v_src_point).length() > v_ps.v_radian )
			return ret;

		if ( v_nearest_pos == v_ps.v_src_point )
			return 1;

		ret = 0;

		// calculate fan angle
		vector3<double>* v_corners = (vector3<double>*)(v_aabb.get_corners());
		for ( int i = 0 ; i <  8 ; i++ )
		{
			vector_3d vec( v_corners[i].x, v_corners[i].y, v_corners[i].z );
			if ( point_in_part_sphere( v_corners[i], v_ps ) == 0 )
				ret++;
		}

		if ( ret > 0 )
			return ret;

		ret = -1;

		//calculate heading
		vector_3d vec_dir = v_ps.v_des_point - v_ps.v_src_point;
		double v_angle = 0.0;
		if ( vec_dir.x == 0 )
		{
			v_angle = (vec_dir.z>0)?math_d::s_half_pi:(3*math_d::s_half_pi);
		}
		else
		{
			v_angle = math_d::atan_(vec_dir.z/vec_dir.x);
			if ( vec_dir.x < 0 )
				v_angle += math_d::s_pi;
			else if ( vec_dir.x > 0 && vec_dir.z < 0 )
				v_angle += math_d::s_two_pi;
		}
		double v_left_angle = v_angle - v_ps.v_fov * 0.5;
		double v_right_angle = v_angle + v_ps.v_fov * 0.5;

		// calculate intersect
		double box_min[2], box_max[2];
		{
			box_min[0] = vec_min.x; box_min[1] = vec_min.z;
			box_max[0] = vec_max.x; box_max[1] = vec_max.z;
		}
		double left_ray_pos[2], left_ray_dir[2];
		{
			left_ray_pos[0] = v_ps.v_src_point.x;
			left_ray_pos[1] = v_ps.v_src_point.z;

			left_ray_dir[0] = v_ps.v_radian * math_d::cos_( v_left_angle );// - v_ps.v_src_point.x;
			left_ray_dir[1] = v_ps.v_radian * math_d::sin_( v_left_angle );// - v_ps.v_src_point.z;
		}
		double right_ray_pos[2], right_ray_dir[2];
		{
			right_ray_pos[0] = v_ps.v_src_point.x;
			right_ray_pos[1] = v_ps.v_src_point.z;

			right_ray_dir[0] = v_ps.v_radian * math_d::cos_( v_right_angle );// - v_ps.v_src_point.x;
			right_ray_dir[1] = v_ps.v_radian * math_d::sin_( v_right_angle );// - v_ps.v_src_point.z;
		}

		double v_coord[2];

		if ( ray_intersect_box_2d( box_min, box_max, left_ray_pos, left_ray_dir, v_coord ) )
		{
			return 1;
		}

		if ( ray_intersect_box_2d( box_min, box_max, right_ray_pos, right_ray_dir, v_coord ) )
		{
			return 1;
		}

		return ret;
	}
	
}