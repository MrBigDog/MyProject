///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_terrain_blend_effect.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_terrain_blend_effect class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_TERRAIN_BLEND_EFFECT_H_
#define _US_D3D9_TERRAIN_BLEND_EFFECT_H_

#include <usGfx/Export.h>
#include <usGfx/us_d3d9_render_effect.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_matrix4.h>
#include <d3d9.h>

namespace uniscope_globe
{
	class hardware_texture;

	class USGFX_EXPORT d3d9_terrain_blend_effect : public d3d9_render_effect
	{
	protected:
		d3d9_terrain_blend_effect(void) {}

	public:
		d3d9_terrain_blend_effect(render_device* device);

		virtual ~d3d9_terrain_blend_effect(void);

	public:
		bool set_transform(const matrix4<float>& wvp);

		bool set_texture_transform(const matrix4<float>& tm);

		bool set_texture(hardware_texture* ht);

	protected:
		virtual bool setup_handle(void);

	protected:
		D3DXHANDLE		m_transform_handle;

		D3DXHANDLE		m_texture_transform_handle;

		D3DXHANDLE		m_texture_handle;

	};
}

#endif // _US_D3D9_TERRAIN_BLEND_EFFECT_H_