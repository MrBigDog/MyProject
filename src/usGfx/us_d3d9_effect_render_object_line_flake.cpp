
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_render_object_line_flake.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_render_object_line_flake class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "us_d3d9_effect_render_object_line_flake.h"

namespace uniscope_globe
{
	d3d9_effect_render_object_line_flake::d3d9_effect_render_object_line_flake( render_device* device )
		:d3d9_effect_base(device)
	{
		load_from_module_resource(IDR_SHADER_GEOMETRY_LINE_FLAKE);	
		//load_from_file(L"c:\\geometry_line.fx");
	}

	d3d9_effect_render_object_line_flake::~d3d9_effect_render_object_line_flake( void )
	{

	}

	bool d3d9_effect_render_object_line_flake::setup_handle( void )
	{
		if (m_d3dx_effect)
		{	
			m_technique					= m_d3dx_effect->GetTechniqueByName("GeometryLineTech");
			m_transform_handle			= m_d3dx_effect->GetParameterByName(0, "matWorldViewProj");
			m_view_proj_matrix_handle	= m_d3dx_effect->GetParameterByName(0, "matViewProj");
			m_view_port_scale_handle	= m_d3dx_effect->GetParameterByName(0, "mViewportScale");
			//m_half_width_handle		= m_d3dx_effect->GetParameterByName(0, "mHalfWidth");
			//m_view_direction_handle   = m_d3dx_effect->GetParameterByName(0, "mViewDir");
		}
		return true;
	}

	bool d3d9_effect_render_object_line_flake::set_transform( const matrix4<float>& wvp )
	{
		m_d3dx_effect->SetMatrix( m_transform_handle, (D3DXMATRIX *)&wvp );

		return true;
	}


	bool d3d9_effect_render_object_line_flake::set_view_proj_matrix( const matrix4<float>& tm )
	{
		m_d3dx_effect->SetMatrix( m_view_proj_matrix_handle, (D3DXMATRIX *)&tm );

		return true;
	}

	bool d3d9_effect_render_object_line_flake::set_line_param( int view_port_width, int view_port_height, float line_width )
	{
		m_d3dx_effect->SetVector( m_view_port_scale_handle, &D3DXVECTOR4( line_width/(float)view_port_width, line_width/(float)view_port_height, 0,0) );

		return true;
	}	
}