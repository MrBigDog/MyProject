///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_shadow_factory.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_shadow_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_geometry_shadow_factory.h"
#include <usGfx/us_geometry_shadow_mesh.h>
#include <usUtil/us_spherical_coords.h>
#include <usUtil/us_vector2.h>
#include <usUtil/us_shape_creator.h>
#include <usUtil/us_triangulation.h>

namespace uniscope_globe
{
	//////////////////////////////////////////////////////////////////////////
	// common
	//////////////////////////////////////////////////////////////////////////
	double geometry_shadow_factory::compute_scaled_length(vector3<double> pos1, vector3<double> pos2, double v_height)
	{
		vector3<double> pos_normal1 = vector3<double>::normalize(pos1);
		vector3<double> pos_normal2 = vector3<double>::normalize(pos2);

		double half_angle = math<double>::acos_(vector3<double>::dot(pos_normal1, pos_normal2)) * 0.5;

		return (spherical_coords::s_semi_major + v_height) / math<double>::cos_(half_angle);
	}

	vector_3f geometry_shadow_factory::compute_normal(vector_3f vec1, vector_3f vec2, vector_3f vec3)
	{
		vector3<float> tmp1 = vec2 - vec1;
		vector3<float> tmp2 = vec3 - vec1;
		tmp1.normalize();
		tmp2.normalize();
		vector3<float> normal = vector3<float>::cross(tmp1, tmp2);
		normal.normalize();

		return normal;
	}

	void geometry_shadow_factory::generate_polygon_mesh(vector_3d* in_pos_array, ulong v_size, double v_height,
		std::vector<vector_3f> &out_points,
		std::vector<usx_tri_face_index>& out_index, axis_aligned_box_d& out_box)
	{
		// 三角化
		int max_ni = 0;
		double max_z = -math_d::s_max_real;
		std::vector<vector_2d> tri_array;
		for (int ni = 0; ni < (int)v_size; ni++)
		{
			tri_array.push_back(vector_2d(in_pos_array[ni].x, in_pos_array[ni].z));
			if (in_pos_array[ni].z > max_z)
			{
				max_ni = ni;
				max_z = in_pos_array[ni].z;
			}
		}

		int in_triangle_start_mode = 0;
		vector_2d prev_pos, next_pos, curr_pos;
		curr_pos = tri_array[max_ni];
		if (max_ni == 0 || max_ni == v_size - 1)
		{
			prev_pos = tri_array[v_size - 2];
			next_pos = tri_array[1];
		}
		else
		{
			prev_pos = tri_array[max_ni - 1];
			next_pos = tri_array[max_ni + 1];
		}
		prev_pos = prev_pos - curr_pos;
		prev_pos.normalize();
		prev_pos = prev_pos + curr_pos;
		next_pos = next_pos - curr_pos;
		next_pos.normalize();
		next_pos = next_pos + curr_pos;

		if (prev_pos.x < next_pos.x)
		{
			in_triangle_start_mode = 1;
		}
		else
		{
			in_triangle_start_mode = 0;
		}

		// 三角化
		triangulation<double> v_triangulation_algorithm;
		std::vector<ushort> v_temp_index_array;
		v_temp_index_array.resize((v_size - 2) * 3);
		v_triangulation_algorithm.triangulation_polygon(tri_array.begin()._Ptr, v_size, v_temp_index_array.begin()._Ptr);

		//
		// 组织整个多面体
		//
		// 上表面
		for (int i = 0; i < v_temp_index_array.size(); i += 3)
		{
			if (in_triangle_start_mode == 0)
			{
				// CW
				out_index.push_back(usx_tri_face_index(v_temp_index_array[i], v_temp_index_array[i + 1], v_temp_index_array[i + 2]));
			}
			else if (in_triangle_start_mode == 1)
			{
				// CCW
				out_index.push_back(usx_tri_face_index(v_temp_index_array[i], v_temp_index_array[i + 2], v_temp_index_array[i + 1]));
			}
		}

		// 下表面
		for (int i = 0; i < v_temp_index_array.size(); i += 3)
		{
			if (in_triangle_start_mode == 0)
			{
				// CW
				out_index.push_back(usx_tri_face_index(v_temp_index_array[i] + v_size, v_temp_index_array[i + 2] + v_size, v_temp_index_array[i + 1] + v_size));
			}
			else if (in_triangle_start_mode == 1)
			{
				// CCW
				out_index.push_back(usx_tri_face_index(v_temp_index_array[i] + v_size, v_temp_index_array[i + 1] + v_size, v_temp_index_array[i + 2] + v_size));
			}
		}

		// 侧面
		for (int i = 0; i < (int)v_size; i++)
		{
			if (in_triangle_start_mode == 0)
			{
				// CW
				out_index.push_back(usx_tri_face_index(i, i + v_size, (i + 1) % v_size + v_size));
				out_index.push_back(usx_tri_face_index(i, (i + 1) % v_size + v_size, (i + 1) % v_size));
			}
			else if (in_triangle_start_mode == 1)
			{
				// CCW	
				out_index.push_back(usx_tri_face_index(i, (i + 1) % v_size + v_size, i + v_size));
				out_index.push_back(usx_tri_face_index(i, (i + 1) % v_size, (i + 1) % v_size + v_size));
			}
		}

		// 生成顶点集合
		out_points.resize(v_size * 2);
		for (int i = 0; i < (int)v_size; i++)
		{
			out_points[i].x = in_pos_array[i].x;
			out_points[i].y = in_pos_array[i].y;
			out_points[i].z = in_pos_array[i].z;

			out_box.combine(in_pos_array[i]);
		}

		vector_3d vec;
		int v_crsr = 0;
		for (int i = 0; i < (int)v_size; i++)
		{
			v_crsr = i + v_size;
			vec = in_pos_array[i];
			vec.y += v_height;

			out_points[v_crsr].x = vec.x;
			out_points[v_crsr].y = vec.y;
			out_points[v_crsr].z = vec.z;

			out_box.combine(vec);
		}
	}

