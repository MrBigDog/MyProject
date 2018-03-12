//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_gauss_projection.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : angle template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////

#include "us_gauss_projection.h"
#include <usUtil/us_math.h>

namespace uniscope_globe
{
	gauss_projection::gauss_projection(void)
	{
		m_zone_wide = 6;

		m_proj_type = US_PRJ_TYPE_GAUSS;
		m_zone_index = -1;

		m_offset_east = 500000.0;
		m_offset_north = 0.0;
	}

	gauss_projection::~gauss_projection(void)
	{
	}

	void gauss_projection::set_zone_wide(int wide)
	{
		m_zone_wide = wide;
		m_proj_type = m_zone_wide / 3;
	}

	void gauss_projection::decimal_to_degree_minute_second(double decimal, int* degree, int* minute, double* second)
	{
		*degree = (int)decimal;
		*minute = (int)((decimal - *degree) * 60);
		*second = (decimal * 60 - *degree * 60 - *minute) * 60;
	}

	void gauss_projection::degree_minute_second_to_decimal(int degree, int minute, double second, double *decimal)
	{
		*decimal = degree + minute / 60.0 + second / 60.0 / 60.0;
	}

	bool gauss_projection::projection_caculate(double longitude, double latitude, double* coord_x, double* coord_y)
	{

		// *
		double lon = math_d::s_deg_to_rad * longitude;
		double lat = math_d::s_deg_to_rad * latitude;


		if (lon < -PI || lon > PI || lat < -HALF_PI || lat > HALF_PI)
		{
			return false;
		}

		double delta_lon = 0.0;	        //Delta longitude (Given longitude - center
		double sin_phi, cos_phi;        //math<double>::sin_ and math<double>::cos_ value
		double al, als;		            //temporary values
		double c, t, tq;	            //temporary values
		double con, n, ml;	            //cone constant, small m

		m_es = 1.0 - math<double>::pow_(m_semi_minor / m_semi_major, 2);
		m_e = math<double>::sqrt_(m_es);
		m_e0 = (1.0 - 0.25 * m_es * (1.0 + m_es / 16.0 * (3.0 + 1.25 * m_es)));
		m_e1 = (0.375 * m_es * (1.0 + 0.25 * m_es * (1.0 + 0.46875 * m_es)));
		m_e2 = (0.05859375 * m_es * m_es * (1.0 + 0.75 * m_es));
		m_e3 = (m_es * m_es * m_es * (35.0 / 3072.0));
		m_ml0 = m_semi_major * (m_e0 * m_lat_origin - m_e1 * math<double>::sin_(2.0 * m_lat_origin) + m_e2 * math<double>::sin_(4.0 * m_lat_origin) - m_e3 * math<double>::sin_(6.0 * m_lat_origin));
		m_esp = m_es / (1.0 - m_es);

		// 计算投影带
		if (m_has_set_central == false)
		{
			int proj_num = 0;
			if (m_zone_wide == 6)
			{
				proj_num = (int)(longitude / m_zone_wide) + 1;
				if (m_zone_index != -1) proj_num = m_zone_index;
				m_central_meridian = math_d::s_deg_to_rad * (proj_num * m_zone_wide - 3);
			}
			else
			{
				proj_num = (int)((longitude - 1.5) / m_zone_wide) + 1;
				if (m_zone_index != -1) proj_num = m_zone_index;
				m_central_meridian = math_d::s_deg_to_rad * proj_num * m_zone_wide;
			}
			m_offset_east = 1000000L * proj_num + m_false_east;
		}
		else m_offset_east = m_false_east;

		delta_lon = adjust_lon(lon - m_central_meridian);

		sin_phi = math<double>::sin_(lat);
		cos_phi = math<double>::cos_(lat);

		al = cos_phi * delta_lon;
		als = math<double>::pow_(al, 2);
		c = m_esp * math<double>::pow_(cos_phi, 2);
		tq = math<double>::tan_(lat);
		t = math<double>::pow_(tq, 2);
		con = 1.0 - m_es * math<double>::pow_(sin_phi, 2);
		n = m_semi_major / math<double>::sqrt_(con);
		ml = m_semi_major * (m_e0 * lat - m_e1 * math<double>::sin_(2.0 * lat) + m_e2 * math<double>::sin_(4.0 * lat) - m_e3 * math<double>::sin_(6.0 * lat));


		*coord_x = m_scale_factor * n * al * (1.0 + als / 6.0 * (1.0 - t + c + als / 20.0 *
			(5.0 - 18.0 * t + math<double>::pow_(t, 2) + 72.0 * c - 58.0 * m_esp))) + m_offset_east;

		*coord_y = m_scale_factor * (ml - m_ml0 + n * tq * (als * (0.5 + als / 24.0 *
			(5.0 - t + 9.0 * c + 4.0 * math<double>::pow_(c, 2) + als / 30.0 * (61.0 - 58.0 * t
				+ math<double>::pow_(t, 2) + 600.0 * c - 330.0 * m_esp))))) + m_offset_north;

		return true;

	}

