///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mouse_device_impl.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mouse_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MOUSE_DEVICE_IMPL_H_
#define _US_MOUSE_DEVICE_IMPL_H_

namespace uniscope_globe
{
	class LIB_GFX_API mouse_device_impl
		: public input_device_base
	{
	public:
		mouse_device_impl(){}

	public:
		mouse_device_impl(IDirectInputDevice8* in_direct_mouse);

		virtual ~mouse_device_impl(void);

	public:
		bool create( mouse_argument* args );

		virtual void destroy( void );

		virtual bool refresh( void );

	protected:
		void collect_mouse_info( void );

	private:
		IDirectInputDevice8*	m_direct_mouse;

		mouse_state m_cur_state;

		mouse_argument* m_args;
	};
}

#endif // _US_MOUSE_DEVICE_IMPL_H_