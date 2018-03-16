///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mouse_3d_device_impl.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mouse_3d_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MOUSE_3D_DEVICE_IMPL_H_
#define _US_MOUSE_3D_DEVICE_IMPL_H_
#include <usGfx/Export.h>
#include <usGfx/us_input_device_base.h>
#include <usGfx/us_joystick_argument.h>
#include <usUtil/us_common_file.h>

#include <dinput.h>

namespace uniscope_globe
{
	class input_device_argument;

	class USGFX_EXPORT mouse_3d_device_impl
		: public input_device_base
	{
	public:
		mouse_3d_device_impl(IDirectInputDevice8* in_direct_mouse);

		virtual ~mouse_3d_device_impl(void);

	public:
		bool create(input_device_argument* args);

		virtual void destroy(void);

		virtual bool refresh(void);

	public:
		static BOOL CALLBACK enum_objects_callback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext);

	protected:
		void collect_mouse_info(void);

	public:
		IDirectInputDevice8* m_direct_mouse;

		joystick_state m_cur_state;

		joystick_state m_last_state;

		input_device_argument* m_args;
	};
}

#endif // _US_MOUSE_3D_DEVICE_IMPL_H_