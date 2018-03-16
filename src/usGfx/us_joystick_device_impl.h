///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_joystick_device_impl.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : joystick_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_JOYSTICK_DEVICE_IMPL_H_
#define _US_JOYSTICK_DEVICE_IMPL_H_
#include <usGfx/Export.h>
#include <usGfx/us_joystick_argument.h>
#include <usGfx/us_input_device_base.h>
#include <usUtil/us_common_file.h>

//class IDirectInputDevice8;
#include <dinput.h>
#include <windows.h>

namespace uniscope_globe
{
	class USGFX_EXPORT joystick_device_impl
		: public input_device_base
	{
	protected:
		joystick_device_impl() {}

	public:
		joystick_device_impl(IDirectInputDevice8* in_direct_joystick);

		virtual ~joystick_device_impl(void);

	public:
		bool create(input_device_argument* args);

		virtual void destroy(void);

		virtual bool refresh(void);

	public:
		static BOOL CALLBACK enum_objects_callback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);

	protected:
		void collect_joystick_info(void);

	protected:
		IDirectInputDevice8*	m_direct_joystick;

		joystick_state m_cur_state;

		input_device_argument* m_args;

	};
}

#endif // _US_JOYSTICK_DEVICE_IMPL_H_