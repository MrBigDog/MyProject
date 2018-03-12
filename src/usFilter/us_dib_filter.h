///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_dib_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : dib_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_DIB_FILTER_H_
#define _US_DIB_FILTER_H_

#include <usFilter\Export.h>

namespace uniscope_globe
{
	class USFILTER_EXPORT dib_filter
		: public filter_base
	{
	public:
		dib_filter( void ) {}

		virtual ~dib_filter( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer ); 
		 
	};
}

#endif // _US_DIB_FILTER_H_