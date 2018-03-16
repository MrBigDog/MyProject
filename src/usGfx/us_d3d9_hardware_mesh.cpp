///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_hardware_mesh.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_hardware_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
////#include "Stdafx.h"
#include "us_d3d9_hardware_mesh.h"
#include <usGfx/us_d3d9_effect_common_mesh.h>
#include <usGfx/us_d3d9_intersect_helper.h>

#include <usCore/us_common_manager_group.h>
#include <usCore/us_hardware_creator.h>
#include <usCore/us_hardware_texture.h>
#include <usCore/us_render_state.h>
#include <usCore/us_render_argument.h>
#include <usCore/us_render_device.h>

#include <d3d9.h>
#include <windows.h>

namespace uniscope_globe
{
	d3d9_hardware_mesh::d3d9_hardware_mesh(void)
	{
		m_rtti = US_RTTI_D3D9_HARDWARE_MESH;
		m_d3d_mesh = NULL;
		m_topology = NULL;
		m_texture_manager = NULL;

	}

	d3d9_hardware_mesh::~d3d9_hardware_mesh(void)
	{
		destroy();
	}

	long d3d9_hardware_mesh::create(render_device* in_device)
	{
		if (m_valid)
			return 0;

		if (m_d3d_mesh != NULL)
			return 0;

		d3d9_hardware_mesh_buffer* v_buffer = dynamic_cast<d3d9_hardware_mesh_buffer*>(this);

		m_mem_size = 0;

		//
		// 创建普通渲染对象
		//
		HRESULT hr = 0;
		if (FAILED(hr = D3DXCreateMeshFVF(v_buffer->get_num_of_triangles(),
			v_buffer->get_num_of_vertices(),
			D3DXMESH_MANAGED | (v_buffer->is_32bit_index() ? D3DXMESH_32BIT : 0x000),
			v_buffer->get_fvf(),
			(LPDIRECT3DDEVICE9)(in_device->get_device()),
			&m_d3d_mesh)))
		{
			return m_mem_size;
		}


		int vertexNum = m_d3d_mesh->GetNumVertices();
		int perVertexBytes = m_d3d_mesh->GetNumBytesPerVertex();
		//vertex
		void* v_vertices = NULL;
		hr = m_d3d_mesh->LockVertexBuffer(0, (void**)&v_vertices);
		{
			memcpy((byte*)v_vertices, v_buffer->get_vertex_buffer(), v_buffer->get_vertex_buffer_size());
		}
		hr = m_d3d_mesh->UnlockVertexBuffer();
		m_mem_size += v_buffer->get_vertex_buffer_size();

		void* v_indices = NULL;
		hr = m_d3d_mesh->LockIndexBuffer(0, (void**)&v_indices);
		{
			memcpy((byte*)v_indices, v_buffer->get_index_buffer(), v_buffer->get_index_buffer_size());
		}
		hr = m_d3d_mesh->UnlockIndexBuffer();
		m_mem_size += v_buffer->get_index_buffer_size();


		if (v_buffer->m_attribute_buffer != NULL)
		{
			DWORD* v_attribute;
			hr = m_d3d_mesh->LockAttributeBuffer(0, &v_attribute);
			{
				memcpy((byte*)v_attribute, v_buffer->get_attribute_buffer(), v_buffer->get_attribute_buffer_size());
			}
			hr = m_d3d_mesh->UnlockAttributeBuffer();
			m_mem_size += v_buffer->get_attribute_buffer_size();
		}

		m_d3d_mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);

		update_bound_box();

		clear_buffer();

		m_valid = true;

