///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_reflection_surface.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_reflection_surface class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_REFLECTION_SURFACE_H_
#define _US_D3D9_EFFECT_REFLECTION_SURFACE_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_effect_reflection_surface : public d3d9_effect_base
	{
	protected:
		d3d9_effect_reflection_surface(void){}

	public:
		d3d9_effect_reflection_surface(render_device* device);		

		virtual ~d3d9_effect_reflection_surface( void );

	public:
		bool set_view_proj_matrix( const matrix4<float>& mat );

		bool set_world_matrix( const matrix4<float>& mat );

		bool set_eye_positon( const vector3<float>& vec );

		bool set_sky_texture( ULONG_PTR ht );

		bool set_bump_texture( ULONG_PTR ht );

		bool set_ref_texture( ULONG_PTR ht );

		//bool set_sun_alpha( float sun_alpha );

		//bool set_sun_theta( float sun_theta );

		bool set_sun_shininess( float sun_shininess );

		bool set_sun_strength( float sun_strength );

		//bool set_sun_distance( float sun_distance );

		bool set_sun_position( const vector3<float>& vec );

		bool set_water_color( const color_value& vec );

		bool set_water_bump_repeat(float repeat );

		bool begin_render( render_argument* args);

		bool end_render( render_argument* args);

	protected:
		virtual bool setup_handle( void );

	protected:
		D3DXHANDLE		m_view_proj_matrix_handle;

		D3DXHANDLE		m_world_matrix_handle;

		D3DXHANDLE		m_eye_positon_handle;

		D3DXHANDLE		m_sun_alpha_handle;

		D3DXHANDLE		m_sun_theta_handle;

		D3DXHANDLE		m_sun_shininess_handle;

		D3DXHANDLE		m_sun_strength_handle;

		D3DXHANDLE		m_sun_distance_handle;

		D3DXHANDLE		m_sky_texture_handle;	

		D3DXHANDLE		m_bump_texture_handle;

		D3DXHANDLE		m_ref_texture_handle;

		D3DXHANDLE		m_sun_position_handle;

		D3DXHANDLE		m_water_color_handle;

		D3DXHANDLE		m_water_bump_repeat_handle;

		LPDIRECT3DCUBETEXTURE9	m_sky_texture;

		LPDIRECT3DTEXTURE9		m_water_bump_texture;

		std::vector<LPDIRECT3DTEXTURE9>	m_ocean_bump_texture;
		 
	};
}

#endif // _US_D3D9_EFFECT_REFLECTION_SURFACE_H_