///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_font_manager.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_font_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_font_manager.h"

namespace uniscope_globe
{
	d3d9_font_manager::d3d9_font_manager( render_device* rd )
	{
		d3d9_render_font* v_font = NULL;
		v_font = new d3d9_render_font();
		m_font_array.push_back(v_font);
		v_font = new d3d9_render_font();
		m_font_array.push_back(v_font);
		v_font = new d3d9_render_font();
		m_font_array.push_back(v_font);
	}

	d3d9_font_manager::~d3d9_font_manager( void )
	{
		render_font_array::iterator itr = m_font_array.begin();
		for( ; itr != m_font_array.end(); itr++ )
		{
			AUTO_DELETE( *itr );
		}
		m_font_array.clear();
	}

	ULONG_PTR d3d9_font_manager::get_font( long font_handle )
	{
		return (ULONG_PTR)(m_font_array[font_handle]);
	}

	long d3d9_font_manager::on_lost_device( void )
	{
		render_font_array::iterator itr = m_font_array.begin();
		for( ; itr != m_font_array.end(); itr++ )
		{
			(*itr)->on_lost_device();
		}
		return S_OK;
	}

	long d3d9_font_manager::on_reset_device( void )
	{
		render_font_array::iterator itr = m_font_array.begin();
		for( ; itr != m_font_array.end(); itr++ )
		{
			(*itr)->on_reset_device();
		}
		return S_OK;
	}

	
}