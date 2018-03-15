///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_match_model_f_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : match_model_f_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MATCH_MODEL_F_FILTER_H_
#define _US_MATCH_MODEL_F_FILTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class USFILTER_EXPORT match_model_f_filter : public filter_base
	{
	public:
		match_model_f_filter( void ){}

		virtual ~match_model_f_filter( void ){}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );
		 
	};
}

#endif // _US_MATCH_MODEL_F_FILTER_H_ 