///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_hardware_texture.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_hardware_texture class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_hardware_texture.h"

namespace uniscope_globe
{

	//static int s_tex_count = 0;
	d3d9_hardware_texture::d3d9_hardware_texture( void )
	{
		m_d3d_texture = NULL;
		m_raw_buffer = NULL;
		m_copy_mode = false;
		m_valid = false;

		//s_tex_count ++;
	}

	d3d9_hardware_texture::d3d9_hardware_texture( raw_buffer* in_buffer, texture_info& tex_info, bool copy_mode )
	{
		m_d3d_texture	= NULL;
		m_raw_buffer	= in_buffer;
		m_texture_info	= tex_info;
		m_copy_mode		= copy_mode;
		m_valid = false;

		//s_tex_count ++;
	}

	d3d9_hardware_texture::d3d9_hardware_texture( void* text_handle )
	{
		m_d3d_texture = LPDIRECT3DTEXTURE9( text_handle );
		m_raw_buffer = NULL;
		m_valid = true;
		//s_tex_count ++;
	}

	d3d9_hardware_texture::~d3d9_hardware_texture( void )
	{
		destroy();	

		//s_tex_count--;
		//US_TRACE1( L"tex %d /n", s_tex_count );
	}

	bool d3d9_hardware_texture::create_from_file( render_device* device, const LPCTSTR texture_path )
	{
		if ( m_valid ) return true;

		m_valid = device->create_texture_from_file( texture_path, (void**)&m_d3d_texture );

		return m_valid;
	}

	bool d3d9_hardware_texture::create_from_resource( render_device* device, ULONG_PTR handle )
	{
		if ( m_valid ) return true;

		m_valid = device->create_texture_from_resource( handle, (void**)&m_d3d_texture );

		return m_valid;
	}

	long d3d9_hardware_texture::create( render_device* device )
	{
		if ( !m_raw_buffer )
		{
			return 0;
		}

		if ( m_valid )
		{
			return 0;
		}

		if ( device->can_create_hardware() == false )
			return 0;

		if( m_copy_mode )
		{
			m_mem_size = device->create_texture_from_buffer( m_raw_buffer, &m_texture_info, (void**)&m_d3d_texture );
		}
		else
		{
			m_mem_size = device->create_texture_from_file_in_memory( m_raw_buffer, (void**)&m_d3d_texture );
		}

		AUTO_DELETE( m_raw_buffer );

		m_valid = m_mem_size>0?true:false;
		
		return m_mem_size;
	}

	bool d3d9_hardware_texture::destroy( void )
	{
		AUTO_DELETE( m_raw_buffer );
		AUTO_RELEASE( m_d3d_texture );
		m_valid = false;
		return true;
	}

	bool d3d9_hardware_texture::refresh( void )
	{
		if ( !m_valid )
		{
			//(*singleton_common_manager_group::instance().get_texture_creator()) += this;
			singleton_common_manager_group::instance().get_hardware_creator()->insert_texture( this );
		}

		return m_valid;
	}

	void d3d9_hardware_texture::draw( render_device* device, uint stage_index )
	{
		device->set_texture( stage_index, (texture_handle)m_d3d_texture );
	}

	ULONG_PTR d3d9_hardware_texture::get_handle(void)
	{
		return (ULONG_PTR)m_d3d_texture;
	}
}