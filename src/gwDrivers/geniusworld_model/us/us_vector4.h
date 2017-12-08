//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_vector4.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : vector4 template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_VECTOR4_H_
#define _US_VECTOR4_H_

//#include <vector>

namespace uniscope_globe
{
	template <class real>
	class vector4
	{
	public:
		union
		{
			struct
			{
				real        x;
				real        y;
				real        z;
				real		 w;
			};

			real m_tuple[4];
		};

	public:
		// construction
		vector4();  // uninitialized
		vector4(real x, real y, real z, real w);
		vector4(const vector4& v);


		//
		// assignment
		//
		vector4& operator= (const vector4& v);

		//
		// comparison
		//
		bool operator== (const vector4& v) const;

		bool operator!= (const vector4& v) const;


		//
		// arithmetic operations
		//
		vector4 operator+ (const vector4& v) const;

		vector4 operator- (const vector4& v) const;

		vector4 operator* (real scalar) const;

		friend vector4 operator* (real scalar, const vector4& v);

		vector4 operator/ (real scalar) const;

		vector4 operator- () const;


		//
		// arithmetic updates
		//
		vector4& operator+= (const vector4& v);

		vector4& operator-= (const vector4& v);

		vector4& operator*= (real scalar);

		vector4& operator/= (real scalar);

		//
		// vector operations
		//
		real length() const;

		real squared_length() const;

		real dot(const vector4& v) const;

		real normalize();

		// special vectors
		static const vector4 s_zero;

		static const vector4 s_unit_x;

		static const vector4 s_unit_y;

		static const vector4 s_unit_z;

		static const vector4 s_unit_w;
	};

#include "us_vector4.inl"

	typedef vector4<float> Vector4f;

	typedef vector4<double> Vector4d;

#ifdef WIN32
	template<> const vector4<float> vector4<float>::s_zero(0.0f, 0.0f, 0.0f, 0.0f);
	template<> const vector4<float> vector4<float>::s_unit_x(1.0f, 0.0f, 0.0f, 0.0f);
	template<> const vector4<float> vector4<float>::s_unit_y(0.0f, 1.0f, 0.0f, 0.0f);
	template<> const vector4<float> vector4<float>::s_unit_z(0.0f, 0.0f, 1.0f, 0.0f);
	template<> const vector4<float> vector4<float>::s_unit_w(0.0f, 0.0f, 0.0f, 1.0f);

	template<> const vector4<double> vector4<double>::s_zero(0.0, 0.0, 0.0, 0.0);
	template<> const vector4<double> vector4<double>::s_unit_x(1.0, 0.0, 0.0, 0.0);
	template<> const vector4<double> vector4<double>::s_unit_y(0.0, 1.0, 0.0, 0.0);
	template<> const vector4<double> vector4<double>::s_unit_z(0.0, 0.0, 1.0, 0.0);
	template<> const vector4<double> vector4<double>::s_unit_w(0.0, 0.0, 0.0, 1.0);
#endif //WIN32

}

#endif //_US_VECTOR4_H_