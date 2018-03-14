///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_texture.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_texture class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_hardware_texture.h"

namespace uniscope_globe
{

	hardware_texture::hardware_texture(void)
	{
		m_creator = NULL;
	}

	hardware_texture::hardware_texture( raw_buffer* buffer )
	{
		m_creator = NULL;

	}

	hardware_texture::~hardware_texture(void)
	{

	}

	long hardware_texture::create( render_device* device )
	{
		return 0;
	}

	bool hardware_texture::destroy( void )
	{
		return false;
	}

	bool hardware_texture::clear_buffer()
	{
		return true;
	}

	void hardware_texture::draw( render_device* device, uint stage_index )
	{

	}

	texture_info hardware_texture::get_texture_info( void )
	{
		return m_texture_info;
	}
}