	void geometry_shadow_factory::insert_triangle(ulong o1, ulong o2, ulong n1, ulong n2,
		edge_map& in_edge_map, std::vector<usx_tri_face_index>& in_index_array)
	{
		edge_map::iterator itr = in_edge_map.find(make_pair(o2, o1));
		if (itr != in_edge_map.end())
		{
			degenerate_quad& quad = itr->second;

			usx_tri_face_index v_new_face;

			v_new_face.a = n1;
			v_new_face.b = quad.m_v2;
			v_new_face.c = quad.m_v1;
			in_index_array.push_back(v_new_face);

			v_new_face.a = n1;
			v_new_face.b = quad.m_v1;
			v_new_face.c = n2;
			in_index_array.push_back(v_new_face);

			in_edge_map.erase(itr);
		}
		else
		{
			degenerate_quad quad;
			quad.m_v1 = n1;
			quad.m_v2 = n2;
			in_edge_map.insert(make_pair(make_pair(o1, o2), quad));
		}
	}

	void geometry_shadow_factory::generate_shadow_mesh(vector_3f* v_vertex, int vertex_num,
		usx_tri_face_index* v_indices, int face_num,
		std::vector<position_normal_color>& new_vertex,
		std::vector<usx_tri_face_index>& new_indices,
		ulong v_fill_color)
	{
		//
		// 添加退化四边形
		//
		edge_map v_edge_map;
		for (int i = 0; i < face_num; i++)
		{
			vector_3f vec_a = v_vertex[v_indices[i].a];
			vector_3f vec_b = v_vertex[v_indices[i].b];
			vector_3f vec_c = v_vertex[v_indices[i].c];

			// 计算法矢
			vector3<float> v_normal = compute_normal(vec_a, vec_b, vec_c);

			// 增加顶点
			position_normal_color v1, v2, v3;
			v1.x = vec_a.x; v1.y = vec_a.y; v1.z = vec_a.z;
			v1.nx = v_normal.x; v1.ny = v_normal.y; v1.nz = v_normal.z;
			v1.color = v_fill_color;

			v2.x = vec_b.x; v2.y = vec_b.y; v2.z = vec_b.z;
			v2.nx = v_normal.x; v2.ny = v_normal.y; v2.nz = v_normal.z;
			v2.color = v_fill_color;

			v3.x = vec_c.x; v3.y = vec_c.y; v3.z = vec_c.z;
			v3.nx = v_normal.x; v3.ny = v_normal.y; v3.nz = v_normal.z;
			v3.color = v_fill_color;

			// 增加三角形
			usx_tri_face_index v_new_face;
			v_new_face.a = (int)new_vertex.size();
			v_new_face.b = v_new_face.a + 1;
			v_new_face.c = v_new_face.b + 1;
			new_indices.push_back(v_new_face);

			new_vertex.push_back(v1);
			new_vertex.push_back(v2);
			new_vertex.push_back(v3);

			insert_triangle(v_indices[i].a, v_indices[i].b, v_new_face.a, v_new_face.b, v_edge_map, new_indices);
			insert_triangle(v_indices[i].b, v_indices[i].c, v_new_face.b, v_new_face.c, v_edge_map, new_indices);
			insert_triangle(v_indices[i].c, v_indices[i].a, v_new_face.c, v_new_face.a, v_edge_map, new_indices);
		}
	}

