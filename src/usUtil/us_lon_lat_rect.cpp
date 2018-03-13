///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_lon_lat_rect.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : lon_lat_rect class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_lon_lat_rect.h"
#include <usUtil/us_math.h>
#include <usUtil/us_vector3.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_spherical_coords.h>
#include <usUtil/us_geocentric_coords.h>
#include <usUtil/us_base_algorithm.h>
#include <usUtil/us_spherical_transform.h>
#include <usUtil/us_lon_lat_region.h>

#include <algorithm>
#include <stdlib.h>

namespace uniscope_globe
{
	lon_lat_rect::lon_lat_rect(void)
	{
		m_north = -HALF_PI;
		m_south = HALF_PI;
		m_east = -PI;
		m_west = PI;
	}

	lon_lat_rect::~lon_lat_rect(void)
	{

	}

	lon_lat_rect::lon_lat_rect(double north, double south, double east, double west)
	{
		m_north = north;
		m_south = south;
		m_east = east;
		m_west = west;
	}

	void lon_lat_rect::make_invalid(void)
	{
		m_north = -HALF_PI;
		m_south = HALF_PI;
		m_east = -PI;
		m_west = PI;
	}

	bool lon_lat_rect::operator == (const lon_lat_rect& vec) const
	{
		return !(*this != vec);
	}

	bool lon_lat_rect::operator != (const lon_lat_rect& vec) const
	{
		if (m_east == vec.m_east &&
			m_west == vec.m_west &&
			m_south == vec.m_south &&
			m_north == vec.m_north)
			return true;

		return false;
	}

	vector_3d lon_lat_rect::nearest_point_sphr(vector_3d& vec_sphr)
	{
		vector3<double> vec_min(m_west, m_south, 0.0);
		vector3<double> vec_max(m_east, m_north, 0.0);

		vector3<double> v_nearest_pos = vec_sphr;

		v_nearest_pos.x = (std::max)(vec_min.x, v_nearest_pos.x);
		v_nearest_pos.x = (std::min)(vec_max.x, v_nearest_pos.x);

		v_nearest_pos.y = (std::max)(vec_min.y, v_nearest_pos.y);
		v_nearest_pos.y = (std::min)(vec_max.y, v_nearest_pos.y);

		return v_nearest_pos;
	}

	bool lon_lat_rect::contain(vector3<double>& in_vector) const
	{
		return (in_vector.x <= m_east  && in_vector.x >= m_west &&
			in_vector.y <= m_north && in_vector.y >= m_south);
	}


	void lon_lat_rect::get_corners(vector3<double>* vec) const
	{
		vec[0].x = m_west;
		vec[0].y = m_north;
		vec[0].z = spherical_coords::s_semi_major;

		vec[1].x = m_east;
		vec[1].y = m_north;
		vec[1].z = spherical_coords::s_semi_major;

		vec[2].x = m_east;
		vec[2].y = m_south;
		vec[2].z = spherical_coords::s_semi_major;

		vec[3].x = m_west;
		vec[3].y = m_south;
		vec[3].z = spherical_coords::s_semi_major;
	}

	vector3<double> lon_lat_rect::get_conner(int type) const
	{
		switch (type)
		{
		case 0:
			return vector3<double>(m_west, m_north, spherical_coords::s_semi_major);
			break;
		case 1:
			return vector3<double>(m_east, m_north, spherical_coords::s_semi_major);
			break;
		case 2:
			return vector3<double>(m_east, m_south, spherical_coords::s_semi_major);
			break;
		case 3:
			return vector3<double>(m_west, m_south, spherical_coords::s_semi_major);
			break;
		}
		return vector3<double>(m_west, m_north, spherical_coords::s_semi_major);
	}

	bool lon_lat_rect::intersect(const lon_lat_rect& in_rect) const
	{
		bool b_ret = (m_north >= in_rect.m_south && m_south <= in_rect.m_north &&
			m_east >= in_rect.m_west  && m_west <= in_rect.m_east);

		return b_ret;
	}

	bool lon_lat_rect::intersect(const lon_lat_region& in_region) const
	{
		// test if the vertices of the region go into the rect.
		int v_num_of_vertices = in_region.get_count();
		for (int i = 0; i < v_num_of_vertices; i++)
		{
			if (contain(in_region[i]))
			{
				return true;
			}
		}

		// test if the vertices of the region go into the region.
		std::vector< vector3<double> > rect_pos_array;
		rect_pos_array.push_back(vector3<double>(m_west, m_south, 0.0));
		rect_pos_array.push_back(vector3<double>(m_west, m_north, 0.0));
		rect_pos_array.push_back(vector3<double>(m_east, m_north, 0.0));
		rect_pos_array.push_back(vector3<double>(m_east, m_south, 0.0));

		for (int i = 0; i < rect_pos_array.size(); i++)
		{
			if (in_region.contain(rect_pos_array[i]))
				return true;
		}

		//////////////////////////////////////////////////////////////////////////
		vector3<double> v_center = get_center();
		v_center.z = spherical_coords::s_semi_major;
		spherical_transform st;
		st.create_from_sphr(v_center.x, v_center.y, v_center.z, 0.0, 0.0, 0.0);

		vector3<double> south_west = geocentric_coords::from_spherical_d(vector3<double>(m_west, m_south, spherical_coords::s_semi_major)) * st.m_mat_tnb_inverse;
		south_west.y = south_west.z;
		vector3<double> north_west = geocentric_coords::from_spherical_d(vector3<double>(m_west, m_north, spherical_coords::s_semi_major)) * st.m_mat_tnb_inverse;
		north_west.y = north_west.z;
		vector3<double> north_east = geocentric_coords::from_spherical_d(vector3<double>(m_east, m_north, spherical_coords::s_semi_major)) * st.m_mat_tnb_inverse;
		north_east.y = north_east.z;
		vector3<double> south_east = geocentric_coords::from_spherical_d(vector3<double>(m_east, m_south, spherical_coords::s_semi_major)) * st.m_mat_tnb_inverse;
		south_east.y = south_east.z;

		std::vector< vector3<double> > v_region_array;
		v_region_array.resize(v_num_of_vertices);

		for (int i = 0; i < v_num_of_vertices; i++)
		{
			v_region_array[i] = geocentric_coords::from_spherical_d(in_region[i]) * st.m_mat_tnb_inverse;
			v_region_array[i].y = v_region_array[i].z;
		}


		for (int i = 0; i < v_num_of_vertices; i++)
		{
			vector3<double> v_pos1 = v_region_array[i];
			vector3<double> v_pos2 = v_region_array[(i + 1) % v_num_of_vertices];

			if (us_base_algorithm<double>::line_line_intersect_simp(v_pos1, v_pos2, south_west, north_west))
				return true;

			if (us_base_algorithm<double>::line_line_intersect_simp(v_pos1, v_pos2, north_west, north_east))
				return true;

			if (us_base_algorithm<double>::line_line_intersect_simp(v_pos1, v_pos2, north_east, south_east))
				return true;

			if (us_base_algorithm<double>::line_line_intersect_simp(v_pos1, v_pos2, south_east, south_west))
				return true;
		}

		return false;
	}

