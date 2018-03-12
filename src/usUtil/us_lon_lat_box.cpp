//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_lon_lat_box.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : angle template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////

#include "us_lon_lat_box.h"
#include <usUtil/us_math.h>
#include <usUtil/us_spherical_coords.h>
#include <usUtil/us_geocentric_coords.h>

namespace uniscope_globe
{

	lon_lat_box::lon_lat_box(void)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	/*
	   5-----4
	  /|    /|
	 / |   / |
	1-----2  |
	|  6--|--7
	| /   | /
	|/    |/
	0-----3
	*/
	//////////////////////////////////////////////////////////////////////////
	lon_lat_box::lon_lat_box(lon_lat_rect& rect)
	{
		set_extent(rect);
	}

	lon_lat_box::lon_lat_box(lon_lat_rect& rect, double val1, double val2)
	{
		set_extent(rect, val1, val2);
	}

	lon_lat_box::lon_lat_box(double north, double south, double east, double west, double val1, double val2)
	{
		set_extent(north, south, east, west, val1, val2);
	}

	lon_lat_box::~lon_lat_box(void)
	{
	}

	void lon_lat_box::set_extent(lon_lat_rect& rect)
	{
		double val1 = spherical_coords::s_datum_plane;
		double val2 = spherical_coords::s_semi_major + 8.85;

		double scale = val2 / (val1 * math<double>::cos_(rect.get_width() * 0.5));

		corner_value[0] = geocentric_coords::from_spherical_d(rect.m_west, rect.m_south, val1);
		corner_value[1] = corner_value[0] * scale;
		corner_value[3] = geocentric_coords::from_spherical_d(rect.m_east, rect.m_south, val1);
		corner_value[2] = corner_value[3] * scale;
		corner_value[6] = geocentric_coords::from_spherical_d(rect.m_west, rect.m_north, val1);
		corner_value[5] = corner_value[6] * scale;
		corner_value[7] = geocentric_coords::from_spherical_d(rect.m_east, rect.m_north, val1);
		corner_value[4] = corner_value[7] * scale;
	}

	void lon_lat_box::set_extent(lon_lat_rect& rect, double val1, double val2)
	{
		double scale = val2 / (val1 * math<double>::cos_(rect.get_width() * 0.5));
		corner_value[0] = geocentric_coords::from_spherical_d(rect.m_west, rect.m_south, val1);
		corner_value[1] = corner_value[0] * scale;
		corner_value[3] = geocentric_coords::from_spherical_d(rect.m_east, rect.m_south, val1);
		corner_value[2] = corner_value[3] * scale;
		corner_value[6] = geocentric_coords::from_spherical_d(rect.m_west, rect.m_north, val1);
		corner_value[5] = corner_value[6] * scale;
		corner_value[7] = geocentric_coords::from_spherical_d(rect.m_east, rect.m_north, val1);
		corner_value[4] = corner_value[7] * scale;
	}

	void lon_lat_box::set_extent(double north, double south, double east, double west, double val1, double val2)
	{
		double scale = val2 / (val1 * math<double>::cos_((east - west) * 0.5));
		corner_value[0] = geocentric_coords::from_spherical_d(west, south, val1);
		corner_value[1] = corner_value[0] * scale;
		corner_value[3] = geocentric_coords::from_spherical_d(east, south, val1);
		corner_value[2] = corner_value[3] * scale;
		corner_value[6] = geocentric_coords::from_spherical_d(west, north, val1);
		corner_value[5] = corner_value[6] * scale;
		corner_value[7] = geocentric_coords::from_spherical_d(east, north, val1);
		corner_value[4] = corner_value[7] * scale;
	}

	vector_3d lon_lat_box::get_center(void)
	{
		vector3<double> val;
		for (int ni = 0; ni < 8; ni++)
		{
			val += corner_value[ni];
		}
		val = val * 0.125f;
		return val;
	}

	axis_aligned_box_d lon_lat_box::get_aabb(void)
	{
		axis_aligned_box_d v_aabb;
		v_aabb.make_invalid();

		for (int ni = 0; ni < 8; ni++)
		{
			v_aabb.combine(corner_value[ni]);
		}

		return v_aabb;
	}

	axis_aligned_box_d lon_lat_box::get_aabb(matrix_4d& mat)
	{
		axis_aligned_box_d v_aabb;
		v_aabb.make_invalid();

		for (int ni = 0; ni < 8; ni++)
		{
			v_aabb.combine(corner_value[ni] * mat);
		}

		return v_aabb;
	}
}
