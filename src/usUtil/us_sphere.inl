//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_sphere.inl
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shpere template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
template<class real>
sphere<real>::sphere( const vector3<real>& center, real radius )
{
	sphere_center = center;

	sphere_radius = radius;
}

template<class real>
sphere<real>::sphere( const sphere& src )
{
	sphere_center = src.sphere_center;

	sphere_radius = src.sphere_radius;
}

template<class real>
sphere<real>& sphere<real>::operator = ( const sphere& src )
{
	sphere_center = src.sphere_center;
	sphere_radius = src.sphere_radius;
	return *this;
}
