///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_ust_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : ust_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_ust_filter.h"

namespace uniscope_globe
{
	object_base* ust_filter::parse( raw_buffer* in_buffer )
	{
		square_buffer* v_square_buffer = (square_buffer*)in_buffer;
		ustring v_path = v_square_buffer->get_path();

		// ust [1/24/2018 mahc]
		//ust_buffer* v_ust_buffer = new ust_buffer(v_path.c_str(), in_buffer);
		//v_ust_buffer->initialize();

		//return new d3d9_hardware_mesh(v_ust_buffer);

		return NULL;
	}	

	
}