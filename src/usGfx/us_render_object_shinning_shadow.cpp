///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_shinning_shadow.cpp
//  Author  : Uniscope Team
//  Modifier: Uniscope Team
//  Created :
//  Purpose : render_object_shinning_shadow class
//	Reference :
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_render_object_shinning_shadow.h"
#include <usGfx/us_d3d9_hardware_mesh_buffer.h>
#include <usGfx/us_d3d9_effect_common_mesh.h>

#include <usCore/us_render_argument.h>
#include <usCore/us_render_device.h>
#include <usCore/us_common_manager_group.h>
#include <usCore/us_hardware_creator.h>

#include <usUtil/us_raw_buffer.h>
#include <usUtil/us_cartesian_coords.h>

namespace uniscope_globe
{
	render_object_shinning_shadow::render_object_shinning_shadow(d3d9_hardware_mesh_buffer* v_mesh_buffer)
	{
		m_d3d_mesh = NULL;
		m_valid = false;
		m_time_stamp = 0;
		m_height_offset = 0.0;
		m_parent_mesh_buffer = v_mesh_buffer;

		m_32bit_index = false;
	}

	render_object_shinning_shadow::~render_object_shinning_shadow()
	{
		destroy();

		AUTO_DELETE(m_parent_mesh_buffer);
	}

	render_object_shinning_shadow* render_object_shinning_shadow::create_shared_instance(d3d9_hardware_mesh_buffer* v_mesh)
	{
		render_object_shinning_shadow* v_ro = new render_object_shinning_shadow(v_mesh);
		v_ro->add_ref();

		return v_ro;
	}

	long render_object_shinning_shadow::create(render_device* device)
	{
		if (m_vertex_array.size() == 0)
		{
			return 0;
		}

		if (m_valid)
		{
			return 0;
		}

		// create mesh
		if (m_32bit_index)
		{
			assert(!m_vertex_array.empty());
			assert(!m_index_array_32.empty());

			raw_buffer vectex_buffer((byte*)&m_vertex_array[0], m_vertex_array.size() * sizeof(position_normal));
			raw_buffer indices_buffer((byte*)&m_index_array_32[0], m_index_array_32.size() * sizeof(ulong));

			m_mem_size = device->create_mesh_from_buffer(&vectex_buffer,
				&indices_buffer,
				NULL,
				m_vertex_array.size(),
				m_index_array_32.size(),
				position_normal::fvf,
				D3DXMESH_MANAGED | D3DXMESH_32BIT,
				(void**)&m_d3d_mesh);
		}
		else
		{
			assert(!m_vertex_array.empty());
			assert(!m_index_array.empty());
			raw_buffer vectex_buffer((byte*)&m_vertex_array[0], m_vertex_array.size() * sizeof(position_normal));
			raw_buffer indices_buffer((byte*)&m_index_array[0], m_index_array.size() * sizeof(ushort));

			m_mem_size = device->create_mesh_from_buffer(&vectex_buffer,
				&indices_buffer,
				NULL,
				m_vertex_array.size(),
				m_index_array.size(),
				position_normal::fvf,
				(void**)&m_d3d_mesh);
		}

		m_shadow_object = this;

		m_valid = true;

		m_vertex_array.clear();
		m_index_array.clear();
		m_index_array_32.clear();

		return m_mem_size;
	}

	bool render_object_shinning_shadow::destroy(void)
	{
		AUTO_RELEASE(m_d3d_mesh);

		m_valid = false;

		return true;
	}

	bool render_object_shinning_shadow::create_shadow_object(render_argument* args)
	{
		if (m_parent_mesh_buffer == NULL) { return false; }

		if (this->get_time_stamp() == args->m_shadow_param.m_stamp) { return false; }

		this->destroy();

		m_parent_mesh_buffer->lock();

		//if ( m_parent_mesh_buffer->get_num_of_vertices() > 32767 )
		//    {
		//    m_32bit_index = true;
		//    }
		//else
		//    {
		//    m_32bit_index = false;
		//    }
		// 顶点个数超过65536时，会按m_32bit_index=true使用   [9/18/2017 mahc]
		m_32bit_index = m_parent_mesh_buffer->m_32bit_index;

		generate_indices(m_parent_mesh_buffer->get_face_normal(),
			m_parent_mesh_buffer->get_edge(),
			m_parent_mesh_buffer->get_index_buffer(),
			m_parent_mesh_buffer->get_num_of_vertices(),
			args->m_shadow_param.m_light);

		generate_vertices((float*)m_parent_mesh_buffer->get_vertex_buffer(),
			m_parent_mesh_buffer->get_num_of_vertices(),
			m_parent_mesh_buffer->get_stride() >> 2,
			args->m_shadow_param.m_light);

		m_time_stamp = args->m_shadow_param.m_stamp;

		m_parent_mesh_buffer->unlock();

		create(args->m_device);

		m_shadow_object = this;

		return true;
	}


