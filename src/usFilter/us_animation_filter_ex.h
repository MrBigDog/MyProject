///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_animation_filter_ex.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : animation_filter_ex class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_ANIMATION_FILTER_EX_H_
#define _US_ANIMATION_FILTER_EX_H_

#include <usFilter\Export.h>
#include <usCore\us_filter_base.h>

namespace uniscope_globe
{
	class USFILTER_EXPORT animation_filter_ex
		: public filter_base
	{
	public:
		animation_filter_ex(void);

		virtual ~animation_filter_ex(void);

	public:
		virtual object_base* parse(raw_buffer* in_buffer);

	};
}

#endif // _US_ANIMATION_FILTER_EX_H_