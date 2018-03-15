#include "StdAfx.h"
#include "us_d3d9_effect_flood.h"

namespace uniscope_globe
{
	d3d9_effect_flood::d3d9_effect_flood( render_device* rd )
		: d3d9_effect_base( rd )
	{
		load_from_module_resource(IDR_SHADER_FLOOD_ANALYSIS);
	}


	d3d9_effect_flood::~d3d9_effect_flood(void)
	{
	}

	bool d3d9_effect_flood::set_texture(ULONG_PTR ht)
	{
		m_d3dx_effect->SetTexture( _ColorTexture, (LPDIRECT3DBASETEXTURE9)ht );
		return true;
	}

	bool d3d9_effect_flood::set_view_matrix(const matrix4<double>& matrix)
	{
		m_d3dx_effect->SetMatrix(_matView,(D3DXMATRIX *)&matrix);
		return true;
	}

	bool d3d9_effect_flood::set_proj_matrix(const matrix4<double>& matrix)
	{
		m_d3dx_effect->SetMatrix(_matProject,(D3DXMATRIX *)&matrix);
		return true;
	}

	bool d3d9_effect_flood::set_world_matrix(const matrix4<double>& matrix)
	{
		m_d3dx_effect->SetMatrix(_matWorld,(D3DXMATRIX *)&matrix);
		return true;
	}

	bool d3d9_effect_flood::set_flood_offset(const float offset)
	{
		m_d3dx_effect->SetFloat(_UVOffset,offset);
		return true;
	}

	bool d3d9_effect_flood::set_flood_uvdistance(const float uvdistance)
	{
		m_d3dx_effect->SetFloat(_UVDistance,uvdistance);
		return true;
	}

	bool d3d9_effect_flood::set_upDataTime(const float time)
	{
		m_d3dx_effect->SetFloat(_upDataTime,time);
		return true;
	}

	bool d3d9_effect_flood::setup_handle(void)
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
			_upDataTime	            = m_d3dx_effect->GetParameterByName(0, "g_fTime");

		}
		return true;
	}

}
