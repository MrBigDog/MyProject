//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_math.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//
//////////////////////////////////////////////////////////////////////////

#include "us_math.h"

namespace uniscope_globe
{
#ifdef LINUX26
	template<> const float math<float>::s_epsilon  = FLT_EPSILON;
	template<> const float math<float>::s_zero_tolerance = 1e-06f;
	template<> const float math<float>::s_min_real = FLT_MIN;
	template<> const float math<float>::s_max_real = FLT_MAX;
	template<> const float math<float>::s_pi = (float)PI;
	template<> const float math<float>::s_two_pi = (float)(PI * 2.0);
	template<> const float math<float>::s_half_pi = (float)(PI * 0.5);
	template<> const float math<float>::s_deg_to_rad = (float)(PI / 180.0);
	template<> const float math<float>::s_rad_to_deg = (float)(180.0 / PI);


	template<> const double math<double>::s_epsilon  = DBL_EPSILON;
	template<> const double math<double>::s_zero_tolerance = 1e-10;
	template<> const double math<double>::s_min_real = DBL_MIN;
	template<> const double math<double>::s_max_real = DBL_MAX;
	template<> const double math<double>::s_pi = PI;
	template<> const double math<double>::s_two_pi = PI * 2.0;
	template<> const double math<double>::s_half_pi = PI * 0.5;
	template<> const double math<double>::s_deg_to_rad = PI / 180.0;
	template<> const double math<double>::s_rad_to_deg = 180.0 / PI;
#endif //LINUX26

}
