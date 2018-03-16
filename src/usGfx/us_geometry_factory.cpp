///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_factory.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_geometry_factory.h"
#include <usGfx/us_geometry_mesh.h>
#include <usUtil/us_spherical_coords.h>

namespace uniscope_globe
{

	//////////////////////////////////////////////////////////////////////////
	// common
	//////////////////////////////////////////////////////////////////////////
	double geometry_factory::compute_scaled_length(vector3<double> pos1, vector3<double> pos2, double v_height)
	{
		vector3<double> pos_normal1 = vector3<double>::normalize(pos1);
		vector3<double> pos_normal2 = vector3<double>::normalize(pos2);

		double half_angle = math<double>::acos_(vector3<double>::dot(pos_normal1, pos_normal2)) * 0.5;

		return (spherical_coords::s_semi_major + v_height) / math<double>::cos_(half_angle);
	}

	///////////////////////////////////////////////////////////////////////////
	// geometry_mesh
	//////////////////////////////////////////////////////////////////////////
	void geometry_factory::collapse_geometry_mesh(geometry_mesh* v_mesh, matrix_4f v_mat)
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

	matrix_4d geometry_factory::collapse_geometry_mesh(geometry_mesh* v_mesh, vector_3d& vec_cen)
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

	void geometry_factory::generate_line_mesh(geometry_mesh* geo_mesh,
		vector_3d* v_src,
		coordinates_array& out_array,
		ulong v_size,
		double v_width,
		ulong v_color,
		double v_height,
		double v_lower_height)
	{
		geo_mesh->set_shadow_color(v_color);

		coordinates_array v_vertex_array;
		coordinates_array v_upper_coords_array;
		coordinates_array v_lower_coords_array;

		// 两个点的情况
		if (v_size == 2)
		{
			vector3<double> pos_geo1 = v_src[0];
			vector3<double> pos_geo2 = v_src[1];
			vector3<double> direction = vector3<double>::cross(pos_geo1, pos_geo2);
			direction.normalize();

			// 第一个点
			vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * v_height;

			v_upper_coords_array.push_back(pos_geo1_scaled - direction * v_width);
			v_upper_coords_array.push_back(pos_geo1_scaled + direction * v_width);

			pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * v_lower_height;

			v_lower_coords_array.push_back(pos_geo1_scaled - direction * v_width);
			v_lower_coords_array.push_back(pos_geo1_scaled + direction * v_width);

			// 第二个点
			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_height;

			v_upper_coords_array.push_back(pos_geo2_scaled - direction * v_width);
			v_upper_coords_array.push_back(pos_geo2_scaled + direction * v_width);

			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_lower_height;

			v_lower_coords_array.push_back(pos_geo2_scaled - direction * v_width);
			v_lower_coords_array.push_back(pos_geo2_scaled + direction * v_width);

			v_vertex_array.insert(v_vertex_array.end(), v_upper_coords_array.begin(), v_upper_coords_array.end());
			v_vertex_array.insert(v_vertex_array.end(), v_lower_coords_array.begin(), v_lower_coords_array.end());

			insert_vertex_to_line_mesh(geo_mesh, v_size, v_vertex_array, out_array, v_color);

			return;
		}


		// 三个点以上的情况

		// 第一个点
		{
			vector3<double> pos_geo1 = v_src[0];
			vector3<double> pos_geo2 = v_src[1];
			vector3<double> direction = vector3<double>::cross(pos_geo1, pos_geo2);
			direction.normalize();

			vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * v_height;

			v_upper_coords_array.push_back(pos_geo1_scaled - direction * v_width);
			v_upper_coords_array.push_back(pos_geo1_scaled + direction * v_width);

			pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * v_lower_height;

			v_lower_coords_array.push_back(pos_geo1_scaled - direction * v_width);
			v_lower_coords_array.push_back(pos_geo1_scaled + direction * v_width);
		}

		for (int i = 1; i < int(v_size - 1); i++)
		{
			vector3<double> pos_geo1 = v_src[i - 1];
			vector3<double> pos_geo2 = v_src[i];
			vector3<double> pos_geo3 = v_src[i + 1];

			// direction
			vector3<double> direction1 = vector3<double>::cross(pos_geo1, pos_geo2);
			direction1.normalize();

			vector3<double> direction2 = vector3<double>::cross(pos_geo2, pos_geo3);
			direction2.normalize();

			vector3<double> direction = (direction1 + direction2) * 0.5;
			direction.normalize();

			vector3<double> v_temp_normal = vector3<double>::normalize(pos_geo1 - pos_geo2);
			double dot_val = vector3<double>::dot(v_temp_normal, direction);
			if (dot_val < 0.0)
			{
				dot_val = vector3<double>::dot(v_temp_normal, -direction);
			}

			double width;
			if (math<double>::acos_(dot_val) < 1 * math_d::s_deg_to_rad)
			{
				width = v_width;
			}
			else
			{
				width = v_width / math<double>::sin_(math<double>::acos_(dot_val));
			}

			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_height;
			v_upper_coords_array.push_back(pos_geo2_scaled - direction * width);
			v_upper_coords_array.push_back(pos_geo2_scaled + direction * width);

			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_lower_height;

			v_lower_coords_array.push_back(pos_geo2_scaled - direction * width);
			v_lower_coords_array.push_back(pos_geo2_scaled + direction * width);
		}

		// 最后一个点
		{
			vector3<double> pos_geo1 = v_src[v_size - 2];
			vector3<double> pos_geo2 = v_src[v_size - 1];

			vector3<double> direction = vector3<double>::cross(pos_geo1, pos_geo2);
			direction.normalize();

			vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_height;

			v_upper_coords_array.push_back(pos_geo2_scaled - direction * v_width);
			v_upper_coords_array.push_back(pos_geo2_scaled + direction * v_width);

			pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_lower_height;

			v_lower_coords_array.push_back(pos_geo2_scaled - direction * v_width);
			v_lower_coords_array.push_back(pos_geo2_scaled + direction * v_width);
		}

		v_vertex_array.insert(v_vertex_array.end(), v_upper_coords_array.begin(), v_upper_coords_array.end());
		v_vertex_array.insert(v_vertex_array.end(), v_lower_coords_array.begin(), v_lower_coords_array.end());

		insert_vertex_to_line_mesh(geo_mesh, v_size, v_vertex_array, out_array, v_color);
	}

	void geometry_factory::insert_vertex_to_line_mesh(geometry_mesh* v_mesh, ulong size, coordinates_array& in_array, coordinates_array& out_array, ulong v_color)
	{
		ulong v_size = size << 1;
		ulong start_index = out_array.size();

		for (int i = start_index; i < (int)(start_index + v_size - 3); i += 2)
		{
			v_mesh->m_index_array.push_back(i);
			v_mesh->m_index_array.push_back(i + 3);
			v_mesh->m_index_array.push_back(i + 1);

			v_mesh->m_index_array.push_back(i);
			v_mesh->m_index_array.push_back(i + 2);
			v_mesh->m_index_array.push_back(i + 3);
		}

		for (int i = start_index; i < (int)(start_index + v_size - 3); i += 2)
		{
			v_mesh->m_index_array.push_back(i + v_size);
			v_mesh->m_index_array.push_back(i + 1 + v_size);
			v_mesh->m_index_array.push_back(i + 3 + v_size);

			v_mesh->m_index_array.push_back(i + v_size);
			v_mesh->m_index_array.push_back(i + 3 + v_size);
			v_mesh->m_index_array.push_back(i + 2 + v_size);
		}

		for (int i = start_index; i < (int)(start_index + v_size - 3); i += 2)
		{
			v_mesh->m_index_array.push_back(i);
			v_mesh->m_index_array.push_back(i + v_size);
			v_mesh->m_index_array.push_back(i + 2 + v_size);

			v_mesh->m_index_array.push_back(i);
			v_mesh->m_index_array.push_back(i + 2 + v_size);
			v_mesh->m_index_array.push_back(i + 2);

			v_mesh->m_index_array.push_back(i + 1);
			v_mesh->m_index_array.push_back(i + 3 + v_size);
			v_mesh->m_index_array.push_back(i + 1 + v_size);


			v_mesh->m_index_array.push_back(i + 1);
			v_mesh->m_index_array.push_back(i + 3);
			v_mesh->m_index_array.push_back(i + 3 + v_size);
		}

		// 前后两个面
		v_mesh->m_index_array.push_back(start_index);
		v_mesh->m_index_array.push_back(start_index + 1 + v_size);
		v_mesh->m_index_array.push_back(start_index + v_size);

		v_mesh->m_index_array.push_back(start_index);
		v_mesh->m_index_array.push_back(start_index + 1);
		v_mesh->m_index_array.push_back(start_index + 1 + v_size);

		v_mesh->m_index_array.push_back(start_index + v_size - 1);
		v_mesh->m_index_array.push_back(start_index + 2 * v_size - 2);
		v_mesh->m_index_array.push_back(start_index + 2 * v_size - 1);

		v_mesh->m_index_array.push_back(start_index + v_size - 1);
		v_mesh->m_index_array.push_back(start_index + v_size - 2);
		v_mesh->m_index_array.push_back(start_index + 2 * v_size - 2);

		for (int i = 0; i < (int)in_array.size(); i++)
		{
			v_mesh->m_render_aabb.combine(in_array[i]);
		}
		//v_mesh->m_center = v_mesh->m_render_aabb.center();

		//for ( int i = 0; i < (int)in_array.size(); i ++ )
		//{
		//	position_color pc;
		//	pc.x = in_array[i].x;
		//	pc.y = in_array[i].y;
		//	pc.z = in_array[i].z;
		//	pc.color = v_color;
		//	v_mesh->m_vertex_array.push_back( pc );
		//}

		out_array.insert(out_array.end(), in_array.begin(), in_array.end());
	}



	geometry_mesh* geometry_factory::create_mesh_from_multiple_line(vector3<double>* in_vertex_buffer, ulong in_vertex_size, ulong* in_index_buffer, ulong in_index_size, double in_width, ulong in_color, double in_height, double in_lower_height)
	{
		if (in_vertex_size <= 1 || in_index_size == 0)
			return NULL;

		geometry_mesh* geo_mesh = new geometry_mesh;

		coordinates_array v_array;

		ulong cur_index = 0;
		for (int i = 0; i < (int)in_index_size; i++)
		{
			if (in_index_buffer[i] <= 1)
			{
				cur_index += in_index_buffer[i];
				continue;
			}

			generate_line_mesh(geo_mesh, &in_vertex_buffer[cur_index], v_array, in_index_buffer[i], in_width, in_color, in_height, in_lower_height);

			cur_index += in_index_buffer[i];
		}

		geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		for (int i = 0; i < (int)v_array.size(); i++)
		{
			position_color pc;
			pc.x = v_array[i].x - geo_mesh->m_center.x;
			pc.y = v_array[i].y - geo_mesh->m_center.y;
			pc.z = v_array[i].z - geo_mesh->m_center.z;
			pc.color = in_color;
			geo_mesh->m_vertex_array.push_back(pc);
		}


		matrix_4d v_collapse_mat = matrix4<double>::translation(geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z);
		geo_mesh->set_collapse(v_collapse_mat);

		return geo_mesh;
	}