	///////////////////////////////////////////////////////////////////////////
	// geometry_shadow_mesh
	//////////////////////////////////////////////////////////////////////////
	void geometry_shadow_factory::collapse_geometry_mesh(geometry_shadow_mesh* v_mesh, matrix_4f v_mat)
	{
		vector_3f vec_tmp;

		for (int i = 0; i < (int)v_mesh->m_frame_vertex_array.size(); i++)
		{
			vec_tmp.x = v_mesh->m_frame_vertex_array[i].x;
			vec_tmp.y = v_mesh->m_frame_vertex_array[i].y;
			vec_tmp.z = v_mesh->m_frame_vertex_array[i].z;
			vec_tmp = vec_tmp * v_mat;

			v_mesh->m_frame_vertex_array[i].x = vec_tmp.x;
			v_mesh->m_frame_vertex_array[i].y = vec_tmp.y;
			v_mesh->m_frame_vertex_array[i].z = vec_tmp.z;
		}

		for (int i = 0; i < (int)v_mesh->m_vertex_array.size(); i++)
		{
			vec_tmp.x = v_mesh->m_vertex_array[i].x;
			vec_tmp.y = v_mesh->m_vertex_array[i].y;
			vec_tmp.z = v_mesh->m_vertex_array[i].z;
			vec_tmp = vec_tmp * v_mat;

			v_mesh->m_vertex_array[i].x = vec_tmp.x;
			v_mesh->m_vertex_array[i].y = vec_tmp.y;
			v_mesh->m_vertex_array[i].z = vec_tmp.z;
		}
	}

	matrix_4d geometry_shadow_factory::collapse_geometry_mesh(geometry_shadow_mesh* v_mesh, vector_3d& vec_cen)
	{
		for (int i = 0; i < (int)v_mesh->m_frame_vertex_array.size(); i++)
		{
			v_mesh->m_frame_vertex_array[i].x -= vec_cen.x;
			v_mesh->m_frame_vertex_array[i].y -= vec_cen.y;
			v_mesh->m_frame_vertex_array[i].z -= vec_cen.z;
		}

		for (int i = 0; i < (int)v_mesh->m_vertex_array.size(); i++)
		{
			v_mesh->m_vertex_array[i].x -= vec_cen.x;
			v_mesh->m_vertex_array[i].y -= vec_cen.y;
			v_mesh->m_vertex_array[i].z -= vec_cen.z;
		}

		return matrix_4d::translation(vec_cen.x, vec_cen.y, vec_cen.z);
	}

	void geometry_shadow_factory::create_mesh_from_lon_lat_box(geometry_shadow_mesh* geo_mesh, lon_lat_box& v_box, ulong line_color, ulong fill_color)
	{
		geo_mesh->clear();

		geo_mesh->m_vertex_array.resize(8);
		geo_mesh->m_index_array.resize(36);

		geo_mesh->m_render_aabb = axis_aligned_box<double>(v_box.corner_value, 8);
		geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		std::vector<vector_3f> vertex_array;
		vertex_array.resize(8);
		for (int i = 0; i < 8; i++)
		{
			vertex_array[i].x = v_box.corner_value[i].x - geo_mesh->m_center.x;
			vertex_array[i].y = v_box.corner_value[i].y - geo_mesh->m_center.y;
			vertex_array[i].z = v_box.corner_value[i].z - geo_mesh->m_center.z;
		}

		ulong index_array[36] =
		{
			0,1,3,
			1,2,3,
			3,2,7,
			2,4,7,
			7,4,6,
			4,5,6,
			6,5,0,
			5,1,0,
			1,5,2,
			5,4,2,
			0,3,6,
			3,7,6
		};

		std::vector<usx_tri_face_index> tri_face_array;
		generate_shadow_mesh(vertex_array.begin()._Ptr, vertex_array.size(), (usx_tri_face_index*)index_array, 12,
			geo_mesh->m_vertex_array, tri_face_array, fill_color);

		geo_mesh->m_index_array.resize(tri_face_array.size() * 3);
		memcpy(geo_mesh->m_index_array.begin()._Ptr, tri_face_array.begin()._Ptr, sizeof(ulong) * geo_mesh->m_index_array.size());

		// 边框
		geo_mesh->m_frame_vertex_array.resize(vertex_array.size());
		for (int i = 0; i < (int)geo_mesh->m_frame_vertex_array.size(); i++)
		{
			geo_mesh->m_frame_vertex_array[i].x = vertex_array[i].x;
			geo_mesh->m_frame_vertex_array[i].y = vertex_array[i].y;
			geo_mesh->m_frame_vertex_array[i].z = vertex_array[i].z;
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}

		ushort frame_index_array[24] =
		{
			0,1, 1,2, 2,3, 3,0,
			4,5, 5,6, 6,7, 7,4,
			0,6, 3,7, 1,5, 2,4
		};
		geo_mesh->m_frame_index_array.resize(24);
		memcpy(geo_mesh->m_frame_index_array.begin()._Ptr, frame_index_array, sizeof(ushort) * 24);

		matrix_4d v_collapse_mat = matrix_4d::translation(geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z);
		geo_mesh->set_collapse(v_collapse_mat);
	}

