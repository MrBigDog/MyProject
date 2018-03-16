///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_vertex_blend.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_vertex_blend class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_effect_vertex_blend.h"

namespace uniscope_globe
{
	d3d9_effect_vertex_blend::d3d9_effect_vertex_blend( render_device* device, ulong resource_id )
		: d3d9_effect_common_mesh(device)
	{		
		load_from_module_resource(resource_id);		
	}

	d3d9_effect_vertex_blend::~d3d9_effect_vertex_blend( void )
	{

	}

	bool d3d9_effect_vertex_blend::setup_handle( void )
	{
		if (m_d3dx_effect)
		{	
			m_technique					= m_d3dx_effect->GetTechniqueByName("VertexBlendingTech");
			m_world_view_projection		= m_d3dx_effect->GetParameterByName(0, "WorldViewProj");
			m_transformation			= m_d3dx_effect->GetParameterByName(0, "FinalTransforms");
			m_texture					= m_d3dx_effect->GetParameterByName(0, "Tex");
		}
		return true;
	}

	void d3d9_effect_vertex_blend::set_texture( ulong stage_index, texture_handle handle )
	{
		if ( handle == NULL )
		{
			m_d3dx_effect->SetTexture( m_texture, NULL );
		}
		else
		{
			m_d3dx_effect->SetTexture( m_texture, (LPDIRECT3DBASETEXTURE9)handle );
		}		
	}

	void d3d9_effect_vertex_blend::set_world_matrix( matrix4<float>& in_world_mat )
	{
		m_world_matrix = in_world_mat;

		//m_wvp_matrix = m_world_matrix * m_view_matrix * m_projection_matrix;

		//m_d3dx_effect->SetMatrix( m_world_view_projection, (D3DXMATRIX *)&m_wvp_matrix );
	}

	void d3d9_effect_vertex_blend::set_view_matrix( matrix4<float>& in_view_mat )
	{
		m_view_matrix = in_view_mat;

		//m_wvp_matrix = m_world_matrix * m_view_matrix * m_projection_matrix;

		//m_d3dx_effect->SetMatrix( m_world_view_projection, (D3DXMATRIX *)&m_wvp_matrix );
	}

	void d3d9_effect_vertex_blend::set_projection_matrix( matrix4<float>& in_proj_mat )
	{
		m_projection_matrix = in_proj_mat;

		//m_wvp_matrix = m_world_matrix * m_view_matrix * m_projection_matrix;

		//m_d3dx_effect->SetMatrix( m_world_view_projection, (D3DXMATRIX *)&m_wvp_matrix );
	}

	void d3d9_effect_vertex_blend::push_transform( void )
	{
		m_world_matrix_stack.push( m_world_matrix );
	}

	void d3d9_effect_vertex_blend::multiply_transform( matrix4<float>& mat )
	{
		m_world_matrix = mat * m_world_matrix;

		//m_wvp_matrix = m_world_matrix * m_view_matrix * m_projection_matrix;

		//m_d3dx_effect->SetMatrix( m_world_view_projection, (D3DXMATRIX *)&m_wvp_matrix );
	}

	void d3d9_effect_vertex_blend::pop_transform( void )
	{
		if( m_world_matrix_stack.size() == 0) return;

		m_world_matrix = m_world_matrix_stack.top();

		//m_wvp_matrix = m_world_matrix * m_view_matrix * m_projection_matrix;

		//m_d3dx_effect->SetMatrix( m_world_view_projection, (D3DXMATRIX *)&m_wvp_matrix );

		m_world_matrix_stack.pop();
	}

	bool d3d9_effect_vertex_blend::commit_changes( void )
	{
		m_wvp_matrix = m_world_matrix * m_view_matrix * m_projection_matrix;

		m_d3dx_effect->SetMatrix( m_world_view_projection, (D3DXMATRIX *)&m_wvp_matrix );

		return d3d9_effect_common_mesh::commit_changes();
	}

	bool d3d9_effect_vertex_blend::set_matrix_array(void* matrix_array, ulong count )
	{
		m_d3dx_effect->SetMatrixArray(m_transformation, (const D3DXMATRIX*)matrix_array, count );

		return true;
	}

	
}