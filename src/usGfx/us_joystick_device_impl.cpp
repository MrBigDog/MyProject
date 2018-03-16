///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_joystick_device_impl.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : joystick_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_joystick_device_impl.h"
#include <usUtil/us_math.h>
#include <dinput.h>
namespace uniscope_globe
{
	//struct XINPUT_DEVICE_NODE
	//{
	//	DWORD dwVidPid;
	//	XINPUT_DEVICE_NODE* pNext;
	//};

	//struct DI_ENUM_CONTEXT
	//{
	//	DIJOYCONFIG* pPreferredJoyCfg;
	//	bool bPreferredJoyCfgValid;
	//};

	joystick_device_impl::joystick_device_impl(IDirectInputDevice8* in_direct_joystick)
	{
		// 		m_direct_input = NULL;
		m_direct_joystick = in_direct_joystick;
	}

	joystick_device_impl::~joystick_device_impl(void)
	{
		destroy();
	}

	bool joystick_device_impl::create(input_device_argument* args)
	{
		HRESULT hr;



		//Set the data format to "simple joystick" - a predefined data format 

		//A data format specifies which controls on a device we are interested in,
		//and how they should be reported. This tells DInput that we will be
		//passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
		if (FAILED(hr = m_direct_joystick->SetDataFormat(&c_dfDIJoystick2)))
			return false;//???????

		//Set the cooperative level to let DInput know how this device should
		//interact with the system and with other DInput applications.
		//if( FAILED( hr = m_direct_joystick->SetCooperativeLevel( m_hwnd, DISCL_EXCLUSIVE | 
		//	DISCL_FOREGROUND ) ) )
		//	return hr;

		//Enumerate the joystick objects. The callback function enabled user
		//interface elements for objects that are found, and sets the min/max
		//values property for discovered axes.
		if (FAILED(hr = m_direct_joystick->EnumObjects(enum_objects_callback, (VOID*)this, DIDFT_ALL)))
			return false;//???????????

		//hr = m_lpDI->CreateDevice(JoystickGUID,&m_lpDIDevice,NULL);


		// get the direct mouse device
		//if ( FAILED( hr = di->CreateDevice( GUID_Joystick, &m_direct_joystick, NULL ) ) )
		//	return false;
		//if( FAILED( hr = m_direct_joystick->SetDataFormat( &c_dfDIJoystick ) ) )
		//	return false;

		hr = m_direct_joystick->Acquire();

		m_args = args;

		memset((void*)&m_cur_state, 0, sizeof(joystick_state));

		//joystick_argument* v_joystick_args	= new joystick_argument;
		//v_joystick_args->m_vid = vid;
		return true;
	}

	void joystick_device_impl::destroy(void)
	{
		if (m_direct_joystick)
		{
			m_direct_joystick->Unacquire();
		}

		AUTO_RELEASE(m_direct_joystick);
	}

	void joystick_device_impl::collect_joystick_info(void)
	{
		HRESULT     hr;
		//DIJOYSTATE2 js;					// DInput joystick state 

		if (NULL == m_direct_joystick) return;

		// Poll the device to read the current state
		hr = m_direct_joystick->Poll();
		if (FAILED(hr))
		{
			if (hr == DIERR_NOTACQUIRED)
			{
				m_direct_joystick->Unacquire();
				m_direct_joystick->Acquire();
			}
			else if (hr == DIERR_INPUTLOST)
			{
				m_direct_joystick->Acquire();
			}

			// DInput is telling us that the input stream has been
			// interrupted. We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done. We
			// just re-acquire and try again.

			//hr = m_direct_joystick->Acquire();
			//while( hr == DIERR_INPUTLOST ) 
			//	hr = m_direct_joystick->Acquire();

			// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
			// may occur when the app is minimized or in the process of 
			// switching, so just try again later 
			return;
		}

		// Get the input's device state
		if (FAILED(hr = m_direct_joystick->GetDeviceState(sizeof(DIJOYSTATE2), (DIJOYSTATE2*)&m_cur_state)))
			return; // The device should have been acquired during the Poll()




		//args->m_joystick_state.m_axis_x = js.lX;
		//args->m_joystick_state.m_axis_y = js.lY;
		//args->m_joystick_state.m_axis_z = js.lZ;
		//args->m_joystick_state.m_axis_x_rotate = js.lRx;
		//args->m_joystick_state.m_axis_y_rotate = js.lRy;
		//args->m_joystick_state.m_axis_z_rotate = js.lRz;
		//args->m_joystick_state.button[0] = js.

		// Display joystick state to dialog
	}

