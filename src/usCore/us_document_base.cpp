///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_document_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : document_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_document_base.h"

namespace uniscope_globe
{
	document_base* document_base::s_document_ptr = NULL;

	document_base::document_base( void )
	{
		m_application = NULL;

		m_view = NULL;

		m_common_manager_group = NULL;

		m_pick_terrain_positon_geo = vector_3d::s_zero;

		m_map_render_mode = false;
	}

	document_base::document_base( application_base* in_app, view_base* in_view )
	{
		m_application = in_app;

		m_view = in_view;

		m_common_manager_group = m_application->get_manager_group();

		m_pick_terrain_positon_geo = vector_3d::s_zero;

		m_map_render_mode = false;
	}

	document_base::~document_base( void )
	{
		m_application = NULL;

		m_view = NULL;

		m_common_manager_group = NULL;
	}

	bool document_base::register_object( spatial_object* in_object )
	{
		if ( in_object->get_guid().size() == 0 )
			return false;

		object_map::iterator itr = m_object_map.find( in_object->get_guid() );
		if( itr == m_object_map.end() )
		{
			m_object_map.insert( make_pair( in_object->get_guid(), in_object ) );
			return true;
		}

		return false;
	}

	bool document_base::unregister_object( spatial_object* in_object )
	{
		if ( in_object->get_guid().size() == 0 )
			return false;

		object_map::iterator itr = m_object_map.find( in_object->get_guid() );
		if( itr != m_object_map.end() )
		{
			m_object_map.erase(itr);
			return true;
		}

		return false;
	}

	spatial_object* document_base::get_register_object( cpstr id )
	{
		object_map::iterator itr = m_object_map.find( id );
		if( itr != m_object_map.end() )
			return itr->second;

		return NULL;
	}

	void document_base::delete_object( cpstr id )
	{
		object_map::iterator itr = m_object_map.find( id );
		if( itr != m_object_map.end() )
		{
			spatial_object* v_object = itr->second;
			v_object->dissolve_attachment();
			AUTO_DELETE( v_object );

			m_object_map.erase( itr );
		}
	}	
}