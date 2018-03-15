
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_pp_combine_hsl.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_pp_combine_hsl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_effect_pp_combine_hsl.h"

namespace uniscope_globe
{
	d3d9_effect_pp_combine_hsl::d3d9_effect_pp_combine_hsl( render_device* device )
		:d3d9_effect_pp(device)
	{
		//load_from_file(L"c:\\shader\\CombineHDR.fx");
		load_from_module_resource(IDR_SHADER_PP_COMBINE_HSL);
	}

	d3d9_effect_pp_combine_hsl::~d3d9_effect_pp_combine_hsl( void )
	{

	}

	bool d3d9_effect_pp_combine_hsl::setup_handle( void )
	{
		if (m_d3dx_effect)
		{	
			m_technique						= m_d3dx_effect->GetTechniqueByName("PostProcess");
			m_src_texture_handle			= m_d3dx_effect->GetParameterByName(0, "g_txSrcColor");
			m_dest_texture_handle			= m_d3dx_effect->GetParameterByName(0, "g_txSceneColor");
			m_saturation_handle				= m_d3dx_effect->GetParameterByName(0, "g_fSaturation");
			m_luminance_handle				= m_d3dx_effect->GetParameterByName(0, "g_fLuminance");
			m_contract_handle				= m_d3dx_effect->GetParameterByName(0, "g_fContract");
		}
		return true;
	}

	bool d3d9_effect_pp_combine_hsl::set_src_texture( ULONG_PTR ht )
	{
		m_d3dx_effect->SetTexture( m_src_texture_handle, (LPDIRECT3DBASETEXTURE9)ht );
		return true;
	}

	bool d3d9_effect_pp_combine_hsl::set_dest_texture( ULONG_PTR ht )
	{
		m_d3dx_effect->SetTexture( m_dest_texture_handle, (LPDIRECT3DBASETEXTURE9)ht );
		return true;
	}

	bool d3d9_effect_pp_combine_hsl::set_saturation( float val )
	{
		m_d3dx_effect->SetFloat( m_saturation_handle, val );
		return true;
	}

	bool d3d9_effect_pp_combine_hsl::set_luminance( float val )
	{
		m_d3dx_effect->SetFloat( m_luminance_handle, val );
		return true;
	}

	bool d3d9_effect_pp_combine_hsl::set_contract( float val )
	{
		m_d3dx_effect->SetFloat( m_contract_handle, val );
		return true;
	}


}