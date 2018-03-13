///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_polygon_triangulation.cpp
//  Author  : Uniscope Team
//  Modifier: Uniscope Team
//  Created :
//  Purpose : polygon_triangulation class
//	Reference :
//
///////////////////////////////////////////////////////////////////////////

#include "us_polygon_triangulation.h"
#include <usUtil/us_math.h>
#include <usUtil/us_vector2.h>
#include <usUtil/us_vector3.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_base_algorithm.h>
#include <usUtil/TriangleX.h>
#include <usUtil/us_triangulation.h>
#include <usUtil/us_triangulation3.h>
#include <usUtil/us_xml_parser_ext.h>
#include <assert.h>

namespace uniscope_globe
{
	polygon_triangulation::polygon_triangulation(void)
	{

	}

	polygon_triangulation::~polygon_triangulation(void)
	{

	}

	void polygon_triangulation::clear()
	{
		for (edge_tri_map::iterator itr = m_edge_tri_map.begin(); itr != m_edge_tri_map.end(); itr++)
		{
			itr->second.clear();
		}
		m_edge_tri_map.clear();

		for (ver_tri_map::iterator itr = m_ver_tri_map.begin(); itr != m_ver_tri_map.end(); itr++)
		{
			itr->second.clear();
		}
		m_ver_tri_map.clear();

	}

	void polygon_triangulation::rebuild()
	{
		clear();
		delete_invalid_tri();
		add_tris_to_map(m_array_tri_index, 0);
	}

	bool polygon_triangulation::do_trianglulate_rh(vector<coordinates_array> polygon, vector<int>& tri_index)
	{
		if (polygon.size() == 0) { return false; }
		if (polygon[0].size() < 3) { return false; }

		// initialize
		m_polygon = polygon;
		initialize();

		// delaunay triangulation
		delaunay_triangulation();
		add_tris_to_map(m_array_tri_index, 0);

		// check edge
		int ring_num = m_array_ring.size();
		for (int i = 0; i < ring_num; i++)
		{
			vector<int> ring = m_array_ring[i];
			int pnt_num = ring.size();
			for (int j = 0; j < pnt_num; j++)
			{
				edge_key edge = edge_key(ring[j], ring[(j + 1) % pnt_num]);
				check_edge(edge);
			}
		}

		// delete invalid triangles
		get_tris_outside();
		delete_invalid_tri();

		// get output tri index
		tri_index.clear();
		int tri_num = m_array_tri_index.size();
		for (int i = 0; i < tri_num; i++)
		{
			tri_index.push_back(m_array_tri_index[i].v1);
			tri_index.push_back(m_array_tri_index[i].v2);
			tri_index.push_back(m_array_tri_index[i].v3);
		}
		return true;
	}

	bool polygon_triangulation::do_trianglulate_lh(coordinates_array& v_points_array, std::vector<int>& part_info, vector<int>& tri_index)
	{
		if (part_info.size() == 0) { return false; }
		if (v_points_array.size() < 3) { return false; }

		// initialize
		int v_s = 0, v_e = 0;
		int ring_num = part_info.size();
		for (int i = 0; i < ring_num; i++)
		{
			int v_ring_num = part_info[i];

			v_s = v_e;
			v_e += v_ring_num;

			vector_3d vec;
			double temp;
			coordinates_array new_ring;
			for (int j = v_s; j < v_e; j++)
			{
				vec = v_points_array[j];

				temp = vec.y;
				vec.y = vec.z;
				vec.z = temp;

				new_ring.push_back(vec);
			}

			m_polygon.push_back(new_ring);
		}

		initialize();

		// delaunay triangulation
		delaunay_triangulation();
		add_tris_to_map(m_array_tri_index, 0);

		// check edge
		ring_num = m_array_ring.size();
		for (int i = 0; i < ring_num; i++)
		{
			vector<int> ring = m_array_ring[i];
			int pnt_num = ring.size();
			for (int j = 0; j < pnt_num; j++)
			{
				edge_key edge = edge_key(ring[j], ring[(j + 1) % pnt_num]);
				check_edge(edge);
			}
		}

		// delete invalid triangles
		delete_invalid_tri();
		get_tris_outside();
		delete_invalid_tri();

		// get output tri index
		tri_index.clear();
		int tri_num = m_array_tri_index.size();
		for (int i = 0; i < tri_num; i++)
		{
			tri_index.push_back(m_array_tri_index[i].v1);
			tri_index.push_back(m_array_tri_index[i].v2);
			tri_index.push_back(m_array_tri_index[i].v3);
		}
	}

