///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_triangulation<T>.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : triangulation<T> class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TRIANGULATION_H_
#define _US_TRIANGULATION_H_

#include <usUtil\Export.h>
#include <usUtil\us_vector2.h>
#include <usUtil\us_math.h>

namespace uniscope_globe
{
	template< typename T >
	class triangulation
	{
	public:
		triangulation( void );

		virtual ~triangulation( void );

		// interface:
		// input : the pointer to the vertex data and the vertex number.
		// output: the triangle index. the memory of the index should be allocated beforehand, and release after hand.
		bool triangulation_polygon(vector2<T>* ptr_poly_vertex, short n_poly_vertex, unsigned short* pp_triangle_index);
		bool triangulation_polygon_2(vector2<T>* ptr_poly_vertex, short n_poly_vertex, unsigned short* pp_triangle_index);
	private:

		// triangulation recursive
		void basic_triangulation(vector2<T>* ptr_vertex, short* ptr_index, short n_vertex );

		// sub-function used in triangulation:
		short first_clip_index(vector2<T>* ptr_vertex, short n_vertex);
		short second_clip_index(vector2<T>* ptr_vertex, short n_vertex);

		short second_clip_index_max(vector2<T>* ptr_vertex, short n_vertex);

		bool  is_point_in_triangle(vector2<T> point_judge, vector2<T> tri_point1, vector2<T> tri_point2, vector2<T> tri_point3);
		bool  intersect( vector2<T>& v1, vector2<T>& v2, vector2<T>& p1, vector2<T>& p2 );
		int   judge_side(vector2<T> point_judge, vector2<T> section_point1, vector2<T> section_point2);


		short min_angle_index(vector2<T>* ptr_vertex, short n_vertex, double angle_limit = math_d::s_two_pi);
		short max_angle_index(vector2<T>* ptr_vertex, short n_vertex);


		short* m_ptr_tri_index;			// index of triangle after triangulation

		int m_n_added_cnt;				// to note the number of index that had been added to tri_index;

	};

	template< typename T >
	triangulation<T>::triangulation( void )
	{
		m_n_added_cnt  = 0;

		m_ptr_tri_index   = NULL;
	}

	template< typename T >
	triangulation<T>::~triangulation( void )
	{
		if (m_ptr_tri_index)
			delete[] m_ptr_tri_index;

		m_ptr_tri_index   = NULL;
	}

	template< typename T >
	bool  triangulation<T>::triangulation_polygon(vector2<T> *ptr_poly_vertex, short n_poly_vertex, unsigned short *pp_triangle_index)
	{
		if (n_poly_vertex < 3)
			return false;

		short v_n_vertex_num = n_poly_vertex;
		if (m_ptr_tri_index)
		{
			delete[] m_ptr_tri_index;
			m_ptr_tri_index = NULL;
		}

		vector2<T>* v_ptr_poly_vertex = new vector2<T>[n_poly_vertex];
		short* v_ptr_ver_index = new short[n_poly_vertex];
		short v_n_tri_num = n_poly_vertex - 2;
		m_ptr_tri_index = new short[v_n_tri_num * 3];

		for (short i = 0; i < n_poly_vertex; i ++)
		{
			v_ptr_poly_vertex[i] = ptr_poly_vertex[i];
			v_ptr_ver_index[i] = i;
		}

		m_n_added_cnt = 0;

		basic_triangulation(v_ptr_poly_vertex, v_ptr_ver_index, v_n_vertex_num);

		memcpy(pp_triangle_index, m_ptr_tri_index, v_n_tri_num * 6);

		AUTO_DELETE( v_ptr_ver_index );
		AUTO_DELETE_ARRAY( v_ptr_poly_vertex );

		return true;
	}

