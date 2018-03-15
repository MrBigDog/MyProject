///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_spatial_object.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : spatial_object class
//	Reference : Ogre OgreNode.h
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SPATIAL_OBJECT_H_
#define _US_SPATIAL_OBJECT_H_

#include <usCore\Export.h>
#include <usUtil\us_object_base.h>
#include <usUtil\us_axis_aligned_box.h>
#include <usCore\us_event_argument.h>
#include <usCore\us_event_base.h>

namespace uniscope_globe
{
#define US_OBJECT_FLAGS_NONE				0x0
#define US_OBJECT_FLAGS_STATIC_OBJECT		0x1
#define US_OBJECT_FLAGS_UNDERGROUND_OBJECT  0x2
#define US_OBJECT_FLAGS_DUMMY_OBJECT		0x4

	class spatial_node;
	class document_base;
	class update_argument;

	class USCORE_EXPORT spatial_object : virtual public object_base
	{
	public:
		spatial_object(void);

		spatial_object(document_base* in_doc);

		virtual ~spatial_object(void);

	public:
		virtual void initialize(document_base* v_doc) { m_document = v_doc; }

		virtual void finalize(void);

		virtual bool update(update_argument* v_args) { return true; }

		virtual void copy_from(spatial_object* v_obj);

	public:
		virtual void set_document(document_base* v_document) { m_document = v_document; }
		virtual document_base* get_document(void) { return m_document; }

		// visible
		virtual void set_visible(bool is_enable);
		virtual bool is_visible(void) { return m_is_visible; }

		// intersect
		virtual void set_intersectable(bool in_enable) { m_is_intersectable = in_enable; }
		virtual bool is_intersectable(void) { return m_is_intersectable; }

		// selectable
		virtual void set_selectable(bool in_enable) { m_is_selectable = in_enable; }
		virtual bool is_selectable(void) { return m_is_selectable; }

		// object flags
		virtual void  set_object_flags(int flags) { m_object_flags = flags; }
		virtual int get_object_flags(void) { return m_object_flags; }

		// script
		virtual void set_script(cpstr in_script) { m_script = in_script; }
		virtual ustring get_script(void) { return m_script; }

	public:
		virtual matrix_4d get_matrix_cart2geo(void) { return matrix_4d::s_identity; }

		// bounding box
		virtual void set_aabb(axis_aligned_box<double>& in_box) { m_aabb = in_box; }
		virtual axis_aligned_box<double> get_aabb() { return m_aabb; }

		virtual void set_aabb_dirty(void) { m_is_aabb_dirty = true; }

		virtual void set_parent_object(spatial_object* val) { m_parent_object = val; }
		virtual spatial_object* get_parent_object(void) { return m_parent_object; }

	public:
		// node
		virtual void notify_attached(spatial_node* node);

		virtual void dissolve_attachment(void);

		virtual spatial_node* get_attached_node(void);

		virtual bool is_attached(void) const;

		virtual bool refresh(void) { return false; }

	protected:
		document_base*				m_document;
		bool						m_is_visible;
		bool						m_is_intersectable;
		bool						m_is_selectable;
		int							m_object_flags;
		axis_aligned_box<double>	m_aabb;
		bool						m_is_aabb_dirty;
		ustring						m_script;

	protected:
		spatial_node*				m_node_attached;
		spatial_object*				m_parent_object;

		// event
	public:
		event_base					m_on_object_finalize;
	};

	typedef std::vector<spatial_object> spatial_object_array;

	class spatial_object_final_release_argument
		: public event_argument
	{
	public:
		spatial_object* m_object;
	};
}

#endif // _US_SPATIAL_OBJECT_H_