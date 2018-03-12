///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_FILTER_H_
#define _US_USX_FILTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class LIB_FILTER_API usx_filter
		: public filter_base
	{
	public:
		usx_filter( void ) {};

		virtual ~usx_filter( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );
		 
	};
}

#endif // _US_USX_FILTER_H_