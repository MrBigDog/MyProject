///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_render_blend.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_render_blend class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_RENDER_BLEND_H_
#define _US_GEOMETRY_RENDER_BLEND_H_
#include <usGfx/Export.h>
#include <usUtil/us_common_file.h>
#include <usCore/us_common_render.h>

namespace uniscope_globe
{
	class d3d9_effect_common_mesh;
	class render_device;
	class render_argument;

	class USGFX_EXPORT geometry_render_blend : public common_render
	{
	protected:
		geometry_render_blend(void) {}

	public:
		geometry_render_blend(render_device* device);

		virtual ~geometry_render_blend(void);

	public:
		virtual void draw(render_argument* args);

		virtual void clear_background(void);

	public:
		void set_render(d3d9_effect_common_mesh* v_render);

	protected:
		d3d9_effect_common_mesh* m_render;
	};
}

#endif // _US_GEOMETRY_RENDER_BLEND_H_ 