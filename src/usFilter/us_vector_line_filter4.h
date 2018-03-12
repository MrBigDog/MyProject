
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_vector_line_filter4.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : vector_line_filter4 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_VECTOR_LINE_FILTER4_H_
#define _US_VECTOR_LINE_FILTER4_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class vector_line_filter4
		: public filter_base
	{
	public:
		vector_line_filter4( void ) {}

		virtual ~vector_line_filter4( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );

	};
}

#endif // _US_VECTOR_LINE_FILTER4_H_ 