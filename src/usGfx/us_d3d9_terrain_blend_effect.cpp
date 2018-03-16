///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_terrain_blend_effect.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_terrain_blend_effect class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_terrain_blend_effect.h"
#include "resource.h"
#include <usCore/us_hardware_texture.h>

namespace uniscope_globe
{
	d3d9_terrain_blend_effect::d3d9_terrain_blend_effect(render_device* device)
		:d3d9_render_effect(device)
	{
		load_form_module_resource(IDR_SHADER_TERRAIN_BLEND);
	}

	d3d9_terrain_blend_effect::~d3d9_terrain_blend_effect(void)
	{

	}

	bool d3d9_terrain_blend_effect::setup_handle(void)
	{
		if (m_d3dx_effect)
		{
			m_technique = m_d3dx_effect->GetTechniqueByName("TerrainBlendTech");
			m_transform_handle = m_d3dx_effect->GetParameterByName(0, "m_world_view_proj");
			m_texture_transform_handle = m_d3dx_effect->GetParameterByName(0, "m_texture_matrix");
			m_texture_handle = m_d3dx_effect->GetParameterByName(0, "m_terrain_image");
		}
		return true;
	}

	bool d3d9_terrain_blend_effect::set_transform(const matrix4<float>& wvp)
	{
		m_d3dx_effect->SetMatrix(m_transform_handle, (D3DXMATRIX *)&wvp);

		return true;
	}

	bool d3d9_terrain_blend_effect::set_texture(hardware_texture* ht)
	{
		if (ht == NULL)
		{
			m_d3dx_effect->SetTexture(m_texture_handle, NULL);
		}
		else
		{
			m_d3dx_effect->SetTexture(m_texture_handle, (LPDIRECT3DBASETEXTURE9)ht->get_handle());
		}
		return true;
	}

	bool d3d9_terrain_blend_effect::set_texture_transform(const matrix4<float>& tm)
	{
		m_d3dx_effect->SetMatrix(m_transform_handle, (D3DXMATRIX *)&tm);

		return true;
	}


}