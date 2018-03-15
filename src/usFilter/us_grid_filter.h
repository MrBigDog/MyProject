///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_grid_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : grid_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GRID_FILTER_H_
#define _US_GRID_FILTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class USFILTER_EXPORT grid_filter
		: public filter_base
	{
	public:
		grid_filter( void ) {}

		virtual ~grid_filter( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );

	private:
		object_base* parse_from_compressed_stream( raw_buffer* stream, interior_index index );
	};

	class USFILTER_EXPORT grid_filter_v2
		: public filter_base
	{
	public:
		grid_filter_v2( void ) {}

		virtual ~grid_filter_v2( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );

	private:
		object_base* parse_from_compressed_stream( raw_buffer* stream, interior_index index );

	};
}

#endif // _US_GRID_FILTER_H_