	bool render_object_shinning_shadow::refresh(void)
	{
		if (m_vertex_array.size() == 0 || m_index_array.size() == 0) { return true; }
		if (!m_valid)
		{
			singleton_common_manager_group::instance().get_hardware_creator()->insert_mesh(this);
		}
		return true;
	}

	void render_object_shinning_shadow::draw(render_argument* args)
	{
		if (!m_d3d_mesh) { return; }

		d3d9_effect_common_mesh* v_mesh_render = (d3d9_effect_common_mesh*)args->m_render;

		matrix_4d v_mat = m_world_matrix;
		v_mat.m41 -= cartesian_coords::s_reference_position_geo.x;
		v_mat.m42 -= cartesian_coords::s_reference_position_geo.y;
		v_mat.m43 -= cartesian_coords::s_reference_position_geo.z;

		v_mesh_render->push_transform();
		v_mesh_render->multiply_transform(matrix_4f((double*)v_mat.m));
		v_mesh_render->draw((void*)m_d3d_mesh, 0);
		v_mesh_render->pop_transform();
	}

	void render_object_shinning_shadow::generate_vertices(float* in_vertices, ulong num_of_vertices, long stride, vector4<double>& in_light)
	{
		m_vertex_array.resize(num_of_vertices * 2);

		vector3<float> v_light_dir((double*)&in_light);
		double v_shadow_length = vector3<float>::dot(v_light_dir, -vector3<float>::s_unit_y);

		for (int ni = 0; ni < (int)num_of_vertices; ni++)
		{
			vector3<float> v_extruded_point = *((vector_3f*)(in_vertices + ni * stride));
			double v_length = (v_extruded_point.y + m_height_offset) / v_shadow_length + 100.010;

			m_vertex_array[ni].x = v_extruded_point.x;
			m_vertex_array[ni].y = v_extruded_point.y;
			m_vertex_array[ni].z = v_extruded_point.z;
			m_vertex_array[ni].nx = 0.0f;
			m_vertex_array[ni].ny = 0.0f;
			m_vertex_array[ni].nz = 0.0f;

			v_extruded_point = v_extruded_point + v_light_dir  * v_length;
			m_vertex_array[ni + num_of_vertices].x = v_extruded_point.x;
			m_vertex_array[ni + num_of_vertices].y = v_extruded_point.y;
			m_vertex_array[ni + num_of_vertices].z = v_extruded_point.z;
			m_vertex_array[ni + num_of_vertices].nx = 1.0f;
			m_vertex_array[ni + num_of_vertices].ny = 1.0f;
			m_vertex_array[ni + num_of_vertices].nz = 1.0f;
		}
	}

