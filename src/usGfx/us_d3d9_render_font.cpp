///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_font.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_font class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_render_font.h"

namespace uniscope_globe
{
	d3d9_render_font::d3d9_render_font( void )
	{

	}

	d3d9_render_font::~d3d9_render_font( void )
	{

	}

	long d3d9_render_font::on_lost_device( void )
	{
		//return m_d3dx_effect->OnLostDevice();
		return 0;
	}

	long d3d9_render_font::on_reset_device( void )
	{
		//return m_d3dx_effect->OnResetDevice();
		return 0;
	}

	
}