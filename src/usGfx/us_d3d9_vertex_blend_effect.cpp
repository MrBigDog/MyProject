///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_vertex_blend_effect.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_vertex_blend_effect class
//	Reference : Book "Advanced Animation With DirectX"
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_vertex_blend_effect.h"
#include "resource.h"

namespace uniscope_globe
{
	d3d9_vertex_blend_effect::d3d9_vertex_blend_effect( render_device* device )
		: d3d9_render_effect(device)
	{
		load_form_module_resource(IDR_SHADER_VERTEX_BLEND);		
	}

	d3d9_vertex_blend_effect::~d3d9_vertex_blend_effect( void )
	{
	
	}

	bool d3d9_vertex_blend_effect::setup_handle( void )
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

	bool d3d9_vertex_blend_effect::set_transform( const matrix4<float>& wvp)
	{
		m_d3dx_effect->SetMatrix(m_world_view_projection, (D3DXMATRIX *)&wvp );

		return true;
	}

	bool d3d9_vertex_blend_effect::set_texture( hardware_texture* ht )
	{
		if ( ht == NULL )
		{
			m_d3dx_effect->SetTexture(m_texture, NULL );
		}
		else
		{
			m_d3dx_effect->SetTexture(m_texture, (LPDIRECT3DBASETEXTURE9)ht->get_handle() );
		}
		return true;
	}

	bool d3d9_vertex_blend_effect::set_matrix_array(void* matrix_array, ulong count )
	{
		assert(m_d3dx_effect);

		m_d3dx_effect->SetMatrixArray(m_transformation, (const D3DXMATRIX*)matrix_array, count );

		return true;
	}


	
}