	bool polygon_triangulation::do_trianglulate_lh(vector<coordinates_array> polygon, vector<int>& tri_index)
	{
		if (polygon.size() == 0) { return false; }
		if (polygon[0].size() < 3) { return false; }

		// initialize
		int ring_num = polygon.size();
		for (int i = 0; i < ring_num; i++)
		{
			coordinates_array ring = polygon[i];
			coordinates_array new_ring;

			int pnt_num = ring.size();
			vector_3d vec;
			double temp;

			for (int j = 0; j < pnt_num; j++)
			{
				vec = ring[j];

				temp = vec.y;
				vec.y = vec.z;
				vec.z = temp;

				new_ring.push_back(vec);
			}

			m_polygon.push_back(new_ring);
		}

		initialize();

		// delaunay triangulation
		delaunay_triangulation();
		add_tris_to_map(m_array_tri_index, 0);

		// check edge
		ring_num = m_array_ring.size();
		for (int i = 0; i < ring_num; i++)
		{
			vector<int> ring = m_array_ring[i];
			int pnt_num = ring.size();
			for (int j = 0; j < pnt_num; j++)
			{
				edge_key edge = edge_key(ring[j], ring[(j + 1) % pnt_num]);
				check_edge(edge);
			}
		}

		// delete invalid triangles
		get_tris_outside();
		delete_invalid_tri();

		// get output tri index
		tri_index.clear();
		int tri_num = m_array_tri_index.size();
		for (int i = 0; i < tri_num; i++)
		{
			tri_index.push_back(m_array_tri_index[i].v1);
			tri_index.push_back(m_array_tri_index[i].v2);
			tri_index.push_back(m_array_tri_index[i].v3);
		}
		return true;
	}

	void polygon_triangulation::initialize()
	{
		int ring_num = m_polygon.size();
		int index_curr = 0;

		for (int i = 0; i < ring_num; i++)
		{
			coordinates_array ring = m_polygon[i];
			int pnt_num = ring.size();
			vector<int> ring_index;
			for (int j = 0; j < pnt_num; j++)
			{
				m_array_point.push_back(ring[j]);
				ring_index.push_back(index_curr++);
			}
			m_array_ring.push_back(ring_index);
		}

	}

	void polygon_triangulation::delaunay_triangulation()
	{
		triangulateio in;
		memset(&in, 0, sizeof(in));

		//for triangulation
		in.numberofpoints = m_array_point.size();
		in.numberofpointattributes = 1;
		in.pointlist = new TRI_REAL[in.numberofpoints * 2];
		in.pointattributelist = new TRI_REAL[in.numberofpoints];

		int n_accu = 0;
		for (int i = 0; i < in.numberofpoints; i++)
		{
			in.pointlist[i * 2] = (m_array_point[i].x);
			in.pointlist[i * 2 + 1] = (m_array_point[i].y);
			in.pointattributelist[i] = i;
			n_accu++;
		}

		in.numberofsegments = in.numberofpoints;
		in.numberofholes = 0;
		in.numberofregions = 0;
		in.regionlist = NULL;

		in.trianglelist = (int*)NULL;

		in.triangleattributelist = (TRI_REAL*)NULL;

		TriangleX triangle;
		triangle.Build("-z,-v", &in);
		triangulateio* out;
		out = triangle.GetOut();

		delete in.pointlist;
		delete in.pointattributelist;

		m_array_tri_index.clear();
		tri_key tri;
		for (int i = 0; i < out->numberoftriangles; i++)
		{
			tri.v1 = (ulong)out->pointattributelist[out->trianglelist[i * 3]];
			tri.v2 = out->pointattributelist[out->trianglelist[i * 3 + 1]];
			tri.v3 = out->pointattributelist[out->trianglelist[i * 3 + 2]];
			m_array_tri_index.push_back(tri);
		}

	}

