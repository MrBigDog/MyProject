///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_tga_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : tga_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_tga_filter.h"

namespace uniscope_globe
{
	object_base* tga_filter::parse( raw_buffer* in_buffer )
	{
		managed_buffer* v_buffer = new managed_buffer( in_buffer->get_size() );

		//	// ´´½¨DDSÎÆÀí
		DDSURFACEDESC2 ddsd;
		memcpy(&ddsd, in_buffer->get_buffer() + 4, sizeof(ddsd));

		texture_info tex_info;
		tex_info.width = ddsd.dwWidth;
		tex_info.height = ddsd.dwHeight;
		tex_info.format = ddsd.ddpfPixelFormat.dwFourCC;

		memcpy( v_buffer->get_buffer(), in_buffer->get_buffer(), in_buffer->get_size() );

		return (object_base*)(new d3d9_hardware_texture( v_buffer, tex_info , false ));	
	}

	
}