//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_vector4.inl
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : vector4 template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_VECTOR4_INL_
#define _US_VECTOR4_INL_

template <class real>
vector4<real>::vector4 ()
{

}

template <class real>
vector4<real>::vector4 (real fx, real fy, real fz, real fw)
{
	m_tuple[0] = fx;
	m_tuple[1] = fy;
	m_tuple[2] = fz;
	m_tuple[3] = fw;
}


template <class real>
vector4<real>::vector4 (const vector4& v)
{
	m_tuple[0] = v.m_tuple[0];
	m_tuple[1] = v.m_tuple[1];
	m_tuple[2] = v.m_tuple[2];
	m_tuple[3] = v.m_tuple[3];
}


template <class real>
vector4<real>& vector4<real>::operator= (const vector4& v)
{
	m_tuple[0] = v.m_tuple[0];
	m_tuple[1] = v.m_tuple[1];
	m_tuple[2] = v.m_tuple[2];
	m_tuple[3] = v.m_tuple[3];
	return *this;
}



//
// compare
//
template <class real>
bool vector4<real>::operator== (const vector4& v) const
{
	real diff_x = m_tuple[0] - v.m_tuple[0];
	real diff_y = m_tuple[1] - v.m_tuple[1];
	real diff_z = m_tuple[2] - v.m_tuple[2];
	real diff_w = m_tuple[3] - v.m_tuple[3];

	return	(diff_x <= math<real>::s_zero_tolerance ) &&
		(diff_y <= math<real>::s_zero_tolerance) &&
		(diff_z <= math<real>::s_zero_tolerance) &&
		(diff_w <= math<real>::s_zero_tolerance);
}

template <class real>
bool vector4<real>::operator!= (const vector4& v) const
{
	real diff_x = m_tuple[0] - v.m_tuple[0];
	real diff_y = m_tuple[1] - v.m_tuple[1];
	real diff_z = m_tuple[2] - v.m_tuple[2];
	real diff_w = m_tuple[3] - v.m_tuple[3];

	return	(diff_x > math<real>::s_zero_tolerance) ||
		(diff_y > math<real>::s_zero_tolerance) ||
		(diff_z > math<real>::s_zero_tolerance) ||
		(diff_w > math<real>::s_zero_tolerance);
}


//
// arithmetic
//
template <class real>
vector4<real> vector4<real>::operator+ (const vector4& v) const
{
	return vector4(
		m_tuple[0] + v.m_tuple[0],
		m_tuple[1] + v.m_tuple[1],
		m_tuple[2] + v.m_tuple[2],
		m_tuple[3] + v.m_tuple[3]);
}

template <class real>
vector4<real> vector4<real>::operator- (const vector4& v) const
{
	return vector4(
		m_tuple[0] - v.m_tuple[0],
		m_tuple[1] - v.m_tuple[1],
		m_tuple[2] - v.m_tuple[2],
		m_tuple[3] - v.m_tuple[3]);
}

template <class real>
vector4<real> vector4<real>::operator* (real scalar) const
{
	return vector4(
		scalar * m_tuple[0],
		scalar * m_tuple[1],
		scalar * m_tuple[2],
		scalar * m_tuple[3]);
}

template <class real>
vector4<real> vector4<real>::operator/ (real scalar) const
{
	vector4 quot;

	if ( scalar != (real)0.0 )
	{
		real inv_scalar = ((real)1.0)/scalar;
		quot.m_tuple[0] = inv_scalar * m_tuple[0];
		quot.m_tuple[1] = inv_scalar * m_tuple[1];
		quot.m_tuple[2] = inv_scalar * m_tuple[2];
		quot.m_tuple[3] = inv_scalar * m_tuple[3];
	}
	else
	{
		quot.m_tuple[0] = math<real>::s_max_real;
		quot.m_tuple[1] = math<real>::s_max_real;
		quot.m_tuple[2] = math<real>::s_max_real;
		quot.m_tuple[3] = math<real>::s_max_real;
	}

	return quot;
}

template <class real>
vector4<real> vector4<real>::operator- () const
{
	return vector4(
		-m_tuple[0],
		-m_tuple[1],
		-m_tuple[2],
		-m_tuple[3]);
}

template <class real>
vector4<real> operator* (real scalar, const vector4<real>& v)
{
	return vector4<real>(
		scalar * v[0],
		scalar * v[1],
		scalar * v[2],
		scalar * v[3]);
}

template <class real>
vector4<real>& vector4<real>::operator+= (const vector4& v)
{
	m_tuple[0] += v.m_tuple[0];
	m_tuple[1] += v.m_tuple[1];
	m_tuple[2] += v.m_tuple[2];
	m_tuple[3] += v.m_tuple[3];
	return *this;
}

template <class real>
vector4<real>& vector4<real>::operator-= (const vector4& v)
{
	m_tuple[0] -= v.m_tuple[0];
	m_tuple[1] -= v.m_tuple[1];
	m_tuple[2] -= v.m_tuple[2];
	m_tuple[3] -= v.m_tuple[3];
	return *this;
}

template <class real>
vector4<real>& vector4<real>::operator*= (real scalar)
{
	m_tuple[0] *= scalar;
	m_tuple[1] *= scalar;
	m_tuple[2] *= scalar;
	m_tuple[3] *= scalar;
	return *this;
}

template <class real>
vector4<real>& vector4<real>::operator/= (real scalar)
{
	if ( scalar != (real)0.0 )
	{
		real inv_scalar = ((real)1.0)/scalar;
		m_tuple[0] *= inv_scalar;
		m_tuple[1] *= inv_scalar;
		m_tuple[2] *= inv_scalar;
		m_tuple[3] *= inv_scalar;
	}
	else
	{
		m_tuple[0] = math<real>::s_max_real;
		m_tuple[1] = math<real>::s_max_real;
		m_tuple[2] = math<real>::s_max_real;
		m_tuple[3] = math<real>::s_max_real;
	}

	return *this;
}

template <class real>
real vector4<real>::length () const
{
	return math<real>::sqrt_(
		m_tuple[0]*m_tuple[0] +
		m_tuple[1]*m_tuple[1] +
		m_tuple[2]*m_tuple[2] +
		m_tuple[3]*m_tuple[3]);
}
//----------------------------------------------------------------------------
template <class real>
real vector4<real>::squared_length () const
{
	return
		m_tuple[0]*m_tuple[0] +
		m_tuple[1]*m_tuple[1] +
		m_tuple[2]*m_tuple[2] +
		m_tuple[3]*m_tuple[3];
}
//----------------------------------------------------------------------------
template <class real>
real vector4<real>::dot (const vector4& v) const
{
	return
		m_tuple[0]*v.m_tuple[0] +
		m_tuple[1]*v.m_tuple[1] +
		m_tuple[2]*v.m_tuple[2] +
		m_tuple[3]*v.m_tuple[3];
}
//----------------------------------------------------------------------------
template <class real>
real vector4<real>::normalize ()
{
	real length_val = length();

	if ( length_val > math<real>::s_zero_tolerance )
	{
		real inv_length = ((real)1.0)/length_val;
		m_tuple[0] *= inv_length;
		m_tuple[1] *= inv_length;
		m_tuple[2] *= inv_length;
		m_tuple[3] *= inv_length;
	}
	else
	{
		length_val = (real)0.0;
		m_tuple[0] = (real)0.0;
		m_tuple[1] = (real)0.0;
		m_tuple[2] = (real)0.0;
		m_tuple[3] = (real)0.0;
	}

	return length_val;
}


#endif //_US_VECTOR4_INL_

