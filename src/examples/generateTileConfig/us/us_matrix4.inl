//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_matrix4.inl
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : matrix4x4 template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////

//
// constuctors
//
//
template <class real>
matrix4<real>::matrix4 ( const matrix4& mat )
{
	memcpy( (void*)m, (void*)(mat.m), sizeof(real)<< 4 );
}

template <class real>
matrix4<real>::matrix4 ( real _11, real _12, real _13, real _14, real _21, real _22, real _23, real _24, 
						 real _31, real _32, real _33, real _34, real _41, real _42, real _43, real _44 )
{
	m11 = _11,  m12 = _12,   m13 = _13,  m14 = _14;
	m21 = _21,  m22 = _22,   m23 = _23,  m24 = _24;
	m31 = _31,  m32 = _32,   m33 = _33,  m34 = _34;
	m41 = _41,  m42 = _42,   m43 = _43,  m44 = _44;
}

template <class real>
matrix4<real>::matrix4( const float* p )
{
	for ( int i = 0; i < 16; i++ )
	{
		m[i] = (real)p[i];
	}
}

template <class real>
matrix4<real>::matrix4( const double* p )
{
	for ( int i = 0; i < 16; i++ )
	{
		m[i] = (real)p[i];
	}
}

//----------------------------------------------------------------------------


template <class real>
int matrix4<real>::I ( int row, int col )
{
	assert( 0 <= row && row < 4 && 0 <= col && col < 4 );
	return col + row * 4;
}

template <class real>
int matrix4<real>::compare_arrays ( const matrix4& mat ) const
{
	return memcmp( m, mat.m, sizeof(real) << 4 );
}
//----------------------------------------------------------------------------


template <class real>
void matrix4<real>::zero()
{
	memset( m, 0, sizeof(real) << 4 );
}

template <class real>
bool matrix4<real>::is_identity(void)
{
	return matrix4<real>::s_identity == *this;
}

template <class real>
void matrix4<real>::identity ()
{
	memset( m, 0, sizeof(real)<<4 );
	m[0]  = (real)1.0;
	m[5]  = (real)1.0;
	m[10] = (real)1.0;
	m[15] = (real)1.0;
}
//----------------------------------------------------------------------------


template <class real>
matrix4<real>& matrix4<real>::operator = ( const matrix4& mat )
{
	memcpy( m, mat.m, sizeof(real) * 16 );
	return *this;
}

template <class real>
bool matrix4<real>::operator == (const matrix4& mat) const
{
	return compare_arrays(mat) == 0;
}

template <class real>
bool matrix4<real>::operator != (const matrix4& mat) const
{
	return compare_arrays(mat) != 0;
}
//----------------------------------------------------------------------------


template <class real>
matrix4<real> matrix4<real>::operator + (const matrix4& mat) const
{
	matrix4 mat_ret;
	for (int i = 0; i < 16; i++)
	{
		mat_ret.m[i] = m[i] + mat.m[i];
	}
	return mat_ret;
}

template <class real>
matrix4<real> matrix4<real>::operator - (const matrix4& mat) const
{
	matrix4 mat_ret;
	for (int i = 0; i < 16; i++)
	{
		mat_ret.m[i] = m[i] - mat.m[i];
	}
	return mat_ret;
}

template <class real>
matrix4<real> matrix4<real>::operator * (const matrix4& mat) const
{
	matrix4 mat_ret;
	for ( int row = 0; row < 4; row++ )
	{
		for ( int col = 0; col < 4; col++ )
		{
			int i = I( row, col );
			mat_ret.m[i] = (real)0.0;
			for ( int mid = 0; mid < 4; mid++ )
			{
				mat_ret.m[i] += m[I(row,mid)] * mat.m[I(mid,col)];
			}
		}
	}
	return mat_ret;
}

template <class real>
matrix4<real> matrix4<real>::operator * ( real scale ) const
{
	matrix4 mat_ret;
	for (int i = 0; i < 16; i++)
	{
		mat_ret.m[i] = scale * m[i];
	}
	return mat_ret;
}

template <class real>
matrix4<real> matrix4<real>::operator / ( real scale ) const
{
	assert( scale == real(0.0) );
	matrix4<real> mat_ret;

	real inv_scale = ((real)1.0) / scale;
	for ( int i = 0; i < 16; i++ )
	{
		mat_ret.m[i] = inv_scale * m[i];
	}

	return mat_ret;
}

template <class real>
matrix4<real> matrix4<real>::operator - () const
{
	matrix4 mat_ret;
	for ( int i = 0; i < 16; i++ )
	{
		mat_ret.m[i] = -m[i];
	}
	return mat_ret;
}
//----------------------------------------------------------------------------



template <class real>
matrix4<real>& matrix4<real>::operator += ( const matrix4& mat )
{
	for (int i = 0; i < 16; i++)
	{
		m[i] += mat.m[i];
	}
	return *this;
}

template <class real>
matrix4<real>& matrix4<real>::operator -= ( const matrix4& mat )
{
	for (int i = 0; i < 16; i++)
	{
		m[i] -= mat.m[i];
	}
	return *this;
}

