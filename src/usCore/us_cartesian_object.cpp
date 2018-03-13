///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_cartesian_object.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : cartesian_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_cartesian_object.h"

namespace uniscope_globe
{
	cartesian_object::cartesian_object( void )
		: cartesian_transform()
	{
		m_parent_object = NULL;
	}

	cartesian_object::~cartesian_object( void )
	{

	}

	void cartesian_object::release( void )
	{
		if( __use_count__ - 1 == 0 )
		{
			finalize();
		}

		shared_data::release();
	}

	matrix_4d cartesian_object::get_matrix_cart2geo( void )
	{
		return get_matrix_d();
	}

}