///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_ust_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : ust_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_UST_FILTER_H_
#define _US_UST_FILTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class ust_filter
		: public filter_base
	{
	public:
		ust_filter( void ){}

		virtual ~ust_filter( void ){}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );
		 
	};
}

#endif // _US_UST_FILTER_H_