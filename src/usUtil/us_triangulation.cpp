///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_triangulation_2d.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : triangulation_2d class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_triangulation.h"

namespace uniscope_globe
{
	//triangulation_2d::triangulation_2d( void )
	//{
	//	m_n_vertex_num = 0;
	//	m_n_added_cnt  = 0;

	//	m_ptr_poly_vertex = NULL;
	//	m_ptr_ver_index   = NULL;
	//	m_ptr_tri_index   = NULL;
	//}

	//triangulation_2d::~triangulation_2d( void )
	//{
	//	if (m_ptr_poly_vertex)
	//		delete[] m_ptr_poly_vertex;
	//	if (m_ptr_ver_index)
	//		delete[] m_ptr_ver_index;
	//	if (m_ptr_tri_index)
	//		delete[] m_ptr_tri_index;

	//	m_ptr_poly_vertex = NULL;
	//	m_ptr_ver_index   = NULL;
	//	m_ptr_tri_index   = NULL;
	//}

	//bool  triangulation_2d::triangulation_polygon(us_vertex2d *ptr_poly_vertex, short n_poly_vertex, unsigned short *pp_triangle_index)
	//{
	//	if (n_poly_vertex < 3)
	//		return false;
	//	m_n_vertex_num = n_poly_vertex;

	//	if (m_ptr_poly_vertex)
	//	{
	//		delete[] m_ptr_poly_vertex;
	//		m_ptr_poly_vertex = NULL;
	//	}
	//	if (m_ptr_ver_index)
	//	{
	//		delete[] m_ptr_ver_index;
	//		m_ptr_ver_index = NULL;
	//	}
	//	if (m_ptr_tri_index)
	//	{
	//		delete[] m_ptr_tri_index;
	//		m_ptr_tri_index = NULL;
	//	}

	//	m_ptr_poly_vertex = new us_vertex2d[n_poly_vertex];
	//	m_ptr_ver_index = new short[n_poly_vertex];
	//	short v_n_tri_num = n_poly_vertex - 2;
	//	m_ptr_tri_index = new short[v_n_tri_num * 3];
	//	for (short i = 0; i < n_poly_vertex; i ++)
	//	{
	//		m_ptr_poly_vertex[i] = ptr_poly_vertex[i];
	//		m_ptr_ver_index[i] = i;
	//	}

	//	m_n_added_cnt = 0;

	//	triangulation(m_ptr_poly_vertex, m_ptr_ver_index, m_n_vertex_num);

	//	memcpy(pp_triangle_index, m_ptr_tri_index, v_n_tri_num * 6);

	//	return true;
	//}

	//void  triangulation_2d::triangulation(us_vertex2d *ptr_vertex, short* ptr_index, short n_vertex)
	//{
	//	if (n_vertex < 3)
	//		return;

	//	if (n_vertex == 3)
	//	{
	//		m_ptr_tri_index[m_n_added_cnt++] = ptr_index[0];
	//		m_ptr_tri_index[m_n_added_cnt++] = ptr_index[1];
	//		m_ptr_tri_index[m_n_added_cnt++] = ptr_index[2];
	//		return;
	//	}

	//	// if the vertex number is more than 3, they should be triangulated.
	//	short v_n_first_clip = first_clip_index(ptr_vertex, n_vertex);
	//	us_vertex2d* v_ptr_vertex_tmp = new us_vertex2d[n_vertex];
	//	short* v_ptr_index_tmp = new short[n_vertex];

	//	// adjust the order and let the first clip point to be the first point of the polygon.
	//	memcpy(v_ptr_vertex_tmp, ptr_vertex + v_n_first_clip, (n_vertex - v_n_first_clip) * sizeof(us_vertex2d));
	//	memcpy(v_ptr_vertex_tmp + n_vertex - v_n_first_clip, ptr_vertex, v_n_first_clip * sizeof(us_vertex2d));
	//	memcpy(ptr_vertex, v_ptr_vertex_tmp, n_vertex * sizeof(us_vertex2d));
	//	memcpy(v_ptr_index_tmp, ptr_index + v_n_first_clip, (n_vertex - v_n_first_clip) * sizeof(short));
	//	memcpy(v_ptr_index_tmp + n_vertex - v_n_first_clip, ptr_index, v_n_first_clip * sizeof(short));
	//	memcpy(ptr_index, v_ptr_index_tmp, n_vertex * sizeof(short));

	//	delete[] v_ptr_vertex_tmp;
	//	delete[] v_ptr_index_tmp;
	//	v_ptr_vertex_tmp = NULL;
	//	v_ptr_index_tmp  = NULL;

	//	// get the second clip point.
	//	short v_n_second_clip = second_clip_index(ptr_vertex, n_vertex);

	//	// if there is no second clip point
	//	if (v_n_second_clip == -1)
	//	{
	//		// add the first triangle
	//		m_ptr_tri_index[m_n_added_cnt++] = ptr_index[0];
	//		m_ptr_tri_index[m_n_added_cnt++] = ptr_index[1];
	//		m_ptr_tri_index[m_n_added_cnt++] = ptr_index[n_vertex - 1];

