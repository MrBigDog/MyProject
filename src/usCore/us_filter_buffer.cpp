///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_filter_buffer.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : filter_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_filter_buffer.h"
#include <usUtil/us_object_base.h>

namespace uniscope_globe
{
	filter_buffer::filter_buffer(void* in_buffer, int in_size, object_base* in_parent, object_base* in_container)
	{
		m_buffer = (byte*)in_buffer;

		m_size = in_size;

		m_parent = in_parent;

		m_container = in_container;
	}

	filter_buffer::~filter_buffer(void)
	{

	}


}