	void geometry_factory::create_mesh_from_multiple_line(geometry_mesh* geo_mesh, vector3<double>* in_vertex_buffer, ulong in_vertex_size, ulong* in_index_buffer, ulong in_index_size, double in_width, ulong in_color, double in_height, double in_lower_height)
	{
		if (in_vertex_size <= 1 || in_index_size == 0)
			return;

		geo_mesh->clear();

		coordinates_array v_array;

		ulong cur_index = 0;
		for (int i = 0; i < (int)in_index_size; i++)
		{
			if (in_index_buffer[i] <= 1)
			{
				cur_index += in_index_buffer[i];
				continue;
			}

			generate_line_mesh(geo_mesh, &in_vertex_buffer[cur_index], v_array, in_index_buffer[i], in_width, in_color, in_height, in_lower_height);

			cur_index += in_index_buffer[i];
		}

		geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		for (int i = 0; i < (int)v_array.size(); i++)
		{
			position_color pc;
			pc.x = v_array[i].x - geo_mesh->m_center.x;
			pc.y = v_array[i].y - geo_mesh->m_center.y;
			pc.z = v_array[i].z - geo_mesh->m_center.z;
			pc.color = in_color;
			geo_mesh->m_vertex_array.push_back(pc);
		}

		matrix_4d v_collapse_mat = matrix4<double>::translation(geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z);
		geo_mesh->set_collapse(v_collapse_mat);
	}

	void geometry_factory::create_mesh_from_lon_lat_box(geometry_mesh* geo_mesh, lon_lat_box& v_box, ulong line_color, ulong fill_color)
	{
		geo_mesh->clear();

		geo_mesh->m_vertex_array.resize(8);
		geo_mesh->m_index_array.resize(36);

		geo_mesh->m_render_aabb = axis_aligned_box<double>(v_box.corner_value, 8);
		geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		ushort index_array[36] =
		{
			0,1,3,1,2,3,
			3,2,7,2,4,7,
			7,4,6,4,5,6,
			6,5,0,5,1,0,
			1,5,2,5,4,2,
			0,3,6,3,7,6
		};
		memcpy(geo_mesh->m_index_array.begin()._Myptr, index_array, sizeof(ushort) * 36);

		for (int i = 0; i < 8; i++)
		{
			geo_mesh->m_vertex_array[i].x = v_box.corner_value[i].x - geo_mesh->m_center.x;
			geo_mesh->m_vertex_array[i].y = v_box.corner_value[i].y - geo_mesh->m_center.y;
			geo_mesh->m_vertex_array[i].z = v_box.corner_value[i].z - geo_mesh->m_center.z;
			geo_mesh->m_vertex_array[i].color = fill_color;
		}

		geo_mesh->m_frame_vertex_array.resize(geo_mesh->m_vertex_array.size());
		std::copy(geo_mesh->m_vertex_array.begin(), geo_mesh->m_vertex_array.end(), geo_mesh->m_frame_vertex_array.begin());
		for (int i = 0; i < (int)geo_mesh->m_frame_vertex_array.size(); i++)
		{
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}

		// 边框
		ushort frame_index_array[24] =
		{
			0,1, 1,2, 2,3, 3,0,
			4,5, 5,6, 6,7, 7,4,
			0,6, 3,7, 1,5, 2,4
		};
		geo_mesh->m_frame_index_array.resize(24);
		memcpy(geo_mesh->m_frame_index_array.begin()._Myptr, frame_index_array, sizeof(ushort) * 24);

		vector_3d vec_min = geo_mesh->m_render_aabb.get_min();
		vector_3d vec_max = geo_mesh->m_render_aabb.get_max();
		vector_3d vec_collapse = (vec_min + vec_max) * 0.5;

		matrix_4d v_collapse_mat = matrix_4d::translation(vec_collapse.x, vec_collapse.y, vec_collapse.z);
		geo_mesh->set_collapse(v_collapse_mat);
	}