		return m_mem_size;
	}

	bool d3d9_hardware_mesh::destroy(void)
	{
		AUTO_RELEASE(m_d3d_mesh);
		for (int i = 0; i < (int)m_material_entry_array.size(); i++)
		{
			AUTO_RELEASE_SHARED_DATA(m_material_entry_array[i].m_diffuse);
			AUTO_RELEASE_SHARED_DATA(m_material_entry_array[i].m_self_illuminant);
			AUTO_RELEASE_SHARED_DATA(m_material_entry_array[i].m_specular);
			AUTO_RELEASE_SHARED_DATA(m_material_entry_array[i].m_bump);
		}
		m_material_entry_array.clear();

		clear_buffer();

		AUTO_DELETE(m_topology);

		return true;
	}

	//void d3d9_hardware_mesh::reset( render_device* in_device )
	//{
	//	US_LOCK_AUTO_MUTEX

	//	AUTO_RELEASE( m_d3d_mesh );

	//	for ( int i = 0; i < (int)m_material_entry_array.size(); i++ )
	//	{
	//		AUTO_RELEASE( m_material_entry_array[i].m_diffuse );
	//		AUTO_RELEASE( m_material_entry_array[i].m_self_illuminant );
	//		AUTO_RELEASE( m_material_entry_array[i].m_specular );
	//		AUTO_RELEASE( m_material_entry_array[i].m_bump );
	//	}

	//	m_material_entry_array.clear();

	//	m_texture_array.clear();

	//	m_valid = false;

	//	this->create( in_device );
	//}

	bool d3d9_hardware_mesh::refresh(void)
	{
		//US_LOCK_AUTO_MUTEX

		if (!m_valid)
		{
			singleton_common_manager_group::instance().get_hardware_creator()->insert_mesh(this);
			return false;
		}

		m_is_texture_ready = true;
		for (int i = 0; i < (int)m_material_entry_array.size(); i++)
		{
			if (m_material_entry_array[i].m_diffuse)
			{
				m_texture_array[i].m_diffuse = (hardware_texture*)m_material_entry_array[i].m_diffuse->get_resource();
				if (m_texture_array[i].m_diffuse)
				{
					m_is_texture_ready = m_is_texture_ready && m_texture_array[i].m_diffuse->refresh();
				}
			}
		}

		for (int i = 0; i < (int)m_material_entry_array.size(); i++)
		{
			if (m_material_entry_array[i].m_self_illuminant)
			{
				m_texture_array[i].m_self_illuminant = (hardware_texture*)m_material_entry_array[i].m_self_illuminant->get_resource();
				if (m_texture_array[i].m_self_illuminant)
				{
					m_is_texture_ready = m_is_texture_ready && m_texture_array[i].m_self_illuminant->refresh();
				}
			}
		}

		return true;
	}

	hardware_mesh* d3d9_hardware_mesh::clone(void)
	{
		d3d9_hardware_mesh* v_new_mesh = new d3d9_hardware_mesh();

		this->lock();
		v_new_mesh->copy_buffer_from(this);
		this->unlock();

		v_new_mesh->m_material_entry_array.resize(m_material_entry_array.size());
		v_new_mesh->m_texture_array.resize(m_texture_array.size());
		v_new_mesh->m_texture_manager = this->m_texture_manager;

		for (int i = 0; i < m_material_entry_array.size(); i++)
		{
			v_new_mesh->m_material_entry_array[i] = m_material_entry_array[i];

			material_entry& entry = v_new_mesh->m_material_entry_array[i];

			if (entry.m_diffuse != NULL)
				entry.m_diffuse->add_ref();

			if (entry.m_self_illuminant != NULL)
				entry.m_self_illuminant->add_ref();

			if (entry.m_specular != NULL)
				entry.m_specular->add_ref();

			if (entry.m_bump != NULL)
				entry.m_bump->add_ref();
		}

		v_new_mesh->calculate_topological_data();

		return v_new_mesh;
	}

	void d3d9_hardware_mesh::draw_subset(render_argument* args, int material_index)
	{
		args->m_device->draw_mesh((void*)m_d3d_mesh, material_index);
	}

	void d3d9_hardware_mesh::draw(render_argument* args)
	{
		d3d9_effect_common_mesh* v_render = (d3d9_effect_common_mesh*)(args->m_render);

		if (m_fvf & D3DFVF_NORMAL)
		{
			render_state v_render_state(args->m_device);
			v_render_state.set_state(D3DRS_LIGHTING, TRUE);
			if (m_fvf & D3DFVF_DIFFUSE)
			{
				v_render_state.set_state(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
			}
			else
			{
				v_render_state.set_state(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
			}
			//v_render_state.set_state( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

			v_render->draw(this);
		}
		else
		{
			v_render->draw(this);
		}
	}

	bool d3d9_hardware_mesh::intersect(const ray<double>& in_ray, intersect_result& out_result)
	{
		if (m_d3d_mesh == NULL) return false;

		bool val = false;
		if (singleton_d3d9_intersect_helper::instance().intersect((void*)m_d3d_mesh, in_ray, out_result))
		{
			out_result.m_inner_mesh = this;
			val = true;
		}

		return val;
	}

	vector3<double> d3d9_hardware_mesh::get_lowest_point(matrix4<double>& in_mat)
	{
		if (m_d3d_mesh == NULL) return vector3<double>::s_zero;


		vector3<double> v_lowest_point;
		v_lowest_point.x = math<double>::s_max_real;
		v_lowest_point.y = math<double>::s_max_real;
		v_lowest_point.z = math<double>::s_max_real;

		LPDIRECT3DVERTEXBUFFER9 v_d3d_vertex_buffer = NULL;
		m_d3d_mesh->GetVertexBuffer(&v_d3d_vertex_buffer);
		ulong v_vertex_num = m_d3d_mesh->GetNumVertices();
		D3DVERTEXBUFFER_DESC vertex_desc;
		v_d3d_vertex_buffer->GetDesc(&vertex_desc);
		ulong v_stride = m_d3d_mesh->GetNumBytesPerVertex();
		//ulong v_stride = vertex_desc.Size / v_vertex_num;

		void* v_vertices = NULL;
		v_d3d_vertex_buffer->Lock(0, 0, (void**)&v_vertices, 0);

		unsigned char* v_ptr = (unsigned char*)v_vertices;
		for (int i = 0; i < (int)v_vertex_num; i++)
		{
			float* v_flt_ptr = (float*)(v_ptr + i * v_stride);
			vector3<double> v_pos;
			v_pos.x = *v_flt_ptr;
			v_pos.y = *(v_flt_ptr + 1);
			v_pos.z = *(v_flt_ptr + 2);

			v_pos = v_pos * in_mat;
			if (v_lowest_point.y > v_pos.y)
			{
				v_lowest_point = v_pos;
			}
		}

		v_d3d_vertex_buffer->Unlock();

		return v_lowest_point;
	}

	void d3d9_hardware_mesh::update_bound_box(void)
	{
		d3d9_hardware_mesh_buffer* v_mesh_buffer = (d3d9_hardware_mesh_buffer*)this;

		D3DXVECTOR3 minVec;
		D3DXVECTOR3 maxVec;
		D3DXComputeBoundingBox((D3DXVECTOR3*)v_mesh_buffer->m_vertex_buffer, v_mesh_buffer->m_num_of_vertices, v_mesh_buffer->m_vertex_stride, &minVec, &maxVec);

		m_bound_box.set_extent(vector3<double>(&minVec.x), vector3<double>(&maxVec.x));
	}

	// override d3d9_hardware_mesh_buffer
	void d3d9_hardware_mesh::lock()
	{
		if (m_d3d_mesh == NULL) return;

		m_num_of_vertices = m_d3d_mesh->GetNumVertices();
		m_num_of_triangles = m_d3d_mesh->GetNumFaces();

		m_vertex_buffer_size = m_num_of_vertices * m_vertex_stride;
		m_d3d_mesh->LockVertexBuffer(0, (void**)&m_vertex_buffer);

		m_index_buffer_size = m_32bit_index ? m_num_of_triangles * 3 * sizeof(ulong) : m_num_of_triangles * 3 * sizeof(ushort);
		m_d3d_mesh->LockIndexBuffer(0, (void**)&m_index_buffer);

		m_attribute_buffer_size = m_num_of_triangles * sizeof(ulong);
		m_d3d_mesh->LockAttributeBuffer(0, (ulong**)&m_attribute_buffer);
	}

	void d3d9_hardware_mesh::unlock()
	{
		m_d3d_mesh->UnlockVertexBuffer();
		m_d3d_mesh->UnlockIndexBuffer();
		m_d3d_mesh->UnlockAttributeBuffer();

		m_vertex_buffer = NULL;
		m_index_buffer = NULL;
		m_attribute_buffer = NULL;

		m_vertex_buffer_size = 0;
		m_index_buffer_size = 0;
		m_attribute_buffer_size = 0;

		if (m_buffer_is_dirty)
		{
			m_buffer_is_dirty = false;
			update_bound_box();
		}
	}

	void d3d9_hardware_mesh::clear_buffer(void)
	{
		AUTO_DELETE(m_vertex_buffer);
		AUTO_DELETE(m_index_buffer);
		AUTO_DELETE(m_attribute_buffer);

		m_vertex_buffer_size = 0;
		m_index_buffer_size = 0;
		m_attribute_buffer_size = 0;
	}

	void d3d9_hardware_mesh::generate_topological_data(void)
	{
		this->lock();
		this->calculate_topological_data();
		this->unlock();
	}

	void d3d9_hardware_mesh::calculate_topological_data(void)
	{
		AUTO_DELETE(m_topology);
		m_topology = new mesh_topology();

		ulong* ulong_ptr = (ulong*)this->m_attribute_buffer;

		std::map<ulong, std::vector<ulong> > v_attr_triangle_map;
		for (ulong i = 0; i < this->m_num_of_triangles; i++)
		{
			std::map<ulong, std::vector<ulong> >::iterator itr = v_attr_triangle_map.find(*(ulong_ptr + i));
			if (itr != v_attr_triangle_map.end())
			{
				itr->second.push_back(i);
			}
			else
			{
				std::vector<ulong> v_array;
				v_array.push_back(i);
				v_attr_triangle_map.insert(std::make_pair(*(ulong_ptr + i), v_array));
			}
		}

		std::map<ulong, std::vector<ulong> >::iterator itr = v_attr_triangle_map.begin();
		for (; itr != v_attr_triangle_map.end(); itr++)
		{
			d3d9_mesh_subset* v_object = new d3d9_mesh_subset();
			v_object->create(this, itr->first, itr->second);
			v_object->update();

			m_topology->m_mesh_subset_map.insert(std::make_pair(itr->first, v_object));
		}
	}

	void d3d9_hardware_mesh::generate_adjacency(float v_eclipse, ulong* v_adjacency)
	{
		if (m_d3d_mesh)
		{
			m_d3d_mesh->GenerateAdjacency(v_eclipse, v_adjacency);
		}

	}

	void d3d9_hardware_mesh::generate_point_rep(ulong* v_adj_info, ulong* v_point_rep)
	{
		if (m_d3d_mesh)
		{
			m_d3d_mesh->ConvertAdjacencyToPointReps(v_adj_info, v_point_rep);
		}
	}
}