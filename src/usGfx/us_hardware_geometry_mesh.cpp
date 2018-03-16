///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_geometry_mesh.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_geometry_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_hardware_geometry_mesh.h"
#include <usGfx/us_d3d9_vertex_declear.h>
#include <usCore/us_mesh_creator.h>
#include <usCore/us_render_device.h>
#include <usUtil/us_matrix4.h>

namespace uniscope_globe
{
	hardware_geometry_mesh::hardware_geometry_mesh(hardware_creator* in_creator)
	{
		m_trans_matrix = matrix_4d::s_identity;

		m_vertex_buffer = NULL;
		m_index_buffer = NULL;

		m_mesh_creator = in_creator;
	}

	hardware_geometry_mesh::hardware_geometry_mesh(const hardware_geometry_mesh& v_mesh)
	{

	}

	hardware_geometry_mesh::~hardware_geometry_mesh(void)
	{
		AUTO_RELEASE(m_vertex_buffer);
		AUTO_RELEASE(m_index_buffer);
	}

	long hardware_geometry_mesh::create(render_device* device)
	{
		if (m_valid)
			return 0;

		m_num_of_vertices = m_vertex_array.size();
		AUTO_RELEASE(m_vertex_buffer);
		if (!device->create_vertex_buffer(m_num_of_vertices * position_color::stride, D3DUSAGE_WRITEONLY, position_color::fvf, D3DPOOL_MANAGED, (void**)&m_vertex_buffer, NULL))
			return 0;

		void* data = NULL;
		m_vertex_buffer->Lock(0, 0, &data, 0);
		memcpy(data, (void*)&m_vertex_array[0], m_num_of_vertices * position_color::stride);
		m_vertex_buffer->Unlock();



		m_num_of_indices = m_index_array.size();
		AUTO_RELEASE(m_index_buffer);
		if (!device->create_index_buffer(m_num_of_indices * sizeof(ushort), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, (void**)&m_index_buffer, NULL))
			return 0;

		data = NULL;
		m_index_buffer->Lock(0, 0, &data, 0);
		memcpy(data, (void*)&m_index_array[0], m_num_of_indices * sizeof(ushort));
		m_index_buffer->Unlock();

		m_mem_size = m_num_of_vertices * position_color::stride + m_num_of_indices * sizeof(ushort);
		m_valid = true;

		return m_mem_size;
	}

	bool hardware_geometry_mesh::destroy(void)
	{
		AUTO_RELEASE(m_vertex_buffer);
		AUTO_RELEASE(m_index_buffer);
		return true;
	}

	void hardware_geometry_mesh::draw(render_argument* args)
	{
		if (!m_valid)
		{
			m_mesh_creator->insert_mesh(this);
			return;
		}

		if (m_vertex_buffer == NULL ||
			m_index_buffer == NULL)
			return;

		matrix_4d v_mat = m_collapse_matrix * m_trans_matrix;
		v_mat.m41 = v_mat.m41 - cartesian_coords::s_reference_position_geo.x;
		v_mat.m42 = v_mat.m42 - cartesian_coords::s_reference_position_geo.y;
		v_mat.m43 = v_mat.m43 - cartesian_coords::s_reference_position_geo.z;

		d3d9_mesh_render* v_mesh_render = (d3d9_mesh_render*)args->m_render;

		args->m_device->set_vertex_declaration(position_color::fvf);

		v_mesh_render->push_transform();
		v_mesh_render->multiply_transform(matrix_4f(v_mat.m));

		v_mesh_render->commit_changes();

		LPDIRECT3DDEVICE9 v_d3d_device = (LPDIRECT3DDEVICE9(args->m_device->get_device()));
		v_d3d_device->SetStreamSource(0, m_vertex_buffer, 0, position_color::stride);
		v_d3d_device->SetIndices(m_index_buffer);
		v_d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_num_of_vertices, 0, m_num_of_indices / 3);

		v_mesh_render->pop_transform();
	}
}