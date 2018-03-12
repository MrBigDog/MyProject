//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_math.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _US_MATH_H_
#define _US_MATH_H_

#ifdef WIN32
#include <usUtil/us_common_file.h>
#endif

namespace uniscope_globe
{
	template <class real>
	class math
	{
	public:
		math(void) {}

		virtual ~math() {}

		// Wrappers to hide implementations of functions.  The ACos and ASin
		// functions clamp the input argument to [-1,1] to avoid NaN issues
		// when the input is slightly larger than 1 or slightly smaller than -1.
		// Other functions have the potential for using a fast and approximate
		// algorithm rather than calling the standard math library functions.

		static real acos_(real value);

		static real asin_(real value);

		static real atan_(real value);

		static real atan2_(real fY, real fX);

		static real ceil_(real value);

		static real cos_(real value);

		static real exp_(real value);

		static real fabs_(real value);

		static real floor_(real value);

		static real fmod_(real fX, real fY);

		static real inv_sqrt_(real value);

		static real log_(real value);

		static real pow_(real fBase, real fExponent);

		static real sin_(real value);

		static real sqr_(real value);

		static real sqrt_(real value);

		static real tan_(real value);

		static real cot_(real value);


	public:
		static const real s_epsilon;

		static const real s_zero_tolerance;

		static const real s_roll_zero;

		static const real s_max_real;

		static const real s_min_real;

		static const real s_pi;

		static const real s_two_pi;

		static const real s_half_pi;

		static const real s_deg_to_rad;

		static const real s_rad_to_deg;
	};



#include "us_math.inl"

	typedef math<float> math_f;

	typedef math<double> math_d;

#ifdef WIN32
	//#include <usUtil/us_common_file.h>
	template<> const float math<float>::s_epsilon = FLT_EPSILON;
	template<> const float math<float>::s_zero_tolerance = FLT_EPSILON;
	template<> const float math<float>::s_roll_zero = FLT_EPSILON;

	template<> const float math<float>::s_min_real = FLT_MIN;
	template<> const float math<float>::s_max_real = FLT_MAX;
	template<> const float math<float>::s_pi = (float)PI;
	template<> const float math<float>::s_two_pi = (float)(PI * 2.0);
	template<> const float math<float>::s_half_pi = (float)(PI * 0.5);
	template<> const float math<float>::s_deg_to_rad = (float)(PI / 180.0);
	template<> const float math<float>::s_rad_to_deg = (float)(180.0 / PI);


	template<> const double math<double>::s_epsilon = DBL_EPSILON;
	template<> const double math<double>::s_zero_tolerance = DBL_EPSILON;
	template<> const double math<double>::s_roll_zero = 1.0e-012;

	template<> const double math<double>::s_min_real = DBL_MIN;
	template<> const double math<double>::s_max_real = DBL_MAX;
	template<> const double math<double>::s_pi = PI;
	template<> const double math<double>::s_two_pi = PI * 2.0;
	template<> const double math<double>::s_half_pi = PI * 0.5;
	template<> const double math<double>::s_deg_to_rad = PI / 180.0;
	template<> const double math<double>::s_rad_to_deg = 180.0 / PI;
#endif //WIN32
}

#endif //_US_MATH_H_