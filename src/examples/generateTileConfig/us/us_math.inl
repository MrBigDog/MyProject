//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_math.inl
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//
//////////////////////////////////////////////////////////////////////////


template <class real>
real math<real>::acos_ (real value)
{
	if ( -(real)1.0 < value )
	{
		if ( value < (real)1.0 )
			return (real)acos((double)value);
		else
			return (real)0.0;
	}
	else
	{
		return s_pi;
	}
}

template <class real>
real math<real>::asin_ (real value)
{
	if ( -(real)1.0 < value )
	{
		if ( value < (real)1.0 )
			return (real)asin((double)value);
		else
			return s_half_pi;
	}
	else
	{
		return -s_half_pi;
	}
}

template <class real>
real math<real>::atan_ (real value)
{
	return (real)atan((double)value);
}

template <class real>
real math<real>::atan2_ (real fY, real fX)
{
	return (real)atan2((double)fY,(double)fX);
}

template <class real>
real math<real>::ceil_ (real value)
{
	return (real)ceil((double)value);
}

template <class real>
real math<real>::cos_ (real value)
{
	return (real)cos((double)value);
}

template <class real>
real math<real>::exp_ (real value)
{
	return (real)exp((double)value);
}

template <class real>
real math<real>::fabs_ (real value)
{
	return (real)fabs((double)value);
}

template <class real>
real math<real>::floor_ (real value)
{
	return (real)floor((double)value);
}

template <class real>
real math<real>::fmod_ (real fX, real fY)
{
	return (real)fmod((double)fX,(double)fY);
}

template <class real>
real math<real>::inv_sqrt_ (real value)
{
	return (real)(1.0/sqrt((double)value));
}

template <class real>
real math<real>::log_ (real value)
{
	return (real)log((double)value);
}

template <class real>
real math<real>::pow_ (real fBase, real fExponent)
{
	return (real)pow((double)fBase,(double)fExponent);
}

template <class real>
real math<real>::sin_ (real value)
{
	return (real)sin((double)value);
}

template <class real>
real math<real>::sqr_ (real value)
{
	return value*value;
}

template <class real>
real math<real>::sqrt_ (real value)
{
	return (real)sqrt((double)value);
}

template <class real>
real math<real>::tan_ (real value)
{
	return (real)tan((double)value);
}


template <class real>
real math<real>::cot_ (real value)
{
	return (real)1.0/(real)tan((double)value);
}