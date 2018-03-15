///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_message_argument.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : message_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_message_argument.h"

namespace uniscope_globe
{
	message_argument::message_argument( void )
	{
		m_type = input_argument_type::US_INPUT_WINMSG;
		reset();
	}

	message_argument::~message_argument( void )
	{

	}

	void message_argument::reset( void )
	{
		m_message = 0;
		m_w_param  = 0;
		m_l_param  = 0;
	}
	
}