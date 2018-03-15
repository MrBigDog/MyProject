///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_font_manager.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : font_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "Stdafx.h"
#include "us_font_manager.h"

namespace uniscope_globe
{
	font_manager* singleton_font_manager::s_ins = NULL;

	font_manager::font_manager( void )
	{
		m_device = 0;
	}

	font_manager::font_manager( render_device* device )
	{
		m_device = device;
	}

	font_manager::~font_manager( void )
	{
		font_map::iterator itr = m_font_map.begin();
		for ( ; itr != m_font_map.end(); itr++ )
		{
			AUTO_RELEASE( itr->second );
		}
		m_font_map.clear();
	}

	void font_manager::create(  render_device* device  )
	{
		m_device = device;
	}

	void font_manager::destroy( void )
	{

	}

	void* font_manager::get_font( font_description& font_desp )
	{
		wchar_t key[128];
		memset( key, 0, sizeof(wchar_t)<<7 );
		wsprintf( key, L"%s_%d_%d_%d", font_desp.face_name.c_str(), font_desp.height, font_desp.weight, font_desp.italic );

		LPD3DXFONT ret_font = 0;
		font_map::iterator itr = m_font_map.find( key );
		if ( itr != m_font_map.end() )
		{
			ret_font = itr->second;
		}
		else
		{
			//m_device->create_font()
			D3DXCreateFont( (LPDIRECT3DDEVICE9)m_device->get_device(), font_desp.height, font_desp.width, font_desp.weight, 
							font_desp.mip, font_desp.italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
							DEFAULT_QUALITY, DEFAULT_PITCH, font_desp.face_name.c_str(), &ret_font );

			
			m_font_map[key] = ret_font;
		}

		return (void*)ret_font;
	}

}