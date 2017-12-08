///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_circle.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : circle class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_CIRCLE_H_
#define _US_CIRCLE_H_

#include "us_vector3.h"

namespace uniscope_globe
{
	class circle
	{
	public:
		circle( void ) {}
		circle( vector3<double>& v_center, double v_radius );
		virtual ~circle( void ) {}

	public:
		vector3<double> m_center;
		double			m_radius;
	};
}

#endif // _US_CIRCLE_H_