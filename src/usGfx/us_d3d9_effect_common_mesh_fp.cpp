///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_common_mesh_fp.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_common_mesh_fp class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_effect_common_mesh_fp.h"
#include <usCore/us_render_device.h>
namespace uniscope_globe
{
	d3d9_effect_common_mesh_fp::d3d9_effect_common_mesh_fp( render_device* device )
		: d3d9_effect_common_mesh( device )
	{
	}

	d3d9_effect_common_mesh_fp::~d3d9_effect_common_mesh_fp( void )
	{

	}

	void d3d9_effect_common_mesh_fp::set_vertex_declaration( uint decl )
	{
		m_device->set_vertex_declaration( decl );
	}

	void d3d9_effect_common_mesh_fp::set_texture( ulong stage_index, texture_handle handle )
	{
		m_device->set_texture( stage_index, handle );
	}

	void d3d9_effect_common_mesh_fp::set_blank_texture( ulong stage_index )
	{
		m_device->set_blank_texture( stage_index );
	}

	void d3d9_effect_common_mesh_fp::set_transparency_texture( ulong stage_index, ulong texture_idx )
	{
		m_device->set_transparency_texture( stage_index, texture_idx );
	}

	void d3d9_effect_common_mesh_fp::set_material( ULONG_PTR material_handle )
	{
		m_device->set_material( material_handle );
	}

	void d3d9_effect_common_mesh_fp::set_world_matrix( matrix4<float>& in_world_mat )
	{
		m_device->set_world_matrix( in_world_mat );
	}

	void d3d9_effect_common_mesh_fp::set_view_matrix( matrix4<float>& in_view_mat )
	{
		m_device->set_view_matrix( in_view_mat );
	}

	void d3d9_effect_common_mesh_fp::set_projection_matrix( matrix4<float>& in_proj_mat )
	{
		m_device->set_projection_matrix( in_proj_mat );
	}

	void d3d9_effect_common_mesh_fp::push_transform( void )
	{
		m_device->push_transform();
	}

	void d3d9_effect_common_mesh_fp::multiply_transform( matrix4<float>& mat )
	{
		m_device->multiply_transform( mat );
	}

	void d3d9_effect_common_mesh_fp::pop_transform( void )
	{
		m_device->pop_transform();
	}

	bool d3d9_effect_common_mesh_fp::commit_changes(void)
	{
		return true;
	}

}