	//		// delete first clip point and go on triangulation;
	//		// triangulation(ptr_vertex + 1, ptr_index + 1, n_vertex - 1);		// ********* need to test if this is effective.
	//		n_vertex --;
	//		for (int i = 0; i < n_vertex; i ++)
	//		{
	//			ptr_vertex[i] = ptr_vertex[i + 1];
	//			ptr_index[i]  = ptr_index[i + 1];
	//		}
	//		triangulation(ptr_vertex, ptr_index, n_vertex);
	//	}
	//	else
	//	{
	//		// clip to two new polygon, and triangulation each other
	//		int v_n_vernum1 = v_n_second_clip + 1;
	//		int v_n_vernum2 = n_vertex -v_n_second_clip + 1;

	//		// first
	//		v_ptr_vertex_tmp = new us_vertex2d[v_n_vernum1];
	//		v_ptr_index_tmp  = new short[v_n_vernum1];
	//		memcpy(v_ptr_vertex_tmp, ptr_vertex, v_n_vernum1 * sizeof(us_vertex2d));
	//		memcpy(v_ptr_index_tmp,  ptr_index,  v_n_vernum1 * sizeof(short));
	//		triangulation(v_ptr_vertex_tmp, v_ptr_index_tmp, v_n_vernum1);

	//		delete[] v_ptr_vertex_tmp;
	//		delete[] v_ptr_index_tmp;
	//		v_ptr_vertex_tmp = NULL;
	//		v_ptr_index_tmp  = NULL;

	//		// second
	//		for (int i = 0; i < v_n_vernum2 - 1; i ++)
	//		{
	//			ptr_vertex[i + 1] = ptr_vertex[i + v_n_second_clip];
	//			ptr_index[i + 1]  = ptr_index[i + v_n_second_clip];
	//		}
	//		triangulation(ptr_vertex, ptr_index, v_n_vernum2);
	//	}
	//}

	//short triangulation_2d::first_clip_index(us_vertex2d* ptr_vertex, short n_vertex)
	//{
	//	short v_n_first_index = 0;
	//	us_vertex2d v_first_clip_point = ptr_vertex[0];

	//	for (int i = 0; i < n_vertex; i ++)
	//	{
	//		if (ptr_vertex[i].x < v_first_clip_point.x)
	//		{
	//			v_first_clip_point = ptr_vertex[i];
	//			v_n_first_index = i;
	//		}
	//	}

	//	return v_n_first_index;
	//}

	//short triangulation_2d::second_clip_index(us_vertex2d* ptr_vertex, short n_vertex)
	//{
	//	short v_n_second_index = -1;
	//	us_vertex2d v_second_clip_point;

	//	for (int i = 2; i < n_vertex - 1; i ++)
	//	{
	//		if (is_point_in_triangle(ptr_vertex[i], ptr_vertex[0], ptr_vertex[1], ptr_vertex[n_vertex - 1]))
	//		{
	//			if (v_n_second_index == -1 || ptr_vertex[i].x < v_second_clip_point.x)
	//			{
	//				v_second_clip_point = ptr_vertex[i];
	//				v_n_second_index = i;
	//			}
	//		}
	//	}

	//	return v_n_second_index;
	//}

	//bool  triangulation_2d::is_point_in_triangle(us_vertex2d point_judge, us_vertex2d tri_point1, us_vertex2d tri_point2, us_vertex2d tri_point3)
	//{
	//	// to each edge of the triangle, 
	//	// if the judge point and the third point of the triangle is in different side of the edge,
	//	// the judge point is out of the triangle. 
	//	if (judge_side(point_judge, tri_point1, tri_point2) != judge_side(tri_point3, tri_point1, tri_point2))
	//		return false;
	//	if (judge_side(point_judge, tri_point1, tri_point3) != judge_side(tri_point2, tri_point1, tri_point3))
	//		return false;
	//	if (judge_side(point_judge, tri_point2, tri_point3) != judge_side(tri_point1, tri_point2, tri_point3))
	//		return false;
	//	// else, the point is in triangle.
	//	return true;
	//}

	//int triangulation_2d::judge_side(us_vertex2d point_judge, us_vertex2d section_point1, us_vertex2d section_point2)
	//{
	//	double v_lf_sum1 = (point_judge.y - section_point1.y) * (point_judge.x - section_point2.x);
	//	double v_lf_sum2 = (point_judge.y - section_point2.y) * (point_judge.x - section_point1.x);
	//	// v_lf_sum1 < v_lf_sum2: left  hand;
	//	// v_lf_sum1 > v_lf_sum2: right hand;
	//	return((v_lf_sum1<v_lf_sum2) ? -1 : ((v_lf_sum1>v_lf_sum2) ? 1 : 0));
	//}
}