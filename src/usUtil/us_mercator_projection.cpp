///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mercator_projection.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mercator_projection class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_mercator_projection.h"
#include <usUtil/us_math.h>

namespace uniscope_globe
{
	mercator_projection::mercator_projection(void)
	{
		m_interative_times = 10;     // 迭代次数为 10
		m_interative_value = 0;      // 迭代初始值为 0

		m_proj_type = US_PRJ_TYPE_MERCATOR;
	}

	mercator_projection::~mercator_projection(void)
	{
	}

	bool mercator_projection::set_central(double central_meridian, double ref_lat)
	{
		if (central_meridian < -180 || central_meridian > 180 || ref_lat < -90 || ref_lat > 90)
		{
			return false;
		}
		m_central_meridian = central_meridian * math_d::s_deg_to_rad;
		m_lat_origin = ref_lat * math_d::s_deg_to_rad;
		return true;
	}

	bool mercator_projection::projection_caculate(double longitude, double latitude, double* coord_x, double* coord_y)
	{
		longitude *= math_d::s_deg_to_rad;
		latitude *= math_d::s_deg_to_rad;

		double f/*扁率*/, e/*第一偏心率*/, e_/*第二偏心率*/, NB0/*卯酉圈曲率半径*/, K, dtemp;
		double E = math_d::exp_(1);

		if (longitude < -PI || longitude > PI || latitude < -HALF_PI || latitude > HALF_PI)
		{
			return false;
		}

		if (m_semi_major <= 0 || m_semi_minor <= 0)
		{
			return false;
		}
		f = (m_semi_major - m_semi_minor) / m_semi_major;

		dtemp = 1 - (m_semi_minor / m_semi_major) * (m_semi_minor / m_semi_major);
		if (dtemp < 0)
		{
			return false;
		}
		e = sqrt(dtemp);


		dtemp = (m_semi_major / m_semi_minor) * (m_semi_major / m_semi_minor) - 1;
		if (dtemp < 0)
		{
			return false;
		}
		e_ = sqrt(dtemp);

		NB0 = ((m_semi_major * m_semi_major) / m_semi_minor) / sqrt(1 + e_ * e_ * cos(m_lat_origin) * cos(m_lat_origin));
		K = NB0 * cos(m_lat_origin);
		*coord_x = K * (longitude - m_central_meridian);
		*coord_y = K * log(tan(PI / 4 + latitude / 2) * pow((1 - e * sin(latitude)) / (1 + e * sin(latitude)), e / 2));

		return true;
	}

	bool mercator_projection::projection_inverse_caculate(double coord_x, double coord_y, double* longitude, double* latitude)
	{
		double f/*扁率*/, e/*第一偏心率*/, e_/*第二偏心率*/, NB0/*卯酉圈曲率半径*/, K, dtemp;
		double E = math_d::exp_(1);

		if (m_semi_major <= 0 || m_semi_minor <= 0)
		{
			return false;
		}
		f = (m_semi_major - m_semi_minor) / m_semi_major;

		dtemp = 1 - (m_semi_minor / m_semi_major) * (m_semi_minor / m_semi_major);
		if (dtemp < 0)
		{
			return false;
		}
		e = sqrt(dtemp);

		dtemp = (m_semi_major / m_semi_minor) * (m_semi_major / m_semi_minor) - 1;
		if (dtemp < 0)
		{
			return false;
		}
		e_ = sqrt(dtemp);

		NB0 = ((m_semi_major * m_semi_major) / m_semi_minor) / sqrt(1 + e_ * e_ * cos(m_lat_origin) * cos(m_lat_origin));
		K = NB0 * cos(m_lat_origin);

		double L, B;
		L = coord_x / K + m_central_meridian;
		B = m_interative_value;

		for (int i = 0; i < m_interative_times; i++)
		{
			B = HALF_PI - 2 * atan(pow(E, (-coord_y / K)) * pow(E, (e / 2) * log((1 - e * sin(B)) / (1 + e * sin(B)))));
		}

		*longitude = L * math_d::s_rad_to_deg;
		*latitude = B * math_d::s_rad_to_deg;
		return true;
	}
}