	void render_object_shinning_shadow::generate_indices(face_normal_array& in_normal_array, edge_array& in_edge_array, void* v_triangle, ulong num_of_vertices, vector4<double>& in_light)
	{
		ulong num_of_triangles = in_normal_array.size();
		m_facing_array.clear();
		m_facing_array.resize(num_of_triangles);

		if (m_32bit_index)
		{
			m_index_array_32.resize((num_of_triangles + in_edge_array.size()) * 6);

			ulong* triangle = (ulong*)(v_triangle);

			// check face visible & create_cap_triangles
			ulong v_cursor = 0;
			for (int i = 0; i < (int)num_of_triangles; i++)
			{
				if ((in_normal_array[i].a * in_light.x +
					in_normal_array[i].b * in_light.y +
					in_normal_array[i].c * in_light.z +
					in_normal_array[i].d * in_light.w) <= 0.0)
				{
					m_facing_array[i] = 0x01;

					int a = triangle[i * 3];
					int b = triangle[i * 3 + 1];
					int c = triangle[i * 3 + 2];

					m_index_array_32[v_cursor++] = a;
					m_index_array_32[v_cursor++] = b;
					m_index_array_32[v_cursor++] = c;

					m_index_array_32[v_cursor++] = a + num_of_vertices;
					m_index_array_32[v_cursor++] = c + num_of_vertices;
					m_index_array_32[v_cursor++] = b + num_of_vertices;
				}
				else
				{
					m_facing_array[i] = 0;
				}
			}

			// create_silhouette_triangles
			for (int i = 0; i < (int)in_edge_array.size(); i++)
			{

				byte f1 = (in_edge_array[i].left_poly == -1) ? 0x00 : m_facing_array[in_edge_array[i].left_poly];
				byte f2 = (in_edge_array[i].right_poly == -1) ? 0x00 : m_facing_array[in_edge_array[i].right_poly];

				if (f1 != f2)
				{
					int i1 = in_edge_array[i].start_point;
					int i2 = in_edge_array[i].end_point;

					if (f1 == 0x01)
					{
						m_index_array_32[v_cursor++] = i1;
						m_index_array_32[v_cursor++] = i2;
						m_index_array_32[v_cursor++] = i2 + num_of_vertices;

						m_index_array_32[v_cursor++] = i1;
						m_index_array_32[v_cursor++] = i2 + num_of_vertices;
						m_index_array_32[v_cursor++] = i1 + num_of_vertices;
					}
					else
					{
						m_index_array_32[v_cursor++] = i1;
						m_index_array_32[v_cursor++] = i1 + num_of_vertices;
						m_index_array_32[v_cursor++] = i2 + num_of_vertices;

						m_index_array_32[v_cursor++] = i1;
						m_index_array_32[v_cursor++] = i2 + num_of_vertices;
						m_index_array_32[v_cursor++] = i2;
					}
				}
			}
			m_index_array_32.resize(v_cursor);
		}
		// 16位顶点
		else
		{
			m_index_array.resize((num_of_triangles + in_edge_array.size()) * 6);

			ushort* triangle = (ushort*)(v_triangle);

			// check face visible & create_cap_triangles
			ulong v_cursor = 0;
			for (int i = 0; i < (int)num_of_triangles; i++)
			{
				if ((in_normal_array[i].a * in_light.x +
					in_normal_array[i].b * in_light.y +
					in_normal_array[i].c * in_light.z +
					in_normal_array[i].d * in_light.w) <= 0.0)
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
			for (int i = 0; i < (int)in_edge_array.size(); i++)
			{

				byte f1 = (in_edge_array[i].left_poly == -1) ? 0x00 : m_facing_array[in_edge_array[i].left_poly];
				byte f2 = (in_edge_array[i].right_poly == -1) ? 0x00 : m_facing_array[in_edge_array[i].right_poly];

				if (f1 != f2)
				{
					int i1 = in_edge_array[i].start_point;
					int i2 = in_edge_array[i].end_point;

					if (f1 == 0x01)
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
	}


	//void render_object_shinning_shadow::generate_indices( face_normal_array& in_normal_array, edge_array& in_edge_array, ushort* triangle, ulong num_of_vertices, vector4<double>& in_light )
	//{
	//	ulong num_of_triangles = in_normal_array.size();
	//	m_facing_array.clear();
	//	m_facing_array.resize( num_of_triangles );

	//	m_index_array.resize( (num_of_triangles + in_edge_array.size()) * 6 );

	//	// check face visible & create_cap_triangles
	//	ulong v_cursor = 0;
	//	for ( int i = 0; i < (int)num_of_triangles; i++ )
	//	{
	//		if( (	in_normal_array[i].a * in_light.x +
	//			in_normal_array[i].b * in_light.y +
	//			in_normal_array[i].c * in_light.z +
	//			in_normal_array[i].d * in_light.w ) <= 0.0 )
	//		{
	//			m_facing_array[i] = 0x01;

	//			int a = triangle[i * 3];
	//			int b = triangle[i * 3 + 1];
	//			int c = triangle[i * 3 + 2];

	//			m_index_array[v_cursor++] = a;
	//			m_index_array[v_cursor++] = b;
	//			m_index_array[v_cursor++] = c;

	//			m_index_array[v_cursor++] = a + num_of_vertices;
	//			m_index_array[v_cursor++] = c + num_of_vertices;
	//			m_index_array[v_cursor++] = b + num_of_vertices;
	//		}
	//		else
	//		{
	//			m_facing_array[i] = 0;
	//		}
	//	}


	//	// create_silhouette_triangles
	//	for ( int i = 0; i < (int)in_edge_array.size(); i ++ )
	//	{

	//		byte f1 = ( in_edge_array[i].left_poly == -1 ) ? 0x00 : m_facing_array[ in_edge_array[i].left_poly ];
	//		byte f2 = ( in_edge_array[i].right_poly == -1 ) ? 0x00 : m_facing_array[ in_edge_array[i].right_poly ];

	//		if( f1 != f2 )
	//		{
	//			int i1 = in_edge_array[i].start_point;
	//			int i2 = in_edge_array[i].end_point;

	//			if( f1 == 0x01 )
	//			{
	//				m_index_array[v_cursor++] = i1;
	//				m_index_array[v_cursor++] = i2;
	//				m_index_array[v_cursor++] = i2 + num_of_vertices;

	//				m_index_array[v_cursor++] = i1;
	//				m_index_array[v_cursor++] = i2 + num_of_vertices;
	//				m_index_array[v_cursor++] = i1 + num_of_vertices;
	//			}
	//			else
	//			{
	//				m_index_array[v_cursor++] = i1;
	//				m_index_array[v_cursor++] = i1 + num_of_vertices;
	//				m_index_array[v_cursor++] = i2 + num_of_vertices;

	//				m_index_array[v_cursor++] = i1;
	//				m_index_array[v_cursor++] = i2 + num_of_vertices;
	//				m_index_array[v_cursor++] = i2;
	//			}
	//		}
	//	}
	//	m_index_array.resize(v_cursor);
	//}


}