//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_rect.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : rect template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#include "us_radian.h"
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	radian::radian(void)
	{

	}

	radian::~radian(void)
	{
	}

	double radian::to_degree(double rad)
	{
		return rad * DEGREE;
	}

	double radian::from_degree(double deg)
	{
		return deg * RADIAN;
	}
}