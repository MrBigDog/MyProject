///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_annotation_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : annotation_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_ANNOTATION_FILTER_H_
#define _US_ANNOTATION_FILTER_H_

#include <usFilter\Export.h>

namespace uniscope_globe
{
	class USFILTER_EXPORT annotation_filter : public filter_base
	{
	public:
		annotation_filter(void);

		virtual ~annotation_filter(void);

		virtual object_base* parse(raw_buffer* in_buffer);

	protected:
		annotation_square_resource* parse_from_stream(raw_buffer* stream);

	};
}

#endif // _US_ANNOTATION_FILTER_H_