//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_coordinate_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : coordinate base class
//	Reference :
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_COORDINATE_BASE_H_
#define _US_COORDINATE_BASE_H_

#include <usUtil/Export.h>
#include <usUtil/us_vector3.h>
#include <usUtil/us_matrix4.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT coordinate_base
	{
	protected:
		coordinate_base(void);

	public:
		coordinate_base(vector3<double> in_origin_geo, vector3<double> in_direction, vector3<double> in_up);

		virtual ~coordinate_base(void);

		// spherical
		virtual vector3<double> from_spherical(vector3<double> in_pos);

		virtual vector3<double> from_spherical(double in_longitude, double in_latitude);

		virtual vector3<double> to_spherical(vector3<double> in_pos_local);


		// geocentric
		virtual vector3<double> from_geocentric(vector3<double> in_pos);

		virtual vector3<double> to_geocentric(vector3<double> in_pos);


		// sampled geocentric
		virtual vector3<int> from_geocentric_with_sampling(vector3<double> in_pos);

		virtual vector3<double> to_geocentric_with_sampling(vector3<int> in_pos);


		virtual void set_sampling_interval(double in_sampling_interval);

		virtual double get_sampling_interval(void);

	public:
		vector3<double> m_origin_sphr;
		vector3<double> m_origin_geo;

		matrix4<double> m_mat_to_geo;
		matrix4<double> m_mat_to_tangent_plane;

		vector3<double> x_axis;
		vector3<double> y_axis;
		vector3<double> z_axis;

		double m_sampling_interval;
	};
}

#endif // _US_COORDINATE_BASE_H_