template <class real>
matrix4<real>& matrix4<real>::operator *= ( const matrix4& mat )
{
	*this = *this * mat;
	return *this;
}

template <class real>
matrix4<real>& matrix4<real>::operator*= ( real scale )
{
	for (int i = 0; i < 16; i++)
	{
		m[i] *= scale;
	}
	return *this;
}

template <class real>
matrix4<real>& matrix4<real>::operator /= ( real scale )
{
	assert( scale == real(0.0) );

	real inv_scalar = ((real)1.0) / scale;
	for (int i = 0; i < 16; i++ )
	{
		m[i] *= inv_scalar;
	}

	return *this;
}
//----------------------------------------------------------------------------


// vec4_mat
template <class real>
vector4<real> matrix4<real>::operator * ( const vector4<real>& vec ) const
{
	return vector4<real>(
		vec.x * m[0] + vec.y * m[4] + vec.z * m[8] + vec.w * m[12],
		vec.x * m[1] + vec.y * m[5] + vec.z * m[9] + vec.w * m[13],
		vec.x * m[2] + vec.y * m[6] + vec.z * m[10] + vec.w * m[14],
		vec.x * m[3] + vec.y * m[7] + vec.z * m[11] + vec.w * m[15] );
}
//----------------------------------------------------------------------------


// vec3_mat
template <class real>
vector3<real> matrix4<real>::operator * ( const vector3<real>& vec ) const
{
	vector3<real> vec_ret;
	
	vec_ret.x = m11 * vec.x + m21 * vec.y + m31 * vec.z + m41;
	vec_ret.y = m12 * vec.x + m22 * vec.y + m32 * vec.z + m42;
	vec_ret.z = m13 * vec.x + m23 * vec.y + m33 * vec.z + m43;

	return vec_ret;
}

template <class real>
vector3<real> matrix4<real>::transform_coordinate( const vector3<real>& vec, const matrix4<real>& mat )
{
	vector3<real> vec_ret;

	vec_ret.x = mat.m11 * vec.x + mat.m21 * vec.y + mat.m31 * vec.z + mat.m41;
	vec_ret.y = mat.m12 * vec.x + mat.m22 * vec.y + mat.m32 * vec.z + mat.m42;
	vec_ret.z = mat.m13 * vec.x + mat.m23 * vec.y + mat.m33 * vec.z + mat.m43;

	real det = mat.m14 * vec.x + mat.m24 * vec.y + mat.m34 * vec.z + mat.m44;
	assert( det != real(0.0) );
	det = real(1.0)/det;

	vec_ret.x *= det;
	vec_ret.y *= det;
	vec_ret.z *= det;	

	return vec_ret;
}

template <class real>
vector3<real> matrix4<real>::transform_normal( const vector3<real>& vec, const matrix4<real>& mat )
{
	vector3<real> vec_ret;

	vec_ret.x = mat.m11 * vec.x + mat.m21 * vec.y + mat.m31 * vec.z;
	vec_ret.y = mat.m12 * vec.x + mat.m22 * vec.y + mat.m32 * vec.z;
	vec_ret.z = mat.m13 * vec.x + mat.m23 * vec.y + mat.m33 * vec.z;

	return vec_ret;
}

//----------------------------------------------------------------------------
//template <class real>
//vector3<real> matrix4<real>::rotate_normal( const vector3<real>& vec, const matrix4<real>& mat )
//{
//	vector3<real> vec_ret;
//
//	vec_ret.x = mat.m11 * vec.x + mat.m12 * vec.y + mat.m13 * vec.z;
//	vec_ret.y = mat.m21 * vec.x + mat.m22 * vec.y + mat.m23 * vec.z;
//	vec_ret.z = mat.m31 * vec.x + mat.m32 * vec.y + mat.m33 * vec.z;	
//
//	return vec_ret;
//}

template <class real>
vector3<real> matrix4<real>::rotate_normal( const vector3<real>& vec, const matrix4<real>& mat )
//vector3<real> matrix4<real>::rotate_normal( const matrix4<real>& mat, const vector3<real>& vec )
{
	vector3<real> vec_ret;

	vec_ret.x = mat.m11 * vec.x + mat.m21 * vec.y + mat.m31 * vec.z;
	vec_ret.y = mat.m12 * vec.x + mat.m22 * vec.y + mat.m32 * vec.z;
	vec_ret.z = mat.m13 * vec.x + mat.m23 * vec.y + mat.m33 * vec.z;

	return vec_ret;
}


