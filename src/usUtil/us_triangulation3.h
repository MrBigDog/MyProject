///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_triangulation3.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : triangulation3 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TRIANGULATION3_H_
#define _US_TRIANGULATION3_H_

#include <usUtil/us_base_algorithm_lh.h>

namespace uniscope_globe
{
	template< typename T >
	class triangulation3
	{
	public:
		triangulation3( void );
		virtual ~triangulation3( void );

	public:
		// poly_dir: 为0表示输出逆时针； 为1 表示输出为顺时针
		bool triangulate_polygon_lh(vector3<T>* ptr_poly_vertex, short n_poly_vertex, short poly_dir, ushort** pp_triangle_index );
		bool triangulate_polygon_rh(vector3<T>* ptr_poly_vertex, short n_poly_vertex, short poly_dir, ushort** pp_triangle_index );

		bool triangulate_polygon_rh_v2(vector3<T>* ptr_poly_vertex, short n_poly_vertex, short poly_dir, ushort** pp_triangle_index );
		bool triangulate_polygon_rh_v3(vector3<T>* ptr_poly_vertex, short n_poly_vertex, short poly_dir, ushort** pp_triangle_index );

	private:

		// triangulation recursive
		void tri(vector3<T>* ptr_vertex, short* ptr_index, short n_vertex);

		// sub-function used in triangulation:
		short first_clip_index(vector3<T>* ptr_vertex, short n_vertex);
		short second_clip_index(vector3<T>* ptr_vertex, short n_vertex);
		bool is_point_in_triangle(vector3<T> point_judge, vector3<T> tri_point1, vector3<T> tri_point2, vector3<T> tri_point3);
		int  judge_side(vector3<T> point_judge, vector3<T> section_point1, vector3<T> section_point2);

	private:

		short min_angle_index(vector3<T>* ptr_vertex, short n_vertex, double angle_limit = math_d::s_two_pi);
		short max_angle_index(vector3<T>* ptr_vertex, short n_vertex);

		bool  intersect( vector3<T>& v1, vector3<T>& v2, vector3<T>& p1, vector3<T>& p2 );
		short second_clip_index_max(vector3<T>* ptr_vertex, short n_vertex);


		vector3<T>* m_ptr_poly_vertex;
		short* m_ptr_ver_index;			// index of the vertex
		short* m_ptr_tri_index;			// index of triangle after triangulation
		short  m_n_vertex_num;			// the total number of the vertex

		int m_n_added_cnt;				// to note the number of index that had been added to tri_index;

		bool m_b_left_hand;
	};

	template< typename T >
	triangulation3<T>::triangulation3()
	{
		m_n_vertex_num = 0;
		m_n_added_cnt  = 0;

		m_ptr_poly_vertex = NULL;
		m_ptr_ver_index   = NULL;
		m_ptr_tri_index   = NULL;

		m_b_left_hand = false;
	}

	template< typename T >
	triangulation3<T>::~triangulation3()
	{
		AUTO_DELETE_ARRAY( m_ptr_poly_vertex );
		AUTO_DELETE_ARRAY( m_ptr_ver_index );
		AUTO_DELETE_ARRAY( m_ptr_tri_index );
	}

