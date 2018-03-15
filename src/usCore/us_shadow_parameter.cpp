///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shadow_parameter.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shadow_parameter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_shadow_parameter.h"

namespace uniscope_globe
{
	shadow_parameter::shadow_parameter( void )
	{
		m_azimuth_angle = 145 * RADIAN;
		m_elevation_angle = 65 * RADIAN;
		m_enable = false;
		m_stamp = 0;
	}

	shadow_parameter::shadow_parameter( double v_azimuth, double v_elevation )
	{
		m_azimuth_angle = v_azimuth;
		m_elevation_angle = v_elevation;
		m_enable = false;
		m_stamp = 0;
	}

	shadow_parameter::~shadow_parameter( void )
	{

	}

	
}