	void polygon_triangulation::add_tris_to_map(vector<tri_key> tris, int begin_pos)
	{
		tri_key tri;
		int tri_num = tris.size();
		for (int i = 0; i < tri_num; i++)
		{
			tri = tris[i];

			// ver_tri_map:
			// a
			ver_tri_map::iterator itr_vt = m_ver_tri_map.find(tri.v1);
			if (itr_vt == m_ver_tri_map.end())
			{
				vector<int> tri_array;
				tri_array.push_back(i + begin_pos);
				m_ver_tri_map.insert(make_pair(tri.v1, tri_array));
			}
			else
			{
				itr_vt->second.push_back(i + begin_pos);
			}
			// b
			itr_vt = m_ver_tri_map.find(tri.v2);
			if (itr_vt == m_ver_tri_map.end())
			{
				vector<int> tri_array;
				tri_array.push_back(i + begin_pos);
				m_ver_tri_map.insert(make_pair(tri.v2, tri_array));
			}
			else
			{
				itr_vt->second.push_back(i + begin_pos);
			}
			// c
			itr_vt = m_ver_tri_map.find(tri.v3);
			if (itr_vt == m_ver_tri_map.end())
			{
				vector<int> tri_array;
				tri_array.push_back(i);
				m_ver_tri_map.insert(make_pair(tri.v3, tri_array));
			}
			else
			{
				itr_vt->second.push_back(i + begin_pos);
			}

			// edge_tri_map
			// ab
			edge_key curr_edge = edge_key(tri.v1, tri.v2);
			edge_tri_map::iterator itr_et = m_edge_tri_map.find(curr_edge);
			if (itr_et == m_edge_tri_map.end())
			{
				vector<int> tri_array;
				tri_array.push_back(i + begin_pos);
				m_edge_tri_map.insert(make_pair(curr_edge, tri_array));
			}
			else
			{
				itr_et->second.push_back(i + begin_pos);
			}
			// bc
			curr_edge = edge_key(tri.v2, tri.v3);
			itr_et = m_edge_tri_map.find(curr_edge);
			if (itr_et == m_edge_tri_map.end())
			{
				vector<int> tri_array;
				tri_array.push_back(i + begin_pos);
				m_edge_tri_map.insert(make_pair(curr_edge, tri_array));
			}
			else
			{
				itr_et->second.push_back(i + begin_pos);
			}
			// ac
			curr_edge = edge_key(tri.v1, tri.v3);
			itr_et = m_edge_tri_map.find(curr_edge);
			if (itr_et == m_edge_tri_map.end())
			{
				vector<int> tri_array;
				tri_array.push_back(i + begin_pos);
				m_edge_tri_map.insert(make_pair(curr_edge, tri_array));
			}
			else
			{
				itr_et->second.push_back(i + begin_pos);
			}
		}
	}