	template< typename T >
	bool  triangulation<T>::triangulation_polygon_2(vector2<T> *ptr_poly_vertex, short n_poly_vertex, unsigned short *pp_triangle_index)
	{
		if (n_poly_vertex < 3)
			return false;
		short v_n_vertex_num = n_poly_vertex;

		if (m_ptr_tri_index)
		{
			delete[] m_ptr_tri_index;
			m_ptr_tri_index = NULL;
		}

		vector2<T>* v_ptr_poly_vertex = new vector2<T>[n_poly_vertex];
		short* v_ptr_ver_index = new short[n_poly_vertex];
		short v_n_tri_num = n_poly_vertex - 2;
		m_ptr_tri_index = new short[v_n_tri_num * 3];
		for (short i = 0; i < n_poly_vertex; i ++)
		{
			v_ptr_poly_vertex[i] = ptr_poly_vertex[i];
			v_ptr_ver_index[i] = i;
		}

		m_n_added_cnt = 0;

		// 去掉递归
		vector<vector2<T>*> array_ver;
		vector<short*> array_index;
		vector<short> array_num;

		array_ver.push_back( v_ptr_poly_vertex );
		array_index.push_back( v_ptr_ver_index );
		array_num.push_back( v_n_vertex_num );

		while ( array_ver.size() > 0 )
		{
			vector2<T>* ptr_vertex = array_ver[0];
			short* ptr_index = array_index[0];
			short n_vertex = array_num[0];

			if (n_vertex == 3)
			{
				m_ptr_tri_index[m_n_added_cnt++] = ptr_index[0];
				m_ptr_tri_index[m_n_added_cnt++] = ptr_index[1];
				m_ptr_tri_index[m_n_added_cnt++] = ptr_index[2];
			}

			else
			{
				// find the largest angle
				bool is_min_angle = false;
				short v_n_first_clip = max_angle_index(ptr_vertex, n_vertex);

				short v_min_angle_index = min_angle_index(ptr_vertex, n_vertex, PI * 0.05);
				if (v_min_angle_index != -1)
				{
					is_min_angle = true;
					v_n_first_clip = v_min_angle_index;
				}

				vector2<T>* v_ptr_vertex_tmp = new vector2<T>[n_vertex];
				short* v_ptr_index_tmp = new short[n_vertex];

				// adjust the order and let the first clip point to be the first point of the polygon.
				memcpy(v_ptr_vertex_tmp, ptr_vertex + v_n_first_clip, (n_vertex - v_n_first_clip) * sizeof(vector2<T>));
				memcpy(v_ptr_vertex_tmp + n_vertex - v_n_first_clip, ptr_vertex, v_n_first_clip * sizeof(vector2<T>));
				memcpy(ptr_vertex, v_ptr_vertex_tmp, n_vertex * sizeof(vector2<T>));
				memcpy(v_ptr_index_tmp, ptr_index + v_n_first_clip, (n_vertex - v_n_first_clip) * sizeof(short));
				memcpy(v_ptr_index_tmp + n_vertex - v_n_first_clip, ptr_index, v_n_first_clip * sizeof(short));
				memcpy(ptr_index, v_ptr_index_tmp, n_vertex * sizeof(short));

				delete[] v_ptr_vertex_tmp;
				delete[] v_ptr_index_tmp;
				v_ptr_vertex_tmp = NULL;
				v_ptr_index_tmp  = NULL;

				// find the opposite point
				short v_n_second_clip = -1;
				if (is_min_angle && second_clip_index(ptr_vertex, n_vertex) == -1)
				{
					m_ptr_tri_index[m_n_added_cnt++] = ptr_index[0];
					m_ptr_tri_index[m_n_added_cnt++] = ptr_index[1];
					m_ptr_tri_index[m_n_added_cnt++] = ptr_index[n_vertex - 1];

					vector2<T>* v_ptr_vertex_sub = new vector2<T>[n_vertex - 1];
					short* v_ptr_index_sub  = new short[n_vertex - 1];
					memcpy(v_ptr_vertex_sub, ptr_vertex + 1, (n_vertex - 1) * sizeof(vector2<T>));
					memcpy(v_ptr_index_sub,  ptr_index + 1,  (n_vertex - 1) * sizeof(short));
					array_ver.push_back(v_ptr_vertex_sub);
					array_index.push_back(v_ptr_index_sub);
					array_num.push_back(n_vertex - 1);


				}
				else
				{
					v_n_second_clip = second_clip_index_max(ptr_vertex, n_vertex);


					// clip to two new polygon, and triangulation each other
					int v_n_vernum1 = v_n_second_clip + 1;
					int v_n_vernum2 = n_vertex -v_n_second_clip + 1;

					// first
					vector2<T>* v_ptr_vertex_part1 = new vector2<T>[v_n_vernum1];
					short* v_ptr_index_part1  = new short[v_n_vernum1];
					memcpy(v_ptr_vertex_part1, ptr_vertex, v_n_vernum1 * sizeof(vector2<T>));
					memcpy(v_ptr_index_part1,  ptr_index,  v_n_vernum1 * sizeof(short));
					array_ver.push_back(v_ptr_vertex_part1);
					array_index.push_back(v_ptr_index_part1);
					array_num.push_back(v_n_vernum1);

					// second
					vector2<T>* v_ptr_vertex_part2 = new vector2<T>[v_n_vernum2];
					short* v_ptr_index_part2  = new short[v_n_vernum2];
					v_ptr_vertex_part2[0] = ptr_vertex[0];
					v_ptr_index_part2[0]  = ptr_index[0];
					for (int i = 0; i < v_n_vernum2 - 1; i ++)
					{
						v_ptr_vertex_part2[i + 1] = ptr_vertex[i + v_n_second_clip];
						v_ptr_index_part2[i + 1]  = ptr_index[i + v_n_second_clip];
					}
					array_ver.push_back(v_ptr_vertex_part2);
					array_index.push_back(v_ptr_index_part2);
					array_num.push_back(v_n_vernum2);

				}

			}

			array_ver.erase(array_ver.begin());
			array_index.erase(array_index.begin());
			array_num.erase(array_num.begin());

			delete[] ptr_vertex;
			ptr_vertex = NULL;
			delete[] ptr_index;
			ptr_index = NULL;			
		}

		memcpy(pp_triangle_index, m_ptr_tri_index, v_n_tri_num * 6);

		return true;
	}

