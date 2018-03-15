///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_sky_sphere.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_sky_sphere class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_SKY_SPHERE_H_
#define _US_D3D9_EFFECT_SKY_SPHERE_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_effect_sky_sphere : public d3d9_effect_base
	{
	protected:
		d3d9_effect_sky_sphere(void){}

	public:
		d3d9_effect_sky_sphere(render_device* device);		

		virtual ~d3d9_effect_sky_sphere( void );

	public:
		bool set_transform( const matrix4<float>& wvp );

		bool set_texture( ULONG_PTR ht );

		bool set_sun_azimuth( float sun_azimuth );

		bool set_sun_elevation( float sun_elevation );

		bool set_sun_shininess( float sun_shininess );

		bool set_sun_strength( float sun_strength );

		bool set_sun_distance( float sun_distance );

	protected:
		virtual bool setup_handle( void );

	protected:
		D3DXHANDLE		m_transform_handle;

		D3DXHANDLE		m_sun_azimuth_handle;

		D3DXHANDLE		m_sun_elevation_handle;

		D3DXHANDLE		m_sun_shininess_handle;

		D3DXHANDLE		m_sun_strength_handle;

		D3DXHANDLE		m_sun_distance_handle;

		D3DXHANDLE		m_cube_texture_handle;		 
	};
}

#endif // _US_D3D9_EFFECT_SKY_SPHERE_H_