template <class real>
matrix4<real> matrix4<real>::transpose () const
{
	matrix4 trans_mat;
	for ( int row = 0; row < 4; row++ )
	{
		for ( int col = 0; col < 4; col++ )
		{
			trans_mat.m[I(row,col)] = m[I(col,row)];
		}
	}
	return trans_mat;
}
//----------------------------------------------------------------------------
// inverse
template <class real>
matrix4<real> matrix4<real>::inverse( const matrix4<real>& mat )
{
	real a0 = mat.m[0] * mat.m[5] - mat.m[1] * mat.m[4];
	real a1 = mat.m[0] * mat.m[6] - mat.m[2] * mat.m[4];
	real a2 = mat.m[0] * mat.m[7] - mat.m[3] * mat.m[4];
	real a3 = mat.m[1] * mat.m[6] - mat.m[2] * mat.m[5];
	real a4 = mat.m[1] * mat.m[7] - mat.m[3] * mat.m[5];
	real a5 = mat.m[2] * mat.m[7] - mat.m[3] * mat.m[6];

	real b0 = mat.m[8]  * mat.m[13] - mat.m[9]  * mat.m[12];
	real b1 = mat.m[8]  * mat.m[14] - mat.m[10] * mat.m[12];
	real b2 = mat.m[8]  * mat.m[15] - mat.m[11] * mat.m[12];
	real b3 = mat.m[9]  * mat.m[14] - mat.m[10] * mat.m[13];
	real b4 = mat.m[9]  * mat.m[15] - mat.m[11] * mat.m[13];
	real b5 = mat.m[10] * mat.m[15] - mat.m[11] * mat.m[14];

	real det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
	if ( math<real>::fabs_(det) <= math<real>::s_zero_tolerance )
		return matrix4<real>::s_zero;

	matrix4 mat_ret;
	mat_ret.m[0]  = + mat.m[5]  * b5 - mat.m[6]  * b4 + mat.m[7]  * b3;
	mat_ret.m[4]  = - mat.m[4]  * b5 + mat.m[6]  * b2 - mat.m[7]  * b1;
	mat_ret.m[8]  = + mat.m[4]  * b4 - mat.m[5]  * b2 + mat.m[7]  * b0;
	mat_ret.m[12] = - mat.m[4]  * b3 + mat.m[5]  * b1 - mat.m[6]  * b0;
	mat_ret.m[1]  = - mat.m[1]  * b5 + mat.m[2]  * b4 - mat.m[3]  * b3;
	mat_ret.m[5]  = + mat.m[0]  * b5 - mat.m[2]  * b2 + mat.m[3]  * b1;
	mat_ret.m[9]  = - mat.m[0]  * b4 + mat.m[1]  * b2 - mat.m[3]  * b0;
	mat_ret.m[13] = + mat.m[0]  * b3 - mat.m[1]  * b1 + mat.m[2]  * b0;
	mat_ret.m[2]  = + mat.m[13] * a5 - mat.m[14] * a4 + mat.m[15] * a3;
	mat_ret.m[6]  = - mat.m[12] * a5 + mat.m[14] * a2 - mat.m[15] * a1;
	mat_ret.m[10] = + mat.m[12] * a4 - mat.m[13] * a2 + mat.m[15] * a0;
	mat_ret.m[14] = - mat.m[12] * a3 + mat.m[13] * a1 - mat.m[14] * a0;
	mat_ret.m[3]  = - mat.m[9]  * a5 + mat.m[10] * a4 - mat.m[11] * a3;
	mat_ret.m[7]  = + mat.m[8]  * a5 - mat.m[10] * a2 + mat.m[11] * a1;
	mat_ret.m[11] = - mat.m[8]  * a4 + mat.m[9]  * a2 - mat.m[11] * a0;
	mat_ret.m[15] = + mat.m[8]  * a3 - mat.m[9]  * a1 + mat.m[10] * a0;

	real inv_det = ((real)1.0) / det;
	for ( int row = 0; row < 4; row++ )
	{
		for ( int col = 0; col < 4; col++ )
		{
			mat_ret.m[(row<<2)+col] *= inv_det;
		}
	}

	return mat_ret;
}


template <class real>
matrix4<real> matrix4<real>::inverse () const
{
	real a0 = m[0] * m[5] - m[1] * m[4];
	real a1 = m[0] * m[6] - m[2] * m[4];
	real a2 = m[0] * m[7] - m[3] * m[4];
	real a3 = m[1] * m[6] - m[2] * m[5];
	real a4 = m[1] * m[7] - m[3] * m[5];
	real a5 = m[2] * m[7] - m[3] * m[6];
 
	real b0 = m[8]  * m[13] - m[9]  * m[12];
	real b1 = m[8]  * m[14] - m[10] * m[12];
	real b2 = m[8]  * m[15] - m[11] * m[12];
	real b3 = m[9]  * m[14] - m[10] * m[13];
	real b4 = m[9]  * m[15] - m[11] * m[13];
	real b5 = m[10] * m[15] - m[11] * m[14];

	real det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;
	if ( math<real>::fabs_(det) <= math<real>::s_zero_tolerance )
		return matrix4<real>::s_zero;

	matrix4 mat_ret;
	mat_ret.m[0]  = + m[5]  * b5 - m[6]  * b4 + m[7]  * b3;
	mat_ret.m[4]  = - m[4]  * b5 + m[6]  * b2 - m[7]  * b1;
	mat_ret.m[8]  = + m[4]  * b4 - m[5]  * b2 + m[7]  * b0;
	mat_ret.m[12] = - m[4]  * b3 + m[5]  * b1 - m[6]  * b0;
	mat_ret.m[1]  = - m[1]  * b5 + m[2]  * b4 - m[3]  * b3;
	mat_ret.m[5]  = + m[0]  * b5 - m[2]  * b2 + m[3]  * b1;
	mat_ret.m[9]  = - m[0]  * b4 + m[1]  * b2 - m[3]  * b0;
	mat_ret.m[13] = + m[0]  * b3 - m[1]  * b1 + m[2]  * b0;
	mat_ret.m[2]  = + m[13] * a5 - m[14] * a4 + m[15] * a3;
	mat_ret.m[6]  = - m[12] * a5 + m[14] * a2 - m[15] * a1;
	mat_ret.m[10] = + m[12] * a4 - m[13] * a2 + m[15] * a0;
	mat_ret.m[14] = - m[12] * a3 + m[13] * a1 - m[14] * a0;
	mat_ret.m[3]  = - m[9]  * a5 + m[10] * a4 - m[11] * a3;
	mat_ret.m[7]  = + m[8]  * a5 - m[10] * a2 + m[11] * a1;
	mat_ret.m[11] = - m[8]  * a4 + m[9]  * a2 - m[11] * a0;
	mat_ret.m[15] = + m[8]  * a3 - m[9]  * a1 + m[10] * a0;

	real inv_det = ((real)1.0) / det;
	for ( int row = 0; row < 4; row++ )
	{
		for ( int col = 0; col < 4; col++ )
		{
			mat_ret.m[(row<<2)+col] *= inv_det;
		}
	}

	return mat_ret;
}
//----------------------------------------------------------------------------

