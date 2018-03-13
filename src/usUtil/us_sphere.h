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
#ifndef _US_SPHERE_H_
#define _US_SPHERE_H_

#include <usUtil\us_ray.h>

namespace uniscope_globe
{
	//class ray;

	template <class real>
	class sphere
	{
	public:
		vector3<real> sphere_center;

		real		  sphere_radius;

		// construct / destruct
	public:
		// initialize sphere with radius = 1.0, center = s_zero
		sphere(void)
			: sphere_radius(1.0)
			, sphere_center(vector3<real>::s_zero)
		{
		}

		sphere(const vector3<real>& center, real radius);

		sphere(const sphere& src);

		// operator
	public:
		sphere& operator = (const sphere& src);

	public:
		// Returns whether or not this sphere intersects another sphere. 
		bool intersects(const sphere& src) const
		{
			return (src.sphere_center - sphere_center).length() <= (src.sphere_radius + sphere_radius);
		}

		// Returns whether or not this sphere intersects a point.
		bool intersects(const vector3<real>& v) const
		{
			return ((v - sphere_center).length() <= sphere_radius);
		}

		int intersects(const ray<real>& in_ray, vector3<real>& result_vec) const
		{
			vector3<real> v_sphere_to_ray = in_ray.ray_origin - sphere_center;

			double v_sphere_to_ray_length_sq = v_sphere_to_ray.dot(v_sphere_to_ray);
			double v_length = v_sphere_to_ray_length_sq - sphere_radius * sphere_radius;

			if (v_length <= 0)
			{
				//v_sphere_to_ray.normalize();
				real a0 = v_sphere_to_ray.dot(in_ray.ray_direction);
				real h0_sq = v_sphere_to_ray_length_sq - a0 * a0;

				real h1 = math<real>::sqrt_(sphere_radius * sphere_radius - h0_sq);
				result_vec = in_ray.ray_origin + in_ray.ray_direction * (h1 - a0);

				return 1;
			}

			real v_length1 = in_ray.ray_direction.dot(v_sphere_to_ray);

			if (v_length1 >= 0) return 0;

			real v_dif = v_length1 * v_length1 - v_length;
			if (v_dif < 0)
			{
				return 0;
			}
			else if (v_dif >= math<real>::s_epsilon)
			{
				real v_root = math<real>::sqrt_(v_dif);
				real a0 = -v_length1 - v_root;
				result_vec = in_ray.ray_origin + a0 * in_ray.ray_direction;
				return 2;
			}
			else
			{
				result_vec = in_ray.ray_origin - v_length1 * in_ray.ray_direction;
				return 1;
			}
		}
	};

#include "us_sphere.inl"

	typedef sphere<float>	sphere_3f;
	typedef sphere<double>	sphere_3d;
}

#endif // _US_SPHERE_H_