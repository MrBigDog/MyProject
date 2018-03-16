///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_building_usd_buffer_shadow_shadow_shadow.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : building_usd_buffer_shadow_shadow_shadow class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
////#include "stdafx.h"
#include "us_building_usd_buffer_shadow.h"

namespace uniscope_globe
{
	building_usd_buffer_shadow::building_usd_buffer_shadow(raw_buffer* v_stream)
		: building_usd_buffer(v_stream)
	{

	}

	building_usd_buffer_shadow::~building_usd_buffer_shadow(void)
	{

	}

	bool building_usd_buffer_shadow::generate_mesh(void)
	{
		//return true;
		if (m_vertex_head.m_fvf == position_texture::fvf)
		{
			generate_mesh_pos_tex();

		}
		else if (m_vertex_head.m_fvf == position_texture2::fvf)
		{
			generate_mesh_pos_tex2();
		}
		else if (m_vertex_head.m_fvf == position_color_texture::fvf)
		{
			generate_mesh_pos_dif_tex();
		}

		return true;
	}


	/*
	bool building_usd_buffer_shadow::generate_topological()
	{
		if( m_lines.size() > 0) return true;

		bldg_line_map v_line_map;
		bldg_line_map::iterator itr;

		int v_off = 0;
		int v1 = 0, v2 = 0, v3 = 0;
		bldg_point_pair v_pair1, v_pair2;

		for ( ulong i = 0 ; i < m_indice_head.m_num; i++ )
		{
			v_off = i * 3;
			v1 = m_indice_buffer[v_off];
			v2 = m_indice_buffer[v_off+1];
			v3 = m_indice_buffer[v_off+2];

			// the first line
			v_pair1 = make_pair( v1, v2 );
			itr = v_line_map.find( v_pair1 );
			if ( itr == v_line_map.end() )
			{
				v_pair2 = make_pair( v2, v1 );
				itr = v_line_map.find( v_pair2 );
				if ( itr == v_line_map.end() )
				{
					edge v_line;
					v_line.start_point = v1;
					v_line.end_point = v2;
					v_line.right_poly = i;
					m_lines.push_back( v_line );

					v_line_map[v_pair1] = m_lines.size() - 1;
				}
				else
				{
					m_lines[itr->second].left_poly = i;
				}
			}

			// the second line
			v_pair1 = make_pair( v2, v3 );
			itr = v_line_map.find( v_pair1 );
			if ( itr == v_line_map.end() )
			{
				v_pair2 = make_pair( v3, v2 );
				itr = v_line_map.find( v_pair2 );
				if ( itr == v_line_map.end() )
				{
					edge v_line;
					v_line.start_point = v2;
					v_line.end_point = v3;
					v_line.right_poly = i;
					m_lines.push_back( v_line );

					v_line_map[v_pair1] = m_lines.size() - 1;
				}
				else
				{
					m_lines[itr->second].left_poly = i;
				}
			}

			// the third line
			v_pair1 = make_pair( v3, v1 );
			itr = v_line_map.find( v_pair1 );
			if ( itr == v_line_map.end() )
			{
				v_pair2 = make_pair( v1, v3 );
				itr = v_line_map.find( v_pair2 );
				if ( itr == v_line_map.end() )
				{
					edge v_line;
					v_line.start_point = v3;
					v_line.end_point = v1;
					v_line.right_poly = i;
					m_lines.push_back( v_line );

					v_line_map[v_pair1] = m_lines.size() - 1;
				}
				else
				{
					m_lines[itr->second].left_poly = i;
				}
			}

		}

		// release
		v_line_map.clear();

		return true;
	}

	//*/

	bool building_usd_buffer_shadow::generate_mesh_pos_tex(void)
	{
		std::vector<position_texture> v_vertex_buffer;
		//position_texture v1, v2, v3;

		//v_vertex_buffer.push_back( )

		//ret_vertex_buffer = new position_texture[m_vertex_head.m_num];
		//m_vertex_buffer_size = sizeof( position_texture ) * m_vertex_head.m_num;
		return true;
	}

