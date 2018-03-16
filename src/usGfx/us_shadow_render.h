///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shadow_render.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shadow_render class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SHADOW_RENDER_H_
#define _US_SHADOW_RENDER_H_
#include <usGfx/Export.h>
#include <usCore/us_render_node.h>
#include <usUtil/us_common_file.h>

#include <map>

namespace uniscope_globe
{
	class shadow_clipper_render;
	class shadow_render_queue;

	class USGFX_EXPORT shadow_render : public render_node
	{
	public:
		typedef std::pair<ulong, ulong> material_key_pair;

		typedef std::map< material_key_pair, shadow_render_queue*> material_map;

	public:
		shadow_render(void);

		virtual ~shadow_render(void);

		// override from render_node
	public:
		virtual void draw(render_argument* args);

		virtual void push(render_object* v_obj);

		virtual void flush(void);

		virtual int get_count(void);

		virtual void clear_background(void);

		virtual void clear(void);

	public:
		void set_clipper_render(shadow_clipper_render* v_render);

		shadow_clipper_render* get_clipper_render(void);

	protected:
		// draw normal scene
		void draw_normal_scene(render_argument* args);

		// draw all shadow volume
		virtual void draw_shadow_volume(render_argument* args);

		// draw clipper volume
		virtual void draw_shadow_clipper_volume(render_argument* args);

		// draw scene pass one
		void draw_scene_pass_one(render_argument* args);

		// draw scene pass two
		void draw_scene_pass_two(render_argument* args);

		// draw all scene which shadow volume can influence
		virtual void draw_scene(render_argument* args);

	protected:
		material_map* m_current_render_map;

		material_map* m_background_render_map;

		shadow_clipper_render* m_clipper_render;

		US_AUTO_MUTEX
	};

}

#endif // _US_SHADOW_RENDER_H_