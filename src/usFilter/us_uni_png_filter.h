///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_uni_png_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : uni_png_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_UNI_PNG_FILTER_H_
#define _US_UNI_PNG_FILTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class LIB_FILTER_API uni_png_filter
		: public filter_base
	{
	public:
		uni_png_filter( void ){}

		virtual ~uni_png_filter( void ){}

	public:
		virtual object_base* parse( raw_buffer* in_buffer ); 
		 
	};
}

#endif // _US_UNI_PNG_FILTER_H_