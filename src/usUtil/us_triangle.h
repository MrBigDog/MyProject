///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_triangle.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : triangle class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TRIANGLE_H_
#define _US_TRIANGLE_H_

#include <usUtil\Export.h>

namespace uniscope_globe
{
	template <class real>
	class triangle
	{
	public:
		// construction
		triangle();
		~triangle();
		triangle( const vector3<real>& rkv0, const vector3<real>& rkv1, const vector3<real>& rkv2 );
		triangle( const vector3<real> akv[3] );

		// distance from the point Q to the triangle
		real distance(const vector3<real>& rkq) const;

		real area() const;

		static bool intersect( ray<real>& in_ray, triangle<real>& in_triangle );

		static bool pick( ray<real>& in_ray, triangle<real>& in_triangle, vector3<real>& out_point, real min_t );

		vector3<real> m_v[3];
	};

#include "us_triangle.inl"

	typedef triangle<float> triangle_f;
	typedef triangle<double> triangle_d;
}

#endif // _US_TRIANGLE_H_