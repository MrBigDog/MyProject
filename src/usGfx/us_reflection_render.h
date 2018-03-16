///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_reflection_render.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : reflection_render class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_REFLECTION_RENDER_H_
#define _US_REFLECTION_RENDER_H_
#include <usGfx/Export.h>
#include <usCore/us_render_node.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class render_target;

	class USGFX_EXPORT reflection_render : public render_node
	{
	public:
		typedef std::vector<render_object*> render_object_array;

		typedef std::vector<double> reflection_plane_array;

	public:
		reflection_render(void);

		virtual ~reflection_render(void);

	public:
		virtual void draw(render_argument* args);

		virtual void push(render_object* v_obj);

		virtual void flush(void);

		virtual int get_count(void);

		virtual void clear_background(void);

		virtual void clear(void);

	public:
		virtual void add_reflection_plane(double height);

	protected:
		virtual void draw_scene(render_argument* args);

	protected:
		render_object_array* m_current_render_array;

		render_object_array* m_background_render_array;

		render_target*	m_render_target;

		reflection_plane_array m_reflection_plane_array;



		US_AUTO_MUTEX

	};
}

#endif // _US_REFLECTION_RENDER_H_