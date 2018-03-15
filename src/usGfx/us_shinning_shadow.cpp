///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shinning_shadow.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shinning_shadow class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_shinning_shadow.h"

namespace uniscope_globe
{
	shinning_shadow::shinning_shadow()
	{
		m_d3d_mesh = NULL;
		m_valid = false;
		m_time_stamp = 0;
	}

	shinning_shadow::~shinning_shadow()
	{
		destroy();
	}

	long shinning_shadow::create( render_device* device )
	{
		if ( m_vertex_array.size() == 0  || m_index_array.size() == 0 )
			return 0;

		if ( m_valid )
			return 0;
		assert(!m_vertex_array.empty());
		assert(!m_index_array.empty());

		raw_buffer vectex_buffer( (byte*)&m_vertex_array[0], m_vertex_array.size() * sizeof( float ) * 3 );
		raw_buffer indices_buffer( (byte*)&m_index_array[0], m_index_array.size() * sizeof( ushort ) );

		// create mesh
		m_mem_size = device->create_mesh_from_buffer (  &vectex_buffer, 
														&indices_buffer,
														NULL,
														m_vertex_array.size(),
														m_index_array.size(),
														D3DFVF_XYZ,
														(void**)&m_d3d_mesh );

		m_valid = true;

		m_vertex_array.clear();
		m_index_array.clear();

		return m_mem_size;
	}

	bool shinning_shadow::destroy( void )
	{
		AUTO_RELEASE(m_d3d_mesh);
		return true;
	}

	bool shinning_shadow::refresh( void )
	{
		if( m_vertex_array.size() == 0  || m_index_array.size() == 0 ) return true;
		if(!m_valid)
			singleton_common_manager_group::instance().get_hardware_creator()->insert_mesh( this );
		return true;
	}

	void shinning_shadow::draw( render_argument* args )
	{
		if(!m_d3d_mesh) return;

		d3d9_effect_common_mesh* v_mesh_render = (d3d9_effect_common_mesh*)args->m_render;

		matrix_4d v_mat = m_world_matrix;
		v_mat.m41 -= cartesian_coords::s_reference_position_geo.x;
		v_mat.m42 -= cartesian_coords::s_reference_position_geo.y;
		v_mat.m43 -= cartesian_coords::s_reference_position_geo.z;

		v_mesh_render->push_transform();
		v_mesh_render->multiply_transform(  matrix_4f( (double*)v_mat.m ) );
		v_mesh_render->draw( (void*)m_d3d_mesh, 0 );
		v_mesh_render->pop_transform();
	}

	//void shinning_shadow::draw( render_argument* args )
	//{
	//	if(!m_d3d_mesh) return;

	//	matrix_4d v_mat = m_world_matrix;
	//	v_mat.m41 -= cartesian_coords::s_reference_position_geo.x;
	//	v_mat.m42 -= cartesian_coords::s_reference_position_geo.y;
	//	v_mat.m43 -= cartesian_coords::s_reference_position_geo.z;

	//	args->m_device->push_transform( );
	//	args->m_device->multiply_transform( matrix_4f( (double*)v_mat.m ) );
	//	m_d3d_mesh->DrawSubset( 0 );
	//	args->m_device->pop_transform();
	//}

	void shinning_shadow::create_from_mesh( building_usd_buffer* parent_mesh, vector4<double>& in_light, ulong v_stamp )
	{
		parent_mesh->generate_normals();

		parent_mesh->generate_topological();
		////
		//// 计算面的可见性
		////
		//create_facing_array( parent_mesh->m_normals, parent_mesh->m_normals.size(), in_light );

		////
		//// 计算阴影体侧面
		////
		//create_silhouette_triangles( parent_mesh->m_lines );

		////
		//// 计算阴影体上下盖
		////
		//create_cap_triangles( (ushort*)parent_mesh->m_index_buffer, parent_mesh->m_index_head.m_face_num );

		////
		//// 产生阴影体
		////
		//generate_volume( (float*)parent_mesh->m_vertex_buffer, parent_mesh->m_vertex_head.m_stride >> 2 , in_light );

		generate_indices( parent_mesh->m_normals, parent_mesh->m_lines, (ushort*)parent_mesh->m_index_buffer, parent_mesh->m_vertex_head.m_num, in_light );

		generate_vertices( (float*)parent_mesh->m_vertex_buffer, parent_mesh->m_vertex_head.m_num, parent_mesh->m_vertex_head.m_stride >> 2, in_light );

		m_time_stamp = v_stamp;
	}	

