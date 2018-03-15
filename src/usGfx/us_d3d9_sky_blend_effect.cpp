///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_sky_blend_effect.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_sky_blend_effect class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_sky_blend_effect.h"
#include "resource.h"

namespace uniscope_globe
{
	d3d9_sky_blend_effect::d3d9_sky_blend_effect( render_device* device )
		:d3d9_render_effect(device)
	{
		load_form_module_resource(IDR_SHADER_SKY_BLEND);
	}

	d3d9_sky_blend_effect::~d3d9_sky_blend_effect( void )
	{

	}

	bool d3d9_sky_blend_effect::setup_handle( void )
	{
		if (m_d3dx_effect)
		{	
			m_technique					= m_d3dx_effect->GetTechniqueByName("SkyBlendTech");
			m_transform_handle			= m_d3dx_effect->GetParameterByName(0, "matWorldViewProj");
			m_sun_alpha_handle			= m_d3dx_effect->GetParameterByName(0, "fSunAlpha");
			m_sun_theta_handle			= m_d3dx_effect->GetParameterByName(0, "fSunTheta");
			m_sun_shininess_handle		= m_d3dx_effect->GetParameterByName(0, "fSunShininess");
			m_sun_strength_handle		= m_d3dx_effect->GetParameterByName(0, "fSunStrength");
			m_sun_distance_handle		= m_d3dx_effect->GetParameterByName(0, "fSunDistance");
			m_cube_texture_handle		= m_d3dx_effect->GetParameterByName(0, "EnvironmentMap");
		}
		return true;
	}

	bool d3d9_sky_blend_effect::set_transform( const matrix4<float>& wvp )
	{
		m_d3dx_effect->SetMatrix( m_transform_handle, (D3DXMATRIX *)&wvp );

		return true;
	}

	bool d3d9_sky_blend_effect::set_texture( ulong ht )
	{
		m_d3dx_effect->SetTexture( m_cube_texture_handle, (LPDIRECT3DBASETEXTURE9)ht );
		return true;
	}

	bool d3d9_sky_blend_effect::set_sun_alpha( float sun_alpha )
	{
		m_d3dx_effect->SetFloat( m_sun_alpha_handle, sun_alpha );
		return true;
	}

	bool d3d9_sky_blend_effect::set_sun_theta( float sun_theta )
	{
		m_d3dx_effect->SetFloat( m_sun_theta_handle, sun_theta );
		return true;
	}

	bool d3d9_sky_blend_effect::set_sun_shininess( float sun_shininess )
	{
		m_d3dx_effect->SetFloat( m_sun_shininess_handle, sun_shininess );
		return true;
	}

	bool d3d9_sky_blend_effect::set_sun_strength( float sun_strength )
	{
		m_d3dx_effect->SetFloat( m_sun_strength_handle, sun_strength );
		return true;
	}

	bool d3d9_sky_blend_effect::set_sun_distance( float sun_distance )
	{
		m_d3dx_effect->SetFloat( m_sun_distance_handle, sun_distance );
		return true;
	}	
}