	void geometry_factory::generate_polygon_mesh(geometry_mesh* geo_mesh, vector_3d* in_pos_array, coordinates_array& out_array, ulong v_size, ulong v_color, double in_height, double in_lower_height)
	{
		geo_mesh->set_shadow_color(v_color);

		// 计算平均法向量
		vector3<double> v_average_normal = vector3<double>::s_zero;
		for (int i = 0; i < (int)v_size; i++)
		{
			vector3<double> pos_geo = in_pos_array[i];
			pos_geo.normalize();
			v_average_normal += pos_geo;
		}
		v_average_normal /= v_size;
		v_average_normal.normalize();

		// 计算平均法向量上的最小投影点
		double lower_dist = spherical_coords::s_semi_major;
		for (int i = 0; i < (int)v_size; i++)
		{
			vector3<double> pos_geo = in_pos_array[i];

			double cur_dist = vector3<double>::dot(pos_geo, v_average_normal);
			lower_dist = min(lower_dist, cur_dist);
		}

		// 上投影面上的一个点
		vector3<double> v_upper_pos = in_height * v_average_normal;
		// 下投影面上的一个点
		vector3<double> v_lower_pos = in_lower_height * v_average_normal;

		// 将多边形投影到上下投影面上
		coordinates_array v_upper_array;
		coordinates_array v_lower_array;

		double upper_height = v_upper_pos.length();
		double lower_height = v_lower_pos.length();

		for (int i = 0; i < (int)v_size; i++)
		{
			vector3<double> pos_geo;
			pos_geo = in_pos_array[i];
			pos_geo.normalize();
			double v_length = upper_height / vector3<double>::dot(pos_geo, v_average_normal);
			pos_geo = pos_geo * v_length;
			v_upper_array.push_back(pos_geo);

			pos_geo = in_pos_array[i];
			pos_geo.normalize();
			v_length = lower_height / vector3<double>::dot(pos_geo, v_average_normal);
			pos_geo = pos_geo * v_length;
			v_lower_array.push_back(pos_geo);
		}

		// 三角化
		// 建立局部坐标系
		vector3<double> v_x_axis, v_y_axis, v_z_axis;
		v_x_axis = vector3<double>::normalize(v_upper_array[0] - v_upper_pos);
		v_y_axis = v_average_normal;
		v_z_axis = vector3<double>::cross(v_x_axis, v_y_axis);
		v_z_axis.normalize();

		matrix4<double> mat_tnb = matrix4<double>::from_tnbp(v_x_axis, v_y_axis, v_z_axis, v_upper_pos);

		std::vector< vector2<double> > v_upper_array_local;
		ulong max_ni = 0;
		double max_z = 0.0;
		std::vector< vector3<double> > v_upper_array_local_geo;
		for (int ni = 0; ni < (int)v_size; ni++)
		{
			vector3<double> pos_geo = v_upper_array[ni] * mat_tnb.inverse();

			if (math<double>::fabs_(pos_geo.x) < math<double>::s_zero_tolerance)
			{
				pos_geo.x = 0.0;
			}
			if (math<double>::fabs_(pos_geo.y) < math<double>::s_zero_tolerance)
			{
				pos_geo.y = 0.0;
			}
			if (math<double>::fabs_(pos_geo.z) < math<double>::s_zero_tolerance)
			{
				pos_geo.z = 0.0;
			}

			v_upper_array_local_geo.push_back(pos_geo);

			v_upper_array_local.push_back(vector2<double>(pos_geo.x, pos_geo.z));

			if (ni == 0)
			{
				max_ni = 0;
				max_z = pos_geo.z;
			}
			else
			{
				if (max_z < pos_geo.z)
				{
					max_z = pos_geo.z;
					max_ni = ni;
				}
			}
		}

		int in_triangle_start_mode = 0;
		vector3<double> prev_pos, next_pos, curr_pos;
		curr_pos = v_upper_array_local_geo[max_ni];
		for (int i = 0; i < (int)v_size; i++)
		{
			next_pos = v_upper_array_local_geo[(max_ni + 1) % v_size];
			if ((next_pos - curr_pos).length() > 0.000001)
				break;
		}
		for (int i = 0; i < (int)v_size; i++)
		{
			prev_pos = v_upper_array_local_geo[(max_ni + v_size - 1) % v_size];
			if ((prev_pos - curr_pos).length() > 0.000001)
				break;
		}

		// 		if (max_ni == 0 || max_ni == v_size - 1)
		// 		{
		// 			prev_pos = v_upper_array_local_geo[v_size - 2];
		// 			next_pos = v_upper_array_local_geo[1];
		// 		}
		// 		else
		// 		{
		// 			prev_pos = v_upper_array_local_geo[max_ni - 1];
		// 			next_pos = v_upper_array_local_geo[max_ni + 1];
		// 		}
		prev_pos = prev_pos - curr_pos;
		prev_pos.normalize();
		prev_pos = prev_pos + curr_pos;
		next_pos = next_pos - curr_pos;
		next_pos.normalize();
		next_pos = next_pos + curr_pos;

		if (prev_pos.x < next_pos.x)
		{
			in_triangle_start_mode = 0;
		}
		else
		{
			in_triangle_start_mode = 1;
		}

		// 三角化
		triangulation<double> v_triangulation_algorithm;
		std::vector<ushort> v_temp_index_array;
		v_temp_index_array.resize((v_size - 2) * 3);
		v_triangulation_algorithm.triangulation_polygon(v_upper_array_local.begin()._Myptr, v_size, v_temp_index_array.begin()._Myptr);

		//
		// 组织多面体索引
		//

		// 上表面
		ulong start_index = out_array.size();
		for (int i = 0; i < (int)v_temp_index_array.size(); i += 3)
		{
			if (in_triangle_start_mode == 0)
			{
				// CW
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i]);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 1]);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 2]);
			}
			else if (in_triangle_start_mode == 1)
			{
				// CCW
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i]);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 2]);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 1]);
			}
		}

		// 下表面
		for (int i = 0; i < (int)v_temp_index_array.size(); i += 3)
		{
			if (in_triangle_start_mode == 0)
			{
				// CW
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i] + v_size);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 2] + v_size);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 1] + v_size);
			}
			else if (in_triangle_start_mode == 1)
			{
				// CCW
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i] + v_size);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 1] + v_size);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 2] + v_size);
			}
		}

		// 侧面
		for (int i = 0; i < (int)v_size; i++)
		{
			if (in_triangle_start_mode == 0)
			{
				// CW
				geo_mesh->m_index_array.push_back(start_index + i);
				geo_mesh->m_index_array.push_back(start_index + i + v_size);
				geo_mesh->m_index_array.push_back(start_index + (i + 1) % v_size + v_size);

				geo_mesh->m_index_array.push_back(start_index + i);
				geo_mesh->m_index_array.push_back(start_index + (i + 1) % v_size + v_size);
				geo_mesh->m_index_array.push_back(start_index + (i + 1) % v_size);
			}
			else if (in_triangle_start_mode == 1)
			{
				// CCW
				geo_mesh->m_index_array.push_back(start_index + i);
				geo_mesh->m_index_array.push_back(start_index + (i + 1) % v_size + v_size);
				geo_mesh->m_index_array.push_back(start_index + i + v_size);

				geo_mesh->m_index_array.push_back(start_index + i);
				geo_mesh->m_index_array.push_back(start_index + (i + 1) % v_size);
				geo_mesh->m_index_array.push_back(start_index + (i + 1) % v_size + v_size);
			}
		}

		//
		// 生成顶点集合
		//
		for (int i = 0; i < (int)v_upper_array.size(); i++)
		{
			geo_mesh->m_render_aabb.combine(v_upper_array[i]);
			out_array.push_back(v_upper_array[i]);
		}

		for (int i = 0; i < (int)v_lower_array.size(); i++)
		{
			geo_mesh->m_render_aabb.combine(v_lower_array[i]);
			out_array.push_back(v_lower_array[i]);
		}

		//geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		//position_color pc;
		//for ( ulong i = 0 ; i < v_upper_array.size(); i++ )
		//{			
		//	pc.x = v_upper_array[i].x;
		//	pc.y = v_upper_array[i].y;
		//	pc.z = v_upper_array[i].z;
		//	pc.color = v_color;

		//	geo_mesh->m_vertex_array.push_back( pc );
		//}

		//for ( ulong i = 0 ; i < v_lower_array.size(); i++ )
		//{
		//	pc.x = v_lower_array[i].x;
		//	pc.y = v_lower_array[i].y;
		//	pc.z = v_lower_array[i].z;
		//	pc.color = v_color;

		//	geo_mesh->m_vertex_array.push_back( pc );
		//}
	}

	geometry_mesh* geometry_factory::create_mesh_from_polygon(vector_3d* v_src, int v_size, ulong in_color, double in_height, double in_lower_height)
	{
		geometry_mesh* geo_mesh = new geometry_mesh;

		coordinates_array v_array;
		generate_polygon_mesh(geo_mesh, v_src, v_array, v_size, in_color, in_height, in_lower_height);

		geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		for (int i = 0; i < (int)v_array.size(); i++)
		{
			position_color pc;
			pc.x = v_array[i].x - geo_mesh->m_center.x;
			pc.y = v_array[i].y - geo_mesh->m_center.y;
			pc.z = v_array[i].z - geo_mesh->m_center.z;
			pc.color = in_color;
			geo_mesh->m_vertex_array.push_back(pc);
		}

		matrix_4d v_collapse_mat = matrix4<double>::translation(geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z);
		geo_mesh->set_collapse(v_collapse_mat);

		return geo_mesh;
	}

	void geometry_factory::create_mesh_from_polygon(geometry_mesh* geo_mesh, vector_3d* v_src, int v_size, ulong in_color, double in_height, double in_lower_height)
	{
		if (v_size <= 1) return;

		geo_mesh->clear();

		coordinates_array v_array;

		generate_polygon_mesh(geo_mesh, v_src, v_array, v_size, in_color, in_height, in_lower_height);

		geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		for (int i = 0; i < (int)v_array.size(); i++)
		{
			position_color pc;
			pc.x = v_array[i].x - geo_mesh->m_center.x;
			pc.y = v_array[i].y - geo_mesh->m_center.y;
			pc.z = v_array[i].z - geo_mesh->m_center.z;
			pc.color = in_color;
			geo_mesh->m_vertex_array.push_back(pc);
		}

		matrix_4d v_collapse_mat = matrix4<double>::translation(geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z);
		geo_mesh->set_collapse(v_collapse_mat);
	}

	geometry_mesh* geometry_factory::create_mesh_from_multiple_polygon(vector3<double>* in_vertex_buffer, ulong in_vertex_size, ulong* in_index_buffer, ulong in_index_size, ulong in_color, double in_height, double in_lower_height)
	{
		geometry_mesh* geo_mesh = new geometry_mesh;

		coordinates_array v_array;

		ulong cur_index = 0;
		for (int i = 0; i < (int)in_index_size; i++)
		{
			if (in_index_buffer[i] <= 2)
			{
				cur_index += in_index_buffer[i];
				continue;
			}

			generate_polygon_mesh(geo_mesh, &in_vertex_buffer[cur_index], v_array, in_index_buffer[i], in_color, in_height, in_lower_height);

			cur_index += in_index_buffer[i];
		}

		geo_mesh->m_center = geo_mesh->m_render_aabb.center();
		for (int i = 0; i < (int)v_array.size(); i++)
		{
			position_color pc;
			pc.x = v_array[i].x - geo_mesh->m_center.x;
			pc.y = v_array[i].y - geo_mesh->m_center.y;
			pc.z = v_array[i].z - geo_mesh->m_center.z;
			pc.color = in_color;
			geo_mesh->m_vertex_array.push_back(pc);
		}

		matrix_4d v_collapse_mat = matrix4<double>::translation(geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z);
		geo_mesh->set_collapse(v_collapse_mat);

		return geo_mesh;
	}

	void geometry_factory::create_mesh_from_multiple_polygon(geometry_mesh* geo_mesh, vector3<double>* in_vertex_buffer, ulong in_vertex_size, ulong* in_index_buffer, ulong in_index_size, ulong in_color, double in_height, double in_lower_height)
	{
		geo_mesh->clear();

		coordinates_array v_array;
		ulong cur_index = 0;
		for (int i = 0; i < (int)in_index_size; i++)
		{
			if (in_index_buffer[i] <= 2)
			{
				cur_index += in_index_buffer[i];
				continue;
			}

			generate_polygon_mesh(geo_mesh, &in_vertex_buffer[cur_index], v_array, in_index_buffer[i], in_color, in_height, in_lower_height);

			cur_index += in_index_buffer[i];
		}

		geo_mesh->m_center = geo_mesh->m_render_aabb.center();


		for (int i = 0; i < (int)v_array.size(); i++)
		{
			position_color pc;
			pc.x = v_array[i].x - geo_mesh->m_center.x;
			pc.y = v_array[i].y - geo_mesh->m_center.y;
			pc.z = v_array[i].z - geo_mesh->m_center.z;
			pc.color = in_color;
			geo_mesh->m_vertex_array.push_back(pc);
		}

		matrix_4d v_collapse_mat = matrix4<double>::translation(geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z);
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
	//////////////////////////////////////////////////////////////////////////
	geometry_mesh* geometry_factory::create_mesh_from_box(double x_len, double y_len, double z_len, ulong line_color, ulong fill_color)
	{
		geometry_mesh* geo_mesh = new geometry_mesh;

		create_mesh_from_box(geo_mesh, x_len, y_len, z_len, line_color, fill_color);

		return geo_mesh;
	}

	void geometry_factory::generate_polygon_mesh_cart(geometry_mesh* geo_mesh, vector_3d* in_pos_array, ulong v_size, double v_height, ulong fill_color)
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


		// 		int in_triangle_start_mode = 0;
		// 		vector_2d prev_pos = tri_array[ (max_ni - 1 + v_size ) % v_size ];
		// 		vector_2d next_pos = tri_array[ (max_ni + 1 + v_size ) % v_size ];
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
		v_triangulation_algorithm.triangulation_polygon(tri_array.begin()._Myptr, v_size, v_temp_index_array.begin()._Myptr);

		//
		// 组织整个多面体
		//

		// 上表面
		ulong start_index = geo_mesh->m_vertex_array.size();
		for (int i = 0; i < (int)v_temp_index_array.size(); i += 3)
		{
			if (in_triangle_start_mode == 0)
			{
				// CW
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i]);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 1]);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 2]);
			}
			else if (in_triangle_start_mode == 1)
			{
				// CCW
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i]);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 2]);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 1]);
			}
		}

		// 下表面
		for (int i = 0; i < (int)v_temp_index_array.size(); i += 3)
		{
			if (in_triangle_start_mode == 0)
			{
				// CW
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i] + v_size);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 2] + v_size);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 1] + v_size);
			}
			else if (in_triangle_start_mode == 1)
			{
				// CCW
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i] + v_size);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 1] + v_size);
				geo_mesh->m_index_array.push_back(start_index + v_temp_index_array[i + 2] + v_size);
			}
		}

		// 侧面
		for (int i = 0; i < (int)v_size; i++)
		{
			if (in_triangle_start_mode == 0)
			{
				// CW
				geo_mesh->m_index_array.push_back(start_index + i);
				geo_mesh->m_index_array.push_back(start_index + i + v_size);
				geo_mesh->m_index_array.push_back(start_index + (i + 1) % v_size + v_size);

				geo_mesh->m_index_array.push_back(start_index + i);
				geo_mesh->m_index_array.push_back(start_index + (i + 1) % v_size + v_size);
				geo_mesh->m_index_array.push_back(start_index + (i + 1) % v_size);
			}
			else if (in_triangle_start_mode == 1)
			{
				// CCW
				geo_mesh->m_index_array.push_back(start_index + i);
				geo_mesh->m_index_array.push_back(start_index + (i + 1) % v_size + v_size);
				geo_mesh->m_index_array.push_back(start_index + i + v_size);

				geo_mesh->m_index_array.push_back(start_index + i);
				geo_mesh->m_index_array.push_back(start_index + (i + 1) % v_size);
				geo_mesh->m_index_array.push_back(start_index + (i + 1) % v_size + v_size);
			}
		}

		// 生成顶点集合

		geo_mesh->m_vertex_array.resize(v_size * 2);
		for (int i = 0; i < (int)v_size; i++)
		{
			geo_mesh->m_vertex_array[i].x = in_pos_array[i].x;
			geo_mesh->m_vertex_array[i].y = in_pos_array[i].y;
			geo_mesh->m_vertex_array[i].z = in_pos_array[i].z;
			geo_mesh->m_vertex_array[i].color = fill_color;

			geo_mesh->m_render_aabb.combine(in_pos_array[i]);

		}

		vector_3d vec;
		int v_crsr = 0;
		for (int i = 0; i < (int)v_size; i++)
		{
			v_crsr = i + v_size;
			vec = in_pos_array[i];
			vec.y += v_height;

			geo_mesh->m_vertex_array[v_crsr].x = vec.x;
			geo_mesh->m_vertex_array[v_crsr].y = vec.y;
			geo_mesh->m_vertex_array[v_crsr].z = vec.z;
			geo_mesh->m_vertex_array[v_crsr].color = fill_color;

			geo_mesh->m_render_aabb.combine(vec);
		}
	}

	geometry_mesh* geometry_factory::create_mesh_from_volume(vector_3d* in_pos_array, ulong v_size, double v_height, ulong line_color, ulong fill_color)
	{
		geometry_mesh* geo_mesh = new geometry_mesh;

		create_mesh_from_volume(geo_mesh, in_pos_array, v_size, v_height, line_color, fill_color);

		return geo_mesh;
	}

	geometry_mesh* geometry_factory::create_mesh_from_cone(ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color)
	{
		geometry_mesh* geo_mesh = new geometry_mesh;

		create_mesh_from_cone(geo_mesh, in_sides, in_radius, in_height, line_color, fill_color);

		return geo_mesh;
	}

	geometry_mesh* geometry_factory::create_mesh_from_cylinder(ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color)
	{
		// create cylinder
		vector_3d vec_cen(0.0, 0.0, 0.0);
		vector_3d* v_circle = new vector_3d[in_sides + 1];
		shape_creator::create_circle_on_plane(v_circle, vec_cen, in_radius, in_sides);
		v_circle[in_sides] = v_circle[0];

		geometry_mesh* geo_mesh = create_mesh_from_volume(v_circle, in_sides, in_height, line_color, fill_color);

		AUTO_DELETE_ARRAY(v_circle);

		return geo_mesh;
	}


