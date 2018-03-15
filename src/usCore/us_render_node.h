///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_node.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_node class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_NODE_H_
#define _US_RENDER_NODE_H_

#include <usCore\Export.h>
#include <usCore\us_render_object.h>

#include <usUtil/us_mutex.h>

namespace uniscope_globe
{
	enum us_render_node_id
	{
		US_ANNOTATION_RENDER_NODE = 110,
		US_OVERLAY_RENDER_NODE = 100,
		US_SPHERE_RENDER_NODE = 90,
		US_VECTOR_RENDER_NODE = 80,
		US_POI_RENDER_NODE = 70,
		US_USERDATA_RENDER_NODE = 60,
		US_COMMON_RENDER_NODE = 50,
		US_DEFAULT_RENDER_NODE = 30,
		US_BLDG_RENDER_NODE = 10,
		US_BACKGROUND_RENDER_NODE = 0,
	};

	class render_argument;

	class USCORE_EXPORT render_node : public render_object
	{
	public:
		typedef std::vector<render_node *> render_node_array;

	public:
		render_node(void);

		virtual ~render_node(void);

		// attribute
	public:
		int get_qid(void) { return m_qid; }

		//int get_priority( void ) { return m_priority; }
		//void set_priority( int v_priority ) { m_priority = v_priority; }

		// interface 
	public:
		virtual void draw(render_argument* args);

		virtual void draw_alpha(render_argument* args);

		virtual void draw(render_argument* args, void* in_mesh);

		virtual void set_texture(render_argument* args, void* in_texture, uint stage_index);

		virtual void occlusion(render_argument* args);

		virtual void flush(void);

		virtual void push(render_object* ro) { }

		//template<typename KEY>
		//virtual void push( KEY& key, render_object* rb ) {}

	public:
		virtual render_object* get_attach_object(int index) { return NULL; }

		virtual int get_attach_object_count(void) { return 0; }


		// child render
	public:
		virtual void add_child(render_node* in_render);

		virtual render_node* get_child(int v_index);

		virtual int get_child_count(void);

		virtual void remove_child(render_node* in_render);

		virtual void remove_all(void);

		virtual void sort(bool bAscending);

	protected:
		int m_qid;

		int m_priority;

		render_node_array m_child_node_array;

		US_MUTEX(base_mutex)
	};
}

#endif // _US_RENDER_NODE_H_