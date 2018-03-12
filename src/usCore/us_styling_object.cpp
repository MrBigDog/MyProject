///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_styling_object.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : styling_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_styling_object.h"

namespace uniscope_globe
{
	styling_object::styling_object( void )
	{
		m_render_style = NULL;
	}

	styling_object::~styling_object( void )
	{
		AUTO_DELETE( m_render_style );
	}

	void styling_object::set_render_style( style_base* v_style )
	{
		if ( m_render_style != NULL )
		{
			AUTO_DELETE( m_render_style );
		}

		m_render_style = v_style;
	}
	
}