	// refresh
	bool joystick_device_impl::refresh(void)
	{
		//return true;
		collect_joystick_info();

		//
		// 参数解析
		//

		//
		// 速度因子
		//
		double v_speed_factor = (double)(1000 - m_cur_state.rglSlider[0]) / 2000.0;
		v_speed_factor = CLAMP(v_speed_factor, 0.1, 1);

		m_args->m_state.x_move = m_cur_state.lX * v_speed_factor * 0.01;
		m_args->m_state.y_move = m_cur_state.lY * v_speed_factor * 0.01;
		m_args->m_state.z_move = 0.0;

		if (m_cur_state.rgbButtons[10] > 0 || m_cur_state.rgbButtons[2] > 0)
		{
			m_args->m_state.z_move = -0.01 * v_speed_factor;
		}
		else if (m_cur_state.rgbButtons[11] > 0 || m_cur_state.rgbButtons[3] > 0)
		{
			m_args->m_state.z_move = 0.01 * v_speed_factor;
		}

		if (math<double>::fabs_(m_args->m_state.x_move) < 1.0) m_args->m_state.x_move = 0.0;
		if (math<double>::fabs_(m_args->m_state.y_move) < 1.0) m_args->m_state.y_move = 0.0;

		m_args->m_state.y_rot = 0.0;

		m_args->m_state.z_rot = m_cur_state.lRz * 0.02 / 1000;

		ulong v_pov = m_cur_state.rgdwPOV[0];
		if (v_pov != -1)
		{
			m_args->m_state.z_rot = math<double>::sin_(v_pov * 0.01 * RADIAN) * 0.02;
			m_args->m_state.x_rot = math<double>::cos_(v_pov * 0.01 * RADIAN) * 0.02;
		}

		if (m_cur_state.rgbButtons[8] > 0)
		{
			m_args->m_state.x_rot = -0.02;
		}
		else if (m_cur_state.rgbButtons[9] > 0)
		{
			m_args->m_state.x_rot = 0.02;
		}

		NORMALIZE_RADIAN(m_args->m_state.x_rot);
		NORMALIZE_RADIAN(m_args->m_state.z_rot);

		memcpy((void*)m_args->m_state.stick_buttons, (void*)m_cur_state.rgbButtons, 128);


		return true;
	}

	//-----------------------------------------------------------------------------
	// Name: EnumObjectsCallback()
	// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
	//       joystick. This function enables user interface elements for objects
	//       that are found to exist, and scales axes min/max values.
	//-----------------------------------------------------------------------------
	BOOL joystick_device_impl::enum_objects_callback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext)
	{
		HWND hDlg = (HWND)pContext;

		static int nSliderCount = 0;  // Number of returned slider controls
		static int nPOVCount = 0;     // Number of returned POV controls

		joystick_device_impl* v_joystick = (joystick_device_impl*)pContext;

		// For axes that are returned, set the DIPROP_RANGE property for the
		// enumerated axis in order to scale min/max values.
		if (pdidoi->dwType & DIDFT_AXIS)
		{
			DIPROPRANGE diprg;
			diprg.diph.dwSize = sizeof(DIPROPRANGE);
			diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			diprg.diph.dwHow = DIPH_BYID;
			diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
			diprg.lMin = -1000;
			diprg.lMax = +1000;

			// Set the range for the axis
			if (FAILED(v_joystick->m_direct_joystick->SetProperty(DIPROP_RANGE, &diprg.diph)))
				return DIENUM_STOP;

		}
		return DIENUM_CONTINUE;
	}
}