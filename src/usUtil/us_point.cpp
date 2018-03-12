///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_point.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : point class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_point.h"

namespace uniscope_globe
{
	point::point( void )
	{
		x = y = 0;
	}

	point::point( long n_x, long n_y )
	{
		x = n_x;
		y = n_y;
	}

	point::~point( void )
	{

	}

	
}