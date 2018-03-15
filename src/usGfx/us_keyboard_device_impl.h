///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_keyboard_device_impl.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : keyboard_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_KEYBOARD_DEVICE_IMPL_H_
#define _US_KEYBOARD_DEVICE_IMPL_H_

namespace uniscope_globe
{
	class LIB_GFX_API keyboard_device_impl
		: public input_device_base
	{
	protected:
		keyboard_device_impl(){}

	public:
		keyboard_device_impl(IDirectInputDevice8* in_direct_keyboard);

		virtual ~keyboard_device_impl(void);

	public:
		bool create( input_device_argument* args );

		virtual void destroy( void );

		virtual bool refresh( void );

	protected:
		void collect_keyboard_info( void );

	private:     
		IDirectInputDevice8* m_direct_keyoboard;

		input_device_argument* m_args;
	};
}

#endif // _US_KEYBOARD_DEVICE_IMPL_H_