	void shinning_shadow::generate_vertices( float* in_vertices, ulong num_of_vertices, long stride, vector4<double>& in_light )
	{
		m_vertex_array.resize( num_of_vertices * 2 );

		vector3<float> v_light_dir( (double*)&in_light );
		double v_shadow_length = vector3<float>::dot( v_light_dir, -vector3<float>::s_unit_y );

		for ( int ni = 0; ni < (int)num_of_vertices; ni++ )
		{
			vector3<float> v_extruded_point = *((vector_3f*)(in_vertices + ni * stride) );
			double v_length = v_extruded_point.y / v_shadow_length + 100.010;

			m_vertex_array[ni] = v_extruded_point;
			m_vertex_array[ni+num_of_vertices] = v_extruded_point + v_light_dir  * v_length;
		}
	}

	void shinning_shadow::generate_indices( face_normal_array& in_normal_array, edge_array& in_edge_array, ushort* triangle, ulong num_of_vertices, vector4<double>& in_light )
	{
		ulong num_of_triangles = in_normal_array.size();
		m_facing_array.clear();
		m_facing_array.resize( num_of_triangles );

		m_index_array.resize( (num_of_triangles + in_edge_array.size()) * 6 );

		// check face visible & create_cap_triangles
		ulong v_cursor = 0;
		for ( int i = 0; i < (int)num_of_triangles; i++ )
		{
			if( (	in_normal_array[i].a * in_light.x +
					in_normal_array[i].b * in_light.y +
					in_normal_array[i].c * in_light.z +
					in_normal_array[i].d * in_light.w ) <= 0.0 )
			{
				m_facing_array[i] = 0x01;	

				int a = triangle[i * 3];
				int b = triangle[i * 3 + 1];
				int c = triangle[i * 3 + 2];

				m_index_array[v_cursor++] = a;
				m_index_array[v_cursor++] = b;
				m_index_array[v_cursor++] = c;

				m_index_array[v_cursor++] = a + num_of_vertices;
				m_index_array[v_cursor++] = c + num_of_vertices;
				m_index_array[v_cursor++] = b + num_of_vertices;
			}
			else
			{
				m_facing_array[i] = 0;
			}
		}


		// create_silhouette_triangles
		for ( int i = 0; i < (int)in_edge_array.size(); i ++ )
		{

			byte f1 = ( in_edge_array[i].left_poly == -1 ) ? 0x00 : m_facing_array[ in_edge_array[i].left_poly ];
			byte f2 = ( in_edge_array[i].right_poly == -1 ) ? 0x00 : m_facing_array[ in_edge_array[i].right_poly ];

			if( f1 != f2 )
			{
				int i1 = in_edge_array[i].start_point;
				int i2 = in_edge_array[i].end_point;

				if( f1 == 0x01 )
				{
					m_index_array[v_cursor++] = i1;
					m_index_array[v_cursor++] = i2;
					m_index_array[v_cursor++] = i2 + num_of_vertices;

					m_index_array[v_cursor++] = i1;
					m_index_array[v_cursor++] = i2 + num_of_vertices;
					m_index_array[v_cursor++] = i1 + num_of_vertices;
				}
				else
				{
					m_index_array[v_cursor++] = i1;
					m_index_array[v_cursor++] = i1 + num_of_vertices;
					m_index_array[v_cursor++] = i2 + num_of_vertices;

					m_index_array[v_cursor++] = i1;
					m_index_array[v_cursor++] = i2 + num_of_vertices;
					m_index_array[v_cursor++] = i2;
				}
			}
		}
		m_index_array.resize(v_cursor);
	}

