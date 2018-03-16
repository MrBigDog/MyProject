///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_sky_sphere.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_sky_sphere class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_effect_sky_sphere.h"
#include "resource.h"

namespace uniscope_globe
{
	d3d9_effect_sky_sphere::d3d9_effect_sky_sphere( render_device* device )
		:d3d9_effect_base(device)
	{
		load_from_module_resource(IDR_SHADER_SKY_BLEND);
	}

	d3d9_effect_sky_sphere::~d3d9_effect_sky_sphere( void )
	{

	}

	bool d3d9_effect_sky_sphere::setup_handle( void )
	{
		if (m_d3dx_effect)
		{	
			m_technique					= m_d3dx_effect->GetTechniqueByName("SkyBlendTech");
			m_transform_handle			= m_d3dx_effect->GetParameterByName(0, "matWorldViewProj");
			m_sun_azimuth_handle		= m_d3dx_effect->GetParameterByName(0, "fSunAzimuth");
			m_sun_elevation_handle		= m_d3dx_effect->GetParameterByName(0, "fSunElevation");
			m_sun_shininess_handle		= m_d3dx_effect->GetParameterByName(0, "fSunShininess");
			m_sun_strength_handle		= m_d3dx_effect->GetParameterByName(0, "fSunStrength");
			m_sun_distance_handle		= m_d3dx_effect->GetParameterByName(0, "fSunDistance");
			m_cube_texture_handle		= m_d3dx_effect->GetParameterByName(0, "EnvironmentMap");
		}
		return true;
	}

	bool d3d9_effect_sky_sphere::set_transform( const matrix4<float>& wvp )
	{
		m_d3dx_effect->SetMatrix( m_transform_handle, (D3DXMATRIX *)&wvp );

		return true;
	}

	bool d3d9_effect_sky_sphere::set_texture( ULONG_PTR ht )
	{
		m_d3dx_effect->SetTexture( m_cube_texture_handle, (LPDIRECT3DBASETEXTURE9)ht );
		return true;
	}

	bool d3d9_effect_sky_sphere::set_sun_azimuth( float sun_azimuth )
	{
		m_d3dx_effect->SetFloat( m_sun_azimuth_handle, sun_azimuth );
		return true;
	}

	bool d3d9_effect_sky_sphere::set_sun_elevation( float sun_elevation )
	{
		m_d3dx_effect->SetFloat( m_sun_elevation_handle, sun_elevation );
		return true;
	}

	bool d3d9_effect_sky_sphere::set_sun_shininess( float sun_shininess )
	{
		m_d3dx_effect->SetFloat( m_sun_shininess_handle, sun_shininess );
		return true;
	}

	bool d3d9_effect_sky_sphere::set_sun_strength( float sun_strength )
	{
		m_d3dx_effect->SetFloat( m_sun_strength_handle, sun_strength );
		return true;
	}

	bool d3d9_effect_sky_sphere::set_sun_distance( float sun_distance )
	{
		m_d3dx_effect->SetFloat( m_sun_distance_handle, sun_distance );
		return true;
	}

	
}