//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_vector2.inl
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : matrix4x4 template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////

template<class real>
vector2<real>::vector2(void)
{
	x = real(0);
	y = real(0);
}

template<class real>
vector2<real>::~vector2(void)
{
}


template<class real>
vector2<real>::vector2( real v_x, real v_y )
{
	x = v_x;
	y = v_y;
}

template<class real>
vector2<real>::vector2( const vector2& t )
{
	x = real(t.x);
	y = real(t.y);
}

// Assigns the value of the other vector.or param
template<class real>
vector2<real>& vector2<real>::operator = ( const vector2& vec )
{
	x = vec.x;
	y = vec.y;

	return *this;
}

template<class real>
bool vector2<real>::operator == ( const vector2& vec ) const
{
	return !( *this != vec );
}

template<class real>
bool vector2<real>::operator != ( const vector2& vec ) const
{
	return ( ( x != vec.x ) || ( y != vec.y ) );
}

template<class real>
vector2<real> vector2<real>::operator + ( const vector2& vec ) const
{
	vector2 v_vec( x + vec.x, y + vec.y );
	return v_vec;
}

template<class real>
vector2<real> vector2<real>::operator - ( const vector2& vec ) const 
{
	vector2 v_vec( x - vec.x, y - vec.y );
	return v_vec;
}

template<class real>
vector2<real> vector2<real>::operator - () const
{
	vector2 v_ret( -x, -y );
	return v_ret;
}

template<class real>
vector2<real> vector2<real>::operator * ( const real scale_value ) const
{
	vector2 v_vec( x * scale_value, y * scale_value );
	return v_vec;
}


template<class real>
vector2<real> vector2<real>::operator * ( const vector2& vec ) const
{
	vector2 v_vec( vec.x * x, vec.y * y );
	return v_vec;
}

template<class real>
vector2<real> vector2<real>::operator / ( const real scale_value ) const 
{

	real r = 1.0 / scale_value;
	vector2 v_vec( x * r, y * r );
	return v_vec;
}

template<class real>
vector2<real>& vector2<real>::operator += ( const vector2& vec )
{
	x += vec.x;
	y += vec.y;
	return *this;
}

template<class real>
vector2<real>& vector2<real>::operator -= ( const vector2& vec )
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

template<class real>
vector2<real>& vector2<real>::operator *= ( const real scale_value )
{
	x *= scale_value;
	y *= scale_value;
	return *this;
}

template<class real>
vector2<real>& vector2<real>::operator *= ( const vector2& vec )
{
	x *= vec.x;
	y *= vec.y;
	return *this;
}

template<class real>
vector2<real>& vector2<real>::operator /= ( const real scale_value )
{
	real r = 1.0 / scale_value;
	x *= r;
	y *= r;

	return *this;
}

template<class real>
void vector2<real>::scale( const real scale_value )
{
	x *= scale_value;
	y *= scale_value;
}

template<class real>
double vector2<real>::length(void)
{
	return sqrt( x * x + y * y );
}

template<class real>
double vector2<real>::squared_length(void)
{
	return double( x * x + y * y );
}

template<class real>
void vector2<real>::normalize(void)
{
	double v_length = length();
	if ( fabs( v_length ) < 1e-10 )
	{
		return;
	}

	real r = 1.0 / v_length;
	x *= r;
	y *= r;
}

template<class real>
double vector2<real>::dot( const vector2& vec1, const vector2& vec2 )
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

template<class real>
double vector2<real>::cross( const vector2& vec1, const vector2& vec2 )
{
	return vec1.x * vec2.y - vec1.y * vec2.x;
}

template<class real>
vector2<real> vector2<real>::normalize( const vector2& vec )
{
	vector2<real> v_ret = vec;
	v_ret.normalize();
	return v_ret;
}

// Calculates a reflection vector to the plane with the given normal .
// and remarks NB assumes 'this' is pointing AWAY FROM the plane, invert if it is not
template<class real>
vector2<real> vector2<real>::reflect( const vector2& src, const vector2& normal )
{
	return vector2( src - ( 2 * vector2::dot( src, normal ) * normal ) );
}
