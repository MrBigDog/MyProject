///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_common_render.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : common_render class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_common_render.h"

namespace uniscope_globe
{
	common_render::common_render( void )
	{
		m_current_render_array = new render_object_array;

		m_background_render_array = new render_object_array;

		m_qid = US_DEFAULT_RENDER_NODE;
	}

	common_render::~common_render( void )
	{
		clear();
	}

	void common_render::draw( render_argument* args )
	{
		{
			US_LOCK_AUTO_MUTEX

			render_object_array::iterator itr = m_current_render_array->begin();
			for( ; itr != m_current_render_array->end(); itr ++ )
			{
				(*itr)->draw( args );
			}

		}
		render_node::draw( args );
	}

	void common_render::flush( void )
	{
		// begin flip render_object
		{
			US_LOCK_AUTO_MUTEX

			render_object_array* temp_array = m_current_render_array;

			m_current_render_array = m_background_render_array;

			m_background_render_array = temp_array;

		}
		clear_background();

		render_node::flush();
	}

	void common_render::push( render_object* in_object )
	{
		in_object->add_ref();
		m_background_render_array->push_back( in_object );
	}

	void common_render::clear_background( void )
	{
		render_object_array::iterator itr = m_background_render_array->begin();
		for( ; itr != m_background_render_array->end(); itr ++ )
		{
			AUTO_RELEASE_SHARED_DATA( *itr );
		}

		m_background_render_array->clear();
	}

	void common_render::clear( void )
	{
		render_object_array::iterator itr = m_current_render_array->begin();
		for( ; itr != m_current_render_array->end(); itr ++ )
		{
			AUTO_RELEASE_SHARED_DATA( *itr );
		}
		AUTO_DELETE( m_current_render_array );

		itr = m_background_render_array->begin();
		for( ; itr != m_background_render_array->end(); itr ++ )
		{
			AUTO_RELEASE_SHARED_DATA( *itr );
		}

		AUTO_DELETE( m_background_render_array );
	}

	int common_render::get_count( void ) 
	{ 
		return m_current_render_array->size();
	}



	
}