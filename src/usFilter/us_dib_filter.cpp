///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_dib_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : dib_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "us_dib_filter.h"

namespace uniscope_globe
{
	object_base* dib_filter::parse( raw_buffer* in_buffer )
	{
		texture_info v_text_info;
		v_text_info.format = US_TF_A8R8G8B8;
		v_text_info.width = 512;
		v_text_info.height = 512;
		v_text_info.mipmap = 1;
		managed_buffer* v_buffer = new managed_buffer( in_buffer->get_buffer(), in_buffer->get_size() );
		return new d3d9_hardware_texture(v_buffer, v_text_info, true );
	}
}