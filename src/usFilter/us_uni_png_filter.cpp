///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_uni_png_filter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : uni_png_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_uni_png_filter.h"

namespace uniscope_globe
{
	object_base* uni_png_filter::parse( raw_buffer* in_buffer )
	{
		CxImageMNG cx_image_engine;

		//从内存中读取
		byte* png_buff = in_buffer->get_buffer();
		int v_type = 0x474e5089;
		memcpy( png_buff, &v_type, sizeof(v_type) );
#ifndef WIN64
		cx_image_engine.ReadFromMemory(in_buffer->get_buffer(), in_buffer->get_size());
#else
		cx_image_engine.CxImage::Decode(in_buffer->get_buffer(), in_buffer->get_size(),CXIMAGE_FORMAT_PNG);
#endif
		

		if (!cx_image_engine.IsValid())
		{
			//内存读取失败
			cx_image_engine.Clear();
			return NULL;
		}

		ulong v_width    = cx_image_engine.GetWidth();
		ulong v_height   = cx_image_engine.GetHeight();

		int new_width = 0, new_height = 0;
		jpeg_loader::get_nearest_exp_size( v_height, v_width, 0.382f, new_height, new_width );
		if ( v_width != new_width || v_height != new_height )
		{
			cx_image_engine.Resample( new_width, new_height );
			v_width = new_width;
			v_height = new_height;
		}

		texture_info tex_info;
		tex_info.width = v_width;
		tex_info.height = v_height;
		tex_info.format = D3DFMT_DXT5;


		managed_buffer* v_buffer = new managed_buffer( in_buffer->get_size() );
		memcpy( v_buffer->get_buffer(), in_buffer->get_buffer(), in_buffer->get_size() );

		cx_image_engine.Clear();

		return (object_base*)(new d3d9_hardware_texture( v_buffer, tex_info , false ));	
	}
	
}