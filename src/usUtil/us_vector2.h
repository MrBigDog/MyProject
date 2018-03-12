//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_vector2.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : matrix4x4 template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_VECTOR2_H_
#define _US_VECTOR2_H_

namespace uniscope_globe
{
	template<class real>
	class vector2
	{
	public:
		real x, y;

		// special vectors
		static const vector2 s_zero;
		static const vector2 s_unit_x;
		static const vector2 s_unit_y;

	public:
		vector2(void);

		~vector2(void);

	public:
		vector2( real v_x, real v_y );

		vector2( const vector2& t );

	public:

		// Assigns the value of the other vector.or param
		vector2& operator = ( const vector2& vec );

		bool operator == ( const vector2& vec ) const;

		bool operator != ( const vector2& vec ) const;

		vector2 operator + ( const vector2& vec ) const;

		vector2 operator - ( const vector2& vec ) const;

		vector2 operator - () const;

		vector2 operator * ( const real scale_value ) const;

		vector2 operator * ( const vector2& vec ) const;

		vector2 operator / ( const real scale_value ) const;

		// overloaded operators to help vector2
		friend vector2 operator * ( real scale_value, const vector2& vec )
		{
			vector2 v_ret_vec;

			v_ret_vec.x = scale_value * vec.x;
			v_ret_vec.y = scale_value * vec.y;

			return v_ret_vec;
		}

		vector2& operator += ( const vector2& vec );

		vector2& operator -= ( const vector2& vec );

		vector2& operator *= ( const real scale_value );

		vector2& operator *= ( const vector2& vec );

		vector2& operator /= ( const real scale_value );

		void scale( const real scale_value );

		double length(void);

		double squared_length(void);

		void normalize(void);

		static double dot( const vector2& vec1, const vector2& vec2 );

		static double cross( const vector2& vec1, const vector2& vec2 );

		static vector2 normalize( const vector2& vec );

		// Calculates a reflection vector to the plane with the given normal .
		// and remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not
		static vector2 reflect( const vector2& src, const vector2& normal );

	}; // class vector2


	#include "us_vector2.inl"

	typedef vector2<int>	vector_2i;
	typedef vector2<float>	vector_2f;
	typedef vector2<double>	vector_2d;

#ifdef WIN32
	template<> const vector2<int> vector2<int>::s_zero(0, 0);
	template<> const vector2<int> vector2<int>::s_unit_x(1, 0);
	template<> const vector2<int> vector2<int>::s_unit_y(0, 1);

	template<> const vector2<float> vector2<float>::s_zero(0.0f, 0.0f);
	template<> const vector2<float> vector2<float>::s_unit_x(1.0f, 0.0f);
	template<> const vector2<float> vector2<float>::s_unit_y(0.0f, 1.0f);

	template<> const vector2<double> vector2<double>::s_zero(0.0, 0.0);
	template<> const vector2<double> vector2<double>::s_unit_x(1.0, 0.0);
	template<> const vector2<double> vector2<double>::s_unit_y(0.0, 1.0);
#endif //WIN32

} // namespace uniscope_globe

#endif // #define _US_VECTOR2_H_
