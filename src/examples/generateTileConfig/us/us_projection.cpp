///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_projection.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : uniscope_projection class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_projection.h"
#include "us_math.h"
#include "us_lon_lat_rect.h"

namespace uniscope_globe
{
	uniscope_projection::uniscope_projection(void)
	{
		m_semi_major = US_PRJ_SEMI_MAJOR_84;		// 默认 WGS84 的椭球参数
		m_semi_minor = US_PRJ_SEMI_MINOR_84;

		m_proj_type = US_PRJ_TYPE_BL;

		m_scale_factor = 1.0;
		m_central_meridian = 0.0;
		m_lat_origin = 0.0;
		m_false_east = 500000;

		m_has_set_central = false;
	}

	uniscope_projection::~uniscope_projection(void)
	{
	}

	int uniscope_projection::get_type()
	{
		return m_proj_type;
	}

	bool uniscope_projection::set_semi(double major, double minor)
	{
		if (major <= 0 || minor <= 0)
		{
			return false;
		}
		m_semi_major = major;
		m_semi_minor = minor;
		return true;
	}

	bool uniscope_projection::set_central_meridian(double central_meridian)
	{
		if (central_meridian < -180.0 || central_meridian > 180.0)
		{
			return false;
		}
		m_central_meridian = central_meridian * math_d::s_deg_to_rad;
		m_has_set_central = true;
		return true;
	}

	bool uniscope_projection::set_false_east(double false_e)
	{
		m_false_east = false_e;
		return true;
	}

	lon_lat_rect uniscope_projection::rect_projection_calculate(lon_lat_rect rect_bl)
	{
		lon_lat_rect v_rect_xy;
		projection_caculate(rect_bl.m_west, rect_bl.m_south, &(v_rect_xy.m_west), &(v_rect_xy.m_south));
		projection_caculate(rect_bl.m_east, rect_bl.m_north, &(v_rect_xy.m_east), &(v_rect_xy.m_north));

		double v_lf_tmp_x, v_lf_tmp_y;
		projection_caculate(rect_bl.m_east, rect_bl.m_south, &v_lf_tmp_x, &v_lf_tmp_y);
		if (v_lf_tmp_x > v_rect_xy.m_east)
			v_rect_xy.m_east = v_lf_tmp_x;
		if (v_lf_tmp_y < v_rect_xy.m_south)
			v_rect_xy.m_south = v_lf_tmp_y;
		projection_caculate(rect_bl.m_west, rect_bl.m_north, &v_lf_tmp_x, &v_lf_tmp_y);
		if (v_lf_tmp_x < v_rect_xy.m_west)
			v_rect_xy.m_west = v_lf_tmp_x;
		if (v_lf_tmp_y > v_rect_xy.m_north)
			v_rect_xy.m_north = v_lf_tmp_y;

		return v_rect_xy;
	}

	lon_lat_rect uniscope_projection::rect_projection_inverse_calculate(lon_lat_rect rect_xy)
	{
		lon_lat_rect v_rect_bl;
		projection_inverse_caculate(rect_xy.m_west, rect_xy.m_south, &(v_rect_bl.m_west), &(v_rect_bl.m_south));
		projection_inverse_caculate(rect_xy.m_east, rect_xy.m_north, &(v_rect_bl.m_east), &(v_rect_bl.m_north));

		double v_lf_tmp_x, v_lf_tmp_y;
		projection_inverse_caculate(rect_xy.m_east, rect_xy.m_south, &v_lf_tmp_x, &v_lf_tmp_y);
		if (v_lf_tmp_x > v_rect_bl.m_east)
			v_rect_bl.m_east = v_lf_tmp_x;
		if (v_lf_tmp_y < v_rect_bl.m_south)
			v_rect_bl.m_south = v_lf_tmp_y;
		projection_inverse_caculate(rect_xy.m_west, rect_xy.m_north, &v_lf_tmp_x, &v_lf_tmp_y);
		if (v_lf_tmp_x < v_rect_bl.m_west)
			v_rect_bl.m_west = v_lf_tmp_x;
		if (v_lf_tmp_y > v_rect_bl.m_north)
			v_rect_bl.m_north = v_lf_tmp_y;
		return v_rect_bl;

	}
}


