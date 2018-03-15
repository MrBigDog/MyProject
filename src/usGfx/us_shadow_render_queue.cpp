///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shadow_render_queue.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shadow_render_queue class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_shadow_render_queue.h"

namespace uniscope_globe
{
	shadow_render_queue::shadow_render_queue( void )
	{

	}

	shadow_render_queue::~shadow_render_queue( void )
	{
		clear();
	}

	void shadow_render_queue::push( render_object* v_ro )
	{
		v_ro->add_ref();
		m_render_object_queue.push_back( v_ro );
	}

	void shadow_render_queue::clear()
	{
		for ( int ni = 0; ni < (int)m_render_object_queue.size(); ni++ )
		{
			AUTO_RELEASE_SHARED_DATA( m_render_object_queue[ni] );
		}
		m_render_object_queue.clear();
	}
	
}