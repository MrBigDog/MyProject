
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_intersect_helper.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : intersect_helper class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_intersect_helper.h"

namespace uniscope_globe
{
	bool intersect_helper::intersect_projected_aabb_and_rect( observer_base* v_observer, axis_aligned_box_d& v_aabb_geo, const rect& v_select_rect )
	{
		vector3<double> v_center = observer_helper::projection_goecentric( v_observer, v_aabb_geo.center());
		if(v_center.z > 1 || v_center.z < 0) return false;
		if( v_select_rect.contain( point( v_center.x, v_center.y ) ) )
		{
			return true;
		}

		const vector3<double>* corners = v_aabb_geo.get_corners();

		rect v_aabb_rect;
		v_aabb_rect.make_invalid();
		for( int i = 0; i < 8; i++ )
		{
			vector3<double> vec = observer_helper::projection_goecentric( v_observer, corners[i] );
			if(vec.z > 1 || vec.z < 0) continue;
			point pt( vec.x, vec.y );
			if( v_select_rect.contain( pt ) )
			{
				return true;
			}
			v_aabb_rect.combine( pt );
		}

		if ( v_aabb_rect.intersect( v_select_rect ) )
		{
			return true;
		}

		return false;
	}


	bool intersect_helper::intersect_projected_aabb_and_point( observer_base* v_observer, axis_aligned_box_d& v_aabb_geo, const point& v_select_point )
	{
		point v_point = v_select_point;
		ray<double> v_ray_geo = v_observer->calcu_ray(v_point);
		return v_aabb_geo.intersect( v_ray_geo ); 
		//return false;
	}


	
}