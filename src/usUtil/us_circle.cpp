///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_circle.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : circle class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_circle.h"

namespace uniscope_globe
{
	circle::circle( vector3<double>& v_center, double v_radius )
	{
		m_center = v_center;

		m_radius = v_radius;
	}
	
}