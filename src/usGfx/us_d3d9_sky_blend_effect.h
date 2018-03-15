///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_sky_blend_effect.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_sky_blend_effect class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_SKY_BLEND_EFFECT_H_
#define _US_D3D9_SKY_BLEND_EFFECT_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_sky_blend_effect : public d3d9_render_effect
	{
	protected:
		d3d9_sky_blend_effect(void){}

	public:
		d3d9_sky_blend_effect(render_device* device);		

		virtual ~d3d9_sky_blend_effect( void );

	public:
		bool set_transform( const matrix4<float>& wvp );

		bool set_texture( ulong ht );

		bool set_sun_alpha( float sun_alpha );

		bool set_sun_theta( float sun_theta );

		bool set_sun_shininess( float sun_shininess );

		bool set_sun_strength( float sun_strength );

		bool set_sun_distance( float sun_distance );

	protected:
		virtual bool setup_handle( void );

	protected:
		D3DXHANDLE		m_transform_handle;

		D3DXHANDLE		m_sun_alpha_handle;

		D3DXHANDLE		m_sun_theta_handle;

		D3DXHANDLE		m_sun_shininess_handle;

		D3DXHANDLE		m_sun_strength_handle;

		D3DXHANDLE		m_sun_distance_handle;

		D3DXHANDLE		m_cube_texture_handle;		 
	};
}

#endif // _US_D3D9_SKY_BLEND_EFFECT_H_