	bool building_usd_buffer_shadow::generate_mesh_pos_tex2(void)
	{
		//return true;
		std::vector<position_normal_texture2> v_new_vertex_array;
		std::vector<usx_tri_face_index> v_new_face_index;
		std::vector<ulong> v_new_attribute_array;
		ushort* v_face_indices = (ushort*)m_index_buffer;

		//
		// 添加退化四边形
		//
		edge_map v_edge_map;
		edge_triangle_map v_edge_triangle_map;
		triangle_normal_map v_triangle_normal_map;

		int v_number_of_faces = m_index_buffer_size / 6;
		for (int i = 0; i < (int)v_number_of_faces; i++)
		{

			ulong a = v_face_indices[i * 3];
			ulong b = v_face_indices[i * 3 + 1];
			ulong c = v_face_indices[i * 3 + 2];


			// 计算法矢
			vector3<float> v_normal = compute_normal(usx_tri_face_index(a, b, c));

			// 增加顶点
			position_normal_texture2 v1, v2, v3;
			v1 = read_vertex(a, v_normal);
			v2 = read_vertex(b, v_normal);
			v3 = read_vertex(c, v_normal);

			// 增加三角形
			usx_tri_face_index v_new_face;
			v_new_face.a = v_new_vertex_array.size();
			v_new_face.b = v_new_face.a + 1;
			v_new_face.c = v_new_face.b + 1;
			v_new_face_index.push_back(v_new_face);
			v_new_attribute_array.push_back(m_attribute_buffer[i]);
			ulong v_index_count = v_new_face_index.size() - 1;

			v_new_vertex_array.push_back(v1);
			v_new_vertex_array.push_back(v2);
			v_new_vertex_array.push_back(v3);


			//// [三角形-法矢]
			//v_triangle_normal_map.insert( make_pair( v_index_count, v_normal ) );

			//// [边-三角形]
			//v_edge_triangle_map.insert( make_pair( edge( v_new_face.a, v_new_face.b ), v_index_count ) );
			//v_edge_triangle_map.insert( make_pair( edge( v_new_face.b, v_new_face.c ), v_index_count ) );
			//v_edge_triangle_map.insert( make_pair( edge( v_new_face.c, v_new_face.a ), v_index_count ) );

			insert_triangle(edge(a, b), edge(v_new_face.a, v_new_face.b), v_edge_map, v_new_face_index, v_new_attribute_array);
			insert_triangle(edge(b, c), edge(v_new_face.b, v_new_face.c), v_edge_map, v_new_face_index, v_new_attribute_array);
			insert_triangle(edge(c, a), edge(v_new_face.c, v_new_face.a), v_edge_map, v_new_face_index, v_new_attribute_array);
		}


		// 拷贝三角形索引
		AUTO_DELETE(m_index_buffer);
		int v_num_of_triangles = v_new_face_index.size();
		m_index_buffer = new ulong[v_num_of_triangles * 3];
		m_index_buffer_size = sizeof(ulong) * v_num_of_triangles * 3;
		memcpy(m_index_buffer, &v_new_face_index[0], m_index_buffer_size);

		// 拷贝顶点
		AUTO_DELETE(m_vertex_buffer);
		int v_num_of_vertices = v_new_vertex_array.size();
		m_vertex_buffer = new position_normal_texture2[v_num_of_vertices];
		m_vertex_buffer_size = sizeof(position_normal_texture2) * v_num_of_vertices;
		memcpy(m_vertex_buffer, &v_new_vertex_array[0], m_vertex_buffer_size);

		//// 拷贝材质分组
		m_attribute_buffer = new ulong[v_num_of_triangles];
		m_attribute_buffer_size = sizeof(ulong) * v_num_of_triangles;
		memcpy(m_attribute_buffer, &v_new_attribute_array[0], m_attribute_buffer_size);

		m_vertex_head.m_num = v_num_of_vertices;
		m_vertex_head.m_fvf = position_normal_texture2::fvf;
		m_vertex_head.m_stride = position_normal_texture2::stride;

		m_index_head.m_bit_count = 32;
		m_index_head.m_face_num = v_num_of_triangles;

		return true;
	}

	position_normal_texture2 building_usd_buffer_shadow::read_vertex(ulong index, vector3<float>& in_normal)
	{
		position_normal_texture2 v;

		position_texture2* v_vertex_buffer = (position_texture2*)m_vertex_buffer;
		v.x = v_vertex_buffer[index].x;
		v.y = v_vertex_buffer[index].y;
		v.z = v_vertex_buffer[index].z;
		v.nx = in_normal.x;
		v.ny = in_normal.y;
		v.nz = in_normal.z;

		v.tu1 = v_vertex_buffer[index].tu1;
		v.tv1 = v_vertex_buffer[index].tv1;

		v.tu2 = v_vertex_buffer[index].tu2;
		v.tv2 = v_vertex_buffer[index].tv2;

		return v;
	}

	void building_usd_buffer_shadow::insert_triangle(edge old_e, edge new_e, edge_map& in_edge_map, std::vector<usx_tri_face_index>& in_index_array, std::vector<ulong>& in_attribute_array)
	{
		edge_map::iterator itr = in_edge_map.find(edge(old_e.second, old_e.first));
		if (itr != in_edge_map.end())
		{
			edge opposite_edge = itr->second;

			usx_tri_face_index v_new_face;

			v_new_face.a = new_e.first;
			v_new_face.b = opposite_edge.second;
			v_new_face.c = opposite_edge.first;
			in_index_array.push_back(v_new_face);
			in_attribute_array.push_back(0);

			v_new_face.a = new_e.first;
			v_new_face.b = opposite_edge.first;
			v_new_face.c = new_e.second;
			in_index_array.push_back(v_new_face);
			in_attribute_array.push_back(0);

			in_edge_map.erase(itr);
		}
		else
		{
			in_edge_map.insert(make_pair(old_e, new_e));
		}
	}

	vector3<float> building_usd_buffer_shadow::compute_normal(usx_tri_face_index& in_triangle)
	{
		vector3<float> a, b, c;

		position_texture2* v_vertex_buffer = (position_texture2*)m_vertex_buffer;
		a.x = v_vertex_buffer[in_triangle.a].x;
		a.y = v_vertex_buffer[in_triangle.a].y;
		a.z = v_vertex_buffer[in_triangle.a].z;

		b.x = v_vertex_buffer[in_triangle.b].x;
		b.y = v_vertex_buffer[in_triangle.b].y;
		b.z = v_vertex_buffer[in_triangle.b].z;

		c.x = v_vertex_buffer[in_triangle.c].x;
		c.y = v_vertex_buffer[in_triangle.c].y;
		c.z = v_vertex_buffer[in_triangle.c].z;

		vector3<float> tmp1 = b - a;
		vector3<float> tmp2 = c - a;
		vector3<float> normal = vector3<float>::cross(tmp1, tmp2);
		normal.normalize();

		return normal;
	}

	bool building_usd_buffer_shadow::generate_mesh_pos_dif_tex(void)
	{
		return true;
	}


}