	// create from box
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
	void geometry_shadow_factory::create_mesh_from_box(geometry_shadow_mesh* geo_mesh, double x_len, double y_len, double z_len, ulong line_color, ulong fill_color)
	{
		geo_mesh->clear();

		std::vector<vector_3f> v_vertex_array;
		v_vertex_array.resize(8);

		std::vector<ulong> v_index_array;
		v_index_array.resize(36);

		double half_x = x_len * 0.5;
		double half_z = z_len * 0.5;
		geo_mesh->m_render_aabb.set_extent(vector_3d(-half_x, 0.0, -half_z), vector_3d(half_x, y_len, half_z));

		v_vertex_array[0].x = -half_x;
		v_vertex_array[0].y = 0.0;
		v_vertex_array[0].z = -half_z;

		v_vertex_array[1].x = -half_x;
		v_vertex_array[1].y = y_len;
		v_vertex_array[1].z = -half_z;

		v_vertex_array[2].x = half_x;
		v_vertex_array[2].y = y_len;
		v_vertex_array[2].z = -half_z;

		v_vertex_array[3].x = half_x;
		v_vertex_array[3].y = 0.0;
		v_vertex_array[3].z = -half_z;

		v_vertex_array[4].x = half_x;
		v_vertex_array[4].y = y_len;
		v_vertex_array[4].z = half_z;

		v_vertex_array[5].x = -half_x;
		v_vertex_array[5].y = y_len;
		v_vertex_array[5].z = half_z;

		v_vertex_array[6].x = -half_x;
		v_vertex_array[6].y = 0.0;
		v_vertex_array[6].z = half_z;

		v_vertex_array[7].x = half_x;
		v_vertex_array[7].y = 0.0;
		v_vertex_array[7].z = half_z;

		ulong index_array[36] =
		{
			0,1,3,
			1,2,3,
			3,2,7,
			2,4,7,
			7,4,6,
			4,5,6,
			6,5,0,
			5,1,0,
			1,5,2,
			5,4,2,
			0,3,6,
			3,7,6
		};

		std::vector<usx_tri_face_index> tri_face_array;
		generate_shadow_mesh(v_vertex_array.begin()._Ptr, v_vertex_array.size(), (usx_tri_face_index*)index_array, 12,
			geo_mesh->m_vertex_array, tri_face_array, fill_color);

		geo_mesh->m_index_array.resize(tri_face_array.size() * 3);
		memcpy(geo_mesh->m_index_array.begin()._Ptr, tri_face_array.begin()._Ptr, sizeof(ulong) * geo_mesh->m_index_array.size());

		// 边框
		geo_mesh->m_frame_vertex_array.resize(v_vertex_array.size());
		for (int i = 0; i < (int)geo_mesh->m_frame_vertex_array.size(); i++)
		{
			geo_mesh->m_frame_vertex_array[i].x = v_vertex_array[i].x;
			geo_mesh->m_frame_vertex_array[i].y = v_vertex_array[i].y;
			geo_mesh->m_frame_vertex_array[i].z = v_vertex_array[i].z;
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}


		ushort frame_index_array[24] =
		{
			0,1, 1,2, 2,3, 3,0,
			4,5, 5,6, 6,7, 7,4,
			0,6, 3,7, 1,5, 2,4
		};
		geo_mesh->m_frame_index_array.resize(24);
		memcpy(geo_mesh->m_frame_index_array.begin()._Ptr, frame_index_array, sizeof(ushort) * 24);
	}