	vector3<double> lon_lat_rect::get_center(void) const
	{
		return vector3<double>((m_west + m_east) * 0.5f, (m_north + m_south) * 0.5f, 0.0);
	}

	double lon_lat_rect::get_width(void)
	{
		return m_east - m_west;
	}

	double lon_lat_rect::get_height(void)
	{
		return m_north - m_south;
	}

	void lon_lat_rect::combine(const vector3<double>& vertex)
	{
		if (m_west > vertex.x)
			m_west = vertex.x;

		if (m_east < vertex.x)
			m_east = vertex.x;

		if (m_north < vertex.y)
			m_north = vertex.y;

		if (m_south > vertex.y)
			m_south = vertex.y;
	}

	void lon_lat_rect::combine(const lon_lat_rect& v_rect)
	{
		m_east = (m_east < v_rect.m_east) ? v_rect.m_east : m_east;
		m_west = (m_west > v_rect.m_west) ? v_rect.m_west : m_west;

		m_north = (m_north < v_rect.m_north) ? v_rect.m_north : m_north;
		m_south = (m_south > v_rect.m_south) ? v_rect.m_south : m_south;
	}

	bool lon_lat_rect::intersect(const axis_aligned_box_d& aabb) const
	{
		lon_lat_rect v_rect;

		const vector3<double>* v_array = aabb.get_corners();
		for (int i = 0; i < 8; i++)
		{
			vector3<double> v_pos_sphr = spherical_coords::from_geocentric_d(v_array[i]);
			v_rect.combine(v_pos_sphr);
		}

		return this->intersect(v_rect);
	}

	bool lon_lat_rect::intersect(const circle& v_circle) const
	{
		vector_3d npt = lon_lat_rect::rect_nearest_point(this, v_circle.m_center);

		vector_3d vec_len(0.0, 0.0, 0.0);
		vec_len.x = (v_circle.m_center.x - npt.x) * spherical_coords::s_semi_major * math_d::cos_(v_circle.m_center.y);
		vec_len.y = (v_circle.m_center.y - npt.y) * spherical_coords::s_semi_major;
		double v_len = vec_len.length();
		if (v_len <= v_circle.m_radius) return true;

		return false;
	}

	lon_lat_rect lon_lat_rect::from_aabb(vector3<double>& in_pos_sphr, axis_aligned_box_d& in_aabb_cart)
	{
		vector3<double> v_min_vec = in_aabb_cart.get_min();
		vector3<double> v_max_vec = in_aabb_cart.get_max();

		double v_north_offset = v_max_vec.z * 0.001 / spherical_coords::s_semi_major;
		double v_south_offset = v_min_vec.z * 0.001 / spherical_coords::s_semi_major;

		double v_east_offset = v_max_vec.x * 0.001 / spherical_coords::s_semi_major;
		double v_west_offset = v_min_vec.x * 0.001 / spherical_coords::s_semi_major;

		return lon_lat_rect(in_pos_sphr.y + v_north_offset, in_pos_sphr.y + v_south_offset, in_pos_sphr.x + v_east_offset, in_pos_sphr.x + v_west_offset);
	}

	vector_3d lon_lat_rect::rect_nearest_point(const lon_lat_rect* in_rect, const vector_3d& in_pt)
	{
		// calcularate nearest distance
		vector_3d vec_min(in_rect->m_west, in_rect->m_south, 0.0);
		vector_3d vec_max(in_rect->m_east, in_rect->m_north, 0.0);

		vector_3d v_nearest_pos = in_pt;

		v_nearest_pos.x = (std::max)(vec_min.x, v_nearest_pos.x);
		v_nearest_pos.x = (std::min)(vec_max.x, v_nearest_pos.x);

		v_nearest_pos.y = (std::max)(vec_min.y, v_nearest_pos.y);
		v_nearest_pos.y = (std::min)(vec_max.y, v_nearest_pos.y);

		v_nearest_pos.z = (std::max)(vec_min.z, v_nearest_pos.z);
		v_nearest_pos.z = (std::min)(vec_max.z, v_nearest_pos.z);

		return v_nearest_pos;
	}

}
