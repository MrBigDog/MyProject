///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_network_argument.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : network_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "us_network_argument.h"

namespace uniscope_globe
{
	network_argument::network_argument( void )
	{
		m_msg_id	= 0;
		m_flag		= 0;

		m_login		= 2;
		m_enter		= 2;
		
		m_ahead_speed	= 0.0f;
		m_rotate_speed	= 0.0f;
		m_animation_code = 0;
	}

	network_argument::network_argument( int msg_id, string scene_name )
	{
		m_msg_id	= msg_id;
		m_scene     = scene_name;
		m_flag		= 0;

		m_login		= 2;
		m_enter		= 2;

		m_ahead_speed	= 0.0f;
		m_rotate_speed	= 0.0f;
		m_animation_code = 0;
	}

	network_argument::~network_argument( void )
	{
	}

	
}