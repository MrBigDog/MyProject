///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "us_hardware_base.h"

namespace uniscope_globe
{
	hardware_base::hardware_base(void)
	{	
		m_rank			= 0;
		m_raw_buffer	= NULL;	
	}

	hardware_base::~hardware_base( void )
	{
		AUTO_DELETE( m_raw_buffer );
	}

	uint hardware_base::get_rank( void )
	{
		return m_rank;
	}

	void hardware_base::set_rank( uint n_rank )
	{
		m_rank = n_rank;
	}

	bool hardware_base::refresh( void )
	{
		return m_valid;
	}

	ULONG_PTR hardware_base::get_handle( void )
	{
		return 0;
	}

	
}