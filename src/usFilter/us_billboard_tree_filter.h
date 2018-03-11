///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_billboard_tree_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : billboard_tree_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_BILLBOARD_TREE_FILTER_H_
#define _US_BILLBOARD_TREE_FILTER_H_

#include <usFilter\Export.h>

namespace uniscope_globe
{
	class USFILTER_EXPORT billboard_tree_filter: public filter_base
	{
	public:
		billboard_tree_filter( void );

		virtual ~billboard_tree_filter( void );

		virtual object_base* parse( raw_buffer* in_buffer );

	};
}

#endif // _US_BILLBOARD_TREE_FILTER_H_