	void geometry_shadow_factory::create_mesh_from_volume(geometry_shadow_mesh* geo_mesh, vector_3d* in_pos_array, ulong v_size, double v_height, ulong line_color, ulong fill_color)
	{
		geo_mesh->clear();
		geo_mesh->m_fvf = position_normal_color::fvf;

		std::vector<vector_3f> vertex_array;
		std::vector<usx_tri_face_index> index_array;
		generate_polygon_mesh(in_pos_array, v_size, v_height, vertex_array, index_array, geo_mesh->m_render_aabb);

		// 边框
		geo_mesh->m_frame_vertex_array.resize(vertex_array.size());
		for (int i = 0; i < geo_mesh->m_frame_vertex_array.size(); i++)
		{
			geo_mesh->m_frame_vertex_array[i].x = vertex_array[i].x;
			geo_mesh->m_frame_vertex_array[i].y = vertex_array[i].y;
			geo_mesh->m_frame_vertex_array[i].z = vertex_array[i].z;
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}

		ulong v_line_num = v_size * 3;
		geo_mesh->m_frame_index_array.resize(v_line_num * 2);
		ushort* p_index = geo_mesh->m_frame_index_array.begin()._Ptr;

		ulong v_off = 0;
		for (int i = 0; i < int(v_size - 1); i++)
		{
			v_off = i << 1;
			p_index[v_off] = i;
			p_index[v_off + 1] = i + 1;

			v_off = (i + v_size) << 1;
			p_index[v_off] = v_size + i;
			p_index[v_off + 1] = v_size + i + 1;

			v_off = (i + 2 * v_size) << 1;
			p_index[v_off] = i;
			p_index[v_off + 1] = v_size + i;
		}
		v_off = (v_size - 1) << 1;
		p_index[v_off] = v_size - 1;
		p_index[v_off + 1] = 0;

		v_off = (2 * v_size - 1) << 1;
		p_index[v_off] = v_size * 2 - 1;
		p_index[v_off + 1] = v_size;

		v_off = (3 * v_size - 1) << 1;
		p_index[v_off] = v_size * 2 - 1;
		p_index[v_off + 1] = v_size - 1;

		std::vector<usx_tri_face_index> tri_face_array;
		generate_shadow_mesh(vertex_array.begin()._Ptr, vertex_array.size(), index_array.begin()._Ptr, index_array.size(),
			geo_mesh->m_vertex_array, tri_face_array, fill_color);

		geo_mesh->m_index_array.resize(tri_face_array.size() * 3);
		memcpy(geo_mesh->m_index_array.begin()._Ptr, tri_face_array.begin()._Ptr, sizeof(ulong) * geo_mesh->m_index_array.size());
	}

	void geometry_shadow_factory::create_mesh_from_cone(geometry_shadow_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color)
	{
		vector_3d v_point_center(0.0, 0.0, 0.0);
		vector_3d v_point_top(0.0, in_height, 0.0);

		// 构建顶点
		vector_3d* v_circle_points = new vector_3d[in_sides];
		shape_creator::create_circle_on_plane(v_circle_points, v_point_center, in_radius, in_sides);

		geo_mesh->m_render_aabb.combine(v_point_top);
		for (int i = 0; i < (int)in_sides; i++)
		{
			geo_mesh->m_render_aabb.combine(v_circle_points[i]);
		}

		std::vector<vector_3f> vertex_array;
		vertex_array.resize(in_sides + 2);
		for (int i = 0; i < (int)in_sides; i++)
		{
			vertex_array[i].x = v_circle_points[i].x;
			vertex_array[i].y = v_circle_points[i].y;
			vertex_array[i].z = v_circle_points[i].z;
		}

		vertex_array[in_sides].x = v_point_center.x;
		vertex_array[in_sides].y = v_point_center.y;
		vertex_array[in_sides].z = v_point_center.z;

		vertex_array[in_sides + 1].x = v_point_top.x;
		vertex_array[in_sides + 1].y = v_point_top.y;
		vertex_array[in_sides + 1].z = v_point_top.z;

		// 构建底面，同时构建侧面
		int in_triangle_start_mode = 0;
		// 0: 右手，CCW. 1: 左手，CW
		//
		//                    |x1     x2     x3|   
		//	S(P1,P2,P3)   =   |y1     y2     y3|   =   (x1-x3)*(y2-y3)   -   (y1-y3)(x2-x3)   
		//	                  |1      1      1 |   
		//
		double v_area = (v_point_center.x - v_circle_points[1].x) * (v_circle_points[0].y - v_circle_points[1].y) - (v_point_center.y - v_circle_points[1].y) * (v_circle_points[0].x - v_circle_points[1].x);
		if (v_area > 0)
		{
			in_triangle_start_mode = 0;
		}
		else
		{
			in_triangle_start_mode = 1;
		}

		std::vector<usx_tri_face_index> index_array;
		for (int i = 0; i < (int)in_sides; i++)
		{
			if (in_triangle_start_mode == 0) // CCW，右手
			{
				// 底面
				index_array.push_back(usx_tri_face_index(in_sides, (i == (in_sides - 1)) ? 0 : (i + 1), i));

				// 锥面
				index_array.push_back(usx_tri_face_index(in_sides + 1, i, (i == (in_sides - 1)) ? 0 : (i + 1)));
			}
			else // CW，左手
			{
				// 底面
				index_array.push_back(usx_tri_face_index(in_sides, i, (i == (in_sides - 1)) ? 0 : (i + 1)));

				// 锥面
				index_array.push_back(usx_tri_face_index(in_sides + 1, (i == (in_sides - 1)) ? 0 : (i + 1), i));
			}
		}

		// 构建shadow
		std::vector<usx_tri_face_index> tri_face_array;
		generate_shadow_mesh(vertex_array.begin()._Ptr, vertex_array.size(), index_array.begin()._Ptr, index_array.size(),
			geo_mesh->m_vertex_array, tri_face_array, fill_color);

		geo_mesh->m_index_array.resize(tri_face_array.size() * 3);
		memcpy(geo_mesh->m_index_array.begin()._Ptr, tri_face_array.begin()._Ptr, sizeof(ulong) * geo_mesh->m_index_array.size());

		// 构建边框
		geo_mesh->m_frame_vertex_array.resize(vertex_array.size());
		for (int i = 0; i < (int)vertex_array.size(); i++)
		{
			geo_mesh->m_frame_vertex_array[i].x = vertex_array[i].x;
			geo_mesh->m_frame_vertex_array[i].y = vertex_array[i].y;
			geo_mesh->m_frame_vertex_array[i].z = vertex_array[i].z;
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}

		ulong v_line_num = in_sides * 2;
		geo_mesh->m_frame_index_array.resize(v_line_num * 2);
		ushort* p_index = geo_mesh->m_frame_index_array.begin()._Ptr;

		ulong v_off = 0;
		for (int i = 0; i < int(in_sides - 1); i++)
		{
			v_off = i << 1;
			p_index[v_off] = i;
			p_index[v_off + 1] = i + 1;

			v_off = (i + in_sides) << 1;
			p_index[v_off] = in_sides + 1;
			p_index[v_off + 1] = i;
		}
		v_off = (in_sides - 1) << 1;
		p_index[in_sides - 1] = in_sides - 1;
		p_index[in_sides] = 0;

		v_off = (2 * in_sides - 1) << 1;
		p_index[v_off] = in_sides + 1;
		p_index[v_off + 1] = in_sides - 1;
	}

