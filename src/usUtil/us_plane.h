//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_plane.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_PLANE_H_
#define _US_PLANE_H_

namespace uniscope_globe
{
	template <class real>
	class plane
	{
	public:
		plane(void);

		~plane(void);

		plane(const plane& one);

		plane(float* p);

		plane(double* p);

		// specify N and c directly
		plane(const vector3<real>& normal, real constant);

		// N is specified, c = Dot(N,P) where P is on the plane
		plane(const vector3<real>& normal, const vector3<real>& pt);

		// N = Cross(P1-P0,P2-P0)/Length(Cross(P1-P0,P2-P0)), c = Dot(N,P0) where
		// P0, P1, P2 are points on the plane.
		plane(const vector3<real>& p0, const vector3<real>& p1, const vector3<real>& p2);

		void create(const vector3<real>& normal, real constant);
		void create(const vector3<real>& normal, const vector3<real>& pt);
		void create(const vector3<real>& p0, const vector3<real>& p1, const vector3<real>& p2);


		//
		// assignment
		//
		plane& operator= (const plane& one);

		// The "positive side" of the plane is the half space to which the plane
		// normal points.  The "negative side" is the other half space.  The
		// function returns +1 for the positive side, -1 for the negative side,
		// and 0 for the point being on the plane.
		int which_side(const vector3<real>& pt) const;

		// Compute d = Dot(N,Q)-c where N is the plane normal and c is the plane
		// constant.  This is a signed distance.  The sign of the return value is
		// positive if the point is on the positive side of the plane, negative if
		// the point is on the negative side, and zero if the point is on the
		// plane.
		real distance_to(const vector3<real>& q) const;

		real dot_normal(const vector3<real>& vec) const;

		real dot_coord(const vector3<real>& vec) const;

		vector3<real> projection(const vector3<real>& in_vec);

		vector3<real> point_projection(const vector3<real>& in_vec);

		int	calcu_side(const vector3<real>& vec) const;

		double distance_to_point(const vector3<real>& vec) const;

		bool intersect(const ray_3d& v_ray, vector3<real>& ret_vec);

		bool intersect(const vector3<real>& v_bgein, const vector3<real>& v_end, vector3<real>& ret_vec);

		void transform(matrix4<real>& trans_mat);

	public:
		static plane<real> transform(plane<real>& v_plane, matrix4<real>& trans_mat);

	public:
		vector3<real> m_normal;

		real m_constant;
	};

#include "us_plane.inl"

	typedef plane<float> plane_3f;
	typedef plane<double> plane_3d;

}

#endif //_US_PLANE_H_

