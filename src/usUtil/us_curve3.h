///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_curve3.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : curve3 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_CURVE3_H_
#define _US_CURVE3_H_

namespace uniscope_globe
{
	template <class real>
	class curve3
	{
	protected:
		curve3( void ) {}

	public:
		curve3( real min_t, real max_t );
		virtual ~curve3();

		// Interval on which curve parameter is defined.  If you are interested
		// in only a subinterval of the actual domain of the curve, you may set
		// that subinterval with SetTimeInterval.  This function requires that
		// fTMin < fTMax.
		real get_min_time () const;
		real get_max_time () const;
		void set_time_interval( real min_t, real max_t );

		// position and derivatives
		virtual vector3<real> get_position( real t ) const = 0;
		virtual vector3<real> get_first_derivative( real t ) const = 0;
		virtual vector3<real> get_second_derivative( real t ) const = 0;
		virtual vector3<real> get_third_derivative( real t ) const = 0;

		// differential geometric quantities
		virtual real get_length( real t0, real t1 ) const = 0;

		real get_total_length( void ) const;
		real get_speed( real t ) const;
		
		vector3<real> get_tangent( real t ) const;
		vector3<real> get_normal( real t ) const;
		vector3<real> get_binormal (real t) const;
		void get_frame( IN real t, OUT vector3<real>& position, OUT vector3<real>& tangent, 
								   OUT vector3<real>& normal, OUT vector3<real>& binormal ) const;
		real get_curvature( real t ) const;
		real get_torsion( real t ) const;

		// inverse mapping of s = Length(t) given by t = Length^{-1}(s)
		virtual real get_time(real length, int iterations = 32, real tolerance = (real)1e-06) const = 0;

		// subdivision
		void subdivide_by_time( int iNumPoints, OUT vector3<real>*& out_point ) const;
		void subdivide_by_length(int iNumPoints, OUT vector3<real>*& out_point ) const;

		// Subdivision by variation. The pointers pkP0 and pkP1 correspond to the
		// curve points at fT0 and fT1.  If the pointer values are not null, the
		// assumption is that the caller has passed in the curve points.
		// Otherwise, the function computes the curve points.
		virtual real get_variation( real t0, real t1,
									const vector3<real>* p0 = 0, const vector3<real>* p1 = 0 ) const = 0;
		void subdivide_by_variation( IN real min_variation, IN int max_level,
									 OUT int& out_points_count, OUT vector3<real>*& out_point_array ) const;

	protected:
		// curve parameter is t where tmin <= t <= tmax
		real m_min_t, m_max_t;

		// subdivision
		class PointList
		{
		public:
			PointList (const vector3<real>& in_point, PointList* in_next)
			{
				m_point = in_point;
				m_next = in_next;
			}

			vector3<real> m_point;
			PointList* m_next;
		};

		void subdivide_by_variation(	real t0, const vector3<real>& p0,
										real t1, const vector3<real>& p1, real min_variation, int level,
										int& out_point_count, PointList*& out_point_list ) const;
	};

	typedef curve3<float> curve3f;
	typedef curve3<double> curve3d;
}

#endif // _US_CURVE3_H_