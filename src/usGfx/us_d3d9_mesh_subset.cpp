
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_mesh_subset.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_mesh_subset class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_mesh_subset.h"

namespace uniscope_globe
{
	d3d9_mesh_subset::d3d9_mesh_subset( void )
	{
		m_rtti = US_RTTI_D3D9_MESH_SUBSET;
	}

	d3d9_mesh_subset::~d3d9_mesh_subset( void )
	{
	}

	void d3d9_mesh_subset::create( d3d9_hardware_mesh_buffer* in_parent_mesh_buffer, int mat_index, std::vector<ulong>& in_tri_array )
	{
		m_parent_mesh_buffer = in_parent_mesh_buffer;
		m_material_index = mat_index;
		m_face_index_array = in_tri_array;
	}

	void d3d9_mesh_subset::lock( void )
	{
		m_parent_mesh_buffer->lock();

		this->attach_buffer_from( m_parent_mesh_buffer );
	}

	void d3d9_mesh_subset::unlock()
	{
		update_center();

		m_parent_mesh_buffer->set_buffer_is_dirty();
		m_parent_mesh_buffer->unlock();

		this->detach_buffer();
	}

	material_entry* d3d9_mesh_subset::get_material_entry( void )
	{
		d3d9_hardware_mesh* v_mesh = dynamic_cast<d3d9_hardware_mesh*>(m_parent_mesh_buffer);
		if(v_mesh == NULL ) return NULL;
		return &(v_mesh->m_material_entry_array[m_material_index]);
	}

	void d3d9_mesh_subset::update( void )
	{
		update_compact_map();

		update_center();
	}

	void d3d9_mesh_subset::update_compact_map( void )
	{
		m_compact_index_map.clear();

		d3d9_hardware_mesh_buffer* v_mesh_buffer = m_parent_mesh_buffer;

		if( v_mesh_buffer->m_32bit_index )
		{
			for(int ni = 0; ni < m_face_index_array.size(); ni++ )
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
			for(int ni = 0; ni < m_face_index_array.size(); ni++ )
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