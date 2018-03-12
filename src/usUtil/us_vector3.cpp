//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_vector3.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : vector3 template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_vector3.h"

namespace uniscope_globe
{
#ifdef LINUX26
	template<> const vector3<float> vector3<float>::s_zero(0.0f, 0.0f, 0.0f);
	template<> const vector3<float> vector3<float>::s_unit_x(1.0f, 0.0f, 0.0f);
	template<> const vector3<float> vector3<float>::s_unit_y(0.0f, 1.0f, 0.0f);
	template<> const vector3<float> vector3<float>::s_unit_z(0.0f, 0.0f, 1.0f);

	template<> const vector3<double> vector3<double>::s_zero(0.0, 0.0, 0.0);
	template<> const vector3<double> vector3<double>::s_unit_x(1.0, 0.0, 0.0);
	template<> const vector3<double> vector3<double>::s_unit_y(0.0, 1.0, 0.0);
	template<> const vector3<double> vector3<double>::s_unit_z(0.0, 0.0, 1.0);
#endif //LINUX26

} // namespace uniscope_globe
