///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_effect.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_effect class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_RENDER_EFFECT_H_
#define _US_D3D9_RENDER_EFFECT_H_
#include <usGfx/Export.h>
#include <usCore/us_render_effect.h>
#include <usUtil/us_common_file.h>

#include <d3dx9effect.h>

namespace uniscope_globe
{
#define US_EFFECT_VERTEX_BLEND 0
#define US_EFFECT_SKY_BLEND 1
#define US_EFFECT_TERRAIN_RENDER 2

	class USGFX_EXPORT d3d9_render_effect : public render_effect
	{
	protected:
		d3d9_render_effect(void) {}

	public:
		d3d9_render_effect(render_device* rd);

		virtual ~d3d9_render_effect(void);

	public:
		virtual bool load_form_file(const LPCTSTR technique_file);

		virtual bool load_form_module_resource(long resource_handle);

		virtual long on_lost_device(void);

		virtual long on_reset_device(void);

		virtual bool begin(uint* pass, ulong flags);

		virtual bool end(void);

		virtual bool begin_pass(uint pass);

		virtual bool end_pass(void);

		virtual bool set_technique(void);

		virtual bool commit_changes(void);

	protected:
		virtual bool setup_handle(void);

	protected:
		render_device*	m_render_device;

		ID3DXEffect*	m_d3dx_effect;

		D3DXHANDLE		m_technique;
	};
}

#endif // _US_D3D9_RENDER_EFFECT_H_