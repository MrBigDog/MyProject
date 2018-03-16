
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_surface_object.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_surface_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_surface_object.h"
#include <usCore/us_hardware_mesh.h>

namespace uniscope_globe
{
	d3d9_surface_object::d3d9_surface_object(void)
	{
		m_rtti = US_RTTI_SURFACE_OBJECT;
	}

	d3d9_surface_object::~d3d9_surface_object(void)
	{
	}

	void d3d9_surface_object::create(d3d9_hardware_mesh_buffer* in_parent_mesh_buffer, int mat_index, std::vector<ulong>& in_tri_array)
	{
		m_parent_mesh_buffer = in_parent_mesh_buffer;
		m_material_index = mat_index;
		m_face_index_array = in_tri_array;
	}

	void d3d9_surface_object::lock(void)
	{
		m_parent_mesh_buffer->lock();

		this->attach_buffer_from(m_parent_mesh_buffer);
	}

	void d3d9_surface_object::unlock()
	{
		update_center();

		//m_parent_mesh_buffer->update_bound_box();
		m_parent_mesh_buffer->unlock();

		this->detach_buffer();
	}

	void d3d9_surface_object::update(void)
	{
		update_compact_map();

		update_center();
	}

	void d3d9_surface_object::draw(render_argument* args)
	{
		dynamic_cast<hardware_mesh*>(m_parent_mesh_buffer)->draw_subset(args, m_material_index);
	}

	void d3d9_surface_object::update_center(void)
	{
		d3d9_hardware_mesh_buffer* v_mesh_buffer = m_parent_mesh_buffer;

		m_center.x = 0.0f;
		m_center.y = 0.0f;
		m_center.z = 0.0f;

		std::map<ulong, ulong>::iterator itr = m_compact_index_map.begin();
		for (; itr != m_compact_index_map.end(); itr++)
		{
			float* p_vert = (float*)((unsigned char*)v_mesh_buffer->m_vertex_buffer + itr->first * v_mesh_buffer->m_vertex_stride);

			m_center.x += *p_vert;
			m_center.y += *(p_vert + 1);
			m_center.z += *(p_vert + 2);
		}

		m_center /= m_compact_index_map.size();
	}

	void d3d9_surface_object::update_compact_map(void)
	{
		m_compact_index_map.clear();

		d3d9_hardware_mesh_buffer* v_mesh_buffer = m_parent_mesh_buffer;

		if (v_mesh_buffer->m_32bit_index)
		{
			for (int ni = 0; ni < m_face_index_array.size(); ni++)
			{
				ulong face_index = m_face_index_array[ni];
				ulong* p_v1_indx = (ulong*)v_mesh_buffer->m_index_buffer + 3 * face_index;

				m_compact_index_map[*p_v1_indx] = *p_v1_indx;
				m_compact_index_map[*(p_v1_indx + 1)] = *(p_v1_indx + 1);
				m_compact_index_map[*(p_v1_indx + 2)] = *(p_v1_indx + 2);
			}
		}
		else
		{
			for (int ni = 0; ni < m_face_index_array.size(); ni++)
			{
				ushort face_index = m_face_index_array[ni];
				ushort* p_v1_indx = (ushort*)v_mesh_buffer->m_index_buffer + 3 * face_index;

				m_compact_index_map[*p_v1_indx] = *p_v1_indx;
				m_compact_index_map[*(p_v1_indx + 1)] = *(p_v1_indx + 1);
				m_compact_index_map[*(p_v1_indx + 2)] = *(p_v1_indx + 2);
			}
		}
	}
}