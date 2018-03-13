///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_triangulation3.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : triangulation3 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_triangulation3.h"

namespace uniscope_globe
{
	/*
	triangulation3::triangulation3()
	{
		m_n_vertex_num = 0;
		m_n_added_cnt  = 0;

		m_ptr_poly_vertex = NULL;
		m_ptr_ver_index   = NULL;
		m_ptr_tri_index   = NULL;
	}

	triangulation3::~triangulation3()
	{
		if (m_ptr_poly_vertex)
			delete[] m_ptr_poly_vertex;
		if (m_ptr_ver_index)
			delete[] m_ptr_ver_index;
		if (m_ptr_tri_index)
			delete[] m_ptr_tri_index;

		m_ptr_poly_vertex = NULL;
		m_ptr_ver_index   = NULL;
		m_ptr_tri_index   = NULL;
	}

	bool triangulation3::triangulate_polygon(vector3<ushort>* ptr_poly_vertex, ushort n_poly_vertex, ushort** pp_triangle_index)
	{
		if (n_poly_vertex < 3)
			return false;
		m_n_vertex_num = n_poly_vertex;

		if (m_ptr_poly_vertex)
		{
			delete[] m_ptr_poly_vertex;
			m_ptr_poly_vertex = NULL;
		}
		if (m_ptr_ver_index)
		{
			delete[] m_ptr_ver_index;
			m_ptr_ver_index = NULL;
		}
		if (m_ptr_tri_index)
		{
			delete[] m_ptr_tri_index;
			m_ptr_tri_index = NULL;
		}

		m_ptr_poly_vertex = new vector3<ushort>[n_poly_vertex];
		m_ptr_ver_index = new ushort[n_poly_vertex];
		ushort v_n_tri_num = n_poly_vertex - 2;
		m_ptr_tri_index = new ushort[v_n_tri_num * 3];
		for (ushort i = 0; i < n_poly_vertex; i ++)
		{
			m_ptr_poly_vertex[i] = ptr_poly_vertex[i];
			m_ptr_ver_index[i] = i;
		}

		m_n_added_cnt = 0;

		tri(m_ptr_poly_vertex, m_ptr_ver_index, m_n_vertex_num);

		memcpy(*pp_triangle_index, m_ptr_tri_index, v_n_tri_num * 6);

		return true;

	}

	void triangulation3::tri(vector3<ushort>* ptr_vertex, ushort* ptr_index, ushort n_vertex)
	{
		if (n_vertex < 3)
			return;

		if (n_vertex == 3)
		{
			m_ptr_tri_index[m_n_added_cnt++] = ptr_index[0];
			m_ptr_tri_index[m_n_added_cnt++] = ptr_index[1];
			m_ptr_tri_index[m_n_added_cnt++] = ptr_index[2];
			return;
		}

		// if the vertex number is more than 3, they should be triangulated.
		short v_n_first_clip = first_clip_index(ptr_vertex, n_vertex);
		vector3<ushort>* v_ptr_vertex_tmp = new vector3<ushort>[n_vertex];
		ushort* v_ptr_index_tmp = new ushort[n_vertex];

		// adjust the order and let the first clip point to be the first point of the polygon.
		memcpy(v_ptr_vertex_tmp, ptr_vertex + v_n_first_clip, (n_vertex - v_n_first_clip) * sizeof(vector3<ushort>));
		memcpy(v_ptr_vertex_tmp + n_vertex - v_n_first_clip, ptr_vertex, v_n_first_clip * sizeof(vector3<ushort>));
		memcpy(ptr_vertex, v_ptr_vertex_tmp, n_vertex * sizeof(vector3<ushort>));
		memcpy(v_ptr_index_tmp, ptr_index + v_n_first_clip, (n_vertex - v_n_first_clip) * sizeof(short));
		memcpy(v_ptr_index_tmp + n_vertex - v_n_first_clip, ptr_index, v_n_first_clip * sizeof(short));
		memcpy(ptr_index, v_ptr_index_tmp, n_vertex * sizeof(short));

		delete[] v_ptr_vertex_tmp;
		delete[] v_ptr_index_tmp;
		v_ptr_vertex_tmp = NULL;
		v_ptr_index_tmp  = NULL;

		// get the second clip point.
		short v_n_second_clip = second_clip_index(ptr_vertex, n_vertex);

		// if there is no second clip point
		if (v_n_second_clip == -1)
		{
			// add the first triangle
			m_ptr_tri_index[m_n_added_cnt++] = ptr_index[0];
			m_ptr_tri_index[m_n_added_cnt++] = ptr_index[1];
			m_ptr_tri_index[m_n_added_cnt++] = ptr_index[n_vertex - 1];

			// delete first clip point and go on triangulation3;
			// triangulation3(ptr_vertex + 1, ptr_index + 1, n_vertex - 1);		// ********* need to test if this is effective.
			n_vertex --;
			for (int i = 0; i < n_vertex; i ++)
			{
				ptr_vertex[i] = ptr_vertex[i + 1];
				ptr_index[i]  = ptr_index[i + 1];
			}
			tri(ptr_vertex, ptr_index, n_vertex);
		}
		else
		{
			// clip to two new polygon, and triangulation3 each other
			int v_n_vernum1 = v_n_second_clip + 1;
			int v_n_vernum2 = n_vertex -v_n_second_clip + 1;

			// first
			v_ptr_vertex_tmp = new vector3<ushort>[v_n_vernum1];
			v_ptr_index_tmp  = new ushort[v_n_vernum1];
			memcpy(v_ptr_vertex_tmp, ptr_vertex, v_n_vernum1 * sizeof(vector3<ushort>));
			memcpy(v_ptr_index_tmp,  ptr_index,  v_n_vernum1 * sizeof(short));
			tri(v_ptr_vertex_tmp, v_ptr_index_tmp, v_n_vernum1);

			delete[] v_ptr_vertex_tmp;
			delete[] v_ptr_index_tmp;
			v_ptr_vertex_tmp = NULL;
			v_ptr_index_tmp  = NULL;

			// second
			for (int i = 0; i < v_n_vernum2 - 1; i ++)
			{
				ptr_vertex[i + 1] = ptr_vertex[i + v_n_second_clip];
				ptr_index[i + 1]  = ptr_index[i + v_n_second_clip];
			}
			tri(ptr_vertex, ptr_index, v_n_vernum2);
		}
	}

	short triangulation3::first_clip_index(vector3<ushort>* ptr_vertex, ushort n_vertex)
	{
		short v_n_first_index = 0;
		vector3<ushort> v_first_clip_point = ptr_vertex[0];

		for (int i = 0; i < n_vertex; i ++)
		{
			if (ptr_vertex[i].x < v_first_clip_point.x)
			{
				v_first_clip_point = ptr_vertex[i];
				v_n_first_index = i;
			}
		}

		return v_n_first_index;
	}

	short triangulation3::second_clip_index(vector3<ushort>* ptr_vertex, ushort n_vertex)
	{
		short v_n_second_index = -1;
		vector3<ushort> v_second_clip_point;

		for (int i = 2; i < n_vertex - 1; i ++)
		{
			if (is_point_in_triangle(ptr_vertex[i], ptr_vertex[0], ptr_vertex[1], ptr_vertex[n_vertex - 1]))
			{
				if (v_n_second_index == -1 || ptr_vertex[i].x < v_second_clip_point.x)
				{
					v_second_clip_point = ptr_vertex[i];
					v_n_second_index = i;
				}
			}
		}

		return v_n_second_index;

	}

	bool triangulation3::is_point_in_triangle(vector3<ushort> point_judge, vector3<ushort> tri_point1, vector3<ushort> tri_point2, vector3<ushort> tri_point3)
	{
		// to each edge of the triangle, 
		// if the judge point and the third point of the triangle is in different side of the edge,
		// the judge point is out of the triangle. 
		if (judge_side(point_judge, tri_point1, tri_point2) != judge_side(tri_point3, tri_point1, tri_point2))
			return false;
		if (judge_side(point_judge, tri_point1, tri_point3) != judge_side(tri_point2, tri_point1, tri_point3))
			return false;
		if (judge_side(point_judge, tri_point2, tri_point3) != judge_side(tri_point1, tri_point2, tri_point3))
			return false;
		// else, the point is in triangle.
		return true;
	}

	int triangulation3::judge_side(vector3<ushort> point_judge, vector3<ushort> section_point1, vector3<ushort> section_point2)
	{
		double v_lf_sum1 = (point_judge.y - section_point1.y) * (point_judge.x - section_point2.x);
		double v_lf_sum2 = (point_judge.y - section_point2.y) * (point_judge.x - section_point1.x);
		// v_lf_sum1 < v_lf_sum2: left  hand;
		// v_lf_sum1 > v_lf_sum2: right hand;
		return((v_lf_sum1<v_lf_sum2) ? -1 : ((v_lf_sum1>v_lf_sum2) ? 1 : 0));
	}

	//*/

}


 