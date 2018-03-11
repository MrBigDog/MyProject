//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_view_port.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : view port class
//	Reference : 
//
//////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "us_view_port.h"

namespace uniscope_globe
{
	view_port::view_port(void)
	{
		m_z_min = 0.0;
		m_z_max = 0.0;

		m_width = 0;
		m_height = 0;

		m_x = 0;
		m_y = 0;
	}

	view_port::~view_port(void)
	{
	}

}
