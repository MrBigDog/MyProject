///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_sound_device.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : sound_device class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SOUND_DEVICE_H_
#define _US_SOUND_DEVICE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT sound_device : public object_base
	{
	public:
		sound_device( void );

		virtual ~sound_device( void );

	public:
		virtual bool create( HWND hwnd, dword coop_level ) { return true; }

		virtual void destory( void ) {}

		inline ULONG_PTR get_direct_sound( void ) { return 0; }	 
	};
}

#endif // _US_SOUND_DEVICE_H_