//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_frustum.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_FRUSTUM_H_
#define _US_FRUSTUM_H_

//#include <usUtil\Export.h>
#include <usUtil/us_plane.h>
#include <usUtil/us_lon_lat_box.h>

namespace uniscope_globe
{
	template < class real >
	class frustum
	{
	public:
		plane<real> m_near_plane, m_far_plane;
		plane<real> m_left_plane, m_right_plane;
		plane<real> m_up_plane, m_down_plane;

		plane<real>* m_planes[6];

		real m_fov, m_aspect, m_near_plane_range, m_far_plane_range;

		vector3<real> m_camera_pos; //camera position


		vector3<real> m_help_x_normal;//camera x help position
		vector3<real> m_help_y_normal;//camera y help position

		vector3<real> m_positive_x; //near plane y pos
		vector3<real> m_positive_y; //near plane x

		vector3<real> m_negative_x; //near plane negative x
		vector3<real> m_negative_y; //near plane negative y pos 


		vector3<real> m_near_plane_center; //near plane center
		vector3<real> m_far_plane_center; //far near plane center


		vector3<real> m_eye_point;


		vector3<real> m_current;

	public:
		frustum(const frustum& frustum);
		//constructor
		frustum(void);

		~frustum(void);

		void update(real fov, real aspect, real near_plane_range, real far_plane_range, matrix4<real> &mat);

		void update_ex(real v_width, real v_height, real near_plane_range, real far_plane_range, matrix4<real> &mat);


		void update_ex(real v_width, real v_height, real x, real y, real w, real h, real near_plane_range, real far_plane_range, matrix4<real> &mat);

	public:
		//bool Intersects(BoundingSphere& c);
		bool contain(const lon_lat_box& bb);

		bool intersect(const lon_lat_box& lbox);

		bool contain_point(const vector3<real>& v);

		bool intersect(const axis_aligned_box<real> &bbox);

		int  contain_box(const axis_aligned_box<real>& aabb);

	};

#include "us_frustum.inl"

	typedef frustum<float> frustumf;

	typedef frustum<double> frustumd;

}

#endif //_US_FRUSTUM_H_
