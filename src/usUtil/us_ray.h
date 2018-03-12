//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_sphere.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : sphere template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_RAY_H_
#define _US_RAY_H_

#include <usUtil/us_matrix4.h>

namespace uniscope_globe
{
	template<class real>
	class ray
	{
	public:
		vector3<real> ray_origin;

		vector3<real> ray_direction;

		// construct / destruct
	public:
		ray( void )	: ray_origin( vector3<real>::s_zero ), ray_direction( vector3<real>::s_unit_z )
		{
		}

		ray( const vector3<real>& origin, const vector3<real> direction )
			: ray_origin( origin ), ray_direction( direction )
		{
		}

		// method
	public:
		vector3<real> get_point( real t ) const
		{ 
			return (ray_origin + (ray_direction * t));
		}

		void transform( matrix4<real>& in_mat)
		{
			this->ray_origin = matrix4<real>::transform_coordinate( this->ray_origin, in_mat);
			this->ray_direction = matrix4<real>::transform_normal( this->ray_direction, in_mat );
			this->ray_direction.normalize();
		}
	public:
		static ray<real> transform( const ray<real>& in_ray, matrix4<real>& in_mat)
		{
			ray<real> out_ray = in_ray;
			out_ray.transform(in_mat);
			return out_ray;
		}


	}; // class ray

	typedef ray<float>	ray_3f;
	typedef ray<double> ray_3d;

} // namespace uniscope_globe

#endif // _US_RAY_H_


