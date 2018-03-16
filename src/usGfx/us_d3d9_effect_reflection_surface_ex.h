///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_reflection_surface_ex.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_reflection_surface_ex class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_REFLECTION_SURFACE_EX_H_
#define _US_D3D9_EFFECT_REFLECTION_SURFACE_EX_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_effect_base.h>
#include <usUtil/us_matrix4.h>
#include <usUtil/us_common_file.h>
#include <usCore/us_extended_material.h>
namespace uniscope_globe
{
	class render_argument;

	class USGFX_EXPORT d3d9_effect_reflection_surface_ex : public d3d9_effect_base
	{
	protected:
		d3d9_effect_reflection_surface_ex(void) {}

	public:
		d3d9_effect_reflection_surface_ex(render_device* device);

		virtual ~d3d9_effect_reflection_surface_ex(void);

	public:
		bool set_view_proj_matrix(const matrix4<float>& mat);

		bool set_world_matrix(const matrix4<float>& mat);

		bool set_eye_positon(const vector3<float>& vec);

		bool set_sky_texture(ULONG_PTR ht);

		bool set_bump0_texture(ULONG_PTR ht);

		bool set_bump1_texture(ULONG_PTR ht);

		bool set_reflect_texture(ULONG_PTR ht);

		bool set_refract_texture(ULONG_PTR ht);

		bool set_alpha_color_texture(ULONG_PTR ht);

		//bool set_sun_alpha( float sun_alpha );

		//bool set_sun_theta( float sun_theta );

		bool set_sun_shininess(float sun_shininess);

		bool set_sun_strength(float sun_strength);

		//bool set_sun_distance( float sun_distance );

		bool set_water_depth(float water_depth);

		bool set_sun_position(const vector3<float>& vec);

		bool set_water_color(const color_value& vec);

		bool set_water_bump_repeat(float repeat);

		bool begin_render(render_argument* args);

		bool end_render(render_argument* args);

		bool update_offset(float dt);

		bool set_texture_offset(const vector4<float>& vec);

	protected:
		virtual bool setup_handle(void);

	protected:
		D3DXHANDLE		m_view_proj_matrix_handle;

		D3DXHANDLE		m_world_matrix_handle;

		D3DXHANDLE		m_world_matrix_inv_handle;

		D3DXHANDLE		m_eye_positon_handle;

		D3DXHANDLE		m_sun_alpha_handle;

		D3DXHANDLE		m_sun_theta_handle;

		D3DXHANDLE		m_sun_shininess_handle;

		D3DXHANDLE		m_sun_strength_handle;

		D3DXHANDLE		m_sun_distance_handle;

		D3DXHANDLE		m_sky_texture_handle;

		D3DXHANDLE		m_bump0_texture_handle;

		D3DXHANDLE		m_bump1_texture_handle;

		D3DXHANDLE		m_reflect_texture_handle;

		D3DXHANDLE		m_refract_texture_handle;

		D3DXHANDLE		m_alpha_color_texture_handle;

		D3DXHANDLE		m_sun_position_handle;

		D3DXHANDLE		m_water_color_handle;

		D3DXHANDLE		m_water_bump_repeat_handle;

		D3DXHANDLE		m_texture_offset_handle;

		D3DXHANDLE		m_water_depth_handle;

		LPDIRECT3DCUBETEXTURE9	m_sky_texture;

		LPDIRECT3DTEXTURE9		m_water_bump_texture;

		std::vector<LPDIRECT3DTEXTURE9>	m_ocean_bump_texture;

		vector4<float> m_texture_offsets;

		vector3<float> mRateNormMap0;
		vector3<float> mRateNormMap1;

	};
}

#endif // _US_D3D9_EFFECT_REFLECTION_SURFACE_EX_H_