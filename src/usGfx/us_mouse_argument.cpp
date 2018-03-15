///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mouse_argument.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mouse_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_mouse_argument.h"

namespace uniscope_globe
{
	mouse_argument::mouse_argument( void )
	{
		m_type = input_argument_type::US_INPUT_MOUSE;
		reset();
	}

	mouse_argument::~mouse_argument( void )
	{

	}

	void mouse_argument::reset( void )
	{
		memset( (void*)&m_state, 0, sizeof(m_state) );
	};	
}