	bool gauss_projection::projection_inverse_caculate(double coord_x, double coord_y, double* longitude, double* latitude)
	{
		double nation_x, nation_y;
		translate_four_param(coord_x, coord_y, &nation_x, &nation_y);

		double con, phi;		            //temporary angles
		double delta_phi;	                //difference between longitudes
		long i;			                    //counter variable
		double sin_phi, cos_phi, tan_phi;	//math<double>::sin_ math<double>::cos_ and tangent values
		double c, cs, t, ts, n, r, d, ds;	//temporary variables
		long max_iter = 6;			        //maximun number of iterations

		m_es = 1.0 - math<double>::pow_(m_semi_minor / m_semi_major, 2);
		m_e = math<double>::sqrt_(m_es);
		m_e0 = (1.0 - 0.25 * m_es * (1.0 + m_es / 16.0 * (3.0 + 1.25 * m_es)));
		m_e1 = (0.375 * m_es * (1.0 + 0.25 * m_es * (1.0 + 0.46875 * m_es)));
		m_e2 = (0.05859375 * m_es * m_es * (1.0 + 0.75 * m_es));
		m_e3 = (m_es * m_es * m_es * (35.0 / 3072.0));
		m_ml0 = m_semi_major * (m_e0 * m_lat_origin - m_e1 * math<double>::sin_(2.0 * m_lat_origin) + m_e2 * math<double>::sin_(4.0 * m_lat_origin) - m_e3 *math<double>::sin_(6.0 * m_lat_origin));
		m_esp = m_es / (1.0 - m_es);

		// 计算投影带
		if (m_has_set_central == false)
		{
			int prj_num = (int)(nation_x / 1000000L);
			if (m_zone_index != -1) prj_num = m_zone_index;
			if (m_zone_wide == 6)
				m_central_meridian = math_d::s_deg_to_rad * (prj_num * m_zone_wide - 3);
			else
				m_central_meridian = math_d::s_deg_to_rad * (prj_num * m_zone_wide);
			m_offset_east = 1000000L * prj_num + m_false_east;
		}
		else
			m_offset_east = m_false_east;


		double x = nation_x - m_offset_east;
		double y = nation_y - m_offset_north;

		con = (m_ml0 + y / m_scale_factor) / m_semi_major;
		phi = con;
		for (i = 0; ; i++)
		{
			delta_phi = ((con + m_e1 * math<double>::sin_(2.0 * phi) - m_e2 * math<double>::sin_(4.0 * phi) + m_e3 * math<double>::sin_(6.0 * phi)) / m_e0) - phi;
			phi += delta_phi;
			if (math<double>::fabs_(delta_phi) <= 1.0e-10) break;
		}
		if (math<double>::fabs_(phi) < math<double>::s_half_pi)
		{
			sin_phi = math<double>::sin_(phi);
			cos_phi = math<double>::cos_(phi);
			tan_phi = math<double>::tan_(phi);
			c = m_esp * math<double>::pow_(cos_phi, 2);
			cs = math<double>::pow_(c, 2);
			t = math<double>::pow_(tan_phi, 2);
			ts = math<double>::pow_(t, 2);
			con = 1.0 - m_es * math<double>::pow_(sin_phi, 2);
			n = m_semi_major / math<double>::sqrt_(con);
			r = n * (1.0 - m_es) / con;
			d = x / (n * m_scale_factor);
			ds = math<double>::pow_(d, 2);

			double lat = phi - (n * tan_phi * ds / r) * (0.5 - ds / 24.0 * (5.0 + 3.0 * t +
				10.0 * c - 4.0 * cs - 9.0 * m_esp - ds / 30.0 * (61.0 + 90.0 * t +
					298.0 * c + 45.0 * ts - 252.0 * m_esp - 3.0 * cs)));

			double lon = adjust_lon(m_central_meridian + (d * (1.0 - ds / 6.0 * (1.0 + 2.0 * t +
				c - ds / 20.0 * (5.0 - 2.0 * c + 28.0 * t - 3.0 * cs + 8.0 * m_esp +
					24.0 * ts))) / cos_phi));
			*longitude = lon * math_d::s_rad_to_deg;
			*latitude = lat * math_d::s_rad_to_deg;
		}
		else
		{
			*longitude = (math<double>::s_half_pi * (y < 0.0 ? -1 : 1)) * math_d::s_rad_to_deg;
			*latitude = m_central_meridian * math_d::s_rad_to_deg;
		}

		return true;
	}

	double gauss_projection::adjust_lon(double x)
	{
		long count = 0;
		double max_project_long = 2147483647;
		double double_long = 4.61168601e18;
		for (; ; )
		{
			if (math<double>::fabs_(x) <= PI)
			{
				break;
			}
			else
			{
				if (((long)math<double>::fabs_(x / PI)) < 2)
					x = x - ((x < 0.0 ? -1 : 1) * PI * 2.0);
				else
				{
					if (((long)math<double>::fabs_(x / PI * 2.0)) < max_project_long)
					{
						x = x - (((long)(x / PI * 2.0)) * PI * 2.0);
					}
					else
					{
						if (((long)math<double>::fabs_(x / (max_project_long * PI * 2.0))) < max_project_long)
						{
							x = x - (((long)(x / (max_project_long * PI * 2.0))) * (PI * 2.0 * max_project_long));
						}
						else
						{
							if (((long)math<double>::fabs_(x / (double_long * PI * 2.0))) < max_project_long)
							{
								x = x - (((long)(x / (double_long * PI * 2.0))) * (PI * 2.0 * double_long));
							}
							else
							{
								x = x - ((x < 0.0 ? -1 : 1) * PI * 2.0);
							}
						}
					}
				}
				count++;
				if (count > 4) break;
			}
		}
		return x;
	}

	void gauss_projection::translate_four_param(double local_x, double local_y, double* nation_x, double* nation_y)
	{
		// 四参数计算用于地方坐标系和国家坐标系转换
		// 暂时用不到
		*nation_x = local_x;
		*nation_y = local_y;
		//double dX =	335838.18964195251;
		//double dY = 1791561.6603803635;
		//double dK = 0.60192314986080575;
		//double dT = 0.049234956657831667;

		//*nation_x = dK * (localX - dT * localY) + dX + 21000000L;
		//*nation_y = dK * (localY + dT * localX) + dY;
	}

}
