//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_vector4.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : vector4 template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_vector4.h"

namespace uniscope_globe
{
#ifdef LINUX26
	template<> const vector4<float> vector4<float>::s_zero(0.0f,0.0f,0.0f,0.0f);
	template<> const vector4<float> vector4<float>::s_unit_x(1.0f,0.0f,0.0f,0.0f);
	template<> const vector4<float> vector4<float>::s_unit_y(0.0f,1.0f,0.0f,0.0f);
	template<> const vector4<float> vector4<float>::s_unit_z(0.0f,0.0f,1.0f,0.0f);
	template<> const vector4<float> vector4<float>::s_unit_w(0.0f,0.0f,0.0f,1.0f);

	template<> const vector4<double> vector4<double>::s_zero(0.0,0.0,0.0,0.0);
	template<> const vector4<double> vector4<double>::s_unit_x(1.0,0.0,0.0,0.0);
	template<> const vector4<double> vector4<double>::s_unit_y(0.0,1.0,0.0,0.0);
	template<> const vector4<double> vector4<double>::s_unit_z(0.0,0.0,1.0,0.0);
	template<> const vector4<double> vector4<double>::s_unit_w(0.0,0.0,0.0,1.0);
#endif //LINUX26

}