	//////////////////////////////////////////////////////////////////////////
	//void shinning_shadow::create_cap_triangles( ushort* triangle, int num_of_triangles )
	//{
	//	for ( int i = 0; i < num_of_triangles; i++ )
	//	{
	//		if( m_facing_array[i] != 0x01 )
	//			continue;

	//		int a = triangle[i * 3] * 2;
	//		int b = triangle[i * 3 + 1] * 2;
	//		int c = triangle[i * 3 + 2] * 2;

	//		vector3<int> tri;

	//		tri.x = a;
	//		tri.y = b;
	//		tri.z = c;

	//		m_temp_index_array.push_back( tri );

	//		tri.x = a + 1;
	//		tri.y = c + 1;
	//		tri.z = b + 1;

	//		m_temp_index_array.push_back( tri );

	//	}
	//}

	//void shinning_shadow::create_silhouette_triangles( edge_array& in_array )
	//{
	//	for ( int i = 0; i < in_array.size(); i ++ )
	//	{
	//		byte f1;
	//		if( in_array[i].left_poly == -1)
	//			f1 = 0x00;
	//		else
	//			f1 = m_facing_array[ in_array[i].left_poly ];

	//		byte f2;
	//		if( in_array[i].right_poly == -1)
	//			f2 = 0x00;
	//		else
	//			f2 = m_facing_array[ in_array[i].right_poly ];

	//		if( f1 != f2 )
	//		{
	//			int v1 = 2 * in_array[i].start_point;
	//			int v2 = 2 * in_array[i].end_point;

	//			if( f1 == 0x01 )
	//			{
	//				vector3<int> tri;

	//				tri.x = v1;
	//				tri.y = v2;
	//				tri.z = v2 + 1;

	//				m_temp_index_array.push_back( tri );

	//				tri.x = v1;
	//				tri.y = v2 + 1;
	//				tri.z = v1 + 1;

	//				m_temp_index_array.push_back( tri );

	//			}
	//			else
	//			{
	//				vector3<int> tri;

	//				tri.x = v1;
	//				tri.y = v1 + 1;
	//				tri.z = v2 + 1;

	//				m_temp_index_array.push_back( tri );

	//				tri.x = v1;
	//				tri.y = v2 + 1;
	//				tri.z = v2;

	//				m_temp_index_array.push_back( tri );

	//			}
	//		}
	//	}
	//}

	//void shinning_shadow::create_cap_triangles( ushort* triangle, int num_of_triangles )
	//{
	//	for ( int i = 0; i < num_of_triangles; i++ )
	//	{
	//		if( m_facing_array[i] != 0x01 )
	//			continue;

	//		int a = triangle[i * 3] * 2;
	//		int b = triangle[i * 3 + 1] * 2;
	//		int c = triangle[i * 3 + 2] * 2;

	//		vector3<int> tri;

	//		tri.x = a;
	//		tri.y = b;
	//		tri.z = c;

	//		m_temp_index_array.push_back( tri );

	//		tri.x = a + 1;
	//		tri.y = c + 1;
	//		tri.z = b + 1;

	//		m_temp_index_array.push_back( tri );

	//	}
	//}


	//void shinning_shadow::generate_volume( float* in_vertices, long stride, vector4<double> in_light )
	//{
	//	int num_of_triangles = m_temp_index_array.size();

	//	m_vertex_array.clear();

	//	m_reorder_map.clear();

	//	m_index_array.clear();

	//	m_index_array.resize( num_of_triangles * 3 );

	//	m_vertex_array.resize(num_of_triangles * 3);

	//	//for ( int i = 0; i < num_of_triangles; i++ )
	//	//{
	//	//	vector3<int> tri = m_temp_index_array[i];

	//	//	push_in_vertex_array( in_vertices, stride, in_light, trr



	//	for ( int i = 0; i < num_of_triangles; i++ )
	//	{
	//		vector3<int> tri = m_temp_index_array[i];
	//		int a, b, c;