	void polygon_triangulation::check_edge(edge_key edge)
	{
		edge_tri_map::iterator itr = m_edge_tri_map.find(edge);
		if (itr != m_edge_tri_map.end())
		{
			return;
		}
		else
		{
			vector_3d edge_v0 = m_array_point[edge.v[0]];
			vector_3d edge_v1 = m_array_point[edge.v[1]];

			// ---- get related objects -------------------------------------

			vector<int> related_tris;		// 所有相关三角形；
			vector<edge_key> related_edges; // 所有相关边；
			vector<int> next_points;

			int curr_tri = -1, next_tri = -1;
			edge_key curr_edge, next_edge;
			int pre_point = edge.v[0], next_point = edge.v[0];

			// get curr_tri, curr_edge;
			ver_tri_map::iterator itr_vt = m_ver_tri_map.find(edge.v[1]);
			if (itr_vt == m_ver_tri_map.end()) // 先判定后一个点是否被使用；
			{
				return;
			}
			/*ver_tri_map::iterator */itr_vt = m_ver_tri_map.find(edge.v[0]);
			if (itr_vt == m_ver_tri_map.end()) // 没有找到顶点，理论上不会出现；
			{
				return;
			}
			vector<int> tri_array = itr_vt->second;
			for (int i = 0; i < tri_array.size(); i++)
			{
				tri_key temp_tri = m_array_tri_index[tri_array[i]];
				edge_key temp_edge = temp_tri.get_opposite_edge(edge.v[0]);
				if (us_base_algorithm<double>::line_line_intersect_simp(edge_v0, edge_v1, m_array_point[temp_edge.v[0]], m_array_point[temp_edge.v[1]]))
				{
					curr_tri = tri_array[i];
					curr_edge = temp_edge;
					break;
				}
			}
			if (curr_tri == -1) // 没有取到相交的，理论上不会发生的
			{
				return;
			}

			// add curr_tri, curr_edge;
			related_tris.push_back(curr_tri);
			related_edges.push_back(curr_edge);

			// step2: get next_tri, next_point; and add;
			itr = m_edge_tri_map.find(curr_edge);
			if (itr == m_edge_tri_map.end())
			{
				return;
			}
			tri_array = itr->second;
			for (int i = 0; i < tri_array.size(); i++)
			{
				tri_key temp_tri = m_array_tri_index[tri_array[i]];
				if (temp_tri.contain_vertex(pre_point) == false)
				{
					next_tri = tri_array[i];
					next_point = temp_tri.get_opposite_vertex(curr_edge);
					related_tris.push_back(next_tri);
					next_points.push_back(next_point);
					break;
				}
			}

			while (next_point != edge.v[1])
			{
				// get next_edge;
				edge_key temp_edge = edge_key(curr_edge.v[0], next_point);
				if (us_base_algorithm<double>::line_line_intersect_simp(edge_v0, edge_v1, m_array_point[temp_edge.v[0]], m_array_point[temp_edge.v[1]]))
				{
					next_edge = temp_edge;
				}
				else
				{
					next_edge = edge_key(curr_edge.v[1], next_point);
				}

				// add next_edge;
				related_edges.push_back(next_edge);

				// update curr_tri, curr_edge, pre_point;
				pre_point = m_array_tri_index[next_tri].get_opposite_vertex(next_edge);
				curr_tri = next_tri;
				curr_edge = next_edge;

				// get next_tri, get_next_point; and add
				itr = m_edge_tri_map.find(curr_edge);
				if (itr == m_edge_tri_map.end())
				{
					return;
				}
				tri_array = itr->second;

				for (int i = 0; i < tri_array.size(); i++)
				{
					tri_key temp_tri = m_array_tri_index[tri_array[i]];
					if (temp_tri.contain_vertex(pre_point) == false)
					{
						next_tri = tri_array[i];
						next_point = temp_tri.get_opposite_vertex(curr_edge);
						related_tris.push_back(next_tri);
						next_points.push_back(next_point);
						break;
					}
				}

			}

			// ---- deal with locked edges ----------------------------------

			vector<vector<int>> parts_array;	// 所有线段左和线段右多边形的集合
			vector<int> points_left;  // 当前线段左的点
			vector<int> points_right; // 当前线段右的点

			points_left.push_back(edge.v[0]);
			points_right.push_back(edge.v[0]);
			int pre = edge.v[0];		// 用于有分段的状况，记录上一个顶点的索引值；

			int e0 = 0, e1 = 0, e2 = 0;
			edge_key rel_edge;
			for (int i = 0; i < related_edges.size(); i++)
			{
				rel_edge = related_edges[i];
				vector_3d rel_edge_v0 = m_array_point[rel_edge.v[0]];
				vector_3d rel_edge_v1 = m_array_point[rel_edge.v[1]];

				if (i == 0)
				{
					e0 = us_base_algorithm<double>::judge_side(rel_edge_v0, edge_v0, edge_v1);
					e1 = us_base_algorithm<double>::judge_side(rel_edge_v1, edge_v0, edge_v1);

					if (e0 == 1) { points_left.push_back(rel_edge.v[0]); }
					else if (e0 == -1) { points_right.push_back(rel_edge.v[0]); }
					else
					{
						return;
					}

					if (e1 == 1) { points_left.push_back(rel_edge.v[1]); }
					else if (e1 == -1) { points_right.push_back(rel_edge.v[1]); }
					else
					{
						return;
					}
				}
				else
				{
					e2 = us_base_algorithm<double>::judge_side(m_array_point[next_points[i - 1]], edge_v0, edge_v1);


					if (e2 == 1) { points_left.push_back(next_points[i - 1]); }
					else if (e2 == -1) { points_right.push_back(next_points[i - 1]); }
					else
					{
						return;
					}

				}
			}
			points_left.push_back(edge.v[1]);
			points_right.push_back(edge.v[1]);

			parts_array.push_back(points_left);
			parts_array.push_back(points_right);

			// ---- delete old triangles ------------------------------------
			delete_tris_from_map(related_tris);

			// ---- add new triangles ---------------------------------------
			for (int i = 0; i < parts_array.size(); i++)
			{
				re_triangle(parts_array[i]);
			}
		}
	}

