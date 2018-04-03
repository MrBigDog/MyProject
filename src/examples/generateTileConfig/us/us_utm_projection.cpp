///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_utm_projection.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : utm_projection class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_utm_projection.h"
#include "us_math.h"
namespace uniscope_globe
{
	utm_projection::utm_projection(void)
	{
		m_scale_factor = 0.9996;
		m_offset_north = 0.0;
		m_offset_east = 500000.0;

		m_proj_type = US_PRJ_TYPE_UTM;
		m_zone_index = -1;
	}

	utm_projection::~utm_projection(void)
	{
	}

	bool utm_projection::projection_caculate(double longitude, double latitude, double* coord_x, double* coord_y)
	{
		*coord_x = 0.0;
		*coord_y = 0.0;

		int proj_num = (int)((longitude + 180) / 6) + 1;
		if (m_zone_index != -1) proj_num = m_zone_index;

		if (m_has_set_central == false)
		{
			m_central_meridian = math_d::s_deg_to_rad * ((proj_num - 30) * 6 - 3);
			m_offset_east = 1000000L * proj_num + m_false_east;
		}
		else
		{
			m_offset_east = m_false_east;
		}

		// 		m_offset_east = 1000000L * proj_num + m_false_east;
		if (latitude < 0)
			m_offset_north = 10000000.0;

		double lon = math_d::s_deg_to_rad * longitude;
		double lat = math_d::s_deg_to_rad * latitude;


		if (lon < -PI || lon > PI || lat < -HALF_PI || lat > HALF_PI)
		{
			return false;
		}

		m_es = 1.0 - pow(m_semi_minor / m_semi_major, 2);
		m_e = sqrt(m_es);
		m_esp = m_es / (1.0 - m_es);
		m_e1 = (1 - m_semi_minor / m_semi_major) / (1 + m_semi_minor / m_semi_major);


		double e0 = 1.0 - 0.25 * m_es * (1.0 + m_es / 16.0 * (3.0 + 1.25 * m_es));  // 1-m_es/4-3*m_es^2/64-5*m_es^3/256
		double e1 = 0.375 * m_es * (1.0 + 0.25 * m_es * (1.0 + 0.46875 * m_es)); // 3*m_es/8+3*m_es^2/32+45*m_es^3/1024
		double e2 = 0.05859375 * m_es * m_es * (1.0 + 0.75 * m_es); // 15*m_es^2/256+45*m_es^3/1024
		double e3 = m_es * m_es * m_es * (35.0 / 3072.0); // 35*m_es^3/3072


		double t, c, a, m, n; // 正算参数；
		t = math_d::tan_(lat) * math_d::tan_(lat);
		c = m_esp * math_d::cos_(lat) * math_d::cos_(lat);
		a = (lon - m_central_meridian) * math_d::cos_(lat);
		m = m_semi_major * (e0 * lat - e1 * math_d::sin_(2 * lat) + e2 * math_d::sin_(4 * lat) - e3 * math_d::sin_(6 * lat));
		n = m_semi_major / math_d::sqrt_(1 - m_es * math_d::sin_(lat) * math_d::sin_(lat));

		double a2 = a * a;
		double a3 = a * a2;
		double a4 = a2 * a2;
		double a5 = a2 * a3;
		double a6 = a3 * a3;
		*coord_x = m_offset_east + m_scale_factor * n * (a + (1 - t + c) * a3 / 6 + (5 - 18 * t + t * t + 72 * c - 58 * m_esp) * a5 / 120);
		*coord_y = m_offset_north + m_scale_factor * (m + n * math_d::tan_(lat) * (a2 / 2 + (5 - t + 9 * c + 4 * c * c) * a4 / 24 + (61 - 58 * t + t * t + 600 * c - 330 * m_esp) * a6 / 720));

		return true;
	}

	bool utm_projection::projection_inverse_caculate(double coord_x, double coord_y, double* longitude, double* latitude)
	{
		*longitude = 0.0;
		*latitude = 0.0;

		int proj_num = (int)(coord_x / 1000000L);
		if (m_zone_index != -1) proj_num = m_zone_index;

		if (m_has_set_central == false)
		{
			m_central_meridian = math_d::s_deg_to_rad * ((proj_num - 30) * 6 - 3);
			m_offset_east = 1000000L * proj_num + m_false_east;
		}
		else
		{
			m_offset_east = m_false_east;
		}

		if (coord_y > 10000000.0)
			m_offset_north = 10000000.0;

		double x = coord_x - m_offset_east;
		double y = coord_y - m_offset_north;


		m_es = 1.0 - pow(m_semi_minor / m_semi_major, 2);
		m_e = sqrt(m_es);
		m_esp = m_es / (1.0 - m_es);
		m_e1 = (1 - m_semi_minor / m_semi_major) / (1 + m_semi_minor / m_semi_major);

		double e1s = m_e1 * m_e1;

		double e0 = 1.0 - 0.25 * m_es * (1.0 + m_es / 16.0 * (3.0 + 1.25 * m_es));  // 1 - m_es / 4 - 3 * m_es^2 / 64 - 5 * m_es^3 / 256
		double e1 = 1.5 * m_e1 * (1 - 0.5625 * e1s); // 3 * m_e1 / 2 - 27 * m_e1^3 / 32
		double e2 = (42 - 55 * e1s) * e1s / 32; // 21 * m_e1^2 / 16 - 55 * m_e1^4 / 32
		double e3 = 151 * e1s * m_e1 / 96; //151 * m_e1^3 / 96
		double e4 = 1097 * e1s * e1s / 512; // 1097 * m_e1^4 / 512

		double t, c, r, n, d; // 反算参数：

		double m = y / m_scale_factor;
		double phi = m / (m_semi_major * e0);
		double bf = phi + e1 * math_d::sin_(2 * phi) + e2 * math_d::sin_(4 * phi) + e3 * math_d::sin_(6 * phi) + e4 * math_d::sin_(8 * phi);

		double tan_bf = math_d::tan_(bf);
		double sin_bf = math_d::sin_(bf);
		double cos_bf = math_d::cos_(bf);

		t = tan_bf * tan_bf;
		c = m_esp * cos_bf * cos_bf;
		r = m_semi_major * (1 - m_es) / math_d::pow_(1 - m_es * sin_bf * sin_bf, 1.5);
		n = m_semi_major / math_d::sqrt_(1 - m_es * sin_bf * sin_bf);
		d = x / (n * m_scale_factor);

		double d2 = d * d;
		double d3 = d * d2;
		double d4 = d2 * d2;
		double d5 = d2 * d3;
		double d6 = d3 * d3;

		double lon = (d - (1 + 2 * t + c) * d3 / 6 + (5 - 2 * c + 28 * t - 3 * c * c + 8 * m_esp + 24 * t * t) * d5 / 120) / cos_bf + m_central_meridian;
		double lat = bf - n * tan_bf * (d2 * 0.5 - (5 + 3 * t + 10 * c - 4 * c * c - 9 * m_esp) * d4 / 24
			+ (61 + 90 * t + 298 * c + 45 * t * t - 3 * c * c - 252 * m_esp) * d6 / 720) / r;

		*longitude = lon * math_d::s_rad_to_deg;
		*latitude = lat * math_d::s_rad_to_deg;

		return true;
	}
}

