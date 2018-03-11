///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_priority_stack.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : priority_stack class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_priority_stack.h"

namespace uniscope_globe
{
	
	DWORD singleton_priority_stack::m_tls_index = TlsAlloc();

	priority_stack::priority_stack( void )
	{

	}

	priority_stack::~priority_stack( void )
	{

	}
	
}