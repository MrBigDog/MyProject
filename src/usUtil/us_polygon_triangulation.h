///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_polygon_triangulation.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : polygon_triangulation class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_POLYGON_TRIANGULATION_H_
#define _US_POLYGON_TRIANGULATION_H_

#include <usUtil\Export.h>

#include <map>
#include <vector>

using namespace std;

namespace uniscope_globe
{
	class coordinates_array;

	class USUTIL_EXPORT edge_key
	{
	public:
		edge_key(void)
		{
			v[0] = -1;
			v[1] = -1;
		}

		edge_key(const edge_key& one)
		{
			v[0] = one.v[0];
			v[1] = one.v[1];
		}

		edge_key(const int v0, const int v1)
		{
			if (v0 < v1)
			{
				// v0 is minimum
				v[0] = v0;
				v[1] = v1;
			}
			else
			{
				// v1 is minimum
				v[0] = v1;
				v[1] = v0;
			}
		}

		virtual ~edge_key(void) { };

	public:
		bool operator< (const edge_key& key) const;
		bool operator== (const edge_key& key) const;

	public:
		bool contian_vertex(int vertex);

	public:
		int v[2];
	};

	//////////////////////////////////////////////////////////////////////////
	class USUTIL_EXPORT tri_key
	{
	public:
		tri_key(void)
		{
			v1 = -1;
			v2 = -1;
			v3 = -1;
		};

		tri_key(const vector3<int>& in_index)
		{
			v1 = in_index.x;
			v2 = in_index.y;
			v3 = in_index.z;
		};

		tri_key(const int& in_v1, const int& in_v2, const int& in_v3)
		{
			v1 = in_v1;
			v2 = in_v2;
			v3 = in_v3;
		};

		tri_key(const tri_key& in_key)
		{
			v1 = in_key.v1;
			v2 = in_key.v2;
			v3 = in_key.v3;
		};

		virtual ~tri_key(void) { };

	public:
		bool operator== (const tri_key& key) const;


	public:
		bool contain_vertex(int ver_index);
		bool contain_edge(edge_key edge);

		int get_opposite_vertex(edge_key edge);
		edge_key get_opposite_edge(int vertex);

	public:
		int v1;
		int v2;
		int v3;
	};


	class USUTIL_EXPORT polygon_triangulation
	{
	public:
		polygon_triangulation(void);

		virtual ~polygon_triangulation(void);

	public:
		typedef map<int, vector<int>> ver_tri_map;
		typedef map<edge_key, vector<int>> edge_tri_map;

	public:

		// 接口
		// polygon: 输入polygon，第一个子环为外环，其他所有子环均为外环
		// ptr_tri_index: 三角化后的三角面索引
		// lh: 左手坐标系，y向上；rh: 右手坐标系，z向上
		bool do_trianglulate_lh(vector<coordinates_array> polygon, vector<int>& tri_index);
		bool do_trianglulate_lh(coordinates_array& v_points_array, std::vector<int>& part_info, vector<int>& tri_index);
		bool do_trianglulate_rh(vector<coordinates_array> polygon, vector<int>& tri_index);

		static bool is_polygon_ccw(coordinates_array in_poly);

	private:
		void initialize();
		void add_tris_to_map(vector<tri_key> tris, int begin_pos);

		void delaunay_triangulation();
		void check_edge(edge_key edge);

		void get_tris_outside();
		void delete_invalid_tri();


	private:
		void re_triangle(vector<int> part);
		void delete_tris_from_map(vector<int> tris_index);

	private:
		void rebuild();
		void clear();


	private:
		coordinates_array    m_array_point;		// 顶点序列；
		vector<vector<int>>  m_array_ring;		// 环索引；


	private:
		vector<tri_key> m_array_tri_index;	// 三角面序列；
		vector<int> m_array_tri_deleted;	// 无效三角面；

		ver_tri_map m_ver_tri_map;		// 顶点-三角面表；
		edge_tri_map m_edge_tri_map;	// 边-三角面表；

	private:
		vector<coordinates_array> m_polygon;
	};
}

#endif // _US_POLYGON_TRIANGULATION_H_ 