template <class real>
matrix4<real> matrix4<real>::adjoint () const
{
	real a0 = m[0] * m[5] - m[1] * m[4];
	real a1 = m[0] * m[6] - m[2] * m[4];
	real a2 = m[0] * m[7] - m[3] * m[4];
	real a3 = m[1] * m[6] - m[2] * m[5];
	real a4 = m[1] * m[7] - m[3] * m[5];
	real a5 = m[2] * m[7] - m[3] * m[6];

	real b0 = m[8]  * m[13] - m[9]  * m[12];
	real b1 = m[8]  * m[14] - m[10] * m[12];
	real b2 = m[8]  * m[15] - m[11] * m[12];
	real b3 = m[9]  * m[14] - m[10] * m[13];
	real b4 = m[9]  * m[15] - m[11] * m[13];
	real b5 = m[10] * m[15] - m[11] * m[14];

	matrix4 mat_ret;
	mat_ret.m[0]  = + m[5]  * b5 - m[6]  * b4 + m[7]  * b3;
	mat_ret.m[4]  = - m[4]  * b5 + m[6]  * b2 - m[7]  * b1;
	mat_ret.m[8]  = + m[4]  * b4 - m[5]  * b2 + m[7]  * b0;
	mat_ret.m[12] = - m[4]  * b3 + m[5]  * b1 - m[6]  * b0;
	mat_ret.m[1]  = - m[1]  * b5 + m[2]  * b4 - m[3]  * b3;
	mat_ret.m[5]  = + m[0]  * b5 - m[2]  * b2 + m[3]  * b1;
	mat_ret.m[9]  = - m[0]  * b4 + m[1]  * b2 - m[3]  * b0;
	mat_ret.m[13] = + m[0]  * b3 - m[1]  * b1 + m[2]  * b0;
	mat_ret.m[2]  = + m[13] * a5 - m[14] * a4 + m[15] * a3;
	mat_ret.m[6]  = - m[12] * a5 + m[14] * a2 - m[15] * a1;
	mat_ret.m[10] = + m[12] * a4 - m[13] * a2 + m[15] * a0;
	mat_ret.m[14] = - m[12] * a3 + m[13] * a1 - m[14] * a0;
	mat_ret.m[3]  = - m[9]  * a5 + m[10] * a4 - m[11] * a3;
	mat_ret.m[7]  = + m[8]  * a5 - m[10] * a2 + m[11] * a1;
	mat_ret.m[11] = - m[8]  * a4 + m[9]  * a2 - m[11] * a0;
	mat_ret.m[15] = + m[8]  * a3 - m[9]  * a1 + m[10] * a0;

	return mat_ret;
}
//----------------------------------------------------------------------------

template <class real>
real matrix4<real>::determinant () const
{
	real a0 = m[0] * m[5] - m[1] * m[4];
	real a1 = m[0] * m[6] - m[2] * m[4];
	real a2 = m[0] * m[7] - m[3] * m[4];
	real a3 = m[1] * m[6] - m[2] * m[5];
	real a4 = m[1] * m[7] - m[3] * m[5];
	real a5 = m[2] * m[7] - m[3] * m[6];

	real b0 = m[8]  * m[13] - m[9]  * m[12];
	real b1 = m[8]  * m[14] - m[10] * m[12];
	real b2 = m[8]  * m[15] - m[11] * m[12];
	real b3 = m[9]  * m[14] - m[10] * m[13];
	real b4 = m[9]  * m[15] - m[11] * m[13];
	real b5 = m[10] * m[15] - m[11] * m[14];

	real det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

	return det;
}
//----------------------------------------------------------------------------

