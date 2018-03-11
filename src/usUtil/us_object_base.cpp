//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_object_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : object base class
//	Reference : 
//
//////////////////////////////////////////////////////////////////////////

#include "us_object_base.h"

namespace uniscope_globe
{
	object_base::object_base(void)
	{
		m_rtti = US_RTTI_NONE;

		m_mem_size = 0;
	}

	object_base::~object_base(void)
	{

	}
}