	void geometry_shadow_factory::create_mesh_from_cylinder(geometry_shadow_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color)
	{
		// create cylinder
		vector_3d vec_cen(0.0, 0.0, 0.0);
		vector_3d* v_circle = new vector_3d[in_sides + 1];
		shape_creator::create_circle_on_plane(v_circle, vec_cen, in_radius, in_sides);
		v_circle[in_sides] = v_circle[0];

		create_mesh_from_volume(geo_mesh, v_circle, in_sides, in_height, line_color, fill_color);

		AUTO_DELETE_ARRAY(v_circle);
	}

#define NORMALIZE_ANGLE( val ) { if(val > PI * 2) val -= PI *2; if(val < -PI * 2) val += PI *2; }
	void geometry_shadow_factory::create_mesh_from_sphere(geometry_shadow_mesh* geo_mesh, double in_radius, ulong line_color, ulong fill_color)
	{
		int	v_segment = (1 << 4);// + 1;
		double v_lon_scale_factor = 1.0 / v_segment;
		double v_lat_scale_factor = 1.0 / v_segment;

		double v_longitude = -PI;
		double v_latitude = -HALF_PI;
		double v_radius = in_radius;
		double v_delta_longitude = v_lon_scale_factor * PI;
		double v_delta_latitude = v_lat_scale_factor * PI;

		int v_segment_plus_1 = v_segment + 1;
		int v_double_segment_plus_1 = v_segment * 2 + 1;
		int v_double_segment = v_segment * 2;

		int v_vertex_count = v_segment_plus_1 * v_double_segment_plus_1;

		std::vector<vector_3f> vertex_array;
		vertex_array.resize(v_vertex_count);

		vector_3f vec_cen(0.0, v_radius, 0.0);

		vector_3d vec_min(-v_radius, 0.0, -v_radius);
		vector_3d vec_max(v_radius, 2 * v_radius, v_radius);
		geo_mesh->m_render_aabb.set_extent(vec_min, vec_max);

		vector_3f tmp_start;

		int v_index = 0;
		for (int j = 0; j < v_double_segment_plus_1; j++)
		{
			vertex_array[v_index] = vector_3f(0.0, 0.0, 0.0);
			v_index += 1;
		}
		v_latitude += v_delta_latitude;
		NORMALIZE_ANGLE(v_latitude);
		for (int i = 1; i < v_segment_plus_1 - 1; i++)
		{
			double v_longitude = 0;
			double tmp_radius = math_d::cos_(v_latitude) * v_radius;

			tmp_start.x = float(vec_cen.x + math_d::sin_(v_longitude) * tmp_radius);
			tmp_start.y = float(vec_cen.y + math_d::sin_(v_latitude) * v_radius);
			tmp_start.z = float(vec_cen.z + math_d::cos_(v_longitude) * tmp_radius);
			for (int j = 0; j < v_double_segment_plus_1 - 1; j++)
			{
				vertex_array[v_index].x = float(vec_cen.x + math_d::sin_(v_longitude) * tmp_radius);
				vertex_array[v_index].y = float(vec_cen.y + math_d::sin_(v_latitude) * v_radius);
				vertex_array[v_index].z = float(vec_cen.z + math_d::cos_(v_longitude) * tmp_radius);

				v_index += 1;
				v_longitude += v_delta_longitude;
				NORMALIZE_ANGLE(v_longitude);
			}
			vertex_array[v_index].x = tmp_start.x;
			vertex_array[v_index].y = tmp_start.y;
			vertex_array[v_index].z = tmp_start.z;
			v_index += 1;

			v_latitude += v_delta_latitude;
			NORMALIZE_ANGLE(v_latitude);
		}
		for (int j = 0; j < v_double_segment_plus_1; j++)
		{
			vertex_array[v_index] = vector_3f(0.0, 2 * v_radius, 0.0);
			v_index += 1;
		}

		std::vector<usx_tri_face_index> index_array;
		for (int ni = 0; ni < v_segment; ni++)
		{
			for (int j = 0; j < v_double_segment_plus_1; j++)
			{
				index_array.push_back(usx_tri_face_index((ni*v_double_segment_plus_1 + j), (ni*v_double_segment_plus_1 + j + 1), ((ni + 1)*v_double_segment_plus_1 + j)));
				index_array.push_back(usx_tri_face_index((ni*v_double_segment_plus_1 + j + 1), ((ni + 1)*v_double_segment_plus_1 + j + 1), ((ni + 1)*v_double_segment_plus_1 + j)));
			}
		}

		// 构建shadow
		std::vector<usx_tri_face_index> tri_face_array;
		generate_shadow_mesh(vertex_array.begin()._Ptr, vertex_array.size(), index_array.begin()._Ptr, index_array.size(),
			geo_mesh->m_vertex_array, tri_face_array, fill_color);

		geo_mesh->m_index_array.resize(tri_face_array.size() * 3);
		memcpy(geo_mesh->m_index_array.begin()._Ptr, tri_face_array.begin()._Ptr, sizeof(ulong) * geo_mesh->m_index_array.size());

		// 边框
		geo_mesh->m_frame_vertex_array.resize(vertex_array.size());
		geo_mesh->m_frame_index_array.resize(index_array.size() * 3);

		for (int i = 0; i < (int)geo_mesh->m_frame_vertex_array.size(); i++)
		{
			geo_mesh->m_frame_vertex_array[i].x = vertex_array[i].x;
			geo_mesh->m_frame_vertex_array[i].y = vertex_array[i].y;
			geo_mesh->m_frame_vertex_array[i].z = vertex_array[i].z;
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}

		v_latitude = -HALF_PI;
		int v_frame_index_cursor = 0;
		for (int i = 0; i < v_double_segment_plus_1; i += 4)
		{
			for (int j = 0; j < v_segment; j++)
			{
				geo_mesh->m_frame_index_array[v_frame_index_cursor + (j << 1)] = j * v_double_segment_plus_1 + i;
				geo_mesh->m_frame_index_array[v_frame_index_cursor + (j << 1) + 1] = (j + 1) * v_double_segment_plus_1 + i;
			}
			v_frame_index_cursor += v_segment_plus_1 << 1;
		}

		for (int i = 0; i < v_segment; i += 4)
		{
			for (int j = 0; j < v_double_segment_plus_1; j++)
			{
				geo_mesh->m_frame_index_array[v_frame_index_cursor + (j << 1)] = i * v_double_segment_plus_1 + j;
				geo_mesh->m_frame_index_array[v_frame_index_cursor + (j << 1) + 1] = i * v_double_segment_plus_1 + j + 1;
			}
			v_frame_index_cursor += v_double_segment_plus_1 << 1;
		}

		//vector_3d trans( 0.0, in_radius, 0.0 );
		//matrix_4d v_collapse_mat = matrix4<double>::translation( trans.x, trans.y, trans.z );
		//geo_mesh->set_collapse( v_collapse_mat );

	}
	//{
	//	int	v_segment = (1<<4);// + 1;
	//	double v_lon_scale_factor = 1.0 / v_segment;
	//	double v_lat_scale_factor = 1.0 / v_segment;