template <class real>
matrix4<real> matrix4<real>::from_tnbp( const vector3<real>& vec_t, const vector3<real>& vec_n, const vector3<real>& vec_b, const vector3<real>& vec_p )
{
	matrix4<real> ret_mat;
	ret_mat.m11 = vec_t.x,  ret_mat.m12 = vec_t.y,   ret_mat.m13 = vec_t.z,  ret_mat.m14 = real(0);
	ret_mat.m21 = vec_n.x,  ret_mat.m22 = vec_n.y,   ret_mat.m23 = vec_n.z,  ret_mat.m24 = real(0);
	ret_mat.m31 = vec_b.x,  ret_mat.m32 = vec_b.y,   ret_mat.m33 = vec_b.z,  ret_mat.m34 = real(0);
	ret_mat.m41 = vec_p.x,  ret_mat.m42 = vec_p.y,   ret_mat.m43 = vec_p.z,  ret_mat.m44 = real(1);
	return ret_mat;
}

template <class real>
matrix4<real> matrix4<real>::from_quaternion( const quaternion<real>& quat_ref )
{
	matrix4<real> ret_mat = matrix4<real>::s_identity;

	real xx   = quat_ref.x * quat_ref.x;
	real xy   = quat_ref.x * quat_ref.y;
	real xz   = quat_ref.x * quat_ref.z;
	real xw   = quat_ref.x * quat_ref.w;

	real yy	  = quat_ref.y * quat_ref.y;
	real yz	  = quat_ref.y * quat_ref.z;
	real yw	  = quat_ref.y * quat_ref.w;

	real zz	  = quat_ref.z * quat_ref.z;
	real zw	  = quat_ref.z * quat_ref.w;

	ret_mat.m11  = 1 - 2 * ( yy + zz );
	ret_mat.m12  =     2 * ( xy + zw );
	ret_mat.m13  =     2 * ( xz - yw );

	ret_mat.m21  =     2 * ( xy - zw );
	ret_mat.m22  = 1 - 2 * ( xx + zz );
	ret_mat.m23  =     2 * ( yz + xw );

	ret_mat.m31  =     2 * ( xz + yw );
	ret_mat.m32  =     2 * ( yz - xw );		
	ret_mat.m33  = 1 - 2 * ( xx + yy );

	ret_mat.m14  = ret_mat.m24 = ret_mat.m34 = ret_mat.m41 = ret_mat.m42 = ret_mat.m43 = 0;
	ret_mat.m44  = 1;

	return ret_mat;
}


//-----------------------------------------------------------------------------

template <class real>
matrix4<real> matrix4<real>::oblique_projection ( const vector3<real>& normal, const vector3<real>& point, const vector3<real>& dir )
{
	// The projection plane is Dot(N,X-P) = 0 where N is a 3-by-1 unit-length
	// normal vector and P is a 3-by-1 point on the plane.  The projection
	// is oblique to the plane, in the direction of the 3-by-1 vector D.
	// Necessarily Dot(N,D) is not zero for this projection to make sense.
	// Given a 3-by-1 point U, compute the intersection of the line U+t*D
	// with the plane to obtain t = -Dot(N,U-P)/Dot(N,D).  Then
	//
	//   projection(U) = P + [I - D*N^T/Dot(N,D)]*(U-P)
	//
	// A 4-by-4 homogeneous transformation representing the projection is
	//
	//       +-                               -+
	//   M = | D*N^T - Dot(N,D)*I   -Dot(N,P)D |
	//       |          0^T          -Dot(N,D) |
	//       +-                               -+
	//
	// where M applies to [U^T 1]^T by M*[U^T 1]^T.  The matrix is chosen so
	// that M[3][3] > 0 whenever Dot(N,D) < 0 (projection is onto the
	// "positive side" of the plane).

	real normal_dot_dir   = normal.dot(dir);

	real normal_dot_point = normal.dot(point);

	//m[0] = dir.x * normal.x - normal_dot_dir;
	//m[1] = dir.x * normal.y;
	//m[2] = dir.x * normal.z;
	//m[3] = -normal_dot_point * dir.x;

	//m[4] = dir.y * normal.x;
	//m[5] = dir.y * normal.y - normal_dot_dir;
	//m[6] = dir.y * normal.z;
	//m[7] = -normal_dot_point * dir.y;

	//m[8]  = dir.z * normal.x;
	//m[9]  = dir.z * normal.y;
	//m[10] = dir.z * normal.z - normal_dot_dir;
	//m[11] = -normal_dot_point * dir.z;

	//m[12] = 0.0f;
	//m[13] = 0.0f;
	//m[14] = 0.0f;
	//m[15] = -normal_dot_dir;

	//transpose();

	//return *this;

	matrix4<real> mat;

	mat.m[0] = dir.x * normal.x - normal_dot_dir;
	mat.m[1] = dir.x * normal.y;
	mat.m[2] = dir.x * normal.z;
	mat.m[3] = -normal_dot_point * dir.x;

	mat.m[4] = dir.y * normal.x;
	mat.m[5] = dir.y * normal.y - normal_dot_dir;
	mat.m[6] = dir.y * normal.z;
	mat.m[7] = -normal_dot_point * dir.y;

	mat.m[8]  = dir.z * normal.x;
	mat.m[9]  = dir.z * normal.y;
	mat.m[10] = dir.z * normal.z - normal_dot_dir;
	mat.m[11] = -normal_dot_point * dir.z;

	mat.m[12] = 0.0f;
	mat.m[13] = 0.0f;
	mat.m[14] = 0.0f;
	mat.m[15] = -normal_dot_dir;

	mat.transpose();

	return mat;
}
//----------------------------------------------------------------------------

