//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_vector3.h
//  Author  : Uniscope Team
//  Modifier: Uniscope Team
//  Created :
//  Purpose : vector3 template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_VECTOR3_H_
#define _US_VECTOR3_H_

#include <vector>

namespace uniscope_globe
{
	template<class real>
	class vector3
	{
	public:
		real x, y, z;

		// special vectors
		static const vector3 s_zero;
		static const vector3 s_unit_x;
		static const vector3 s_unit_y;
		static const vector3 s_unit_z;

	public:
		vector3(void);

		~vector3(void);

	public:
		//vector3( const vector3& rhs )
		//	{
		//	x = rhs.x;
		//	y = rhs.y;
		//	z = rhs.z;
		//	}
		vector3(real v_x, real v_y, real v_z);

		vector3(float* p);

		vector3(double* p);

		vector3(const vector3& t);

	public:

		// Assigns the value of the other vector.or param
		vector3& operator = (const vector3& vec);

		bool operator == (const vector3& vec) const;

		bool operator != (const vector3& vec) const;

		vector3 operator + (const vector3& vec) const;

		vector3 operator - (const vector3& vec) const;

		vector3 operator - () const;

		vector3 operator * (real scale) const;

		vector3 operator * (vector3& vec) const;

		vector3 operator / (vector3& vec) const;

		vector3 operator / (real scale) const;

		// overloaded operators to help vector2
		friend vector3 operator * (real scalar, const vector3& vec)
		{
			vector3 v_ret_vec;

			v_ret_vec.x = scalar * vec.x;
			v_ret_vec.y = scalar * vec.y;
			v_ret_vec.z = scalar * vec.z;

			return v_ret_vec;
		}

		vector3& operator += (const vector3& vec);

		vector3& operator -= (const vector3& vec);

		vector3& operator *= (const real scale_value);

		vector3& operator *= (const vector3& vec);

		vector3& operator /= (const real scale_value);

		void scale(const real scale_value);

		double length(void);

		double squared_length(void);

		void normalize(void);

		real dot(const vector3& vec) const;

		vector3 cross(const vector3& vec) const;

		static real dot(const vector3& vec1, const vector3& vec2);

		static vector3 cross(const vector3& vec1, const vector3& vec2);

		static vector3 normalize(const vector3& vec);

		static real length(const vector3& vec);

		// Calculates a reflection vector to the plane with the given normal .
		// and remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not
		static vector3 reflect(const vector3& src, const vector3& normal);

	}; // class vector3

#include "us_vector3.inl"

	typedef vector3<float>	vector_3f;
	typedef vector3<double>	vector_3d;

	inline vector_3d to_double(const vector_3f& rhs)
	{
		return vector_3d(rhs.x, rhs.y, rhs.z);
	}

	inline vector_3f to_float(const vector_3d& rhs)
	{
		return vector_3f(rhs.x, rhs.y, rhs.z);
	}
	typedef std::vector<vector_3d> coordinates_array;
	typedef std::vector<coordinates_array> coordinates_part_array;

	typedef std::vector<vector_3f> coordinates_array_f;
	typedef std::vector<vector_3d> coordinates_array_d;

#ifdef WIN32
	template<> const vector3<float> vector3<float>::s_zero(0.0f, 0.0f, 0.0f);
	template<> const vector3<float> vector3<float>::s_unit_x(1.0f, 0.0f, 0.0f);
	template<> const vector3<float> vector3<float>::s_unit_y(0.0f, 1.0f, 0.0f);
	template<> const vector3<float> vector3<float>::s_unit_z(0.0f, 0.0f, 1.0f);

	template<> const vector3<double> vector3<double>::s_zero(0.0, 0.0, 0.0);
	template<> const vector3<double> vector3<double>::s_unit_x(1.0, 0.0, 0.0);
	template<> const vector3<double> vector3<double>::s_unit_y(0.0, 1.0, 0.0);
	template<> const vector3<double> vector3<double>::s_unit_z(0.0, 0.0, 1.0);
#endif //WIN32

} // namespace uniscope_globe

#endif