	//	double v_longitude	= - PI;
	//	double v_latitude	= - HALF_PI;
	//	double v_radius		= in_radius;
	//	double v_delta_longitude = v_lon_scale_factor * PI;
	//	double v_delta_latitude  = v_lat_scale_factor * PI;

	//	int v_segment_plus_1 = v_segment + 1;
	//	int v_double_segment_plus_1 = v_segment * 2 + 1;
	//	int v_double_segment = v_segment * 2;

	//	int v_vertex_count = v_segment_plus_1 * v_double_segment_plus_1 ;
	//	int v_index_count =  v_segment * v_double_segment * 2 * 3;

	//	std::vector<vector_3f> vertex_array;
	//	vertex_array.resize( v_vertex_count );
	//	
	//	vector_3f vec_cen( 0.0, 0.0, 0.0 );

	//	vector_3d vec_min( -v_radius,0.0,-v_radius );
	//	vector_3d vec_max( v_radius,2*v_radius,v_radius );
	//	geo_mesh->m_render_aabb.set_extent( vec_min, vec_max );

	//	int v_index = 0;
	//	for ( int i = 0; i < v_segment_plus_1; i++ )
	//	{
	//		double v_longitude = 0;
	//		double tmp_radius = math_d::cos_( v_latitude ) * v_radius;
	//		for ( int j = 0; j < v_double_segment_plus_1; j++ )
	//		{						
	//			vertex_array[v_index].x = float(vec_cen.x + math_d::sin_( v_longitude ) * tmp_radius);
	//			vertex_array[v_index].y = float(vec_cen.y + math_d::sin_( v_latitude ) * v_radius);
	//			vertex_array[v_index].z = float(vec_cen.z + math_d::cos_( v_longitude ) * tmp_radius);