	//		a = get_new_index( tri.x );
	//		if( a == -1 )
	//		{
	//			// 不存在
	//			a = m_vertex_array.size();

	//			m_reorder_map[tri.x] = a;

	//			push_in_vertex_array(in_vertices, stride, in_light, tri.x );

	//		}

	//		b = get_new_index( tri.y );
	//		if( b == -1 )
	//		{
	//			// 不存在
	//			b = m_vertex_array.size();

	//			m_reorder_map[tri.y] = b;

	//			push_in_vertex_array(in_vertices, stride, in_light, tri.y );

	//		}

	//		c = get_new_index( tri.z );
	//		if( c == -1 )
	//		{
	//			// 不存在
	//			c = m_vertex_array.size();

	//			m_reorder_map[tri.z] = c;

	//			push_in_vertex_array(in_vertices, stride, in_light, tri.z );
	//		}

	//		m_index_array[i*3] = a;
	//		m_index_array[i*3+1] = b;
	//		m_index_array[i*3+2] = c;
	//	}

	//	//if( m_vertex_array.size() > 65535 )
	//	//{
	//	//	int k = 0;			
	//	//}
	//}

	//void shinning_shadow::generate_volume( float* in_vertices, long stride, vector4<double> in_light )
	//{
	//	int num_of_triangles = m_temp_index_array.size();

	//	m_vertex_array.clear();

	//	m_reorder_map.clear();

	//	m_index_array.clear();

	//	m_index_array.resize( num_of_triangles * 3 );

	//	

	//	for ( int i = 0; i < num_of_triangles; i++ )
	//	{
	//		vector3<int> tri = m_temp_index_array[i];
	//		int a, b, c;

	//		a = get_new_index( tri.x );
	//		if( a == -1 )
	//		{
	//			// 不存在
	//			a = m_vertex_array.size();

	//			m_reorder_map[tri.x] = a;

	//			push_in_vertex_array(in_vertices, stride, in_light, tri.x );

	//		}

	//		b = get_new_index( tri.y );
	//		if( b == -1 )
	//		{
	//			// 不存在
	//			b = m_vertex_array.size();

	//			m_reorder_map[tri.y] = b;

	//			push_in_vertex_array(in_vertices, stride, in_light, tri.y );

	//		}

	//		c = get_new_index( tri.z );
	//		if( c == -1 )
	//		{
	//			// 不存在
	//			c = m_vertex_array.size();

	//			m_reorder_map[tri.z] = c;

	//			push_in_vertex_array(in_vertices, stride, in_light, tri.z );
	//		}

	//		m_index_array[i*3] = a;
	//		m_index_array[i*3+1] = b;
	//		m_index_array[i*3+2] = c;
	//	}

	//	//if( m_vertex_array.size() > 65535 )
	//	//{
	//	//	int k = 0;			
	//	//}
	//}

	int  shinning_shadow::get_new_index( int index )
	{
		reorder_map::iterator itr = m_reorder_map.find(index);

		if ( itr != m_reorder_map.end() ) return itr->second;

		return -1;
	}

	void shinning_shadow::push_in_vertex_array(  float* vertices, long stride, vector4<double> in_light, int old_index )
	{
		vector3<float> un_extruded_point = *((vector_3f*)(vertices + (old_index/2)*stride) );

		//vector3<float> un_extruded_point = vertices[ old_index / 2 ];
		// 判断奇偶性
		if( old_index % 2 != 0 )
		{
			// 平行光
			vector3<float> dir;
			dir.x = in_light.x;
			dir.y = in_light.y;
			dir.z = in_light.z;

			double val = vector3<float>::dot( dir, -vector3<float>::s_unit_y );

			double len = un_extruded_point.y / val + 0.010;

			m_vertex_array.push_back( un_extruded_point + dir * len );
		}
		else
		{
			m_vertex_array.push_back( un_extruded_point /*+ dir * 0.001*/ );
		}
	}
}