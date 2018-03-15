
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mesh_subset.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mesh_subset class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_mesh_subset.h"

namespace uniscope_globe
{
	mesh_subset::mesh_subset( void )
	{

	}

	mesh_subset::~mesh_subset( void )
	{

	}

	void mesh_subset::draw( render_argument* args )
	{
		dynamic_cast<hardware_mesh*>(m_parent_mesh_buffer)->draw_subset( args, m_material_index );
	}

	material_entry* mesh_subset::get_material_entry( void )
	{
		return NULL;
	}

	void mesh_subset::update_center( void )
	{
		d3d9_hardware_mesh_buffer* v_mesh_buffer = m_parent_mesh_buffer;

		m_center.x = 0.0f;
		m_center.y = 0.0f;
		m_center.z = 0.0f;

		std::map<ulong, ulong>::iterator itr = m_compact_index_map.begin();
		for ( ; itr != m_compact_index_map.end(); itr ++ )
		{
			float* p_vert = (float*)((unsigned char*)v_mesh_buffer->m_vertex_buffer + itr->first * v_mesh_buffer->m_vertex_stride );

			m_center.x += *p_vert;
			m_center.y += *(p_vert + 1);
			m_center.z += *(p_vert + 2);
		}

		m_center /= m_compact_index_map.size();
	}

	void mesh_subset::translate_object( vector_3d& vec_offset_cart )
	{
		this->lock();
		vector_3f vec_offset((double*)&vec_offset_cart);
		std::map<ulong, ulong> ::iterator itr = m_compact_index_map.begin();
		for( ; itr != m_compact_index_map.end(); itr++ )
		{
			this->set_vector3( 0, itr->first, this->get_vector3(0, itr->first) + vec_offset);
		}
		this->unlock();
	}

	void mesh_subset::transform_uv( matrix_4f& mat_uv )
	{
		this->lock();
		std::map<ulong, ulong> ::iterator itr = m_compact_index_map.begin();
		int texcoord_offset = this->get_offset(0);
		for( ; itr != m_compact_index_map.end(); itr++ )
		{
			vector_3f v_mesh_texcoord( this->get_float(texcoord_offset, itr->first), this->get_float(texcoord_offset + 4, itr->first), 0);
			
			v_mesh_texcoord = v_mesh_texcoord * mat_uv;
			this->set_float( texcoord_offset, itr->first, v_mesh_texcoord.x );
			this->set_float( texcoord_offset + 4, itr->first, v_mesh_texcoord.x );
		}
		this->unlock();
	}


	
}