	//			v_index += 1;
	//			v_longitude += v_delta_longitude;
	//			NORMALIZE_ANGLE(v_longitude);
	//		}
	//		v_latitude += v_delta_latitude;
	//		NORMALIZE_ANGLE(v_latitude);
	//	}

	//	std::vector<usx_tri_face_index> index_array;
	//	for ( int i = 0; i < v_segment; i++ )
	//	{
	//		for (int j = 0; j < v_double_segment; j++)
	//		{
	//			index_array.push_back( usx_tri_face_index( (i*v_double_segment_plus_1+j), (i*v_double_segment_plus_1+j+1), ((i+1)*v_double_segment_plus_1+j) ) );
	//			index_array.push_back( usx_tri_face_index( (i*v_double_segment_plus_1+j+1), ((i+1)*v_double_segment_plus_1+j+1), ((i+1)*v_double_segment_plus_1+j) ) );
	//		}
	//	}

	//	// 构建shadow
	//	std::vector<usx_tri_face_index> tri_face_array;
	//	generate_shadow_mesh( vertex_array.begin()._Ptr, vertex_array.size(), index_array.begin()._Ptr, index_array.size(), 
	//		geo_mesh->m_vertex_array, tri_face_array, fill_color );

	//	geo_mesh->m_index_array.resize( tri_face_array.size() * 3 );
	//	memcpy( geo_mesh->m_index_array.begin()._Ptr, tri_face_array.begin()._Ptr, sizeof(ulong) * geo_mesh->m_index_array.size() );

	//	// 边框
	//	geo_mesh->m_frame_vertex_array.resize( vertex_array.size() );
	//	geo_mesh->m_frame_index_array.resize( index_array.size() * 3 );

	//	for ( ulong i = 0; i < geo_mesh->m_frame_vertex_array.size(); i ++ )
	//	{
	//		geo_mesh->m_frame_vertex_array[i].x = vertex_array[i].x;
	//		geo_mesh->m_frame_vertex_array[i].y = vertex_array[i].y;
	//		geo_mesh->m_frame_vertex_array[i].z = vertex_array[i].z;
	//		geo_mesh->m_frame_vertex_array[i].color = line_color;
	//	}

	//	v_latitude = -HALF_PI;
	//	int v_frame_index_cursor = 0;
	//	for ( int i = 0; i < v_double_segment_plus_1; i+= 4 )
	//	{
	//		for ( int j = 0; j < v_segment; j++ )
	//		{
	//			geo_mesh->m_frame_index_array[v_frame_index_cursor+(j<<1)]   = j * v_double_segment_plus_1 + i;
	//			geo_mesh->m_frame_index_array[v_frame_index_cursor+(j<<1)+1] = ( j + 1 ) * v_double_segment_plus_1 + i;
	//		}
	//		v_frame_index_cursor += v_segment_plus_1<<1;
	//	}

	//	for ( int i = 0; i < v_segment; i+= 4 )
	//	{
	//		for ( int j = 0; j < v_double_segment_plus_1; j++ )
	//		{
	//			geo_mesh->m_frame_index_array[v_frame_index_cursor+(j<<1)]   = i * v_double_segment_plus_1 + j;
	//			geo_mesh->m_frame_index_array[v_frame_index_cursor+(j<<1)+1] = i * v_double_segment_plus_1 + j + 1;
	//		}
	//		v_frame_index_cursor += v_double_segment_plus_1<<1;
	//	}

	//	vector_3d trans( 0.0, in_radius, 0.0 );
	//	matrix_4d v_collapse_mat = matrix4<double>::translation( trans.x, trans.y, trans.z );
	//	geo_mesh->set_collapse( v_collapse_mat );

	//}


	void geometry_shadow_factory::reset_mesh_color(geometry_shadow_mesh* geo_mesh, ulong v_color, ulong v_line_color, ulong v_fill_color)
	{
		geo_mesh->set_shadow_color(v_color);

		for (int ni = 0; ni < (int)geo_mesh->m_vertex_array.size(); ni++)
		{
			geo_mesh->m_vertex_array[ni].color = v_fill_color;
		}
	}

}