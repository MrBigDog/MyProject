///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_filter_buffer.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : filter_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_FILTER_BUFFER_H_
#define _US_FILTER_BUFFER_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT filter_buffer: public raw_buffer
	{
	protected:
		filter_buffer( void ){}

	public:
		filter_buffer( void* in_buffer, int in_size, object_base* in_parent, object_base* in_container );

		virtual ~filter_buffer( void );

	public:
		 object_base*	m_parent;

		 object_base*   m_container;
	};
}

#endif // _US_FILTER_BUFFER_H_