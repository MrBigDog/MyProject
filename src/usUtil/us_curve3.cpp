///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_curve3.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : curve3 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_curve3.h"

namespace uniscope_globe
{
	//----------------------------------------------------------------------------
	template <class real>
	curve3<real>::curve3( real min_t, real max_t )
	{
		m_min_t = min_t;
		m_max_t = max_t;
	}
	//----------------------------------------------------------------------------
	template <class real>
	curve3<real>::~curve3( void )
	{
	}
	//----------------------------------------------------------------------------
	template <class real>
	real curve3<real>::get_min_time( void ) const
	{
		return m_min_t;
	}
	//----------------------------------------------------------------------------
	template <class real>
	real curve3<real>::get_max_time( void ) const
	{
		return m_max_t;
	}
	//----------------------------------------------------------------------------
	template <class real>
	void curve3<real>::set_time_interval( real min_t, real max_t )
	{
		assert( min_t < max_t );
		m_min_t = min_t;
		m_max_t = max_t;
	}
	//----------------------------------------------------------------------------
	template <class real>
	real curve3<real>::get_speed(real t) const
	{
		vector3<real> v_velocity = get_first_derivative(t);
		real v_speed = v_velocity.length();
		return v_speed;
	}
	//----------------------------------------------------------------------------
	template <class real>
	real curve3<real>::get_total_length( void ) const
	{
		return get_length( m_min_t, m_max_t );
	}
	//----------------------------------------------------------------------------
	template <class real>
	vector3<real> curve3<real>::get_tangent( real t ) const
	{
		vector3<real> v_velocity = get_first_derivative(t);
		v_velocity.normalize();
		return v_velocity;
	}
	//----------------------------------------------------------------------------
	template <class real>
	vector3<real> curve3<real>::get_normal( real t ) const
	{
		vector3<real> v_velocity = get_first_derivative(t);
		vector3<real> v_acceleration = get_second_derivative(t);
		real v_dot_v = v_velocity.dot(v_velocity);
		real v_dot_a = v_velocity.dot(v_acceleration);
		vector3<real> v_normal = v_dot_v*v_acceleration - v_dot_a*v_velocity;
		v_normal.normalize();
		return v_normal;
	}
	//----------------------------------------------------------------------------
	template <class real>
	vector3<real> curve3<real>::get_binormal( real t ) const
	{
		vector3<real> v_velocity = get_first_derivative(t);
		vector3<real> v_acceleration = get_second_derivative(t);
		real v_dot_v = v_velocity.dot(v_velocity);
		real v_dot_a = v_velocity.dot(v_acceleration);
		vector3<real> v_normal = v_dot_v*v_acceleration - v_dot_a*v_velocity;
		v_normal.normalize();
		v_velocity.normalize();
		vector3<real> v_binormal = v_velocity.cross(v_normal);
		return v_binormal;
	}
	//----------------------------------------------------------------------------
	template <class real>
	void curve3<real>::get_frame(	real t, 
									/*OUT*/ vector3<real>& in_position,
									/*OUT*/ vector3<real>& in_tangent,
									/*OUT*/ vector3<real>& in_normal,
									/*OUT*/ vector3<real>& in_binormal ) const
	{
		in_position = get_position(t);
		vector3<real> v_velocity = get_first_derivative(t);
		vector3<real> v_acceleration = get_second_derivative(t);
		real v_dot_v = v_velocity.dot(v_velocity);
		real v_dot_a = v_velocity.dot(v_acceleration);
		in_normal = v_dot_v*v_acceleration - v_dot_a*v_velocity;
		in_normal.normalize();
		in_tangent = v_velocity;
		in_tangent.normalize();
		in_binormal = in_tangent.cross(in_normal);
	}
	//----------------------------------------------------------------------------
	template <class real>
	real curve3<real>::get_curvature( real t ) const
	{
		vector3<real> v_velocity = get_first_derivative(t);
		real v_speed_sqr = v_velocity.squared_length();

		if ( v_speed_sqr >= math<real>::s_epsilon )
		{
			vector3<real> v_acceleration = get_second_derivative(t);
			vector3<real> v_cross = v_velocity.cross(v_acceleration);
			real v_numer = v_cross.length();
			real v_denom = math<real>::pow_(v_speed_sqr,(real)1.5);
			return v_numer/v_denom;
		}
		else
		{
			// curvature is indeterminate, just return 0
			return (real)0.0;
		}
	}
	//----------------------------------------------------------------------------
	template <class real>
	real curve3<real>::get_torsion(real t) const
	{
		vector3<real> v_velocity = get_first_derivative(t);
		vector3<real> v_acceleration = get_second_derivative(t);
		vector3<real> v_cross = v_velocity.cross(v_acceleration);
		real v_denom = v_cross.squared_length();

		if ( v_denom >= math<real>::s_epsilon )
		{
			vector3<real> v_jerk = get_third_derivative(t);
			real v_numer = v_cross.dot(v_jerk);
			return v_numer/v_denom;
		}
		else
		{
			// torsion is indeterminate, just return 0
			return (real)0.0;
		}
	}
	//----------------------------------------------------------------------------
	template <class real>
	void curve3<real>::subdivide_by_time( int in_point_count, vector3<real>*& out_points ) const
	{
		assert( in_point_count >= 2 );
		out_points = new vector3<real>[in_point_count];

		real v_delta = (m_max_t - m_min_t)/(in_point_count - 1);
		for (int i = 0; i < in_point_count; i++)
		{
			real t = m_min_t + v_delta*i;
			out_points[i] = get_position(t);
		}
	}
	//----------------------------------------------------------------------------
	template <class real>
	void curve3<real>::subdivide_by_length ( int in_point_count, vector3<real>*& out_points ) const
	{
		assert( in_point_count >= 2 );
		out_points = new vector3<real>[in_point_count];

		real v_delta = get_total_length()/(in_point_count-1);

		for (int i = 0; i < in_point_count; i++)
		{
			real v_length = v_delta*i;
			real t = get_time(v_length);
			out_points[i] = get_position(t);
		}
	}
	//----------------------------------------------------------------------------
	template <class real>
	void curve3<real>::subdivide_by_variation( real t0, const vector3<real>& p0,
											   real t1, const vector3<real>& p1, real min_variation, int level,
											   int& out_point_count, PointList*& out_point_list) const
	{
		if ( level > 0 && get_variation(t0,t1,&p0,&p1) > min_variation )
		{
			// too much variation, subdivide interval
			level--;
			real v_mid_t = ((real)0.5)*(t0+t1);
			vector3<real> v_mid_p = get_position(v_mid_t);

			subdivide_by_variation(t0, p0, v_mid_t, v_mid_p, min_variation,level, out_point_count, out_point_list);

			subdivide_by_variation(v_mid_t, v_mid_p, t1, p1, min_variation, level, out_point_count, out_point_list);
		}
		else
		{
			// add right end point, left end point was added by neighbor
			out_point_list = new PointList(p1, out_point_list);
			out_point_count++;
		}
	}
	//----------------------------------------------------------------------------
	template <class real>
	void curve3<real>::subdivide_by_variation ( /*IN*/ real min_variation, /*IN*/ int max_level,
												 /*OUT*/ int& out_points_count, /*OUT*/ vector3<real>*& out_point_array ) const
	{
		// compute end points of curve
		vector3<real> v_min_pos = get_position(m_min_t);
		vector3<real> v_max_pos = get_position(m_max_t);

		// add left end point to list
		PointList* v_point_list = new PointList(v_min_pos, 0);
		out_points_count = 1;

		// binary subdivision, leaf nodes add right end point of subinterval
		subdivide_by_variation(m_min_t,v_min_pos,m_max_t,v_max_pos,min_variation,max_level,out_points_count,v_point_list->m_next);

		// repackage points in an array
		assert( out_points_count >= 2 );
		out_point_array = new vector3<real>[out_points_count];
		for (int i = 0; i < out_points_count; i++)
		{
			assert( v_point_list );
			out_point_array[i] = v_point_list->m_point;

			PointList* pkSave = v_point_list;
			v_point_list = v_point_list->m_next;
			delete pkSave;
		}
		assert( v_point_list == 0 );
	}

	template class curve3<float>;

	// double
	template class curve3<double>;

	
}