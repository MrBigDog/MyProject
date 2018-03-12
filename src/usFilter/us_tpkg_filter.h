///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_tpkg_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : tpkg_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TPKG_FILTER_H_
#define _US_TPKG_FILTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class LIB_FILTER_API tpkg_filter
		: public filter_base
	{
	public:
		tpkg_filter( void ){}

		virtual ~tpkg_filter( void ){}

	public:
		virtual object_base* parse( raw_buffer* in_buffer ); 
	};
}

#endif // _US_TPKG_FILTER_H_