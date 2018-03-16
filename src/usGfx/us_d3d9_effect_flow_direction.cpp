//#include "StdAfx.h"
#include "us_d3d9_effect_flow_direction.h"
#include <usGfx/resource.h>

namespace uniscope_globe
{
	d3d9_effect_flow_direction::d3d9_effect_flow_direction( render_device* rd )
		: d3d9_effect_base( rd )
	{
		//load_from_file(L"E:\\fxl\\gwv2-x64\\activeX_src\\shader\\post_process\\waterFlow.fx");
		load_from_module_resource(IDR_SHADER_FLOW_DIRECTION);
	}


	d3d9_effect_flow_direction::~d3d9_effect_flow_direction(void)
	{
	}

	bool d3d9_effect_flow_direction::setup_handle( void )
	{
		if (m_d3dx_effect)
		{	
			m_technique				= m_d3dx_effect->GetTechniqueByName("DefaultTech");
			_ColorTexture			= m_d3dx_effect->GetParameterByName(0, "g_ColorTexture");
			_matWorld		        = m_d3dx_effect->GetParameterByName(0, "g_matWorld");
			_matView		        = m_d3dx_effect->GetParameterByName(0, "g_matView");
			_matProject		        = m_d3dx_effect->GetParameterByName(0, "g_matProject");
			_UVOffset		        = m_d3dx_effect->GetParameterByName(0, "g_UVOffset");
			_UVDistance		        = m_d3dx_effect->GetParameterByName(0, "g_UVDistance");
			_matViewWorldProj	    = m_d3dx_effect->GetParameterByName(0, "g_matWorldViewProj");
		
		}
		return true;

	}

	bool d3d9_effect_flow_direction::set_world_matrix( const matrix4<double>& matrix )
	{
		m_d3dx_effect->SetMatrix(_matWorld,(D3DXMATRIX *)&matrix);
		return true;
	}

	bool d3d9_effect_flow_direction::set_flow_offset( const float offset )
	{
		m_d3dx_effect->SetFloat(_UVOffset,offset);
		return true;
	}

	bool d3d9_effect_flow_direction::set_flow_uvdistance( const float uvdistance )
	{
		m_d3dx_effect->SetFloat(_UVDistance,uvdistance);
		return true;
	}

	bool d3d9_effect_flow_direction::set_view_matrix( const matrix4<double>& matrix )
	{
		m_d3dx_effect->SetMatrix(_matView,(D3DXMATRIX *)&matrix);
		return true;
	}
	bool d3d9_effect_flow_direction::set_proj_matrix( const matrix4<double>& matrix )
	{
		m_d3dx_effect->SetMatrix(_matProject,(D3DXMATRIX *)&matrix);
		return true;
	}

	bool d3d9_effect_flow_direction::set_texture( ULONG_PTR ht )
	{
		m_d3dx_effect->SetTexture( _ColorTexture, (LPDIRECT3DBASETEXTURE9)ht );
		return true;
	}

	bool d3d9_effect_flow_direction::set_viewworld_proj( const matrix4<double>& matrix )
	{
		m_d3dx_effect->SetMatrix(_matViewWorldProj,(D3DXMATRIX *)&matrix);
		return true;
	}

}

