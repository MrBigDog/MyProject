///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_tga_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : tga_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TGA_FILTER_H_
#define _US_TGA_FILTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class USFILTER_EXPORT tga_filter
		: public filter_base
	{
	public:
		tga_filter( void ) {}

		virtual ~tga_filter( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer ); 
	};
}

#endif // _US_TGA_FILTER_H_