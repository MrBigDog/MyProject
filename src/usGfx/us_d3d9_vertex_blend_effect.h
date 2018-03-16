///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_vertex_blend_effect.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_vertex_blend_effect class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_VERTEX_BLEND_EFFECT_H_
#define _US_D3D9_VERTEX_BLEND_EFFECT_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_render_effect.h>
#include <usUtil/us_matrix4.h>
//#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class hardware_texture;
	class USGFX_EXPORT d3d9_vertex_blend_effect : public d3d9_render_effect
	{
	protected:
		d3d9_vertex_blend_effect(void) {}

	public:
		d3d9_vertex_blend_effect(render_device* device);

		virtual ~d3d9_vertex_blend_effect(void);

	public:
		bool set_transform(const matrix4<float>& wvp);

		bool set_texture(hardware_texture* ht);

		bool set_matrix_array(void* matrix_array, ulong count);

	protected:
		virtual bool setup_handle(void);

	protected:
		D3DXHANDLE		m_world_view_projection;

		D3DXHANDLE		m_transformation;

		D3DXHANDLE		m_texture;
	};
}

#endif // _US_D3D9_VERTEX_BLEND_EFFECT_H_