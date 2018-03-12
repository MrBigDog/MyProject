
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_uni_jpeg_filter_lm.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : uni_jpeg_filter_lm class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_UNI_JPEG_FILTER_LM_H_
#define _US_UNI_JPEG_FILTER_LM_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class uni_jpeg_filter_lm
		: public filter_base
	{
	public:
		uni_jpeg_filter_lm( void ) { }

		virtual ~uni_jpeg_filter_lm( void ) { }

	public:
		virtual object_base* parse( raw_buffer* in_buffer );
		 
	};
}

#endif // _US_UNI_JPEG_FILTER_LM_H_ 