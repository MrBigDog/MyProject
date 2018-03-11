///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_animation_filter_ex.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : animation_filter_ex class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "us_animation_filter_ex.h"

namespace uniscope_globe
{
	animation_filter_ex::animation_filter_ex( void )
	{

	}

	animation_filter_ex::~animation_filter_ex( void )
	{

	}

	object_base* animation_filter_ex::parse( raw_buffer* in_buffer )
	{
		square_buffer* s_b = (square_buffer* )in_buffer;

		unsigned char* v_buffer = ( unsigned char* )( in_buffer->get_buffer() + 4 );
		ulong v_size = in_buffer->get_size() - 4;

		usx_parser_object_container v_container;

		if ( !singleton_usx_parser_assistant::instance().parse_x_buffer( v_buffer, v_size, &v_container ) ) 
		{
			return NULL;
		}

		animation_set_resource* v_resource = new animation_set_resource;
		v_resource->m_ptr = v_container.m_animation_set_array[0];
		v_container.m_animation_set_array.clear();

		return v_resource;
	}
}