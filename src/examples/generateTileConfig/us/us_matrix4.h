//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_matrix4.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : matrix4x4 template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_MATRIX4_H_
#define _US_MATRIX4_H_

#include "us_vector4.h"
#include "us_vector3.h"
#include "us_quaternion.h"

#include <assert.h>

namespace uniscope_globe
{

	template <class real>
	class matrix4
	{
	public:
		static const matrix4 s_identity;
		static const matrix4 s_zero;

	public:
		union
		{
			struct
			{
				real        m11, m12, m13, m14;
				real        m21, m22, m23, m24;
				real        m31, m32, m33, m34;
				real        m41, m42, m43, m44;
			};
			real m[16];
		};

	public:
		matrix4(void) {}
		~matrix4(void) {}

	public:
		// constructor
		matrix4(const float* p);
		matrix4(const double* p);

		// copy constructor
		matrix4(const matrix4& mat);

		// input mrc is in row r, column c.
		matrix4(real _11, real _12, real _13, real _14, real _21, real _22, real _23, real _24,
			real _31, real _32, real _33, real _34, real _41, real _42, real _43, real _44);

	public:
		void zero(void);
		bool is_identity(void);
		void identity(void);

		// assignment
		matrix4& operator = (const matrix4& mat);

		// comparison
		bool operator == (const matrix4& mat) const;
		bool operator != (const matrix4& mat) const;

		// arithmetic operations
		matrix4 operator + (const matrix4& mat) const;
		matrix4 operator - (const matrix4& mat) const;
		matrix4 operator * (const matrix4& mat) const;
		matrix4 operator * (real scale) const;
		matrix4 operator / (real scale) const;
		matrix4 operator - () const;

		friend matrix4<real> operator * (const real scalar, const matrix4& mat)
		{
			matrix4 ret_mat = mat * scalar;
			return ret_mat;
		}

		// arithmetic updates
		matrix4& operator += (const matrix4& mat);
		matrix4& operator -= (const matrix4& mat);
		matrix4& operator *= (const matrix4& mat);
		matrix4& operator *= (real scale);
		matrix4& operator /= (real scale);

		// mat4 * vector4
		vector4<real> operator * (const vector4<real>& vec) const;
		friend vector4<real> operator * (const vector4<real>& vec, const matrix4<real>& mat)
		{
			return vector4<real>(
				vec.x * mat.m[0] + vec.y * mat.m[4] + vec.z * mat.m[8] + vec.w * mat.m[12],
				vec.x * mat.m[1] + vec.y * mat.m[5] + vec.z * mat.m[9] + vec.w * mat.m[13],
				vec.x * mat.m[2] + vec.y * mat.m[6] + vec.z * mat.m[10] + vec.w * mat.m[14],
				vec.x * mat.m[3] + vec.y * mat.m[7] + vec.z * mat.m[11] + vec.w * mat.m[15]);
		}

		// mat4 * vector3 -----> tanslate coordinate
		vector3<real> operator * (const vector3<real>& vec) const;
		friend vector3<real> operator * (const vector3<real>& vec, const matrix4<real>& mat)
		{
			return vector3<real>(
				vec.x * mat.m[0] + vec.y * mat.m[4] + vec.z * mat.m[8] + mat.m[12],
				vec.x * mat.m[1] + vec.y * mat.m[5] + vec.z * mat.m[9] + mat.m[13],
				vec.x * mat.m[2] + vec.y * mat.m[6] + vec.z * mat.m[10] + mat.m[14]);
		}

		// mat4 * vector3 -----> tansform noraml
		static vector3<real> transform_normal(const vector3<real>& vec, const matrix4<real>& mat);

		// mat4 * vector3 -----> tansform coordinate
		static vector3<real> transform_coordinate(const vector3<real>& vec, const matrix4<real>& mat);

		// mat4 * vector3 -----> rotate noraml
		static vector3<real> rotate_normal(const vector3<real>& vec, const matrix4<real>& mat);

		// mat4 * vector3 -----> rotate noraml
		//static vector3<real> rotate_normal( const matrix4<real>& mat, const vector3<real>& vec );

