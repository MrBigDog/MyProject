///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_command_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : command_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_command_base.h"

namespace uniscope_globe
{
	command_base::command_base( void )
	{

	}

	command_base::~command_base( void )
	{

	}

	bool command_base::execute( void )
	{
		return true;
	}

	bool command_base::reverse_execute( void )
	{
		return true;
	}

	
}