//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_axis_aligned_box.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_AXIS_ALIGNED_BOX_H_
#define _US_AXIS_ALIGNED_BOX_H_

#include <usUtil\Export.h>
#include <usUtil\us_sphere.h>

namespace uniscope_globe
{
	//class ray;

	template < class real >
	class axis_aligned_box
	{
	public:
		axis_aligned_box(void);
		axis_aligned_box(const vector3<real>& min_val, const vector3<real>& max_val);
		axis_aligned_box(vector3<real>* verts, int count);

		virtual ~axis_aligned_box(void);

	public:
		void combine(axis_aligned_box& rBBox);

		void combine(const vector3<real>& vertex);

		bool inside(const vector3<real>& vec);

		bool is_in(axis_aligned_box &bbox);

		real volumn(void);

		real width(void);

		real length(void);

		real height(void);

		real get_max_radius(void) const;

		vector3<real> center(void) const;

		// see implementation ( copy from Graphics GEM1)
		bool HitBoundingBox(real* minB, real* maxB, real* origin, real* dir, real* coord);

		bool intersect(const ray<real>& a_ray);

		bool intersect(const axis_aligned_box& a_box);

		bool intersect(const sphere<real>& rSphere);

		bool is_valid(void);

		void make_invalid();

		void set_extent(const vector3<real>& min_val, const vector3<real>& max_val);

		void update_corners();

		vector3<real> get_min() const;

		vector3<real> get_max() const;

		const vector3<real> *get_corners() const;

		void transform(matrix4<real>& trans_mat);

		void translate(vector3<real>& vec);

		double nearest_distance_to_point(vector3<real>& in_pt);

		vector3<real> nearest_point_to_point(vector3<real>& in_pt);

	public:
		static axis_aligned_box<real> transform(axis_aligned_box<real>& v_aabb, matrix4<real>& trans_mat);

	protected:
		vector3<real> m_min_vector;

		vector3<real> m_max_vector;

		vector3<real> m_corners[8];
	};

#include "us_axis_aligned_box.inl"

	typedef axis_aligned_box<float> axis_aligned_box_f;
	typedef axis_aligned_box<double> axis_aligned_box_d;
}

#endif //_US_AXIS_ALIGNED_BOX_H_
