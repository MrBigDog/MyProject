///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_shadow_volume.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_shadow_volume class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_effect_shadow_volume.h"
#include "resource.h"

namespace uniscope_globe
{
	d3d9_effect_shadow_volume::d3d9_effect_shadow_volume( render_device* device )
		:d3d9_effect_common_mesh( device )
	{
		load_from_module_resource(IDR_SHADER_SHADOW_VOLUME);
	}

	d3d9_effect_shadow_volume::~d3d9_effect_shadow_volume( void )
	{

	}

	bool d3d9_effect_shadow_volume::setup_handle( void )
	{
		if (m_d3dx_effect)
		{	
			m_technique						= m_d3dx_effect->GetTechniqueByName("ShadowVolumeTechnique");
			m_world_view_proj_matrix_handle	= m_d3dx_effect->GetParameterByName(0, "gMatWorldViewProjection");
			m_world_view_matrix_handle		= m_d3dx_effect->GetParameterByName(0, "gMatWorldView");
			m_proj_matrix_handle			= m_d3dx_effect->GetParameterByName(0, "gMatProjection");
			m_light_dir_handle				= m_d3dx_effect->GetParameterByName(0, "gLightDirection" );
			m_far_plane_handle				= m_d3dx_effect->GetParameterByName(0, "gFarPlane" );
			m_line_width_handle				= m_d3dx_effect->GetParameterByName(0, "gHalfLineWidth" );
		}
		return true;
	}

	bool d3d9_effect_shadow_volume::set_light_dir( const vector3<float>& v_dir )
	{
		m_d3dx_effect->SetVector( m_light_dir_handle, &D3DXVECTOR4( v_dir.x, v_dir.y, v_dir.z, 0.0 ) );
		return true;
	}

	bool d3d9_effect_shadow_volume::set_far_plane( const float& in_far_plane )
	{
		m_d3dx_effect->SetFloat( m_far_plane_handle, in_far_plane );
		return true;
	}

	bool d3d9_effect_shadow_volume::set_line_width( const float& in_line_width )
	{
		m_d3dx_effect->SetFloat( m_line_width_handle, in_line_width );
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void d3d9_effect_shadow_volume::set_vertex_declaration( uint decl )
	{
		m_device->set_vertex_declaration(decl);
	}

	void d3d9_effect_shadow_volume::set_texture( ulong stage_index, texture_handle handle )
	{

	}

	void d3d9_effect_shadow_volume::set_world_matrix( matrix4<float>& in_world_mat )
	{
		m_world_matrix = in_world_mat;
	}

	void d3d9_effect_shadow_volume::set_view_matrix( matrix4<float>& in_view_mat )
	{
		m_view_matrix = in_view_mat;
	}

	void d3d9_effect_shadow_volume::set_projection_matrix( matrix4<float>& in_proj_mat )
	{
		m_projection_matrix = in_proj_mat;
	}

	void d3d9_effect_shadow_volume::push_transform( void )
	{
		m_world_matrix_stack.push( m_world_matrix );
	}

	void d3d9_effect_shadow_volume::multiply_transform( matrix4<float>& mat )
	{
		m_world_matrix = mat * m_world_matrix;
	}

	void d3d9_effect_shadow_volume::pop_transform( void )
	{
		if( m_world_matrix_stack.size() == 0) return;

		m_world_matrix = m_world_matrix_stack.top();

		m_world_matrix_stack.pop();
	}

	bool d3d9_effect_shadow_volume::commit_changes( void )
	{
		m_world_view_matrix = m_world_matrix * m_view_matrix;

		m_wvp_matrix = m_world_view_matrix * m_projection_matrix;

		m_d3dx_effect->SetMatrix( m_world_view_proj_matrix_handle, (D3DXMATRIX *)&m_wvp_matrix );
		m_d3dx_effect->SetMatrix( m_world_view_matrix_handle, (D3DXMATRIX *)&m_world_view_matrix );
		m_d3dx_effect->SetMatrix( m_proj_matrix_handle, (D3DXMATRIX *)&m_projection_matrix );

		return d3d9_effect_common_mesh::commit_changes();
	}

	void d3d9_effect_shadow_volume::draw( hardware_mesh* in_mesh )
	{
		d3d9_effect_common_mesh::draw( in_mesh );

		d3d9_hardware_mesh* v_mesh = (d3d9_hardware_mesh*)in_mesh;

		int tex_cnt = (int)v_mesh->m_texture_array.size();

		// patching triangles
		v_mesh->m_d3d_mesh->DrawSubset( tex_cnt );
	}
	
}