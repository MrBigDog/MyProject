///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_mesh.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_usx_mesh.h"
#include <usDotx/us_usx_frame.h>
#include <usDotx/us_usx_material.h>
#include <usDotx/us_usx_data_struct.h>
#include <usDotx/us_usx_skin_info.h>

namespace uniscope_globe
{
	usx_mesh::usx_mesh(void)
	{
		m_number_of_vertices = 0;
		m_valid = false;

		m_parent_frame = NULL;

		m_vertex_data = NULL;
		m_vertex_color = NULL;
		m_normals = NULL;
		m_binormal = NULL;
		m_tangent = NULL;

		m_number_of_faces = 0;
		m_face_index = NULL;
		m_attribute_data = NULL;

		m_skin_info = NULL;

		m_material_safe = true;

		m_fvf = 0;

		m_vertex_stride = 0;
	}

	usx_mesh::~usx_mesh(void)
	{
		AUTO_DELETE(m_attribute_data);
		AUTO_DELETE(m_skin_info);
		AUTO_DELETE_ARRAY(m_vertex_data);
		AUTO_DELETE_ARRAY(m_vertex_color);
		AUTO_DELETE_ARRAY(m_normals);
		AUTO_DELETE_ARRAY(m_binormal);
		AUTO_DELETE_ARRAY(m_tangent);
		AUTO_DELETE_ARRAY(m_face_index);


		for (int i = 0; i < (int)m_vertex_texture_coords.size(); i++)
		{
			AUTO_DELETE(m_vertex_texture_coords[i]);
		}
		m_vertex_texture_coords.clear();

		for (int i = 0; i < (int)m_decl_data.size(); i++)
		{
			AUTO_DELETE(m_decl_data[i]);
		}
		m_decl_data.clear();

		remove_material();

		m_old_vertex_num_per_face.clear();
	}

	void usx_mesh::combine()
	{

	}

	void usx_mesh::remove_material(void)
	{
		for (int i = 0; i < (int)m_materials.size(); i++)
		{
			AUTO_DELETE(m_materials[i]);
		}
		m_materials.clear();
	}

	void usx_mesh::sort(long* in_new_order)
	{
		usx_material_array tmp_mat_array;
		tmp_mat_array.resize(m_materials.size());
		std::copy(m_materials.begin(), m_materials.end(), tmp_mat_array.begin());

		for (int i = 0; i < (int)m_materials.size(); i++)
		{
			m_materials[in_new_order[i]] = tmp_mat_array[i];
		}

		tmp_mat_array.clear();
	}
}