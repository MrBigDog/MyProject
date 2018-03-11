///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_style.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : style_base_xx class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_style_base.h"

namespace uniscope_globe
{
	style_base::style_base( void )
	{	
	}

	style_base::~style_base( void )
	{
		for ( int ni = 0; ni < (int)m_style_array.size(); ni++ )
		{
			AUTO_DELETE(m_style_array[ni]);
		}
	}

	void style_base::Initialize( styling_object* v_styling_obj )
	{

	}
}