///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_sampler_state.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : sampler_state class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_sampler_state.h"
#include <usCore/us_render_device.h>

namespace uniscope_globe
{
	sampler_state::sampler_state(render_device* device)
	{
		m_render_device = device;
	}

	sampler_state::~sampler_state(void)
	{
		restore_state();
	}

	void sampler_state::set_state(ulong stage_index, ulong state_type, ulong state_value)
	{
		sampler_state_block temp_state = { stage_index, state_type, state_value };
		m_render_device->get_sampler_state(stage_index, state_type, &(temp_state.state_value));
		m_state_stack.push(temp_state);
		m_render_device->set_sampler_state(stage_index, state_type, state_value);
	}

	void sampler_state::restore_state(void)
	{
		while (m_state_stack.size() > 0)
		{
			sampler_state_block temp_state = m_state_stack.top();
			m_state_stack.pop();
			m_render_device->set_sampler_state(temp_state.stage_index, temp_state.state_type, temp_state.state_value);
		}
	}


}