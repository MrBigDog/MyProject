///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shared_data.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shared_data class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_shared_data.h"

namespace uniscope_globe
{
	shared_data::shared_data( void )
	{
		__use_count__ = 0;
	}

	void shared_data::add_ref( void )
	{ 
		__use_count__++;
	}

	void shared_data::release( void )
	{
		//__use_count__--;
		//if ( __use_count__ == -1 )
		//{
		//	delete this;
		//}

		__use_count__--;
		if ( __use_count__ == 0 )
		{
			delete this;
		} 
	}
}