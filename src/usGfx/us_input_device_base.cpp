///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_input_device_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : input_device_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_input_device_base.h"

namespace uniscope_globe
{
	input_device_argument::input_device_argument( void )
	{
		reset();
	}

	input_device_argument::~input_device_argument( void )
	{

	}

	void input_device_argument::reset(void)
	{
		m_type = input_argument_type::US_INPUT_DEVICE;

		memset( (void*)&m_state, 0, sizeof(input_device_state) );
	}

	input_device_base::input_device_base( void )
	{
	}

	input_device_base::~input_device_base( void )
	{

	}
}