	// v3 根据角度剖分
	template< typename T >
	bool triangulation3<T>::triangulate_polygon_rh_v3(vector3<T>* ptr_poly_vertex, short n_poly_vertex, short poly_dir, ushort** pp_triangle_index )
	{
		m_b_left_hand = false;

		if (n_poly_vertex < 3)
			return false;
		short v_n_vertex_num = n_poly_vertex;

		if (m_ptr_tri_index)
		{
			delete[] m_ptr_tri_index;
			m_ptr_tri_index = NULL;
		}

		vector3<T>* v_ptr_poly_vertex = new vector3<T>[n_poly_vertex];
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
		vector<vector3<T>*> array_ver;
		vector<short*> array_index;
		vector<short> array_num;

		array_ver.push_back( v_ptr_poly_vertex );
		array_index.push_back( v_ptr_ver_index );
		array_num.push_back( v_n_vertex_num );

		while ( array_ver.size() > 0 )
		{
			vector3<T>* ptr_vertex = array_ver[0];
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

				vector3<T>* v_ptr_vertex_tmp = new vector3<T>[n_vertex];
				short* v_ptr_index_tmp = new short[n_vertex];

				// adjust the order and let the first clip point to be the first point of the polygon.
				memcpy(v_ptr_vertex_tmp, ptr_vertex + v_n_first_clip, (n_vertex - v_n_first_clip) * sizeof(vector3<T>));
				memcpy(v_ptr_vertex_tmp + n_vertex - v_n_first_clip, ptr_vertex, v_n_first_clip * sizeof(vector3<T>));
				memcpy(ptr_vertex, v_ptr_vertex_tmp, n_vertex * sizeof(vector3<T>));
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

					vector3<T>* v_ptr_vertex_sub = new vector3<T>[n_vertex - 1];
					short* v_ptr_index_sub  = new short[n_vertex - 1];
					memcpy(v_ptr_vertex_sub, ptr_vertex + 1, (n_vertex - 1) * sizeof(vector3<T>));
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
					vector3<T>* v_ptr_vertex_part1 = new vector3<T>[v_n_vernum1];
					short* v_ptr_index_part1  = new short[v_n_vernum1];
					memcpy(v_ptr_vertex_part1, ptr_vertex, v_n_vernum1 * sizeof(vector3<T>));
					memcpy(v_ptr_index_part1,  ptr_index,  v_n_vernum1 * sizeof(short));
					array_ver.push_back(v_ptr_vertex_part1);
					array_index.push_back(v_ptr_index_part1);
					array_num.push_back(v_n_vernum1);

					// second
					vector3<T>* v_ptr_vertex_part2 = new vector3<T>[v_n_vernum2];
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


		memcpy(*pp_triangle_index, m_ptr_tri_index, v_n_tri_num * 6);


		vector3<T> vec1 = ptr_poly_vertex[(*pp_triangle_index)[0]];
		vector3<T> vec2 = ptr_poly_vertex[(*pp_triangle_index)[1]];
		vector3<T> vec3 = ptr_poly_vertex[(*pp_triangle_index)[2]];

		double v_area = (vec2.y - vec3.y) * (vec1.x - vec3.x) - (vec2.x - vec3.x) * (vec1.y - vec3.y);
		long v_cur_dir = (v_area>0)?0:1;

		// poly_dir == 0 表示需要使用逆时针
		if ( v_cur_dir != poly_dir )
		{
			int v_index_size = ( n_poly_vertex - 2 ) * 3;
			short tmp = 0;
			for ( int i = 0; i < v_index_size; i += 3 )
			{
				tmp = (*pp_triangle_index)[i+1];
				(*pp_triangle_index)[i+1] = (*pp_triangle_index)[i+2];
				(*pp_triangle_index)[i+2] = tmp;
			}
		}

		return true;
	}


	// v2 去掉递归
	template< typename T >
	bool triangulation3<T>::triangulate_polygon_rh_v2(vector3<T>* ptr_poly_vertex, short n_poly_vertex, short poly_dir, ushort** pp_triangle_index )
	{
		m_b_left_hand = false;

		if (n_poly_vertex < 3)
			return false;
		short v_n_vertex_num = n_poly_vertex;

		if (m_ptr_tri_index)
		{
			delete[] m_ptr_tri_index;
			m_ptr_tri_index = NULL;
		}

		vector3<T>* v_ptr_poly_vertex = new vector3<T>[n_poly_vertex];
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
		vector<vector3<T>*> array_ver;
		vector<short*> array_index;
		vector<short> array_num;

		array_ver.push_back( v_ptr_poly_vertex );
		array_index.push_back( v_ptr_ver_index );
		array_num.push_back( v_n_vertex_num );

		while ( array_ver.size() > 0 )
		{
			vector3<T>* ptr_vertex = array_ver[0];
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
				// if the vertex number is more than 3, they should be triangulated.
				short v_n_first_clip = first_clip_index(ptr_vertex, n_vertex);

				vector3<T>* v_ptr_vertex_tmp = new vector3<T>[n_vertex];
				short* v_ptr_index_tmp = new short[n_vertex];

				// adjust the order and let the first clip point to be the first point of the polygon.
				memcpy(v_ptr_vertex_tmp, ptr_vertex + v_n_first_clip, (n_vertex - v_n_first_clip) * sizeof(vector3<T>));
				memcpy(v_ptr_vertex_tmp + n_vertex - v_n_first_clip, ptr_vertex, v_n_first_clip * sizeof(vector3<T>));
				memcpy(ptr_vertex, v_ptr_vertex_tmp, n_vertex * sizeof(vector3<T>));
				memcpy(v_ptr_index_tmp, ptr_index + v_n_first_clip, (n_vertex - v_n_first_clip) * sizeof(short));
				memcpy(v_ptr_index_tmp + n_vertex - v_n_first_clip, ptr_index, v_n_first_clip * sizeof(short));
				memcpy(ptr_index, v_ptr_index_tmp, n_vertex * sizeof(short));

				AUTO_DELETE_ARRAY (v_ptr_vertex_tmp);
				AUTO_DELETE_ARRAY (v_ptr_index_tmp);

				// get the second clip point.
				short v_n_second_clip = second_clip_index(ptr_vertex, n_vertex);

				// if there is no second clip point
				if (v_n_second_clip == -1)
				{
					m_ptr_tri_index[m_n_added_cnt++] = ptr_index[0];
					m_ptr_tri_index[m_n_added_cnt++] = ptr_index[1];
					m_ptr_tri_index[m_n_added_cnt++] = ptr_index[n_vertex - 1];

					vector3<T>* v_ptr_vertex_sub = new vector3<T>[n_vertex - 1];
					short* v_ptr_index_sub  = new short[n_vertex - 1];
					memcpy(v_ptr_vertex_sub, ptr_vertex + 1, (n_vertex - 1) * sizeof(vector3<T>));
					memcpy(v_ptr_index_sub,  ptr_index + 1,  (n_vertex - 1) * sizeof(short));
					array_ver.push_back(v_ptr_vertex_sub);
					array_index.push_back(v_ptr_index_sub);
					array_num.push_back(n_vertex - 1);
				}
				else
				{
					// clip to two new polygon, and triangulation each other
					int v_n_vernum1 = v_n_second_clip + 1;
					int v_n_vernum2 = n_vertex -v_n_second_clip + 1;

					// first
					vector3<T>* v_ptr_vertex_part1 = new vector3<T>[v_n_vernum1];
					short* v_ptr_index_part1  = new short[v_n_vernum1];
					memcpy(v_ptr_vertex_part1, ptr_vertex, v_n_vernum1 * sizeof(vector3<T>));
					memcpy(v_ptr_index_part1,  ptr_index,  v_n_vernum1 * sizeof(short));
					array_ver.push_back(v_ptr_vertex_part1);
					array_index.push_back(v_ptr_index_part1);
					array_num.push_back(v_n_vernum1);

					// second
					vector3<T>* v_ptr_vertex_part2 = new vector3<T>[v_n_vernum2];
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

		memcpy(*pp_triangle_index, m_ptr_tri_index, v_n_tri_num * 6);


		vector3<T> vec1 = ptr_poly_vertex[(*pp_triangle_index)[0]];
		vector3<T> vec2 = ptr_poly_vertex[(*pp_triangle_index)[1]];
		vector3<T> vec3 = ptr_poly_vertex[(*pp_triangle_index)[2]];

		double v_area = (vec2.y - vec3.y) * (vec1.x - vec3.x) - (vec2.x - vec3.x) * (vec1.y - vec3.y);
		long v_cur_dir = (v_area>0)?0:1; // > 0: 右手，CCW； < 0: 左手，CW；

		// poly_dir == 0 表示需要使用逆时针
		if ( v_cur_dir != poly_dir )
		{
			int v_index_size = ( n_poly_vertex - 2 ) * 3;
			short tmp = 0;
			for ( int i = 0; i < v_index_size; i += 3 )
			{
				tmp = (*pp_triangle_index)[i+1];
				(*pp_triangle_index)[i+1] = (*pp_triangle_index)[i+2];
				(*pp_triangle_index)[i+2] = tmp;
			}
		}

		return true;
	}

	template< typename T >
	bool triangulation3<T>::triangulate_polygon_rh(vector3<T>* ptr_poly_vertex, short n_poly_vertex, short poly_dir, ushort** pp_triangle_index)
	{
		m_b_left_hand = false;

		if ( n_poly_vertex < 3 )
			return false;

		m_n_vertex_num = n_poly_vertex;

		AUTO_DELETE_ARRAY( m_ptr_poly_vertex );
		AUTO_DELETE_ARRAY( m_ptr_ver_index );
		AUTO_DELETE_ARRAY( m_ptr_tri_index );

		m_ptr_poly_vertex = new vector3<T>[n_poly_vertex];
		m_ptr_ver_index = new short[n_poly_vertex];
		long v_n_tri_num = n_poly_vertex - 2;
		m_ptr_tri_index = new short[v_n_tri_num * 3];
		for (long i = 0; i < n_poly_vertex; i ++)
		{
			m_ptr_poly_vertex[i] = ptr_poly_vertex[i];
			m_ptr_ver_index[i] = i;
		}

		m_n_added_cnt = 0;

		tri(m_ptr_poly_vertex, m_ptr_ver_index, m_n_vertex_num);

		memcpy(*pp_triangle_index, m_ptr_tri_index, v_n_tri_num * 6);

		vector3<T> vec1 = ptr_poly_vertex[(*pp_triangle_index)[0]];
		vector3<T> vec2 = ptr_poly_vertex[(*pp_triangle_index)[1]];
		vector3<T> vec3 = ptr_poly_vertex[(*pp_triangle_index)[2]];
		
		double v_area = (vec2.y - vec3.y) * (vec1.x - vec3.x) - (vec2.x - vec3.x) * (vec1.y - vec3.y);
		long v_cur_dir = (v_area>0)?0:1;

		// poly_dir == 0 表示需要使用逆时针
		if ( v_cur_dir != poly_dir )
		{
			int v_index_size = ( n_poly_vertex - 2 ) * 3;
			short tmp = 0;
			for ( int i = 0; i < v_index_size; i += 3 )
			{
				tmp = (*pp_triangle_index)[i+1];
				(*pp_triangle_index)[i+1] = (*pp_triangle_index)[i+2];
				(*pp_triangle_index)[i+2] = tmp;
			}
		}

		return true;

	}

	template< typename T >
	bool triangulation3<T>::triangulate_polygon_lh(vector3<T>* ptr_poly_vertex, short n_poly_vertex, short poly_dir, ushort** pp_triangle_index)
	{
		m_b_left_hand = true;

		if ( n_poly_vertex < 3 )
			return false;

		m_n_vertex_num = n_poly_vertex;

		AUTO_DELETE_ARRAY( m_ptr_poly_vertex );
		AUTO_DELETE_ARRAY( m_ptr_ver_index );
		AUTO_DELETE_ARRAY( m_ptr_tri_index );

		m_ptr_poly_vertex = new vector3<T>[n_poly_vertex];
		m_ptr_ver_index = new short[n_poly_vertex];
		long v_n_tri_num = n_poly_vertex - 2;
		m_ptr_tri_index = new short[v_n_tri_num * 3];
		for (long i = 0; i < n_poly_vertex; i ++)
		{
			m_ptr_poly_vertex[i] = ptr_poly_vertex[i];
			m_ptr_ver_index[i] = i;
		}

		m_n_added_cnt = 0;

		tri(m_ptr_poly_vertex, m_ptr_ver_index, m_n_vertex_num);

		memcpy(*pp_triangle_index, m_ptr_tri_index, v_n_tri_num * 6);

		vector3<T> vec1 = ptr_poly_vertex[(*pp_triangle_index)[0]];
		vector3<T> vec2 = ptr_poly_vertex[(*pp_triangle_index)[1]];
		vector3<T> vec3 = ptr_poly_vertex[(*pp_triangle_index)[2]];

		double v_area = (vec2.y - vec3.y) * (vec1.x - vec3.x) - (vec2.x - vec3.x) * (vec1.y - vec3.y);
		long v_cur_dir = (v_area>0)?0:1; // v_cur_dir > 0: CCW; 反之CW；

		// poly_dir == 0 表示需要使用逆时针
		if ( v_cur_dir != poly_dir )
		{
			int v_index_size = ( n_poly_vertex - 2 ) * 3;
			short tmp = 0;
			for ( int i = 0; i < v_index_size; i += 3 )
			{
				tmp = (*pp_triangle_index)[i+1];
				(*pp_triangle_index)[i+1] = (*pp_triangle_index)[i+2];
				(*pp_triangle_index)[i+2] = tmp;
			}
		}

		return true;

	}

	template< typename T >
	void triangulation3<T>::tri(vector3<T>* ptr_vertex, short* ptr_index, short n_vertex)
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
		vector3<T>* v_ptr_vertex_tmp = new vector3<T>[n_vertex];
		short* v_ptr_index_tmp = new short[n_vertex];

		// adjust the order and let the first clip point to be the first point of the polygon.
		memcpy(v_ptr_vertex_tmp, ptr_vertex + v_n_first_clip, (n_vertex - v_n_first_clip) * sizeof(vector3<T>));
		memcpy(v_ptr_vertex_tmp + n_vertex - v_n_first_clip, ptr_vertex, v_n_first_clip * sizeof(vector3<T>));
		memcpy(ptr_vertex, v_ptr_vertex_tmp, n_vertex * sizeof(vector3<T>));
		memcpy(v_ptr_index_tmp, ptr_index + v_n_first_clip, (n_vertex - v_n_first_clip) * sizeof(short));
		memcpy(v_ptr_index_tmp + n_vertex - v_n_first_clip, ptr_index, v_n_first_clip * sizeof(short));
		memcpy(ptr_index, v_ptr_index_tmp, n_vertex * sizeof(short));

		AUTO_DELETE_ARRAY( v_ptr_vertex_tmp );
		AUTO_DELETE_ARRAY( v_ptr_index_tmp );
		
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
			v_ptr_vertex_tmp = new vector3<T>[v_n_vernum1];
			v_ptr_index_tmp  = new short[v_n_vernum1];
			memcpy(v_ptr_vertex_tmp, ptr_vertex, v_n_vernum1 * sizeof(vector3<T>));
			memcpy(v_ptr_index_tmp,  ptr_index,  v_n_vernum1 * sizeof(short));
			tri(v_ptr_vertex_tmp, v_ptr_index_tmp, v_n_vernum1);

			AUTO_DELETE_ARRAY( v_ptr_vertex_tmp );
			AUTO_DELETE_ARRAY( v_ptr_index_tmp );

			// second
			for (int i = 0; i < v_n_vernum2 - 1; i ++)
			{
				ptr_vertex[i + 1] = ptr_vertex[i + v_n_second_clip];
				ptr_index[i + 1]  = ptr_index[i + v_n_second_clip];
			}
			tri(ptr_vertex, ptr_index, v_n_vernum2);
		}
	}

	template< typename T >
	short triangulation3<T>::first_clip_index(vector3<T>* ptr_vertex, short n_vertex)
	{
		short v_n_first_index = 0;
		vector3<T> v_first_clip_point = ptr_vertex[0];

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
	short triangulation3<T>::second_clip_index(vector3<T>* ptr_vertex, short n_vertex)
	{
		short v_n_second_index = -1;
		vector3<T> v_second_clip_point;

		for (int i = 2; i < n_vertex - 1; i ++)
		{
			point_triangle_ralationship rel = UNKNOWN;
			if (m_b_left_hand)
			{
				rel = us_base_algorithm_lh<T>::point_in_triangle(ptr_vertex[i], ptr_vertex[0], ptr_vertex[1], ptr_vertex[n_vertex - 1]);
			}
			else
			{
				rel = us_base_algorithm<T>::point_in_triangle(ptr_vertex[i], ptr_vertex[0], ptr_vertex[1], ptr_vertex[n_vertex - 1]);
			}
			if (rel == INTERIOR)
			{
				if (v_n_second_index == -1 || ptr_vertex[i].x < v_second_clip_point.x)
				{
					v_second_clip_point = ptr_vertex[i];
					v_n_second_index = i;
				}
			}
			else if (rel == VERTEX_A || rel == BOUNDARY_AB || rel == BOUNDARY_AC)
			{
				// 与边和顶点重合的特殊情况的处理；
				vector3<T> vec = (ptr_vertex[i - 1] + ptr_vertex[i + 1]) / 2;
				point_triangle_ralationship pit = UNKNOWN;
				bool lli = false;
				if (m_b_left_hand)
				{
					pit = us_base_algorithm_lh<T>::point_in_triangle(vec, ptr_vertex[0], ptr_vertex[1], ptr_vertex[n_vertex - 1]);
					lli = us_base_algorithm_lh<T>::line_line_intersect_simp(ptr_vertex[1], ptr_vertex[n_vertex - 1], ptr_vertex[i], vec);
				}
				else
				{
					pit = us_base_algorithm<T>::point_in_triangle(vec, ptr_vertex[0], ptr_vertex[1], ptr_vertex[n_vertex - 1]);
					lli = us_base_algorithm<T>::line_line_intersect_simp(ptr_vertex[1], ptr_vertex[n_vertex - 1], ptr_vertex[i], vec);
				}
				if (lli || pit == INTERIOR)
				{
					if (v_n_second_index == -1 || ptr_vertex[i].x < v_second_clip_point.x)
					{
						v_second_clip_point = ptr_vertex[i];
						v_n_second_index = i;
					}
				}

			}
		}

		return v_n_second_index;

	}

	template< typename T >
	bool triangulation3<T>::is_point_in_triangle(vector3<T> point_judge, vector3<T> tri_point1, vector3<T> tri_point2, vector3<T> tri_point3)
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
	}

	template< typename T >
	int triangulation3<T>::judge_side(vector3<T> point_judge, vector3<T> section_point1, vector3<T> section_point2)
	{
		double v_lf_sum1, v_lf_sum2;
		if (m_b_left_hand)
		{
			v_lf_sum1 = (point_judge.z - section_point1.z) * (point_judge.x - section_point2.x);
			v_lf_sum2 = (point_judge.z - section_point2.z) * (point_judge.x - section_point1.x);
		}
		else
		{
			v_lf_sum1 = (point_judge.y - section_point1.y) * (point_judge.x - section_point2.x);
			v_lf_sum2 = (point_judge.y - section_point2.y) * (point_judge.x - section_point1.x);
		}
		
		return((v_lf_sum1<v_lf_sum2) ? -1 : ((v_lf_sum1>v_lf_sum2) ? 1 : 0));
	}





	template< typename T >
	bool triangulation3<T>::intersect( vector3<T>& v1, vector3<T>& v2, vector3<T>& p1, vector3<T>& p2 )
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
	short triangulation3<T>::second_clip_index_max(vector3<T>* ptr_vertex, short n_vertex)
	{
		int v_second_clip_index = n_vertex - 2;

		vector3<T> A = ptr_vertex[n_vertex - 1];
		vector3<T> O = ptr_vertex[0];
		vector3<T> B = ptr_vertex[1];

		vector3<T> OA = A - O;
		vector3<T> OB = B - O;
		OA.normalize();
		OB.normalize();

		double v_angle_AOB = math_d::acos_( vector3<T>::dot( OA, OB ) );
		double v_area_AOB = (A.x - B.x) * (O.y - B.y) - (A.y - B.y) * (O.x - B.x);
		if ( v_area_AOB > 0.00000001 )
		{
			v_angle_AOB = math_d::s_two_pi - v_angle_AOB;
		}

		double v_half_angel_AOB = v_angle_AOB / 2;

		double v_angle_pre = v_angle_AOB;
		for (int i = 2; i < n_vertex - 1; i ++)
		{
			vector3<T> P = ptr_vertex[i];

			vector3<T> OP = P - O;
			OP.normalize();

			double v_angle_AOP = math_d::acos_( vector3<T>::dot( OA, OP ) );
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
		vector3<T> P = ptr_vertex[v_second_clip_index];
		short intersect_index_u = -1;
		short intersect_index_v = -1;

		for (int i = v_second_clip_index + 1; i < n_vertex - 2; i ++)
		{
			vector3<T> U = ptr_vertex[i];
			vector3<T> V = ptr_vertex[i + 1];
			if (intersect(O, P, U, V))
			{
				intersect_index_u = i;
				intersect_index_v = i + 1;
			}
		}

		if (intersect_index_u != -1)
		{
			vector3<T> U = ptr_vertex[intersect_index_u];
			vector3<T> V = ptr_vertex[intersect_index_v];

			vector3<T> OU = U - O;
			OU.normalize();
			vector3<T> OV = V - O;
			OV.normalize();

			double v_angle_AOU = math_d::acos_( vector3<T>::dot( OA, OU ) );
			double v_area_AOU = (A.x - U.x) * (O.y - U.y) - (A.y - U.y) * (O.x - U.x);
			if ( v_area_AOU > 0.00000001 )
			{
				v_angle_AOU = math_d::s_two_pi - v_angle_AOU;
			}
			double v_angle_AOV = math_d::acos_( vector3<T>::dot( OA, OV ) );
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
	short triangulation3<T>::min_angle_index(vector3<T>* ptr_vertex, short n_vertex, double angle_limit)
	{
		short v_n_min_index = -1;
		double v_angle_min = math_d::s_two_pi;


		for (int i = 0; i < n_vertex; i ++)
		{
			// 得到三个顶点。计算∠v1v2v3。
			vector3<T> v1 = (i == 0) ? ptr_vertex[n_vertex - 1] : ptr_vertex[i - 1];
			vector3<T> v2 = ptr_vertex[i];
			vector3<T> v3 = (i == n_vertex - 1) ? ptr_vertex[0] : ptr_vertex[i + 1];

			vector3<T> v3_v2 = v3 - v2;
			vector3<T> v1_v2 = v1 - v2;
			v3_v2.normalize();
			v1_v2.normalize();

			double v_angle_v2 = math_d::acos_( vector3<T>::dot( v3_v2, v1_v2 ) );
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
	short triangulation3<T>::max_angle_index(vector3<T>* ptr_vertex, short n_vertex)
	{
		short v_n_max_index = 0;
		double v_angle_max = 0;

		for (int i = 0; i < n_vertex; i ++)
		{
			// 得到三个顶点。计算∠v1v2v3。
			vector3<T> v1 = (i == 0) ? ptr_vertex[n_vertex - 1] : ptr_vertex[i - 1];
			vector3<T> v2 = ptr_vertex[i];
			vector3<T> v3 = (i == n_vertex - 1) ? ptr_vertex[0] : ptr_vertex[i + 1];

			vector3<T> v3_v2 = v3 - v2;
			vector3<T> v1_v2 = v1 - v2;
			v3_v2.normalize();
			v1_v2.normalize();

			double v_angle_v2 = math_d::acos_( vector3<T>::dot( v3_v2, v1_v2 ) );
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



}

#endif // _TRIANGULATION_H_ 