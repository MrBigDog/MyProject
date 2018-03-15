///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_hardware_occlusion_query.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_hardware_occlusion_query class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_HARDWARE_OCCLUSION_QUERY_H_
#define _US_D3D9_HARDWARE_OCCLUSION_QUERY_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_hardware_occlusion_query : public hardware_occlusion_query
	{
	public:
		d3d9_hardware_occlusion_query( IDirect3DDevice9* device );

		virtual ~d3d9_hardware_occlusion_query( void );

	public:
		virtual void begin_occlusion_query( void );

		virtual void end_occlusion_query( void );

		virtual void issue_begin( void );

		virtual void issue_end( void );

		virtual bool pull_occlusion_query(uint* num_of_fragments);

		virtual bool is_still_outstanding( void );

	public:
		virtual long on_lost_device( void );

		virtual long on_reset_device( void );

	public:
		IDirect3DDevice9*		m_device;

		LPDIRECT3DQUERY9		m_query;				// The occlusion query

		LPD3DXRENDERTOSURFACE	m_occlusion_render;		// Occlusion's render to surface

		LPDIRECT3DSURFACE9		m_occlusion_surface;   // Occlusion's surface that it uses

		LPDIRECT3DTEXTURE9		m_occlusion_texture;   // Texture to get surface fro

		
	};
}

#endif // _US_D3D9_HARDWARE_OCCLUSION_QUERY_H_