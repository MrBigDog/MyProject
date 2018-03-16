///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_render.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_render class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_RENDER_H_
#define _US_GEOMETRY_RENDER_H_
#include <usGfx/Export.h>
#include <usCore/us_common_render.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class d3d9_effect_common_mesh;

	class USGFX_EXPORT geometry_render : public common_render
	{
	protected:
		geometry_render(void) {}

	public:
		geometry_render(render_device* device);

		virtual ~geometry_render(void);

	public:
		virtual void draw(render_argument* args);

		//virtual void clear_background( void );

	public:
		void set_render(d3d9_effect_common_mesh* v_render);

	protected:
		d3d9_effect_common_mesh* m_render;
	};
}

#endif // _US_GEOMETRY_RENDER_H_