	void polygon_triangulation::get_tris_outside()
	{
		int ring_num = m_polygon.size();
		int tri_num = m_array_tri_index.size();

		for (int i = 0; i < tri_num; i++)
		{
			tri_key tri = m_array_tri_index[i];
			vector_3d vec_center = (m_array_point[tri.v1] + m_array_point[tri.v2] + m_array_point[tri.v3]) / 3;
			assert(!m_polygon[0].empty());
			if (us_base_algorithm<double>::point_in_polygon(vec_center, &(m_polygon[0][0]), m_polygon[0].size()) == false)
			{
				m_array_tri_deleted.push_back(i);
			}
			else
			{
				for (int j = 1; j < ring_num; j++)
				{
					assert(!m_polygon[j].empty());

					if (us_base_algorithm<double>::point_in_polygon(vec_center, &(m_polygon[j][0]), m_polygon[j].size()) == true)
					{
						m_array_tri_deleted.push_back(i);
						break;
					}
				}
			}
		}
	}

	void polygon_triangulation::delete_tris_from_map(vector<int> tris_index)
	{
		int tri_index;
		tri_key tri;
		for (int i = 0; i < tris_index.size(); i++)
		{
			tri_index = tris_index[i];
			tri = m_array_tri_index[tri_index];

			m_array_tri_deleted.push_back(tri_index);

			// 			continue;

			// delete from vers
			// a
			ver_tri_map::iterator itr_vt = m_ver_tri_map.find(tri.v1);
			if (itr_vt != m_ver_tri_map.end())
			{
				vector<int> tri_array = itr_vt->second;
				for (int j = 0; j < tri_array.size(); j++)
				{
					if (tri_array[j] == tri_index)
					{
						itr_vt->second.erase(itr_vt->second.begin() + j);
						break;
					}
				}
			}
			// b
			itr_vt = m_ver_tri_map.find(tri.v2);
			if (itr_vt != m_ver_tri_map.end())
			{
				vector<int> tri_array = itr_vt->second;
				for (int j = 0; j < tri_array.size(); j++)
				{
					if (tri_array[j] == tri_index)
					{
						itr_vt->second.erase(itr_vt->second.begin() + j);
						break;
					}
				}
			}
			// c
			itr_vt = m_ver_tri_map.find(tri.v3);
			if (itr_vt != m_ver_tri_map.end())
			{
				vector<int> tri_array = itr_vt->second;
				for (int j = 0; j < tri_array.size(); j++)
				{
					if (tri_array[j] == tri_index)
					{
						itr_vt->second.erase(itr_vt->second.begin() + j);
						break;
					}
				}
			}

			// delete from edges
			// ab
			edge_key curr_edge = edge_key(tri.v1, tri.v2);
			edge_tri_map::iterator itr_et = m_edge_tri_map.find(curr_edge);
			if (itr_et != m_edge_tri_map.end())
			{
				vector<int> tri_array = itr_et->second;
				for (int j = 0; j < tri_array.size(); j++)
				{
					if (tri_array[j] == tri_index)
					{
						itr_et->second.erase(itr_et->second.begin() + j);
						if (itr_et->second.size() == 0)
						{
							m_edge_tri_map.erase(itr_et);
						}
						break;
					}
				}
			}
			// bc
			curr_edge = edge_key(tri.v2, tri.v3);
			itr_et = m_edge_tri_map.find(curr_edge);
			if (itr_et != m_edge_tri_map.end())
			{
				vector<int> tri_array = itr_et->second;
				for (int j = 0; j < tri_array.size(); j++)
				{
					if (tri_array[j] == tri_index)
					{
						itr_et->second.erase(itr_et->second.begin() + j);
						if (itr_et->second.size() == 0)
						{
							m_edge_tri_map.erase(itr_et);
						}
						break;
					}
				}
			}
			// ca
			curr_edge = edge_key(tri.v1, tri.v3);
			itr_et = m_edge_tri_map.find(curr_edge);
			if (itr_et != m_edge_tri_map.end())
			{
				vector<int> tri_array = itr_et->second;
				for (int j = 0; j < tri_array.size(); j++)
				{
					if (tri_array[j] == tri_index)
					{
						itr_et->second.erase(itr_et->second.begin() + j);
						if (itr_et->second.size() == 0)
						{
							m_edge_tri_map.erase(itr_et);
						}
						break;
					}
				}
			}
		}
	}

