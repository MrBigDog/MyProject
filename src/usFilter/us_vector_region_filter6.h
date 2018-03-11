///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_vector_region_filter6.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : vector_region_filter6 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_VECTOR_REGION_FILTER6_H_
#define _US_VECTOR_REGION_FILTER6_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class vector_region_filter6
		: public filter_base
	{
	public:
		vector_region_filter6( void );

		virtual ~vector_region_filter6( void );

	public:
		virtual object_base* parse( raw_buffer* stream );
		
	};
}

#endif // _US_VECTOR_REGION_FILTER6_H_ 