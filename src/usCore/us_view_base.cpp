///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_view_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : view_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_view_base.h"

namespace uniscope_globe
{
	view_base::view_base( void )
	{
		m_application = NULL;

		m_render_device = NULL;

		m_sound_device = NULL;
	}

	view_base::view_base( application_base* in_app )
	{
		m_application = in_app;

		m_render_device = NULL;

		m_sound_device = NULL;
	}

	view_base::~view_base( void )
	{		
	}

	
}