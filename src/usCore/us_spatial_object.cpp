///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_spatial_object.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : spatial_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_spatial_object.h"

namespace uniscope_globe
{
	spatial_object::spatial_object( void )
	{				
		m_aabb.set_extent( vector3<double>( -1.0, -1.0, -1.0 ), vector3<double>( 1.0, 1.0, 1.0 ) );

		m_is_intersectable = true;

		m_is_visible = true;

		m_is_selectable = true;

		m_is_aabb_dirty = false;

		m_node_attached = NULL;

		m_parent_object = NULL;

		m_document = NULL;

		m_object_flags = US_OBJECT_FLAGS_NONE;
	}

	spatial_object::spatial_object( document_base* in_doc )
	{
		m_aabb.set_extent( vector3<double>( -1.0, -1.0, -1.0 ), vector3<double>( 1.0, 1.0, 1.0 ) );

		m_is_intersectable = true;

		m_is_visible = true;

		m_is_selectable = true;

		m_node_attached = NULL;

		m_document = in_doc;

		m_object_flags = US_OBJECT_FLAGS_NONE;
	}

	spatial_object::~spatial_object( void )
	{
	}

	////////////////////////////////////////////////////////////////////
	//void spatial_object::initialize( document_base* v_doc )
	//{
	//	m_document = v_doc;
	//}

	void spatial_object::copy_from(spatial_object* v_object)
	{
		m_name			   = v_object->m_name;
		m_mem_size		   = v_object->m_mem_size;
		m_document         = v_object->m_document;
		m_is_visible       = v_object->m_is_visible;
		m_is_intersectable = v_object->m_is_intersectable;
		m_is_selectable    = v_object->m_is_selectable;
		m_object_flags     = v_object->m_object_flags;
		m_aabb		       = v_object->m_aabb;
	}

	void spatial_object::finalize( void )
	{
		spatial_object_final_release_argument args;
		args.m_object = this;

		m_on_object_finalize( &args );
		m_on_object_finalize.clear();
	}

	////////////////////////////////////////////////////////////////////
	void spatial_object::set_visible( bool is_enable )
	{
		m_is_visible = is_enable;
	}

	////////////////////////////////////////////////////////////////////
	void spatial_object::notify_attached(spatial_node* node)
	{
		m_node_attached = node;
	}

	// remove link with parent node actively
	void spatial_object::dissolve_attachment( void )
	{
		if ( m_node_attached )
		{
			m_node_attached->detach_object( this );

			m_node_attached = NULL;
		}
	}

	// Returns the scene node to which this object is attached.
	spatial_node* spatial_object::get_attached_node(void)
	{
		return m_node_attached;
	}

	bool spatial_object::is_attached(void) const
	{
		return (m_node_attached != NULL);
	}
}
