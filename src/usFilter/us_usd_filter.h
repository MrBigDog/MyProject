///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_filter_v2.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_filter_v2 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_FILTER_V2_H_
#define _US_USX_FILTER_V2_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class USFILTER_EXPORT usx_filter_v2
		: public filter_base
	{
	public:
		usx_filter_v2( void ) { }

		virtual ~usx_filter_v2( void ) { }

	public:
		virtual object_base* parse( raw_buffer* in_buffer );
		 
	};
}

#endif // _US_USX_FILTER_V2_H_