	template< typename T >
	void  triangulation<T>::basic_triangulation(vector2<T> *ptr_vertex, short* ptr_index, short n_vertex)
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
		vector2<T>* v_ptr_vertex_tmp = new vector2<T>[n_vertex];
		short* v_ptr_index_tmp = new short[n_vertex];

		// adjust the order and let the first clip point to be the first point of the polygon.
		memcpy(v_ptr_vertex_tmp, ptr_vertex + v_n_first_clip, (n_vertex - v_n_first_clip) * sizeof(vector2<T>));
		memcpy(v_ptr_vertex_tmp + n_vertex - v_n_first_clip, ptr_vertex, v_n_first_clip * sizeof(vector2<T>));
		memcpy(ptr_vertex, v_ptr_vertex_tmp, n_vertex * sizeof(vector2<T>));
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

			// delete first clip point and go on triangulation;
			// triangulation(ptr_vertex + 1, ptr_index + 1, n_vertex - 1);		// ********* need to test if this is effective.
			n_vertex --;
			for (int i = 0; i < n_vertex; i ++)
			{
				ptr_vertex[i] = ptr_vertex[i + 1];
				ptr_index[i]  = ptr_index[i + 1];
			}
			basic_triangulation(ptr_vertex, ptr_index, n_vertex);
		}
		else
		{
			// clip to two new polygon, and triangulation each other
			int v_n_vernum1 = v_n_second_clip + 1;
			int v_n_vernum2 = n_vertex -v_n_second_clip + 1;

			// first
			v_ptr_vertex_tmp = new vector2<T>[v_n_vernum1];
			v_ptr_index_tmp  = new short[v_n_vernum1];
			memcpy(v_ptr_vertex_tmp, ptr_vertex, v_n_vernum1 * sizeof(vector2<T>));
			memcpy(v_ptr_index_tmp,  ptr_index,  v_n_vernum1 * sizeof(short));
			basic_triangulation(v_ptr_vertex_tmp, v_ptr_index_tmp, v_n_vernum1);

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
			basic_triangulation(ptr_vertex, ptr_index, v_n_vernum2);
		}
	}

	template< typename T >
	short triangulation<T>::first_clip_index(vector2<T>* ptr_vertex, short n_vertex)
	{

		short v_n_first_index = 0;
		vector2<T> v_first_clip_point = ptr_vertex[0];

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

	template< typename T >
	short triangulation<T>::min_angle_index(vector2<T>* ptr_vertex, short n_vertex, double angle_limit)
	{
		short v_n_min_index = -1;
		double v_angle_min = math_d::s_two_pi;


		for (int i = 0; i < n_vertex; i ++)
		{
			// 得到三个顶点。计算∠v1v2v3。
			vector2<T> v1 = (i == 0) ? ptr_vertex[n_vertex - 1] : ptr_vertex[i - 1];
			vector2<T> v2 = ptr_vertex[i];
			vector2<T> v3 = (i == n_vertex - 1) ? ptr_vertex[0] : ptr_vertex[i + 1];

			vector2<T> v3_v2 = v3 - v2;
			vector2<T> v1_v2 = v1 - v2;
			v3_v2.normalize();
			v1_v2.normalize();

			double v_angle_v2 = math_d::acos_( vector2<T>::dot( v3_v2, v1_v2 ) );
			double v_area = (v1.x - v3.x) * (v2.y - v3.y) - (v1.y - v3.y) * (v2.x - v3.x);
			if (v_area > 0.00000001)
			{
				// CCW
				v_angle_v2 = math_d::s_two_pi - v_angle_v2;
			}

			if ( v_angle_v2 < v_angle_min && v_angle_v2 < angle_limit)
			{
				v_angle_min = v_angle_v2;
				v_n_min_index = i;
			}
		}

		return v_n_min_index;
	}


	template< typename T >
	short triangulation<T>::max_angle_index(vector2<T>* ptr_vertex, short n_vertex)
	{
		short v_n_max_index = 0;
		double v_angle_max = 0;

		for (int i = 0; i < n_vertex; i ++)
		{
			// 得到三个顶点。计算∠v1v2v3。
			vector2<T> v1 = (i == 0) ? ptr_vertex[n_vertex - 1] : ptr_vertex[i - 1];
			vector2<T> v2 = ptr_vertex[i];
			vector2<T> v3 = (i == n_vertex - 1) ? ptr_vertex[0] : ptr_vertex[i + 1];

			vector2<T> v3_v2 = v3 - v2;
			vector2<T> v1_v2 = v1 - v2;
			v3_v2.normalize();
			v1_v2.normalize();

			double v_angle_v2 = math_d::acos_( vector2<T>::dot( v3_v2, v1_v2 ) );
			double v_area = (v1.x - v3.x) * (v2.y - v3.y) - (v1.y - v3.y) * (v2.x - v3.x);
			if (v_area > 0.00000001)
			{
				// CCW
				v_angle_v2 = math_d::s_two_pi - v_angle_v2;
			}

			if ( v_angle_v2 > v_angle_max )
			{
				v_angle_max = v_angle_v2;
				v_n_max_index = i;
			}
		}

		return v_n_max_index;
	}


	template< typename T >
	short triangulation<T>::second_clip_index(vector2<T>* ptr_vertex, short n_vertex)
	{
		short v_n_second_index = -1;
		double v_dist_min = 0;

		for (int i = 2; i < n_vertex - 1; i ++)
		{
			if (is_point_in_triangle(ptr_vertex[i], ptr_vertex[0], ptr_vertex[1], ptr_vertex[n_vertex - 1]))
			{
				double v_dist = ( ptr_vertex[i] - ptr_vertex[0] ).length();
				if (v_n_second_index == -1 || v_dist < v_dist_min)
				{
					v_dist_min = v_dist;
					v_n_second_index = i;
				}
			}
		}

		return v_n_second_index;
	}

	template< typename T >
	short triangulation<T>::second_clip_index_max(vector2<T>* ptr_vertex, short n_vertex)
	{
		int v_second_clip_index = n_vertex - 2;

		vector2<T> A = ptr_vertex[n_vertex - 1];
		vector2<T> O = ptr_vertex[0];
		vector2<T> B = ptr_vertex[1];

		vector2<T> OA = A - O;
		vector2<T> OB = B - O;
		OA.normalize();
		OB.normalize();

		double v_angle_AOB = math_d::acos_( vector2<T>::dot( OA, OB ) );
		double v_area_AOB = (A.x - B.x) * (O.y - B.y) - (A.y - B.y) * (O.x - B.x);
		if ( v_area_AOB > 0.00000001 )
		{
			v_angle_AOB = math_d::s_two_pi - v_angle_AOB;
		}

		double v_half_angel_AOB = v_angle_AOB / 2;

		double v_angle_pre = v_angle_AOB;
		for (int i = 2; i < n_vertex - 1; i ++)
		{
			vector2<T> P = ptr_vertex[i];

			vector2<T> OP = P - O;
			OP.normalize();

			double v_angle_AOP = math_d::acos_( vector2<T>::dot( OA, OP ) );
			double v_area_AOP = (A.x - P.x) * (O.y - P.y) - (A.y - P.y) * (O.x - P.x);
			if ( v_area_AOP > 0.00000001 )
			{
				v_angle_AOP = math_d::s_two_pi - v_angle_AOP;
			}

			if (v_angle_AOP < v_half_angel_AOB)
			{
				if (v_angle_pre - v_half_angel_AOB < v_half_angel_AOB - v_angle_AOP)
				{
					v_second_clip_index = i - 1;
				}
				else
				{
					v_second_clip_index = i;
				}
				break;
			}

			v_angle_pre = v_angle_AOP;
		}

		// todo: 处理凹角问题：
		vector2<T> P = ptr_vertex[v_second_clip_index];
		short intersect_index_u = -1;
		short intersect_index_v = -1;

		for (int i = v_second_clip_index + 1; i < n_vertex - 2; i ++)
		{
			vector2<T> U = ptr_vertex[i];
			vector2<T> V = ptr_vertex[i + 1];
			if (intersect(O, P, U, V))
			{
				intersect_index_u = i;
				intersect_index_v = i + 1;
			}
		}

		if (intersect_index_u != -1)
		{
			vector2<T> U = ptr_vertex[intersect_index_u];
			vector2<T> V = ptr_vertex[intersect_index_v];

			vector2<T> OU = U - O;
			OU.normalize();
			vector2<T> OV = V - O;
			OV.normalize();

			double v_angle_AOU = math_d::acos_( vector2<T>::dot( OA, OU ) );
			double v_area_AOU = (A.x - U.x) * (O.y - U.y) - (A.y - U.y) * (O.x - U.x);
			if ( v_area_AOU > 0.00000001 )
			{
				v_angle_AOU = math_d::s_two_pi - v_angle_AOU;
			}
			double v_angle_AOV = math_d::acos_( vector2<T>::dot( OA, OV ) );
			double v_area_AOV = (A.x - V.x) * (O.y - V.y) - (A.y - V.y) * (O.x - V.x);
			if ( v_area_AOV > 0.00000001 )
			{
				v_angle_AOV = math_d::s_two_pi - v_angle_AOV;
			}

			if (math_d::fabs_(v_angle_AOU - v_half_angel_AOB) < math_d::fabs_(v_angle_AOV - v_half_angel_AOB))
			{
				v_second_clip_index = intersect_index_u;
			}
			else
			{
				v_second_clip_index = intersect_index_v;
			}

		}
		return v_second_clip_index;
	}

	template< typename T >
	bool triangulation<T>::intersect( vector2<T>& v1, vector2<T>& v2, vector2<T>& p1, vector2<T>& p2 )
	{
		int p1_v1v2 = judge_side( p1, v1, v2 );
		int p2_v1v2 = judge_side( p2, v1, v2 );

		if( p1_v1v2 == p2_v1v2 )
		{
			return false;
		}

		int v1_p1p2 = judge_side( v1, p1, p2 );
		int v2_p1p2 = judge_side( v2, p1, p2 );

		if( v1_p1p2 == v2_p1p2 )
		{
			return false;
		}

		return true;
	}




	template< typename T >
	bool  triangulation<T>::is_point_in_triangle(vector2<T> point_judge, vector2<T> tri_point1, vector2<T> tri_point2, vector2<T> tri_point3)
	{

		// to each edge of the triangle, 
		int p_12 = judge_side(point_judge, tri_point1, tri_point2);
		int p_13 = judge_side(point_judge, tri_point1, tri_point3);
		int p_23 = judge_side(point_judge, tri_point2, tri_point3);


		// if the judge point and the third point of the triangle is in different side of the edge,
		// the judge point is out of the triangle. 
		if (p_12 != 0 && p_12 != judge_side(tri_point3, tri_point1, tri_point2))
			return false;
		if (p_13 != 0 && p_13 != judge_side(tri_point2, tri_point1, tri_point3))
			return false;
		if (p_23 != 0 && p_23 != judge_side(tri_point1, tri_point2, tri_point3))
			return false;

		// else, the point is in triangle.
		return true;

		/*
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
		*/

		// 		// to each edge of the triangle, 
		// 		// if the judge point and the third point of the triangle is in different side of the edge,
		// 		// the judge point is out of the triangle. 
		// 		if (judge_side(point_judge, tri_point1, tri_point2) != judge_side(tri_point3, tri_point1, tri_point2))
		// 			return false;
		// 		if (judge_side(point_judge, tri_point1, tri_point3) != judge_side(tri_point2, tri_point1, tri_point3))
		// 			return false;
		// 		if (judge_side(point_judge, tri_point2, tri_point3) != judge_side(tri_point1, tri_point2, tri_point3))
		// 			return false;
		// 		// else, the point is in triangle.
		// 		return true;
	}

	template< typename T >
	int triangulation<T>::judge_side(vector2<T> point_judge, vector2<T> section_point1, vector2<T> section_point2)
	{
		double v_lf_sum1 = (point_judge.y - section_point1.y) * (point_judge.x - section_point2.x);
		double v_lf_sum2 = (point_judge.y - section_point2.y) * (point_judge.x - section_point1.x);
		double v_f_diff = v_lf_sum1 - v_lf_sum2;

		if ( math_d::fabs_( v_f_diff ) < 0.0000001 )
		{
			return 0;
		}
		if ( v_f_diff > 0)
		{
			return 1;
		}
		return -1;
		// v_lf_sum1 < v_lf_sum2: left  hand;
		// v_lf_sum1 > v_lf_sum2: right hand;
		// 		return((v_lf_sum1<v_lf_sum2) ? -1 : ((v_lf_sum1>v_lf_sum2) ? 1 : 0));
	}
}

#endif // _US_TRIANGULATION_H_