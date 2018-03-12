///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_cartesian_transform.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : cartesian_transform class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_CARTESIAN_TRANSFORM_H_
#define _US_CARTESIAN_TRANSFORM_H_

#include <usUtil/Export.h>
#include <usUtil/us_transform_base.h>
#include <usUtil/us_quaternion.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT cartesian_transform : public transform_base
	{
	public:
		cartesian_transform(void);

		cartesian_transform(const cartesian_transform& new_transform);


		virtual ~cartesian_transform(void);

	public:
		virtual matrix_4f get_matrix_f(void);

		virtual matrix_4d get_matrix_d(void);

		virtual void set(vector3<float>& in_pos, vector3<float> in_pivot, vector3<float>& in_scale, quaternion<float>& in_rotation);

		virtual void set_position(vector3<float>& in_pos) { m_position = in_pos; m_is_dirty = true; }

		virtual void set_pivot(vector3<float>& in_pivot) { m_pivot = in_pivot; m_is_dirty = true; }

		virtual void set_scale(vector3<float>& in_scale) { m_scale = in_scale; m_is_dirty = true; }

		virtual void set_rotation(quaternion<float>& in_rotation) { m_rotation = in_rotation; m_is_dirty = true; }

		vector3<float> get_position(void) { return m_position; }

		vector3<float> get_pivot(void) { return m_pivot; }

		vector3<float> get_scale(void) { return m_scale; }

		quaternion<float>  get_rotation(void) { return m_rotation; }

		bool is_identity(void) { return m_is_identity; }

	public:
		virtual void copy_from(cartesian_transform* v_transform);

	protected:
		void update_matrix(void);

	protected:
		vector3<float> m_position;

		vector3<float> m_pivot;

		vector3<float> m_scale;

		quaternion<float> m_rotation;

		matrix4<double> m_transform_matrix_d;

		matrix4<float>	m_transform_matrix_f;

		bool			m_is_identity;

		bool			m_is_dirty;
	};
}

#endif // _US_CARTESIAN_TRANSFORM_H_