#define NORMALIZE_ANGLE( val ) { if(val > PI * 2) val -= PI *2; if(val < -PI * 2) val += PI *2; }
	geometry_mesh* geometry_factory::create_mesh_from_sphere(double in_radius, ulong line_color, ulong fill_color)
	{
		geometry_mesh* geo_mesh = new geometry_mesh;

		create_mesh_from_sphere(geo_mesh, in_radius, line_color, fill_color);

		return geo_mesh;
	}

	geometry_mesh* geometry_factory::create_mesh_from_vertex_and_index(vector_3d* v_vertex, int v__vertex_size, ulong v_color, vector3<ushort>* v_index, int v_index_num)
	{
		geometry_mesh* geo_mesh = new geometry_mesh;
		geo_mesh->set_shadow_color(v_color);

		if (v_vertex == NULL || v_index == NULL)
			return NULL;

		geo_mesh->m_vertex_array.resize(v__vertex_size);
		geo_mesh->m_index_array.resize(v_index_num * 3);

		geo_mesh->m_render_aabb.make_invalid();
		for (int i = 0; i < v__vertex_size; i++)
		{
			geo_mesh->m_render_aabb.combine(v_vertex[i]);
		}

		for (int i = 0; i < v__vertex_size; i++)
		{
			geo_mesh->m_vertex_array[i].x = v_vertex[i].x;
			geo_mesh->m_vertex_array[i].y = v_vertex[i].y;
			geo_mesh->m_vertex_array[i].z = v_vertex[i].z;
			geo_mesh->m_vertex_array[i].color = v_color;
		}

		for (int i = 0; i < v_index_num; i++)
		{
			geo_mesh->m_index_array[i * 3] = v_index[i].x;
			geo_mesh->m_index_array[i * 3 + 1] = v_index[i].y;
			geo_mesh->m_index_array[i * 3 + 2] = v_index[i].z;
		}

		vector_3d vec_min = geo_mesh->m_render_aabb.get_min();
		vector_3d vec_max = geo_mesh->m_render_aabb.get_max();
		vector_3d vec_collapse = (vec_min + vec_max) * 0.5;

		matrix_4d v_collapse_mat = collapse_geometry_mesh(geo_mesh, vec_collapse);
		geo_mesh->set_collapse(v_collapse_mat);

		return geo_mesh;
	}

	geometry_mesh* geometry_factory::create_mesh_from_vertex_and_index_cart(vector_3d* v_vertex, int v__vertex_size, ulong v_color, vector3<ushort>* v_index, int v_index_num)
	{
		geometry_mesh* geo_mesh = new geometry_mesh;
		geo_mesh->set_shadow_color(v_color);

		if (v_vertex == NULL || v_index == NULL)
			return NULL;

		geo_mesh->m_vertex_array.resize(v__vertex_size);
		geo_mesh->m_index_array.resize(v_index_num * 3);

		geo_mesh->m_render_aabb.make_invalid();
		for (int i = 0; i < v__vertex_size; i++)
		{
			geo_mesh->m_render_aabb.combine(v_vertex[i]);
		}

		for (int i = 0; i < v__vertex_size; i++)
		{
			geo_mesh->m_vertex_array[i].x = v_vertex[i].x;
			geo_mesh->m_vertex_array[i].y = v_vertex[i].y;
			geo_mesh->m_vertex_array[i].z = v_vertex[i].z;
			geo_mesh->m_vertex_array[i].color = v_color;
		}

		for (int i = 0; i < v_index_num; i++)
		{
			geo_mesh->m_index_array[i * 3] = v_index[i].x;
			geo_mesh->m_index_array[i * 3 + 1] = v_index[i].y;
			geo_mesh->m_index_array[i * 3 + 2] = v_index[i].z;
		}

		return geo_mesh;
	}

	void geometry_factory::create_mesh_from_line(geometry_mesh* geo_mesh, vector_3d* v_src, int v_size, double v_width, ulong in_color, double in_height, double in_lower_height)
	{
		if (v_size <= 1) return;

		geo_mesh->clear();

		coordinates_array v_array;

		generate_line_mesh(geo_mesh, v_src, v_array, v_size, v_width, in_color, in_height, in_lower_height);

		geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		for (int i = 0; i < (int)v_array.size(); i++)
		{
			position_color pc;
			pc.x = v_array[i].x - geo_mesh->m_center.x;
			pc.y = v_array[i].y - geo_mesh->m_center.y;
			pc.z = v_array[i].z - geo_mesh->m_center.z;
			pc.color = in_color;
			geo_mesh->m_vertex_array.push_back(pc);
		}


		matrix_4d v_collapse_mat = matrix4<double>::translation(geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z);
		geo_mesh->set_collapse(v_collapse_mat);
	}

	geometry_mesh* geometry_factory::create_mesh_from_line(vector_3d* v_src, int v_size, double v_width, ulong in_color, double in_height, double in_lower_height)
	{
		if (v_size <= 1) return NULL;

		geometry_mesh* geo_mesh = new geometry_mesh;

		coordinates_array v_array;

		generate_line_mesh(geo_mesh, v_src, v_array, v_size, v_width, in_color, in_height, in_lower_height);

		geo_mesh->m_center = geo_mesh->m_render_aabb.center();

		for (int i = 0; i < (int)v_array.size(); i++)
		{
			position_color pc;
			pc.x = v_array[i].x - geo_mesh->m_center.x;
			pc.y = v_array[i].y - geo_mesh->m_center.y;
			pc.z = v_array[i].z - geo_mesh->m_center.z;
			pc.color = in_color;
			geo_mesh->m_vertex_array.push_back(pc);
		}


		matrix_4d v_collapse_mat = matrix4<double>::translation(geo_mesh->m_center.x, geo_mesh->m_center.y, geo_mesh->m_center.z);
		geo_mesh->set_collapse(v_collapse_mat);

		return geo_mesh;
	}


	void geometry_factory::create_mesh_from_box(geometry_mesh* geo_mesh, double x_len, double y_len, double z_len, ulong line_color, ulong fill_color)
	{
		geo_mesh->clear();

		geo_mesh->m_vertex_array.resize(8);
		geo_mesh->m_index_array.resize(36);

		double half_x = x_len * 0.5;
		double half_z = z_len * 0.5;

		geo_mesh->m_vertex_array[0].x = -half_x;
		geo_mesh->m_vertex_array[0].y = 0.0;
		geo_mesh->m_vertex_array[0].z = -half_z;
		geo_mesh->m_vertex_array[0].color = fill_color;

		geo_mesh->m_vertex_array[1].x = -half_x;
		geo_mesh->m_vertex_array[1].y = y_len;
		geo_mesh->m_vertex_array[1].z = -half_z;
		geo_mesh->m_vertex_array[1].color = fill_color;

		geo_mesh->m_vertex_array[2].x = half_x;
		geo_mesh->m_vertex_array[2].y = y_len;
		geo_mesh->m_vertex_array[2].z = -half_z;
		geo_mesh->m_vertex_array[2].color = fill_color;

		geo_mesh->m_vertex_array[3].x = half_x;
		geo_mesh->m_vertex_array[3].y = 0.0;
		geo_mesh->m_vertex_array[3].z = -half_z;
		geo_mesh->m_vertex_array[3].color = fill_color;

		geo_mesh->m_vertex_array[4].x = half_x;
		geo_mesh->m_vertex_array[4].y = y_len;
		geo_mesh->m_vertex_array[4].z = half_z;
		geo_mesh->m_vertex_array[4].color = fill_color;

		geo_mesh->m_vertex_array[5].x = -half_x;
		geo_mesh->m_vertex_array[5].y = y_len;
		geo_mesh->m_vertex_array[5].z = half_z;
		geo_mesh->m_vertex_array[5].color = fill_color;

		geo_mesh->m_vertex_array[6].x = -half_x;
		geo_mesh->m_vertex_array[6].y = 0.0;
		geo_mesh->m_vertex_array[6].z = half_z;
		geo_mesh->m_vertex_array[6].color = fill_color;

		geo_mesh->m_vertex_array[7].x = half_x;
		geo_mesh->m_vertex_array[7].y = 0.0;
		geo_mesh->m_vertex_array[7].z = half_z;
		geo_mesh->m_vertex_array[7].color = fill_color;

		geo_mesh->m_render_aabb.set_extent(vector_3d(-half_x, 0.0, -half_z), vector_3d(half_x, y_len, half_z));

		ushort index_array[36] =
		{
			0,1,3,1,2,3,
			3,2,7,2,4,7,
			7,4,6,4,5,6,
			6,5,0,5,1,0,
			1,5,2,5,4,2,
			0,3,6,3,7,6
		};
		memcpy(geo_mesh->m_index_array.begin()._Myptr, index_array, sizeof(ushort) * 36);

		geo_mesh->m_frame_vertex_array.resize(geo_mesh->m_vertex_array.size());
		std::copy(geo_mesh->m_vertex_array.begin(), geo_mesh->m_vertex_array.end(), geo_mesh->m_frame_vertex_array.begin());
		for (int i = 0; i < (int)geo_mesh->m_frame_vertex_array.size(); i++)
		{
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}

		// 边框
		ushort frame_index_array[24] =
		{
			0,1, 1,2, 2,3, 3,0,
			4,5, 5,6, 6,7, 7,4,
			0,6, 3,7, 1,5, 2,4
		};
		geo_mesh->m_frame_index_array.resize(24);
		memcpy(geo_mesh->m_frame_index_array.begin()._Myptr, frame_index_array, sizeof(ushort) * 24);
	}

	void geometry_factory::create_mesh_from_volume(geometry_mesh* geo_mesh, vector_3d* in_pos_array, ulong v_size, double v_height, ulong line_color, ulong fill_color)
	{
		geo_mesh->clear();

		generate_polygon_mesh_cart(geo_mesh, in_pos_array, v_size, v_height, fill_color);

		// 边框
		geo_mesh->m_frame_vertex_array.resize(geo_mesh->m_vertex_array.size());
		std::copy(geo_mesh->m_vertex_array.begin(), geo_mesh->m_vertex_array.end(), geo_mesh->m_frame_vertex_array.begin());
		for (int i = 0; i < (int)geo_mesh->m_frame_vertex_array.size(); i++)
		{
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}

		ulong v_line_num = v_size * 3;
		geo_mesh->m_frame_index_array.resize(v_line_num * 2);
		ushort* p_index = geo_mesh->m_frame_index_array.begin()._Myptr;

		ulong v_off = 0;
		for (int i = 0; i < (int)v_size - 1; i++)
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

	}

	void geometry_factory::reset_mesh_color(geometry_mesh* geo_mesh, ulong v_color, ulong v_line_color, ulong v_fill_color)
	{
		geo_mesh->set_shadow_color(v_color);

		for (int ni = 0; ni < (int)geo_mesh->m_vertex_array.size(); ni++)
		{
			geo_mesh->m_vertex_array[ni].color = v_fill_color;
		}

		for (int ni = 0; ni < (int)geo_mesh->m_frame_vertex_array.size(); ni++)
		{
			geo_mesh->m_frame_vertex_array[ni].color = v_line_color;
		}
	}

	void geometry_factory::create_mesh_from_cone(geometry_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color)
	{
		geo_mesh->clear();

		vector_3d v_point_center(0.0, 0.0, 0.0);
		vector_3d v_point_top(0.0, in_height, 0.0);

		vector_3d* v_circle_points = new vector_3d[in_sides];
		shape_creator::create_circle_on_plane(v_circle_points, v_point_center, in_radius, in_sides);

		geo_mesh->m_render_aabb.combine(v_point_top);
		for (int i = 0; i < (int)in_sides; i++)
		{
			geo_mesh->m_render_aabb.combine(v_circle_points[i]);
		}

		geo_mesh->m_vertex_array.resize(in_sides + 2);
		for (int i = 0; i < (int)in_sides; i++)
		{
			geo_mesh->m_vertex_array[i].x = v_circle_points[i].x;
			geo_mesh->m_vertex_array[i].y = v_circle_points[i].y;
			geo_mesh->m_vertex_array[i].z = v_circle_points[i].z;
			geo_mesh->m_vertex_array[i].color = fill_color;
		}
		geo_mesh->m_vertex_array[in_sides].x = v_point_center.x;
		geo_mesh->m_vertex_array[in_sides].y = v_point_center.y;
		geo_mesh->m_vertex_array[in_sides].z = v_point_center.z;
		geo_mesh->m_vertex_array[in_sides].color = fill_color;

		geo_mesh->m_vertex_array[in_sides + 1].x = v_point_top.x;
		geo_mesh->m_vertex_array[in_sides + 1].y = v_point_top.y;
		geo_mesh->m_vertex_array[in_sides + 1].z = v_point_top.z;
		geo_mesh->m_vertex_array[in_sides + 1].color = fill_color;

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

		// 构建底面，同时构建侧面
		for (int i = 0; i < (int)in_sides; i++)
		{
			if (in_triangle_start_mode == 0) // CCW，右手
			{
				// 底面
				geo_mesh->m_index_array.push_back(in_sides);
				geo_mesh->m_index_array.push_back((i == in_sides - 1) ? 0 : i + 1);
				geo_mesh->m_index_array.push_back(i);
				// 锥面
				geo_mesh->m_index_array.push_back(in_sides + 1);
				geo_mesh->m_index_array.push_back(i);
				geo_mesh->m_index_array.push_back((i == in_sides - 1) ? 0 : i + 1);
			}
			else // CW，左手
			{
				// 底面
				geo_mesh->m_index_array.push_back(in_sides);
				geo_mesh->m_index_array.push_back(i);
				geo_mesh->m_index_array.push_back((i == in_sides - 1) ? 0 : i + 1);
				// 锥面
				geo_mesh->m_index_array.push_back(in_sides + 1);
				geo_mesh->m_index_array.push_back((i == in_sides - 1) ? 0 : i + 1);
				geo_mesh->m_index_array.push_back(i);
			}
		}

		geo_mesh->m_frame_vertex_array.resize(geo_mesh->m_vertex_array.size());
		std::copy(geo_mesh->m_vertex_array.begin(), geo_mesh->m_vertex_array.end(), geo_mesh->m_frame_vertex_array.begin());
		for (int i = 0; i < (int)geo_mesh->m_frame_vertex_array.size(); i++)
		{
			geo_mesh->m_frame_vertex_array[i].color = line_color;
		}

		// 边框
		ulong v_line_num = in_sides * 2;
		geo_mesh->m_frame_index_array.resize(v_line_num * 2);
		ushort* p_index = geo_mesh->m_frame_index_array.begin()._Myptr;

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

	void geometry_factory::create_mesh_from_cylinder(geometry_mesh* geo_mesh, ulong in_sides, double in_radius, double in_height, ulong line_color, ulong fill_color)
	{
		geo_mesh->clear();

		// create cylinder
		vector_3d vec_cen(0.0, 0.0, 0.0);
		vector_3d* v_circle = new vector_3d[in_sides + 1];
		shape_creator::create_circle_on_plane(v_circle, vec_cen, in_radius, in_sides);
		v_circle[in_sides] = v_circle[0];

		create_mesh_from_volume(geo_mesh, v_circle, in_sides, in_height, line_color, fill_color);

		AUTO_DELETE_ARRAY(v_circle);
	}

	void geometry_factory::create_mesh_from_sphere(geometry_mesh* geo_mesh, double in_radius, ulong line_color, ulong fill_color)
	{
		geo_mesh->clear();

		int	v_segment = (1 << 5) + 1;
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
		int v_index_count = v_segment * v_double_segment * 2 * 3;

		geo_mesh->m_vertex_array.resize(v_vertex_count);
		vector_3d vec_cen(0.0, v_radius, 0.0);

		vector_3d vec_min(-v_radius, 0.0, -v_radius);
		vector_3d vec_max(v_radius, 2 * v_radius, v_radius);
		geo_mesh->m_render_aabb.set_extent(vec_min, vec_max);

		int v_index = 0;
		vector_3d vec_out(0.0, 0.0, 0.0);
		for (int i = 0; i < v_segment_plus_1; i++)
		{
			double v_longitude = 0;
			double tmp_radius = math_d::cos_(v_latitude) * v_radius;
			for (int j = 0; j < v_double_segment_plus_1; j++)
			{
				geo_mesh->m_vertex_array[v_index].x = vec_cen.x + math_d::sin_(v_longitude) * tmp_radius;
				geo_mesh->m_vertex_array[v_index].y = vec_cen.y + math_d::sin_(v_latitude) * v_radius;
				geo_mesh->m_vertex_array[v_index].z = vec_cen.z + math_d::cos_(v_longitude) * tmp_radius;
				geo_mesh->m_vertex_array[v_index].color = fill_color;
				v_index += 1;
				v_longitude += v_delta_longitude;
				NORMALIZE_ANGLE(v_longitude);
			}
			v_latitude += v_delta_latitude;
			NORMALIZE_ANGLE(v_latitude);
		}

		geo_mesh->m_index_array.resize(v_index_count);

		v_index = 0;
		for (int i = 0; i < v_segment; i++)
		{
			v_index = (2 * 3 * i * v_double_segment);

			for (int j = 0; j < v_double_segment; j++)
			{
				geo_mesh->m_index_array[v_index] = (short)(i * v_double_segment_plus_1 + j);
				geo_mesh->m_index_array[v_index + 2] = (short)((i + 1) * v_double_segment_plus_1 + j);
				geo_mesh->m_index_array[v_index + 1] = (short)(i * v_double_segment_plus_1 + j + 1);

				geo_mesh->m_index_array[v_index + 3] = (short)(i * v_double_segment_plus_1 + j + 1);
				geo_mesh->m_index_array[v_index + 5] = (short)((i + 1) * v_double_segment_plus_1 + j);
				geo_mesh->m_index_array[v_index + 4] = (short)((i + 1) * v_double_segment_plus_1 + j + 1);

				v_index += 6;
			}
		}

		// 边框
		geo_mesh->m_frame_vertex_array.resize(geo_mesh->m_vertex_array.size());
		geo_mesh->m_frame_index_array.resize(geo_mesh->m_index_array.size());
		std::copy(geo_mesh->m_vertex_array.begin(), geo_mesh->m_vertex_array.end(), geo_mesh->m_frame_vertex_array.begin());
		for (int i = 0; i < geo_mesh->m_frame_vertex_array.size(); i++)
		{
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

	}



	//////////////////////////////////////////////////////////////////////////
	// line string
	//////////////////////////////////////////////////////////////////////////
	matrix_4d geometry_factory::collapse_geometry_line(geometry_line* v_line, vector_3d& vec_cen)
	{
		for (int ni = 0; ni < (int)v_line->m_vertex_array.size(); ni++)
		{
			v_line->m_vertex_array[ni].x -= vec_cen.x;
			v_line->m_vertex_array[ni].y -= vec_cen.y;
			v_line->m_vertex_array[ni].z -= vec_cen.z;
		}

		return matrix_4d::translation(vec_cen.x, vec_cen.y, vec_cen.z);
	}

	void geometry_factory::collapse_geometry_line(geometry_line* v_line, matrix_4f v_mat)
	{
		vector_3f vec_tmp;

		for (int i = 0; i < (int)v_line->m_vertex_array.size(); i++)
		{
			vec_tmp.x = v_line->m_vertex_array[i].x;
			vec_tmp.y = v_line->m_vertex_array[i].y;
			vec_tmp.z = v_line->m_vertex_array[i].z;
			vec_tmp = vec_tmp * v_mat;

			v_line->m_vertex_array[i].x = vec_tmp.x;
			v_line->m_vertex_array[i].y = vec_tmp.y;
			v_line->m_vertex_array[i].z = vec_tmp.z;
		}
	}

	void geometry_factory::generate_line_string(geometry_line* geo_line, vector_3d* v_src, int v_size, ulong v_color)
	{
		geo_line->m_vertex_array.clear();
		geo_line->m_vertex_array.resize(v_size);

		axis_aligned_box_d aabb;
		aabb.make_invalid();
		for (int ni = 0; ni < v_size; ni++)
		{
			aabb.combine(v_src[ni]);
		}
		vector_3d v_center = aabb.center();

		for (int ni = 0; ni < v_size; ni++)
		{
			geo_line->m_vertex_array[ni].x = v_src[ni].x - v_center.x;
			geo_line->m_vertex_array[ni].y = v_src[ni].y - v_center.y;
			geo_line->m_vertex_array[ni].z = v_src[ni].z - v_center.z;
			geo_line->m_vertex_array[ni].color = v_color;
		}
		geo_line->set_collapse(matrix_4d::translation(v_center.x, v_center.y, v_center.z));
		geo_line->set_render_aabb(aabb);
	}

	void geometry_factory::create_line_string(geometry_line* geo_line, vector_3d* v_src, int v_size, ulong color)
	{
		generate_line_string(geo_line, v_src, v_size, color);

		/*vector_3d vec_min = geo_line->m_render_aabb.get_min();
		vector_3d vec_max = geo_line->m_render_aabb.get_max();
		vector_3d vec_collapse = ( vec_min + vec_max ) * 0.5;

		matrix_4d v_collapse_mat = collapse_geometry_line( geo_line, vec_collapse );
		geo_line->set_collapse( v_collapse_mat );*/
	}

	void geometry_factory::create_line_string_cart(geometry_line* geo_line, vector_3d* v_src, int v_size, ulong color)
	{
		geo_line->m_vertex_array.clear();
		geo_line->m_vertex_array.resize(v_size);

		vector_3d vec = v_src[0];
		geo_line->m_vertex_array[0].x = vec.x;
		geo_line->m_vertex_array[0].y = vec.y;
		geo_line->m_vertex_array[0].z = vec.z;
		geo_line->m_vertex_array[0].color = color;

		vector_3d vec_min(vec.x, vec.y, vec.z);
		vector_3d vec_max(vec.x, vec.y, vec.z);

		for (int ni = 1; ni < v_size; ni++)
		{
			vec = v_src[ni];
			geo_line->m_vertex_array[ni].x = vec.x;
			geo_line->m_vertex_array[ni].y = vec.y;
			geo_line->m_vertex_array[ni].z = vec.z;
			geo_line->m_vertex_array[ni].color = color;

			vec_min.x = min(vec_min.x, vec.x);
			vec_min.y = min(vec_min.y, vec.y);
			vec_min.z = min(vec_min.z, vec.z);

			vec_max.x = max(vec_max.x, vec.x);
			vec_max.y = max(vec_max.y, vec.y);
			vec_max.z = max(vec_max.z, vec.z);
		}
		geo_line->m_render_aabb.set_extent(vec_min, vec_max);
	}

	//////////////////////////////////////////////////////////////////////////
	// polygon
	//////////////////////////////////////////////////////////////////////////
	matrix_4d geometry_factory::collapse_geometry_polygon(geometry_polygon* geo_polygon, vector_3d& vec_cen)
	{
		for (int ni = 0; ni < (int)geo_polygon->m_vertex_array.size(); ni++)
		{
			geo_polygon->m_vertex_array[ni].x -= vec_cen.x;
			geo_polygon->m_vertex_array[ni].y -= vec_cen.y;
			geo_polygon->m_vertex_array[ni].z -= vec_cen.z;
		}

		for (int ni = 0; ni < (int)geo_polygon->m_line_array.size(); ni++)
		{
			geo_polygon->m_line_array[ni].x -= vec_cen.x;
			geo_polygon->m_line_array[ni].y -= vec_cen.y;
			geo_polygon->m_line_array[ni].z -= vec_cen.z;
		}

		//vector_3d vec_min = geo_polygon->m_render_aabb.get_min();
		//vector_3d vec_max = geo_polygon->m_render_aabb.get_max();
		//vec_min -= vec_cen;
		//vec_max -= vec_cen;
		//geo_polygon->m_render_aabb.set_extent( vec_min, vec_max );

		return matrix_4d::translation(vec_cen.x, vec_cen.y, vec_cen.z);
	}

	void geometry_factory::collapse_geometry_polygon(geometry_polygon* geo_polygon, matrix_4f v_mat)
	{
		vector_3f vec_tmp;

		for (int i = 0; i < (int)geo_polygon->m_vertex_array.size(); i++)
		{
			vec_tmp.x = geo_polygon->m_vertex_array[i].x;
			vec_tmp.y = geo_polygon->m_vertex_array[i].y;
			vec_tmp.z = geo_polygon->m_vertex_array[i].z;
			vec_tmp = vec_tmp * v_mat;

			geo_polygon->m_vertex_array[i].x = vec_tmp.x;
			geo_polygon->m_vertex_array[i].y = vec_tmp.y;
			geo_polygon->m_vertex_array[i].z = vec_tmp.z;
		}

		for (int i = 0; i < (int)geo_polygon->m_line_array.size(); i++)
		{
			vec_tmp.x = geo_polygon->m_line_array[i].x;
			vec_tmp.y = geo_polygon->m_line_array[i].y;
			vec_tmp.z = geo_polygon->m_line_array[i].z;
			vec_tmp = vec_tmp * v_mat;

			geo_polygon->m_line_array[i].x = vec_tmp.x;
			geo_polygon->m_line_array[i].y = vec_tmp.y;
			geo_polygon->m_line_array[i].z = vec_tmp.z;
		}

		//vector_3d vec_min = geo_polygon->m_render_aabb.get_min();
		//vec_tmp.x = vec_min.x;
		//vec_tmp.y = vec_min.y;
		//vec_tmp.z = vec_min.z;
		//vec_tmp = vec_tmp * v_mat;
		//vec_min.x = vec_tmp.x;
		//vec_min.y = vec_tmp.y;
		//vec_min.z = vec_tmp.z;

		//vector_3d vec_max = geo_polygon->m_render_aabb.get_max();
		//vec_tmp.x = vec_max.x;
		//vec_tmp.y = vec_max.y;
		//vec_tmp.z = vec_max.z;
		//vec_tmp = vec_tmp * v_mat;
		//vec_max.x = vec_tmp.x;
		//vec_max.y = vec_tmp.y;
		//vec_max.z = vec_tmp.z;

		//geo_polygon->m_render_aabb.set_extent( vec_min, vec_max );
	}

	void geometry_factory::generate_triangle_list(geometry_polygon* geo_polygon, vector_3d* v_src, int v_size)
	{
		// calculate direction
		int in_triangle_start_mode = 0;// 0:CW 1:CCW

		vector3<double> temp_vec1 = v_src[1] - v_src[0];
		vector3<double> temp_vec2 = v_src[2] - v_src[0];
		vector3<double> temp_cross = vector3<double>::cross(temp_vec2, temp_vec1);
		double temp_value = vector3<double>::dot(temp_cross, v_src[1]);

		if (temp_value > 0)
		{
			in_triangle_start_mode = 1;
		}
		else
		{
			in_triangle_start_mode = 0;
		}

		// calculate average normal
		vector3<double> v_average_normal = vector3<double>::s_zero;
		for (int i = 0; i < v_size; i++)
		{
			vector3<double> pos_geo = v_src[i];
			pos_geo.normalize();

			v_average_normal += pos_geo;
		}
		v_average_normal /= v_size;
		v_average_normal.normalize();

		// get a point on projection surface
		vector3<double> v_proj_surface_pos = spherical_coords::s_semi_major * v_average_normal;

		// project polygons to the surface
		coordinates_array v_proj_array;
		double proj_height = v_proj_surface_pos.length();
		for (int i = 0; i < v_size; i++)
		{
			vector3<double> pos_geo;
			pos_geo = v_src[i];
			pos_geo.normalize();
			double v_length = proj_height / vector3<double>::dot(pos_geo, v_average_normal);
			pos_geo = pos_geo * v_length;
			v_proj_array.push_back(pos_geo);
		}

		// generate sub coordinates
		vector3<double> v_x_axis, v_y_axis, v_z_axis;
		v_x_axis = vector3<double>::normalize(v_proj_array[0] - v_proj_surface_pos);
		v_y_axis = v_average_normal;
		v_z_axis = vector3<double>::cross(v_x_axis, v_y_axis);
		v_z_axis.normalize();

		matrix4<double> mat_tnb = matrix4<double>::from_tnbp(v_x_axis, v_y_axis, v_z_axis, v_proj_surface_pos);

		std::vector< vector2<double> > v_array_local;
		for (int ni = 0; ni < v_size; ni++)
		{
			vector3<double> pos_geo = v_proj_array[ni] * mat_tnb.inverse();

			if (math<double>::fabs_(pos_geo.x) < math<double>::s_zero_tolerance)
			{
				pos_geo.x = 0.0;
			}
			if (math<double>::fabs_(pos_geo.y) < math<double>::s_zero_tolerance)
			{
				pos_geo.y = 0.0;
			}
			if (math<double>::fabs_(pos_geo.z) < math<double>::s_zero_tolerance)
			{
				pos_geo.z = 0.0;
			}

			v_array_local.push_back(vector2<double>(pos_geo.x, pos_geo.z));
		}

		// generate triangle list
		geo_polygon->m_triangle_index.clear();
		geo_polygon->m_triangle_index.resize((v_size - 2) * 3);

		triangulation<double> v_triangulation_algorithm;
		v_triangulation_algorithm.triangulation_polygon(v_array_local.begin()._Myptr, v_size, geo_polygon->m_triangle_index.begin()._Myptr);
	}

	void geometry_factory::generate_polygon(geometry_polygon* geo_polygon, vector_3d* v_src, int v_size, ulong v_line_color, ulong v_fill_color)
	{
		geo_polygon->m_vertex_array.clear();
		geo_polygon->m_vertex_array.resize(v_size);

		geo_polygon->m_line_array.clear();
		geo_polygon->m_line_array.resize(v_size);

		vector_3d vec = v_src[0];//geocentric_coords::from_spherical_d( v_src[0] );
		geo_polygon->m_line_array[0].x = vec.x;
		geo_polygon->m_line_array[0].y = vec.y;
		geo_polygon->m_line_array[0].z = vec.z;
		geo_polygon->m_line_array[0].color = v_line_color;

		geo_polygon->m_vertex_array[0].x = vec.x;
		geo_polygon->m_vertex_array[0].y = vec.y;
		geo_polygon->m_vertex_array[0].z = vec.z;
		geo_polygon->m_vertex_array[0].color = v_fill_color;

		vector_3d vec_min(vec.x, vec.y, vec.z);
		vector_3d vec_max(vec.x, vec.y, vec.z);

		for (int ni = 1; ni < v_size; ni++)
		{
			vec = v_src[ni];//geocentric_coords::from_spherical_d( v_src[ni] );
			geo_polygon->m_line_array[ni].x = vec.x;
			geo_polygon->m_line_array[ni].y = vec.y;
			geo_polygon->m_line_array[ni].z = vec.z;
			geo_polygon->m_line_array[ni].color = v_line_color;

			geo_polygon->m_vertex_array[ni].x = vec.x;
			geo_polygon->m_vertex_array[ni].y = vec.y;
			geo_polygon->m_vertex_array[ni].z = vec.z;
			geo_polygon->m_vertex_array[ni].color = v_fill_color;

			vec_min.x = min(vec_min.x, vec.x);
			vec_min.y = min(vec_min.y, vec.y);
			vec_min.z = min(vec_min.z, vec.z);

			vec_max.x = max(vec_max.x, vec.x);
			vec_max.y = max(vec_max.y, vec.y);
			vec_max.z = max(vec_max.z, vec.z);
		}
		geo_polygon->m_render_aabb.set_extent(vec_min, vec_max);

		generate_triangle_list(geo_polygon, v_src, v_size);
	}

	void geometry_factory::generate_triangle_list_cart(geometry_polygon* geo_polygon, vector_3d* v_src, int v_size)
	{
		// 三角化
		int max_ni = 0;
		double max_z = -math_d::s_max_real;
		std::vector<vector_2d> tri_array;
		for (int ni = 0; ni < v_size; ni++)
		{
			tri_array.push_back(vector_2d(v_src[ni].x, v_src[ni].z));
			if (v_src[ni].z > max_z)
			{
				max_ni = 0;
				max_z = v_src[ni].z;
			}
		}

		int in_triangle_start_mode = 0;
		vector_2d prev_pos = tri_array[(max_ni - 1 + v_size) % v_size];
		vector_2d next_pos = tri_array[(max_ni + 1 + v_size) % v_size];
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
		geo_polygon->m_triangle_index.clear();
		geo_polygon->m_triangle_index.resize((v_size - 2) * 3);
		v_triangulation_algorithm.triangulation_polygon(tri_array.begin()._Myptr, v_size, geo_polygon->m_triangle_index.begin()._Myptr);
	}

	void geometry_factory::create_fill_polygon(geometry_polygon* geo_polygon, vector_3d* v_src, int v_size, ulong v_line_color, ulong v_fill_color)
	{
		generate_polygon(geo_polygon, v_src, v_size, v_line_color, v_fill_color);

		vector_3d vec_min = geo_polygon->m_render_aabb.get_min();
		vector_3d vec_max = geo_polygon->m_render_aabb.get_max();
		vector_3d vec_collapse = (vec_min + vec_max) * 0.5;

		matrix_4d v_collapse_mat = collapse_geometry_polygon(geo_polygon, vec_collapse);
		geo_polygon->set_collapse(v_collapse_mat);

	}

	void geometry_factory::create_fill_polygon_cart(geometry_polygon* geo_polygon, vector_3d* v_src, int v_size, ulong v_line_color, ulong v_fill_color)
	{
		geo_polygon->m_vertex_array.clear();
		geo_polygon->m_vertex_array.resize(v_size);

		geo_polygon->m_line_array.clear();
		geo_polygon->m_line_array.resize(v_size);

		vector_3d vec = v_src[0];
		geo_polygon->m_line_array[0].x = vec.x;
		geo_polygon->m_line_array[0].y = vec.y;
		geo_polygon->m_line_array[0].z = vec.z;
		geo_polygon->m_line_array[0].color = v_line_color;

		geo_polygon->m_vertex_array[0].x = vec.x;
		geo_polygon->m_vertex_array[0].y = vec.y;
		geo_polygon->m_vertex_array[0].z = vec.z;
		geo_polygon->m_vertex_array[0].color = v_fill_color;

		vector_3d vec_min(vec.x, vec.y, vec.z);
		vector_3d vec_max(vec.x, vec.y, vec.z);

		for (int ni = 1; ni < v_size; ni++)
		{
			vec = v_src[ni];
			geo_polygon->m_line_array[ni].x = vec.x;
			geo_polygon->m_line_array[ni].y = vec.y;
			geo_polygon->m_line_array[ni].z = vec.z;
			geo_polygon->m_line_array[ni].color = v_line_color;

			geo_polygon->m_vertex_array[ni].x = vec.x;
			geo_polygon->m_vertex_array[ni].y = vec.y;
			geo_polygon->m_vertex_array[ni].z = vec.z;
			geo_polygon->m_vertex_array[ni].color = v_fill_color;

			vec_min.x = min(vec_min.x, vec.x);
			vec_min.y = min(vec_min.y, vec.y);
			vec_min.z = min(vec_min.z, vec.z);

			vec_max.x = max(vec_max.x, vec.x);
			vec_max.y = max(vec_max.y, vec.y);
			vec_max.z = max(vec_max.z, vec.z);
		}
		geo_polygon->m_render_aabb.set_extent(vec_min, vec_max);

		generate_triangle_list_cart(geo_polygon, v_src, v_size);
	}

	geometry_polygon* geometry_factory::create_fill_circle(vector_3d& v_center, double v_radius, ulong v_line_color, ulong v_fill_color, ulong v_segment)
	{
		geometry_polygon* geo_polygon = new geometry_polygon;

		int v_vertex_size = v_segment + 1;
		vector_3d* v_circle_points = new vector_3d[v_vertex_size];
		shape_creator::create_circle_not_clamp(v_circle_points, v_center, v_radius, v_segment);
		v_circle_points[v_segment] = v_circle_points[0];

		generate_polygon(geo_polygon, v_circle_points, v_vertex_size, v_line_color, v_fill_color);

		vector_3d vec_min = geo_polygon->m_render_aabb.get_min();
		vector_3d vec_max = geo_polygon->m_render_aabb.get_max();
		vector_3d vec_collapse = (vec_min + vec_max) * 0.5;

		matrix_4d v_collapse_mat = collapse_geometry_polygon(geo_polygon, vec_collapse);
		geo_polygon->set_collapse(v_collapse_mat);

		return geo_polygon;
	}

	geometry_polygon* geometry_factory::create_fill_circle_cart(vector_3d& v_center, double v_radius, ulong v_line_color, ulong v_fill_color, ulong v_segment)
	{
		geometry_polygon* geo_polygon = new geometry_polygon;

		int v_vertex_size = v_segment + 1;
		vector_3d* v_circle_points = new vector_3d[v_vertex_size];
		shape_creator::create_circle_on_plane(v_circle_points, v_center, v_radius, v_segment);
		v_circle_points[v_segment] = v_circle_points[0];

		create_fill_polygon_cart(geo_polygon, v_circle_points, v_vertex_size, v_line_color, v_fill_color);

		return geo_polygon;
	}

	void geometry_factory::create_fill_circle(geometry_polygon* geo_polygon, vector_3d& v_center, double v_radius, ulong v_line_color, ulong v_fill_color, ulong v_segment)
	{
		int v_vertex_size = v_segment + 1;
		vector_3d* v_circle_points = new vector_3d[v_vertex_size];
		shape_creator::create_circle_not_clamp(v_circle_points, v_center, v_radius, v_segment);
		v_circle_points[v_segment] = v_circle_points[0];

		generate_polygon(geo_polygon, v_circle_points, v_vertex_size, v_line_color, v_fill_color);

		vector_3d vec_min = geo_polygon->m_render_aabb.get_min();
		vector_3d vec_max = geo_polygon->m_render_aabb.get_max();
		vector_3d vec_collapse = (vec_min + vec_max) * 0.5;

		matrix_4d v_collapse_mat = collapse_geometry_polygon(geo_polygon, vec_collapse);
		geo_polygon->set_collapse(v_collapse_mat);
	}

	void geometry_factory::create_fill_circle_cart(geometry_polygon* geo_polygon, vector_3d& v_center, double v_radius, ulong v_line_color, ulong v_fill_color, ulong v_segment)
	{
		int v_vertex_size = v_segment + 1;
		vector_3d* v_circle_points = new vector_3d[v_vertex_size];
		shape_creator::create_circle_on_plane(v_circle_points, v_center, v_radius, v_segment);
		v_circle_points[v_segment] = v_circle_points[0];

		create_fill_polygon_cart(geo_polygon, v_circle_points, v_vertex_size, v_line_color, v_fill_color);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//// 创建片状的矢量
	//// 输入为 geocentric坐标下的点集，geocentric坐标系下的点集的中心点位置, 宽度，颜色
	//// 输出为 RP坐标系中的模型和 
	////////////////////////////////////////////////////////////////////////////////////////////////
	//void geometry_factory::create_line_mesh( OUT geometry_line_flake* geo_mesh, IN coordinates_array& in_vec_array, IN vector_3d& center_geo, IN double line_width, IN ulong line_color )
	//{
	//	int n_vector_count = in_vec_array.size();
	//	if ( n_vector_count < 2 )
	//	{
	//		return;
	//	}

	//	if ( n_vector_count == 2)
	//	{
	//		vector3<double> A = in_vec_array[0];
	//		vector3<double> B = in_vec_array[1];
	//		vector3<double> normal = vector3<double>::normalize(vector3<double>::cross( A, B ));

	//		vector3<double> v1,v2,v3,v4;

	//		v1 = A - normal * line_width;
	//		v2 = A + normal * line_width;
	//		v3 = B - normal * line_width;
	//		v4 = B + normal * line_width;

	//		vector3<double> tangent =  vector3<double>::normalize(B - A);

	//		push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent, line_color );
	//		push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent, line_color );

	//		push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent, line_color );
	//		push_tri_strip_point( geo_mesh->m_vertex_array, v3 - center_geo, -tangent, line_color );

	//		push_tri_strip_point( geo_mesh->m_vertex_array, v4 - center_geo,  tangent, line_color );
	//		push_tri_strip_point( geo_mesh->m_vertex_array, v4 - center_geo,  tangent, line_color );

	//		return;
	//	}

	//	// n_vector_count >= 3
	//	{
	//		vector3<double> A = in_vec_array[0];
	//		vector3<double> B = in_vec_array[1];
	//		vector3<double> normal = vector3<double>::normalize(vector3<double>::cross( A, B ));

	//		vector3<double> v1,v2;
	//		v1 = A - normal * line_width;
	//		v2 = A + normal * line_width;

	//		vector3<double> tangent =  vector3<double>::normalize(B - A);

	//		push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent, line_color );
	//		push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent, line_color );
	//		push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent, line_color );
	//	}			


	//	for ( int ni = 1; ni< n_vector_count - 1; ni++ )
	//	{
	//		vector3<double> A  = in_vec_array[ni - 1];
	//		vector3<double> B  = in_vec_array[ni];
	//		vector3<double> C  = in_vec_array[ni + 1];

	//		// normal
	//		vector3<double> normal1 = vector3<double>::normalize(vector3<double>::cross( A, B ));
	//		vector3<double> normal2 = vector3<double>::normalize(vector3<double>::cross( B, C ));
	//		vector3<double> normal  = vector3<double>::normalize( ( normal1 + normal2 ) * 0.5 );
	//		vector3<double> v_temp_normal = vector3<double>::normalize( A - B );
	//		double dot_val = vector3<double>::dot( v_temp_normal, normal );
	//		if ( dot_val < 0.0 )
	//		{
	//			dot_val = vector3<double>::dot( v_temp_normal, -normal );
	//		}

	//		double v_width_scale;

	//		if ( math<double>::acos_( dot_val ) < 1 * RADIAN ) 
	//		{
	//			v_width_scale = 1.0;
	//		}
	//		else
	//		{
	//			v_width_scale = 1.0 / math<double>::sin_( math<double>::acos_( dot_val ) );
	//		}

	//		

	//		vector3<double> v1,v2;
	//		v1 = B - normal * v_width_scale * line_width;
	//		v2 = B + normal * v_width_scale * line_width;

	//		vector3<double> tangent1 =  vector3<double>::normalize( B - A );
	//		vector3<double> tangent2 =  vector3<double>::normalize( C - B );
	//		vector3<double> tangent3 =  vector3<double>::normalize( C - A );
	//		


	//		//push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent1, line_color );
	//		//push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent1, line_color );
	//		//push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent1, line_color );
	//		//push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent2, line_color );
	//		//push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent2, line_color );
	//		//push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent2, line_color );

	//		push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent1, line_color );
	//		push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent1, line_color );
	//		push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  -tangent3, line_color );
	//		push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent3, line_color );
	//		push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo, -tangent2, line_color );
	//		push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent2, line_color );

	//		//push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent1, line_color );
	//		//push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent2, line_color );
	//		//push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent2, line_color );
	//		//push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent2, line_color );

	//	}

	//	{
	//		vector3<double> A = in_vec_array[n_vector_count - 2];
	//		vector3<double> B = in_vec_array[n_vector_count - 1];
	//		vector3<double> normal = vector3<double>::normalize(vector3<double>::cross( A, B ));

	//		vector3<double> v1,v2;
	//		v1 = B - normal * line_width;
	//		v2 = B + normal * line_width;

	//		vector3<double> tangent =  vector3<double>::normalize(B - A);


	//		push_tri_strip_point( geo_mesh->m_vertex_array, v1 - center_geo, -tangent, line_color );
	//		push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent, line_color );
	//		push_tri_strip_point( geo_mesh->m_vertex_array, v2 - center_geo,  tangent, line_color );
	//	}
	//}

	//void geometry_factory::push_tri_strip_point( std::vector<position_normal_color>& triangle_list, vector3<double>& pt, vector3<double>& normal, ulong color )
	//{
	//	position_normal_color vec;
	//	vec.x = pt.x;
	//	vec.y = pt.y;
	//	vec.z = pt.z;
	//	vec.nx = normal.x;
	//	vec.ny = normal.y;
	//	vec.nz = normal.z;
	//	vec.color = color;
	//	triangle_list.push_back( vec );
	//}

	/////////////////////////////////////////////////////////////////////////////
	//void geometry_factory::generate_line_mesh( geometry_mesh_ex* geo_mesh, IN coordinates_array& in_vec_array, IN vector_3d& center_geo, IN double line_width, IN ulong line_color, IN double v_height, IN double v_lower_height )
	//{
	//	// geo_mesh->set_shadow_color( v_color );

	//	coordinates_array v_vertex_array;
	//	coordinates_array v_upper_coords_array;
	//	coordinates_array v_lower_coords_array;

	//	int v_size = in_vec_array.size();
	//	double v_width = line_width;

	//	// 两个点的情况
	//	if ( v_size == 2 )
	//	{	
	//		vector3<double> pos_geo1 = in_vec_array[0];
	//		vector3<double> pos_geo2 = in_vec_array[1];
	//		vector3<double> direction = vector3<double>::cross( pos_geo1, pos_geo2 );
	//		direction.normalize();

	//		// 第一个点
	//		vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * v_height;

	//		v_upper_coords_array.push_back( pos_geo1_scaled - direction * v_width );
	//		v_upper_coords_array.push_back( pos_geo1_scaled + direction * v_width );

	//		pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * v_lower_height;

	//		v_lower_coords_array.push_back( pos_geo1_scaled - direction * v_width );
	//		v_lower_coords_array.push_back( pos_geo1_scaled + direction * v_width );

	//		// 第二个点
	//		vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_height;

	//		v_upper_coords_array.push_back( pos_geo2_scaled - direction * v_width );
	//		v_upper_coords_array.push_back( pos_geo2_scaled + direction * v_width );

	//		pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_lower_height;

	//		v_lower_coords_array.push_back( pos_geo2_scaled - direction * v_width );
	//		v_lower_coords_array.push_back( pos_geo2_scaled + direction * v_width );

	//		v_vertex_array.insert( v_vertex_array.end(), v_upper_coords_array.begin(), v_upper_coords_array.end() );
	//		v_vertex_array.insert( v_vertex_array.end(), v_lower_coords_array.begin(), v_lower_coords_array.end() );

	//		insert_vertex_to_line_mesh( geo_mesh, v_size, v_vertex_array, out_array, v_color );

	//		return;
	//	}


	//	// 三个点以上的情况
	//	// 第一个点
	//	{
	//		vector3<double> pos_geo1 = in_vec_array[0];
	//		vector3<double> pos_geo2 = in_vec_array[1];
	//		vector3<double> direction = vector3<double>::cross( pos_geo1, pos_geo2 );
	//		direction.normalize();

	//		vector3<double> pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * v_height;

	//		v_upper_coords_array.push_back( pos_geo1_scaled - direction * v_width );
	//		v_upper_coords_array.push_back( pos_geo1_scaled + direction * v_width );

	//		pos_geo1_scaled = vector3<double>::normalize(pos_geo1) * v_lower_height;

	//		v_lower_coords_array.push_back( pos_geo1_scaled - direction * v_width );
	//		v_lower_coords_array.push_back( pos_geo1_scaled + direction * v_width );
	//	}	

	//	for( int i = 1; i < int(v_size - 1); i ++ )
	//	{
	//		vector3<double> pos_geo1 = in_vec_array[i-1];
	//		vector3<double> pos_geo2 = in_vec_array[i];
	//		vector3<double> pos_geo3 = in_vec_array[i+1];

	//		// direction
	//		vector3<double> direction1 = vector3<double>::cross( pos_geo1, pos_geo2 );
	//		direction1.normalize();

	//		vector3<double> direction2 = vector3<double>::cross( pos_geo2, pos_geo3 );
	//		direction2.normalize();

	//		vector3<double> direction = ( direction1 + direction2 ) * 0.5;
	//		direction.normalize();

	//		vector3<double> v_temp_normal = vector3<double>::normalize( pos_geo1 - pos_geo2 );
	//		double dot_val = vector3<double>::dot( v_temp_normal, direction );
	//		if ( dot_val < 0.0 )
	//		{
	//			dot_val = vector3<double>::dot( v_temp_normal, -direction );
	//		}

	//		double width;
	//		if ( math<double>::acos_( dot_val ) < 1 * math_d::s_deg_to_rad ) 
	//		{
	//			width = v_width;
	//		}
	//		else
	//		{
	//			width = v_width / math<double>::sin_( math<double>::acos_( dot_val ) );
	//		}

	//		vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_height;
	//		v_upper_coords_array.push_back( pos_geo2_scaled - direction * width );
	//		v_upper_coords_array.push_back( pos_geo2_scaled + direction * width );

	//		pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_lower_height;

	//		v_lower_coords_array.push_back( pos_geo2_scaled - direction * width );
	//		v_lower_coords_array.push_back( pos_geo2_scaled + direction * width );
	//	}

	//	// 最后一个点
	//	{
	//		vector3<double> pos_geo1 = in_vec_array[v_size - 2];
	//		vector3<double> pos_geo2 = in_vec_array[v_size - 1];

	//		vector3<double> direction = vector3<double>::cross( pos_geo1, pos_geo2 );
	//		direction.normalize();

	//		vector3<double> pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_height;

	//		v_upper_coords_array.push_back( pos_geo2_scaled - direction * v_width );
	//		v_upper_coords_array.push_back( pos_geo2_scaled + direction * v_width );

	//		pos_geo2_scaled = vector3<double>::normalize(pos_geo2) * v_lower_height;

	//		v_lower_coords_array.push_back( pos_geo2_scaled - direction * v_width );
	//		v_lower_coords_array.push_back( pos_geo2_scaled + direction * v_width );
	//	}

	//	v_vertex_array.insert( v_vertex_array.end(), v_upper_coords_array.begin(), v_upper_coords_array.end() );
	//	v_vertex_array.insert( v_vertex_array.end(), v_lower_coords_array.begin(), v_lower_coords_array.end() );

	//	insert_vertex_to_line_mesh( geo_mesh, v_size, v_vertex_array, out_array, v_color );		
	//}



	//void geometry_factory::insert_vertex_to_line_mesh( geometry_mesh_ex* v_mesh, ulong size, coordinates_array& in_array, coordinates_array& out_array, ulong v_color )
	//{
	//	ulong v_size = size<<1;
	//	ulong start_index = out_array.size();

	//	for ( int i = start_index;i < (int)(start_index + v_size - 3); i += 2 )
	//	{
	//		v_mesh->m_index_array.push_back( i );
	//		v_mesh->m_index_array.push_back( i + 3 );
	//		v_mesh->m_index_array.push_back( i + 1 );

	//		v_mesh->m_index_array.push_back( i );
	//		v_mesh->m_index_array.push_back( i + 2 );
	//		v_mesh->m_index_array.push_back( i + 3 );
	//	}

	//	for ( int i = start_index;i < (int)(start_index + v_size - 3); i += 2 )
	//	{
	//		v_mesh->m_index_array.push_back( i + v_size );
	//		v_mesh->m_index_array.push_back( i + 1 + v_size );
	//		v_mesh->m_index_array.push_back( i + 3 + v_size );

	//		v_mesh->m_index_array.push_back( i + v_size);
	//		v_mesh->m_index_array.push_back( i + 3 + v_size );
	//		v_mesh->m_index_array.push_back( i + 2 + v_size );			
	//	}

	//	for ( int i = start_index;i < (int)(start_index + v_size - 3); i += 2 )
	//	{
	//		v_mesh->m_index_array.push_back( i );
	//		v_mesh->m_index_array.push_back( i + v_size );
	//		v_mesh->m_index_array.push_back( i + 2 + v_size );

	//		v_mesh->m_index_array.push_back( i );
	//		v_mesh->m_index_array.push_back( i + 2 + v_size );
	//		v_mesh->m_index_array.push_back( i + 2 );

	//		v_mesh->m_index_array.push_back( i + 1 );
	//		v_mesh->m_index_array.push_back( i + 3 + v_size );
	//		v_mesh->m_index_array.push_back( i + 1 + v_size );


	//		v_mesh->m_index_array.push_back( i + 1 );
	//		v_mesh->m_index_array.push_back( i + 3 );
	//		v_mesh->m_index_array.push_back( i + 3 + v_size );		
	//	}

	//	// 前后两个面
	//	v_mesh->m_index_array.push_back( start_index );
	//	v_mesh->m_index_array.push_back( start_index + 1 + v_size );
	//	v_mesh->m_index_array.push_back( start_index + v_size );

	//	v_mesh->m_index_array.push_back( start_index );
	//	v_mesh->m_index_array.push_back( start_index + 1 );
	//	v_mesh->m_index_array.push_back( start_index + 1 + v_size );

	//	v_mesh->m_index_array.push_back( start_index + v_size - 1 );
	//	v_mesh->m_index_array.push_back( start_index + 2 * v_size - 2 );
	//	v_mesh->m_index_array.push_back( start_index + 2 * v_size - 1 );

	//	v_mesh->m_index_array.push_back( start_index + v_size - 1 );
	//	v_mesh->m_index_array.push_back( start_index + v_size - 2 );
	//	v_mesh->m_index_array.push_back( start_index + 2 * v_size - 2 );

	//	for ( int i = 0; i < (int)in_array.size(); i ++ )
	//	{
	//		v_mesh->m_render_aabb.combine( in_array[i] );
	//	}
	//	//v_mesh->m_center = v_mesh->m_render_aabb.center();

	//	//for ( int i = 0; i < (int)in_array.size(); i ++ )
	//	//{
	//	//	position_color pc;
	//	//	pc.x = in_array[i].x;
	//	//	pc.y = in_array[i].y;
	//	//	pc.z = in_array[i].z;
	//	//	pc.color = v_color;
	//	//	v_mesh->m_vertex_array.push_back( pc );
	//	//}

	//	out_array.insert( out_array.end(), in_array.begin(), in_array.end() );
	//}
}