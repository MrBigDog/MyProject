///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_ipkg_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : ipkg_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_IPKG_FILTER_H_
#define _US_IPKG_FILTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class LIB_FILTER_API ipkg_filter
		: public filter_base
	{
	public:
		ipkg_filter( void ) {}

		virtual ~ipkg_filter( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer ); 

	private:
		object_base* parse_from_compressed_stream( raw_buffer*  stream );

	};
}

#endif // _US_IPKG_FILTER_H_