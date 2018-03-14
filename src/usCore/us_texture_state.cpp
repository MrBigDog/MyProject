///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_textue_state.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : texture_state class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_texture_state.h"
#include <usCore/us_render_device.h>

namespace uniscope_globe
{
	texture_state::texture_state(render_device* device)
	{
		m_render_device = device;
	}

	texture_state::~texture_state(void)
	{
		restore_state();
	}

	void texture_state::set_state(ulong stage_index, ulong state_type, ulong state_value)
	{
		texture_state_block temp_state = { stage_index, state_type, state_value };
		m_render_device->get_texture_state(stage_index, state_type, &(temp_state.state_value));
		m_state_stack.push(temp_state);

		m_render_device->set_texture_state(stage_index, state_type, state_value);
	}

	void texture_state::restore_state(void)
	{
		while (m_state_stack.size() > 0)
		{
			texture_state_block temp_state = m_state_stack.top();
			m_state_stack.pop();
			m_render_device->set_texture_state(temp_state.stage_index, temp_state.state_type, temp_state.state_value);
		}
	}


}