		// other operations
		matrix4 transpose() const;
		matrix4 inverse() const;
		matrix4 adjoint() const;
		real determinant() const;

		// construct
		static matrix4 from_tnbp(const vector3<real>& vec_t, const vector3<real>& vec_n, const vector3<real>& vec_b, const vector3<real>& vec_p);

		static matrix4 from_quaternion(const quaternion<real>& q);

		// projection matrices onto a specified plane, oblique
		static matrix4 oblique_projection(const vector3<real>& normal, const vector3<real>& point, const vector3<real>& dir);

		// projection matrices onto a specified plane, perspective
		static matrix4 perspective_projection(const vector3<real>& normal, const vector3<real>& point, const vector3<real>& eye);

		// view matrix
		static matrix4 look_at(const vector3<real>& position, const vector3<real>& target, const vector3<real>& up);

		// projection matrix
		static matrix4 perspective(real fov, real aspect, real near_plane, real far_plane);

		// orthographic matrix
		static matrix4 orthographic(real width, real height, real near_plane, real far_plane);

		// reflection matrix through a specified plane
		static matrix4 reflection(const vector3<real>& normal, const vector3<real>& point);

		// transform
		//  rotate by axis
		static matrix4 rotate_axis_rh(const vector3<real>& from, const vector3<real>& to, real radian_value);

		//  rotate by axis
		static matrix4 rotate_axis_lh(const vector3<real>& from, const vector3<real>& to, real radian_value);

		// rotate by x
		static matrix4 rotate_x(real radian_value);

		// rotate by y
		static matrix4 rotate_y(real radian_value);

		// rotate by z
		static matrix4 rotate_z(real radian_value);

		// tranlate to x ,y ,z
		static matrix4 translation(real x, real y, real z);

		// scale
		static matrix4 scaling(real x, real y, real z);

		// inverse
		static matrix4 inverse(const matrix4<real>& mat);

		// clip

		//dvClipWidth = 2
		//	dvClipHeight = 2
		//	dvClipX = -1
		//	dvClipY = 1
		//	dvMinZ = 0
		//	dvMaxZ = 1
		static matrix4 clip(real clip_x, real clip_y, real clip_width, real clip_height, real min_z, real max_z);

		static matrix4 viewport(real x, real y, real width, real height, real min_z, real max_z);

	private:
		// for indexing into the 1D array of the matrix, iCol+N*iRow
		static int I(int row, int col);

		// support for comparisons
		int compare_arrays(const matrix4& mat) const;

	};

#ifdef WIN32
	template<> const matrix4<float> matrix4<float>::s_zero(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f);

	template<> const matrix4<float> matrix4<float>::s_identity(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);


	template<> const matrix4<double> matrix4<double>::s_zero(
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0);

	template<> const matrix4<double> matrix4<double>::s_identity(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);

#endif //WIN32

#include "us_matrix4.inl"

	typedef matrix4<float> matrix_4f;
	typedef matrix4<double> matrix_4d;


	void /*LIB_UTIL_API*/ multiply_matrix_4f_sse2(matrix_4f* pOut, const matrix_4f* pIn1, const matrix_4f* pIn2);
	void /*LIB_UTIL_API*/ multiply_matrix_4d_sse2(matrix_4d* out_mat, const matrix_4d* mat1, const matrix_4d* mat2);
	void /*LIB_UTIL_API*/ multiply_matrix_4f_c(matrix_4f* pOut, const matrix_4f* pIn1, const matrix_4f* pIn2);
	void /*LIB_UTIL_API*/ multiply_matrix_4d_c(matrix_4d* out_mat, const matrix_4d* mat1, const matrix_4d* mat2);
	void /*LIB_UTIL_API*/ multiply_vector3_matrix_4f_c(vector_3f* pOut, const vector_3f* pVec, const matrix_4f* pMat);
	void /*LIB_UTIL_API*/ multiply_vector3_matrix_4d_c(vector_3d* pOut, const vector_3d* pVec, const matrix_4d* pMat);


} // namespace uniscope_globe

#endif // #define _US_VECTOR2_H_
