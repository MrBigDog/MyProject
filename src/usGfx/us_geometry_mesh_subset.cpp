
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_mesh_subset.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_mesh_subset class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_geometry_mesh_subset.h"
#include <usCore/us_resource_manager.h>

namespace uniscope_globe
{
	geometry_mesh_subset::geometry_mesh_subset(void)
	{
		m_rtti = US_RTTI_GEOMETRY_MESH_SUBSET;
		m_material_entry.m_emissive_color.a = 1.0;
		m_material_entry.m_emissive_color.r = 0;
		m_material_entry.m_emissive_color.g = 0;
		m_material_entry.m_emissive_color.b = 0;

		//m_material_entry.m_specular_color.a= 1.0;
		//m_material_entry.m_specular_color.r= 1.0;
		//m_material_entry.m_specular_color.g= 1.0;
		//m_material_entry.m_specular_color.b= 1.0;

		m_material_entry.m_power = 10;

	}

	geometry_mesh_subset::~geometry_mesh_subset(void)
	{
		AUTO_RELEASE_SHARED_DATA(m_material_entry.m_bump);
		AUTO_RELEASE_SHARED_DATA(m_material_entry.m_diffuse);
		AUTO_RELEASE_SHARED_DATA(m_material_entry.m_self_illuminant);
		AUTO_RELEASE_SHARED_DATA(m_material_entry.m_specular);
	}

	void geometry_mesh_subset::create(d3d9_hardware_mesh_buffer* in_parent_mesh_buffer, int mat_index)
	{
		m_parent_mesh_buffer = in_parent_mesh_buffer;
		m_material_index = mat_index;
	}

	void geometry_mesh_subset::lock(void)
	{
		m_parent_mesh_buffer->lock();

		this->attach_buffer_from(m_parent_mesh_buffer);
	}

	void geometry_mesh_subset::unlock()
	{
		update_center();

		m_parent_mesh_buffer->set_buffer_is_dirty();
		m_parent_mesh_buffer->unlock();

		this->detach_buffer();
	}

	void geometry_mesh_subset::update(void)
	{
		//update_compact_map();

		update_center();
	}

	material_entry* geometry_mesh_subset::get_material_entry(void)
	{
		return &m_material_entry;
	}

	void geometry_mesh_subset::copy_from(geometry_mesh_subset* v_src_data)
	{
		this->m_parent_mesh_buffer = v_src_data->m_parent_mesh_buffer;
		this->m_parent_object = v_src_data->m_parent_object;
		this->m_material_index = v_src_data->m_material_index;
		this->m_center = v_src_data->m_center;

		this->copy_texture_map_desc(v_src_data);

		this->m_material_entry = v_src_data->m_material_entry;
		if (this->m_material_entry.m_diffuse != NULL)
			this->m_material_entry.m_diffuse->add_ref();

		if (this->m_material_entry.m_self_illuminant != NULL)
			this->m_material_entry.m_self_illuminant->add_ref();

		if (this->m_material_entry.m_specular != NULL)
			this->m_material_entry.m_specular->add_ref();

		if (this->m_material_entry.m_bump != NULL)
			this->m_material_entry.m_bump->add_ref();
	}


}