template <class real>
matrix4<real> matrix4<real>::perspective_projection ( const vector3<real>& normal, const vector3<real>& point, const vector3<real>& eye )
{
	//     +-                                                 -+
	// M = | Dot(N,E-P)*I - E*N^T    -(Dot(N,E-P)*I - E*N^T)*E |
	//     |        -N^t                      Dot(N,E)         |
	//     +-                                                 -+
	//
	// where E is the eye point, P is a point on the plane, and N is a
	// unit-length plane normal.

	real normal_dot_eye_sub_point = normal.dot( eye - point );

	//m[0] = normal_dot_eye_sub_point - eye.x * normal.x;
	//m[1] = -eye.x * normal.y;
	//m[2] = -eye.x * normal.z;
	//m[3] = -( m[0] * eye.x + m[1] * eye.y + m[2] * eye.z);

	//m[4] = -eye.y * normal.x;
	//m[5] = normal_dot_eye_sub_point - eye.y * normal.y;
	//m[6] = -eye.y * normal.z;
	//m[7] = -( m[4] * eye.x + m[5] * eye.y + m[6] * eye.z);

	//m[8]  = -eye.z * normal.x;
	//m[9]  = -eye.z * normal.y;
	//m[10] = normal_dot_eye_sub_point- eye.z * normal.z;
	//m[11] = -( m[8] * eye.x + m[9] * eye.y + m[10] * eye.z );

	//m[12] = -normal.x;
	//m[13] = -normal.y;
	//m[14] = -normal.z;
	//m[15] = normal.dot( eye );

	//transpose();

	//return *this;
	matrix4<real> mat;

	mat.m[0] = normal_dot_eye_sub_point - eye.x * normal.x;
	mat.m[1] = -eye.x * normal.y;
	mat.m[2] = -eye.x * normal.z;
	mat.m[3] = -( mat.m[0] * eye.x + mat.m[1] * eye.y + mat.m[2] * eye.z);

	mat.m[4] = -eye.y * normal.x;
	mat.m[5] = normal_dot_eye_sub_point - eye.y * normal.y;
	mat.m[6] = -eye.y * normal.z;
	mat.m[7] = -( mat.m[4] * eye.x + mat.m[5] * eye.y + mat.m[6] * eye.z);

	mat.m[8]  = -eye.z * normal.x;
	mat.m[9]  = -eye.z * normal.y;
	mat.m[10] = normal_dot_eye_sub_point- eye.z * normal.z;
	mat.m[11] = -( mat.m[8] * eye.x + mat.m[9] * eye.y + mat.m[10] * eye.z );

	mat.m[12] = -normal.x;
	mat.m[13] = -normal.y;
	mat.m[14] = -normal.z;
	mat.m[15] = normal.dot( eye );

	mat.transpose();

	return mat;
}
//----------------------------------------------------------------------------

// view matrix
template <class real>
matrix4<real> matrix4<real>::look_at( const vector3<real>& position, const vector3<real>& target, const vector3<real>& up )
{
	matrix4 v_out_matrix;
	vector3<real> xaxis, yaxis, zaxis;

	zaxis = target - position;
	zaxis.normalize();

	xaxis = vector3<real>::cross( up,  zaxis );
	xaxis.normalize();

	yaxis = vector3<real>::cross( zaxis,  xaxis );
	yaxis.normalize();

	v_out_matrix.m11 = xaxis.x, v_out_matrix.m12 = yaxis.x, v_out_matrix.m13 = zaxis.x, v_out_matrix.m14 = 0.0;
	v_out_matrix.m21 = xaxis.y, v_out_matrix.m22 = yaxis.y, v_out_matrix.m23 = zaxis.y, v_out_matrix.m24 = 0.0;
	v_out_matrix.m31 = xaxis.z, v_out_matrix.m32 = yaxis.z, v_out_matrix.m33 = zaxis.z, v_out_matrix.m34 = 0.0;
	v_out_matrix.m41 = - xaxis.dot( position );
	v_out_matrix.m42 = - yaxis.dot( position );
	v_out_matrix.m43 = - zaxis.dot( position );
	v_out_matrix.m44 = 1.0;

	return v_out_matrix;
}

// perspective projection matrix
template <class real>
matrix4<real> matrix4<real>::perspective( real fov, real aspect, real near_plane, real far_plane )
{
	matrix4 v_out_matrix;
	v_out_matrix.zero();
	double fov_value = math<double>::cot_( fov * 0.5 );
	v_out_matrix.m11 = fov_value / aspect;
	v_out_matrix.m22 = fov_value;
	v_out_matrix.m33 = far_plane / ( far_plane - near_plane  );
	v_out_matrix.m34 = 1.0;
	v_out_matrix.m43 = - far_plane * near_plane / ( far_plane - near_plane );

	return v_out_matrix;
}