	void polygon_triangulation::re_triangle(vector<int> part)
	{
		point_list_d array_vertex;
		int ver_num = part.size();
		array_vertex.resize(ver_num);

		for (int i = 0; i < ver_num; i++)
		{
			array_vertex[i] = m_array_point[part[i]];
		}

		int tri_num = ver_num - 2;
		ushort* tri_index = new ushort[tri_num * 3];

		triangulation3<double> tri;
		assert(!array_vertex.empty());
		tri.triangulate_polygon_rh_v2(&(array_vertex[0]), ver_num, 0, &tri_index);

		vector<tri_key> new_tris;
		tri_key new_tri;
		int curr_tri_num = m_array_tri_index.size();
		for (int i = 0; i < tri_num; i++)
		{
			new_tri.v1 = part[tri_index[i * 3]];
			new_tri.v2 = part[tri_index[i * 3 + 1]];
			new_tri.v3 = part[tri_index[i * 3 + 2]];

			m_array_tri_index.push_back(new_tri);
			new_tris.push_back(new_tri);
		}

		add_tris_to_map(new_tris, curr_tri_num);
	}

	void polygon_triangulation::delete_invalid_tri()
	{
		if (m_array_tri_deleted.size() == 0)
		{
			return;
		}

		int tri_num = m_array_tri_index.size();
		vector<int>::iterator itr;
		for (int i = tri_num - 1; i >= 0; i--)
		{
			itr = std::find(m_array_tri_deleted.begin(), m_array_tri_deleted.end(), i);
			if (itr != m_array_tri_deleted.end())
			{
				m_array_tri_index.erase(m_array_tri_index.begin() + i);
			}
		}
		m_array_tri_deleted.clear();
	}

	bool polygon_triangulation::is_polygon_ccw(coordinates_array in_poly)
	{
		int pnt_num = in_poly.size();

		int max_ni = 0;
		double max_z = -math_d::s_max_real;
		std::vector<vector_2d> tri_array;
		for (int ni = 0; ni < pnt_num; ni++)
		{
			tri_array.push_back(vector_2d(in_poly[ni].x, in_poly[ni].z));
			if (in_poly[ni].z > max_z)
			{
				max_ni = ni;
				max_z = in_poly[ni].z;
			}
		}

		vector_2d prev_pos, next_pos, curr_pos;
		curr_pos = tri_array[max_ni];
		prev_pos = (max_ni == 0) ? tri_array[pnt_num - 1] : tri_array[max_ni - 1];
		next_pos = (max_ni == pnt_num - 1) ? tri_array[0] : tri_array[max_ni + 1];

		prev_pos = prev_pos - curr_pos;
		prev_pos.normalize();
		prev_pos = prev_pos + curr_pos;
		next_pos = next_pos - curr_pos;
		next_pos.normalize();
		next_pos = next_pos + curr_pos;

		if (prev_pos.x < next_pos.x) { return true; }
		return false;
	}


	//// edge used ///////////////////////////////////////////////////////////
	bool edge_key::operator< (const edge_key& key) const
	{
		if (v[1] < key.v[1])
		{
			return true;
		}

		if (v[1] > key.v[1])
		{
			return false;
		}

		return v[0] < key.v[0];
	}

	bool edge_key::operator== (const edge_key& key) const
	{
		return (v[0] == key.v[0]) && (v[1] == key.v[1]);
	}

	bool edge_key::contian_vertex(int vertex)
	{
		if (v[0] == vertex || v[1] == vertex)
		{
			return true;
		}
		return false;
	}
	//// tri key used ////////////////////////////////////////////////////////
	bool tri_key::contain_vertex(int ver_index)
	{
		if (v1 == ver_index || v2 == ver_index || v3 == ver_index)
		{
			return true;
		}
		return false;
	}

	bool tri_key::contain_edge(edge_key edge)
	{
		if (contain_vertex(edge.v[0]) && contain_vertex(edge.v[1]))
		{
			return true;
		}
		return false;
	}

	int tri_key::get_opposite_vertex(edge_key edge)
	{
		if (contain_edge(edge) == false)
		{
			return -1;
		}
		if (edge.contian_vertex(v1) == false)
		{
			return v1;
		}
		if (edge.contian_vertex(v2) == false)
		{
			return v2;
		}
		if (edge.contian_vertex(v3) == false)
		{
			return v3;
		}
		return -1;
	}

	edge_key tri_key::get_opposite_edge(int vertex)
	{
		if (vertex == v1)
		{
			return edge_key(v2, v3);
		}
		if (vertex == v2)
		{
			return edge_key(v1, v3);
		}
		if (vertex == v3)
		{
			return edge_key(v1, v2);
		}
		return edge_key(-1, -1);
	}

	bool tri_key::operator== (const tri_key& key) const
	{
		return (v1 == key.v1) && (v2 == key.v2) && (v3 == key.v3);
	}



}