
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_pp_tone_map.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_pp_tone_map class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_effect_pp_tone_map.h"
#include <usGfx/resource.h>

namespace uniscope_globe
{
	d3d9_effect_pp_tone_map::d3d9_effect_pp_tone_map( render_device* device )
		:d3d9_effect_pp(device)
	{
		//load_from_file(L"c:\\shader\\ColorToneMap.fx");
		load_from_module_resource(IDR_SHADER_PP_COLOR_TONE_MAP);
	}

	d3d9_effect_pp_tone_map::~d3d9_effect_pp_tone_map( void )
	{

	}

	bool d3d9_effect_pp_tone_map::setup_handle( void )
	{
		if (m_d3dx_effect)
		{	
			m_technique						= m_d3dx_effect->GetTechniqueByName("PostProcess");
			m_src_texture_handle			= m_d3dx_effect->GetParameterByName(0, "g_txSrcColor");
			m_dest_texture_handle			= m_d3dx_effect->GetParameterByName(0, "g_txSceneColor");
		}
		return true;
	}

	bool d3d9_effect_pp_tone_map::set_src_texture( ULONG_PTR ht )
	{
		m_d3dx_effect->SetTexture( m_src_texture_handle, (LPDIRECT3DBASETEXTURE9)ht );
		return true;
	}

	bool d3d9_effect_pp_tone_map::set_dest_texture( ULONG_PTR ht )
	{
		m_d3dx_effect->SetTexture( m_dest_texture_handle, (LPDIRECT3DBASETEXTURE9)ht );
		return true;
	}


	
}