// orthographic projection matrix
template <class real>
matrix4<real> matrix4<real>::orthographic( real width, real height, real near_plane, real far_plane )
{
	matrix4<real> v_out_matrix = matrix4<real>::s_identity;
	v_out_matrix.m11 = 2 / width;
	v_out_matrix.m22 = 2 / height;
	v_out_matrix.m33 = 1 / ( far_plane - near_plane  );
	v_out_matrix.m43 = near_plane / (  near_plane - far_plane );;

	return v_out_matrix;
}

template <class real>
matrix4<real> matrix4<real>::reflection( const vector3<real>& normal, const vector3<real>& point )
{
	//     +-                         -+
	// M = | I-2*N*N^T    2*Dot(N,P)*N |
	//     |     0^T            1      |
	//     +-                         -+
	//
	// where P is a point on the plane and N is a unit-length plane normal.

	real two_normal_dot_point = ( (real)2.0) * ( normal.dot(point) );

	//m[0] = (real)1.0 - ((real)2.0) * normal.x * normal.x;
	//m[1] = -((real)2.0) * normal.x * normal.y;
	//m[2] = -((real)2.0) * normal.x * normal.z;
	//m[3] = two_normal_dot_point * normal.x;

	//m[4] = -((real)2.0) * normal.y * normal.x;
	//m[5] = (real)1.0 - ((real)2.0) * normal.y * normal.y;
	//m[6] = -((real)2.0) * normal.y * normal.z;
	//m[7] = two_normal_dot_point * normal.y;

	//m[8] = -((real)2.0) * normal.z * normal.x;
	//m[9] = -((real)2.0) * normal.z * normal.y;
	//m[10] = (real)1.0 - ((real)2.0) * normal.z * normal.z;
	//m[11] = two_normal_dot_point * normal.z;

	//m[12] = (real)0.0;
	//m[13] = (real)0.0;
	//m[14] = (real)0.0;
	//m[15] = (real)1.0;

	//transpose();

	//return *this;
	matrix4<real> mat;

	mat.m[0] = (real)1.0 - ((real)2.0) * normal.x * normal.x;
	mat.m[1] = -((real)2.0) * normal.x * normal.y;
	mat.m[2] = -((real)2.0) * normal.x * normal.z;
	mat.m[3] = (real)0.0;

	mat.m[4] = -((real)2.0) * normal.y * normal.x;
	mat.m[5] = (real)1.0 - ((real)2.0) * normal.y * normal.y;
	mat.m[6] = -((real)2.0) * normal.y * normal.z;
	mat.m[7] = (real)0.0;

	mat.m[8] = -((real)2.0) * normal.z * normal.x;
	mat.m[9] = -((real)2.0) * normal.z * normal.y;
	mat.m[10] = (real)1.0 - ((real)2.0) * normal.z * normal.z;
	mat.m[11] = (real)0.0;

	mat.m[12] = two_normal_dot_point * normal.x;
	mat.m[13] = two_normal_dot_point * normal.y;
	mat.m[14] = two_normal_dot_point * normal.z;
	mat.m[15] = (real)1.0;

	mat.transpose();

	return mat;
}

//  rotate by axis
template <class real>
matrix4<real> matrix4<real>::rotate_axis_rh( const vector3<real>& from, const vector3<real>& to, real radian_value )
{
	matrix4<real> mat_rotate = matrix4<real>::s_identity;

	if ( fabs( radian_value ) < math<real>::s_min_real )
	{
		return mat_rotate;		
	}
	
	real from_to_length = ( to - from ).length();

	real cos_a		= (real)cos( radian_value );
	real one_sub_c	= 1.0f - cos_a;
	real sin_a		= (real)sin( radian_value );
	real unit_x	= (to.x - from.x) / from_to_length;
	real unit_y	= (to.y - from.y) / from_to_length;
	real unit_z	= (to.z - from.z) / from_to_length;

	mat_rotate.m11 = unit_x * unit_x * one_sub_c + cos_a;
	mat_rotate.m12 = unit_x * unit_y * one_sub_c - unit_z * sin_a;
	mat_rotate.m13 = unit_x * unit_z * one_sub_c + unit_y * sin_a;

	mat_rotate.m21 = unit_y * unit_x * one_sub_c + unit_z * sin_a;
	mat_rotate.m22 = unit_y * unit_y * one_sub_c + cos_a;
	mat_rotate.m23 = unit_y * unit_z * one_sub_c - unit_x * sin_a;

	mat_rotate.m31 = unit_z * unit_x * one_sub_c - unit_y * sin_a;
	mat_rotate.m32 = unit_z * unit_y * one_sub_c + unit_x * sin_a;
	mat_rotate.m33 = unit_z * unit_z * one_sub_c + cos_a;

	matrix4<real> temp_mat_one = translation( -from.x, -from.y, -from.z );
	matrix4<real> temp_mat_two = translation( from.x, from.y, from.z );
	
	return temp_mat_two * ( mat_rotate * temp_mat_one );
}

