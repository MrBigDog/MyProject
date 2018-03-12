///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_jpeg_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : jpeg_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_JPEG_FILTER_H_
#define _US_JPEG_FILTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class LIB_FILTER_API jpeg_filter 
		: public filter_base
	{
	public:
		jpeg_filter( void ) {}

		virtual ~jpeg_filter( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer ); 
		 
	};
}

#endif // _US_JPEG_FILTER_H_