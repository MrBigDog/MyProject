///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_argument.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_render_argument.h"

namespace uniscope_globe
{
	render_argument::render_argument( void )
	{
		m_device = NULL;
		m_observer = NULL;
		m_frame_counter = NULL;
		m_sound_device = NULL;
		m_render = NULL;
		m_render_manager = NULL;
		m_render_flag = 0;
	}

	render_argument::render_argument( render_device* device, observer_base* in_observer )
	{
		m_device = device;
		m_observer = in_observer;
		m_frame_counter = NULL;
		m_sound_device = NULL;
		m_render = NULL;
		m_render_manager = NULL;
		m_render_flag = 0;

	}

	render_argument::render_argument( render_device* device, observer_base* in_observer, frame_counter* in_counter )
	{
		m_device = device;
		m_observer = in_observer;
		m_frame_counter = in_counter;
		m_sound_device = NULL;
		m_render = NULL;
		m_render_manager = NULL;
		m_render_flag = 0;
	}

	render_argument::render_argument( render_device* device, sound_device* in_sound_device, observer_base* in_observer, frame_counter* in_counter )
	{
		m_device = device;
		m_observer = in_observer;
		m_frame_counter = in_counter;
		m_sound_device = in_sound_device;
		m_render = NULL;
		m_render_manager = NULL;
		m_render_flag = 0;
	}

	render_argument::render_argument( render_device* device, sound_device* in_sound_device, observer_base* in_observer, frame_counter* in_counter, shadow_parameter& in_shadow_param )
	{
		m_device = device;
		m_observer = in_observer;
		m_frame_counter = in_counter;
		m_sound_device = in_sound_device;
		m_shadow_param = in_shadow_param;		
		m_render = NULL;
				m_render_manager = NULL;
		m_render_flag = 0;
	}

	render_argument::render_argument( render_device* device, sound_device* in_sound_device, observer_base* in_observer, frame_counter* in_counter, shadow_parameter& in_shadow_param, render_effect* in_render )
	{
		m_device = device;
		m_observer = in_observer;
		m_frame_counter = in_counter;
		m_sound_device = in_sound_device;
		m_shadow_param = in_shadow_param;
		m_render = in_render;
		m_render_manager = NULL;
		m_render_flag = 0;
	}

	render_argument::~render_argument( void )
	{

	}

	
}