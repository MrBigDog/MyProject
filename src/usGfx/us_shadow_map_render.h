
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shadow_map_render.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shadow_map_render class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SHADOW_MAP_RENDER_H_
#define _US_SHADOW_MAP_RENDER_H_
#include <usGfx/Export.h>
#include <usCore/us_render_node.h>
//#include <usUtil/us_common_file.h>
#include <d3d9.h>
namespace uniscope_globe
{
	class USGFX_EXPORT shadow_map_render : public render_node
	{
	public:
		shadow_map_render( render_device* v_device );

		virtual ~shadow_map_render( void );

	public:
		virtual void draw( render_argument* args );

	public:
		virtual void draw_scene( render_argument* args );

	public:
		LPDIRECT3DTEXTURE9 m_pShadowMap;
		LPDIRECT3DSURFACE9 g_pShadowSurf;

		LPDIRECT3DTEXTURE9 m_pScreenMap;
		LPDIRECT3DSURFACE9 m_pScreenSurf;
	};
}

#endif // _US_SHADOW_MAP_RENDER_H_ 