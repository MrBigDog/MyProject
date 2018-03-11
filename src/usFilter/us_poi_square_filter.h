///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_poi_square_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : poi_square_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_POI_SQUARE_FILTER_H_
#define _US_POI_SQUARE_FILTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{

	class LIB_FILTER_API poi_square_filter
		: public filter_base
	{
	public:
		poi_square_filter( void );

		virtual ~poi_square_filter( void );

		virtual object_base* parse( raw_buffer* in_buffer );
	};
}

#endif // _US_POI_SQUARE_FILTER_H_