//  rotate by axis
template <class real>
matrix4<real> matrix4<real>::rotate_axis_lh( const vector3<real>& from, const vector3<real>& to, real radian_value )
{
	matrix4<real> mat_rotate = matrix4<real>::s_identity;

	if ( fabs( radian_value ) < math<real>::s_min_real )
	{
		return mat_rotate;		
	}

	real from_to_length = ( to - from ).length();

	real cos_a		= (real)cos( radian_value );
	real one_sub_c	= 1.0f - cos_a;
	real sin_a		= (real)sin( radian_value );
	real unit_x	= (to.x - from.x) / from_to_length;
	real unit_y	= (to.y - from.y) / from_to_length;
	real unit_z	= (to.z - from.z) / from_to_length;

	mat_rotate.m11 = unit_x * unit_x * one_sub_c + cos_a;
	mat_rotate.m12 = unit_x * unit_y * one_sub_c + unit_z * sin_a;
	mat_rotate.m13 = unit_x * unit_z * one_sub_c - unit_y * sin_a;

	mat_rotate.m21 = unit_y * unit_x * one_sub_c - unit_z * sin_a;
	mat_rotate.m22 = unit_y * unit_y * one_sub_c + cos_a;
	mat_rotate.m23 = unit_y * unit_z * one_sub_c + unit_x * sin_a;

	mat_rotate.m31 = unit_z * unit_x * one_sub_c + unit_y * sin_a;
	mat_rotate.m32 = unit_z * unit_y * one_sub_c - unit_x * sin_a;
	mat_rotate.m33 = unit_z * unit_z * one_sub_c + cos_a;

	matrix4<real> temp_mat_one = translation( -from.x, -from.y, -from.z );
	matrix4<real> temp_mat_two = translation( from.x, from.y, from.z );

	
	//return temp_mat_two * ( mat_rotate * temp_mat_one );
	return temp_mat_one * ( mat_rotate * temp_mat_two );
}

// rotate by x
template <class real>
matrix4<real> matrix4<real>::rotate_x( real radian_value)
{
	matrix4<real> ret_mat = matrix4<real>::s_identity;
	ret_mat.m22 = ret_mat.m33 = (real)cos(radian_value);
	ret_mat.m23 = ret_mat.m32 = (real)sin(radian_value);
	ret_mat.m32 = -ret_mat.m32;
	return ret_mat;
}

// rotate by y
template <class real>
matrix4<real> matrix4<real>::rotate_y( real radian_value)
{
	matrix4<real> ret_mat = matrix4<real>::s_identity;
	ret_mat.m11 = ret_mat.m33 = cos(radian_value);
	ret_mat.m13 = ret_mat.m31 = sin(radian_value);
	ret_mat.m13 = -ret_mat.m13;
	return ret_mat;
}

// rotate by z
template <class real>
matrix4<real> matrix4<real>::rotate_z( real radian_value)
{
	matrix4<real> ret_mat = matrix4<real>::s_identity;
	ret_mat.m11 = ret_mat.m22 = cos(radian_value);
	ret_mat.m12 = ret_mat.m21 = sin(radian_value);
	ret_mat.m21 = -ret_mat.m21;
	return ret_mat;
}

// tranlate to x ,y ,z
template <class real>
matrix4<real> matrix4<real>::translation( real x, real y, real z )
{
	matrix4<real> ret_mat = matrix4<real>::s_identity;
	ret_mat.m41 = x;
	ret_mat.m42 = y;
	ret_mat.m43 = z;
	return ret_mat;
}

// scale
template <class real>
matrix4<real> matrix4<real>::scaling( real x, real y, real z )
{
	matrix4<real> ret_mat = matrix4<real>::s_identity;
	ret_mat.m11 = x;
	ret_mat.m22 = y;
	ret_mat.m33 = z;
	return ret_mat;
}

template <class real>
matrix4<real> matrix4<real>::clip(real clip_x, real clip_y, real clip_width, real clip_height, real min_z, real max_z )
{
	matrix4<real> ret_mat = matrix4<real>::s_identity;
	ret_mat.m11 = 2 / clip_width;
	ret_mat.m22 = 2 / clip_height;
	ret_mat.m33 = 1 / (max_z - min_z);
	ret_mat.m41 = -1 - 2 * clip_x / clip_width;
	ret_mat.m42 = 1 - 2 * clip_y / clip_height;
	ret_mat.m43 = -  min_z / (max_z - min_z);
	return ret_mat;
}

template <class real>
matrix4<real> matrix4<real>::viewport(real x, real y, real width, real height, real min_z, real max_z)
{
	matrix4<real> ret_mat = matrix4<real>::s_identity;
	ret_mat.m11 = width  * 0.5;
	ret_mat.m22 = height * 0.5;
	ret_mat.m33 = max_z - min_z;
	ret_mat.m41 = x + width * 0.5;
	ret_mat.m42 = y + height * 0.5;
	ret_mat.m43 = min_z;
	return ret_mat;
}




