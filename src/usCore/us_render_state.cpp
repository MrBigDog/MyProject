//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_state.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_state class
//	Reference : 
//
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_render_state.h"

namespace uniscope_globe
{
	render_state::render_state( render_device* device )
	{
		m_render_device = device;
	}

	render_state::~render_state( void )
	{
		restore_state();
	}


	void render_state::set_state( ulong state_type, ulong state_value )
	{
		render_state_block temp_state = { state_type, state_value };
		m_render_device->get_render_state( state_type, &(temp_state.state_value) );
		m_state_stack.push( temp_state );

		m_render_device->set_render_state( state_type, state_value);
	}

	void render_state::restore_state( void )
	{
		while( m_state_stack.size() > 0)
		{
			render_state_block temp_state = m_state_stack.top();
			m_state_stack.pop();
			m_render_device->set_render_state( temp_state.state_type, temp_state.state_value);
		}
	}
}
