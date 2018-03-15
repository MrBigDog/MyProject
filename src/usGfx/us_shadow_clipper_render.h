///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shadow_clipper_render.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shadow_clipper_render class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SHADOW_CLIPPER_RENDER_H_
#define _US_SHADOW_CLIPPER_RENDER_H_

namespace uniscope_globe
{
	class LIB_GFX_API shadow_clipper_render : public render_node
	{
	public:
#if _MSC_VER >= 1900
		typedef std::unordered_map< ulong, shadow_render_queue*> material_map;
#else
		typedef stdext::hash_map< ulong, shadow_render_queue*> material_map;
#endif


	public:
		shadow_clipper_render( void );

		virtual ~shadow_clipper_render( void );

		// override from render_node
	public:
		virtual void draw( render_argument* args );

		virtual void push( render_object* v_obj);

		virtual void flush( void );

		virtual void clear_background( void );

		virtual void clear( void );

	public:
		virtual long get_status( void );

		virtual void begin_clipper( render_object* v_clipper_ro );

		virtual void end_clipper( void );

	protected:
		virtual void draw_clip_mesh( render_argument* args );

		// draw all shadow volume
		virtual void draw_shadow_volume( render_argument* args );

	protected:
		material_map* m_current_render_map;

		material_map* m_background_render_map;

		render_object* m_clipper_ro;

		US_AUTO_MUTEX
	};
}

#endif // _US_SHADOW_CLIPPER_RENDER__H_ 