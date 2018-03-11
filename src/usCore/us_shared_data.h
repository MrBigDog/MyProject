///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shared_data.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shared_data class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SHARED_DATA_H_
#define _US_SHARED_DATA_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT shared_data
	{
	public:
		shared_data( void );

		virtual ~shared_data( void )
		{
		}

	public:
		virtual void add_ref( void ) ;

		virtual void release( void );

	protected:
		int __use_count__;		 
	};
}

#endif // _US_SHARED_DATA_H_