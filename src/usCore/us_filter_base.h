///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_filter_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : filter_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_FILTER_BASE_H_
#define _US_FILTER_BASE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class filter_manager;
	class raw_buffer;

	class USCORE_EXPORT filter_base
	{
	public:
		filter_base( void );

		virtual ~filter_base( void );

	public:
		virtual object_base* parse( raw_buffer* in_buffer ) = 0;

		void set_manager( filter_manager *manager );

	protected:
		filter_manager* m_filter_manager;
		 
	};
}

#endif // _US_FILTER_BASE_H_