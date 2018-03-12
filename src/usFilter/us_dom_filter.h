///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_dom_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : dom_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_DOM_FILTER_H_
#define _US_DOM_FILTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class USFILTER_EXPORT dom_filter
		: public filter_base
	{
	public:
		dom_filter( void ) {}

		virtual ~dom_filter( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer ); 

	private:
		object_base* parse_from_compressed_stream( raw_buffer* in_buffer ); 

		//object_base* parse_from_compressed_stream2( raw_buffer* in_buffer ); 

	};


	class USFILTER_EXPORT dom_filter_v2
		: public filter_base
	{
	public:
		dom_filter_v2( void ) {}

		virtual ~dom_filter_v2( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer ); 

	private:
		//object_base* parse_from_compressed_stream( raw_buffer* in_buffer ); 

		object_base* parse_from_compressed_stream( raw_buffer* in_buffer ); 

	};


	class USFILTER_EXPORT dom_filter_v3
		: public filter_base
	{
	public:
		dom_filter_v3( void ) {}

		virtual ~dom_filter_v3( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer ); 

	private:
		//object_base* parse_from_compressed_stream( raw_buffer* in_buffer ); 

		object_base* parse_from_compressed_stream( raw_buffer* in_buffer ); 

	};
}

#endif // _US_DOM_FILTER_H_