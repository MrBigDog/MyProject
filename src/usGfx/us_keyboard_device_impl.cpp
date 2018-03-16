///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_keyboard_device_impl.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : keyboard_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_keyboard_device_impl.h"
#include <usGfx/us_keyboard_argument.h>



namespace uniscope_globe
{
	keyboard_device_impl::keyboard_device_impl(IDirectInputDevice8* in_direct_keyboard)
	{
		m_direct_keyoboard = in_direct_keyboard;
	}

	keyboard_device_impl::~keyboard_device_impl(void)
	{
		destroy();
	}

	bool keyboard_device_impl::create(input_device_argument* args)
	{
		HRESULT hr;
		if (FAILED(hr = m_direct_keyoboard->SetDataFormat(&c_dfDIKeyboard)))
			return false;

		//		if( FAILED(hr = m_direct_keyoboard->SetCooperativeLevel( hwnd, DISCL_NONEXCLUSIVE |  DISCL_FOREGROUND )) )
		//			return false;	

		hr = m_direct_keyoboard->Acquire();

		m_args = args;

		return true;
	}

	void keyboard_device_impl::destroy(void)
	{
		if (m_direct_keyoboard)
		{
			m_direct_keyoboard->Unacquire();
		}

		AUTO_RELEASE(m_direct_keyoboard);
	}

	void keyboard_device_impl::collect_keyboard_info(void)
	{
		HRESULT hr;
		hr = m_direct_keyoboard->GetDeviceState(US_KEYBOARD_BUFFER_SIZE, (LPVOID)&m_args->m_state.keys);
		if (FAILED(hr)) // if get device false
		{
			hr = m_direct_keyoboard->Acquire();
			while (hr == DIERR_INPUTLOST)  // if lost input device
			{
				hr = m_direct_keyoboard->Acquire();
			}

			return;
		}
	}

	bool keyboard_device_impl::refresh(void)
	{
		collect_keyboard_info();

		return true;
	}


}