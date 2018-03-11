///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_ugp_filter_v1.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : ugp_filter_v1 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_UGP_FILTER_V1_H_
#define _US_UGP_FILTER_V1_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class LIB_FILTER_API ugp_filter_v1
			: public filter_base
	{
	public:
		ugp_filter_v1( void ) { }

		virtual ~ugp_filter_v1( void ) { }

	public:
		virtual object_base* parse( raw_buffer* in_buffer );
		 
	};
}

#endif // _US_UGP_FILTER_V1_H_ 