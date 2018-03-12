
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_texture_fill_style.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : texture_fill_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_texture_fill_style.h"

namespace uniscope_globe
{
	texture_fill_style::texture_fill_style( void )
	{
		m_rtti = US_RTTI_TEXTURE_FILL_STYLE;
		m_fill_color  = 0xff999999;
		m_blend_color = 0x00000000;

		m_tu_start	= 0.0;
		m_tv_start	= 0.0;
		m_tu_end	= 1.0;
		m_tv_end	= 1.0;
		m_texture_rotation = 0.0;

		m_flip_texture_y = false;
		m_two_sided_mode = false;
	}

	texture_fill_style::texture_fill_style( const texture_fill_style& v_style )
	{
		m_rtti = US_RTTI_TEXTURE_FILL_STYLE;
		m_fill_color  = v_style.m_fill_color;
		m_blend_color = v_style.m_blend_color;

		m_tu_start	 = v_style.m_tu_start;
		m_tv_start	 = v_style.m_tv_start;
		m_tu_end	 = v_style.m_tu_end;
		m_tv_end	 = v_style.m_tv_end;
		m_texture_rotation = v_style.m_texture_rotation;

		m_flip_texture_y = false;
		m_two_sided_mode = false;
	}

	texture_fill_style::~texture_fill_style( void )
	{

	}

	//hardware_texture* icon_style::get_texture( void )
	//{
	//	if( m_str_texture_url.length() == 0 ) return NULL;
	//	if ( m_texture == NULL )
	//	{
	//		m_texture = (texture_resource_container*)singleton_common_manager_group::instance().get_texture_manager( L"icon" )->get_resource( m_str_texture_url );
	//		if ( m_texture == NULL )
	//		{
	//			m_texture = new texture_resource_container( m_str_texture_url, m_str_texture_url.c_str(), US_DOWNLOAD_IN_HEAP );
	//			m_texture->add_ref();
	//			singleton_common_manager_group::instance().get_texture_manager( L"icon" )->add_resource( m_str_texture_url, m_texture );
	//		}
	//	}

	//	return (hardware_texture*)m_texture->get_resource();
	//}


	
}