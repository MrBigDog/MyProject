///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mouse_3d_device_impl.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mouse_3d_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_mouse_3d_device_impl.h"
#include <usUtil/us_math.h>

namespace uniscope_globe
{
	mouse_3d_device_impl::mouse_3d_device_impl( IDirectInputDevice8* in_direct_mouse )
	{
		m_direct_mouse = in_direct_mouse;
	}

	mouse_3d_device_impl::~mouse_3d_device_impl( void )
	{
		destroy();
	}

	bool mouse_3d_device_impl::create( input_device_argument* args )
	{
		HRESULT hr;

		if( FAILED( hr = m_direct_mouse->SetDataFormat( &c_dfDIJoystick2 ) ) )
			return false;///????????

		if( FAILED( hr = m_direct_mouse->EnumObjects( enum_objects_callback, (VOID*)this, DIDFT_ALL ) ) )
			return false;///????????

		hr = m_direct_mouse->Acquire();

		memset( (void*)&m_last_state, 0, sizeof(joystick_state) );
		memset( (void*)&m_cur_state, 0, sizeof(joystick_state) );

		m_args = args;

		return true;
	}

	void mouse_3d_device_impl::destroy( void )
	{
		if (m_direct_mouse)
		{
			m_direct_mouse->Unacquire();
		}

		AUTO_RELEASE( m_direct_mouse );

	}

	void mouse_3d_device_impl::collect_mouse_info( void )
	{
		HRESULT     hr;
		//DIJOYSTATE2 js;					// DInput joystick state 

		if( NULL == m_direct_mouse ) return;

		hr = m_direct_mouse->Poll(); 
		if( FAILED(hr) )  
		{
			hr = m_direct_mouse->Acquire();
			while( hr == DIERR_INPUTLOST ) 
				hr = m_direct_mouse->Acquire();

			return; 
		}


		// Get the input's device state
		if( FAILED( hr = m_direct_mouse->GetDeviceState( sizeof(DIJOYSTATE2),(DIJOYSTATE2*)&m_cur_state ) ) )
			return; // The device should have been acquired during the Poll()
		
		m_args->m_state.x_move = ( m_cur_state.lX - m_last_state.lX ) * 0.01;
		m_args->m_state.y_move = ( m_cur_state.lY - m_last_state.lY ) * 0.01;
		m_args->m_state.z_move = ( m_cur_state.lZ - m_last_state.lZ ) * 0.001;
		m_args->m_state.z_move = - 0.01 * CLAMP( m_args->m_state.z_move, -1, 1 );

		m_args->m_state.x_rot = ( m_cur_state.lRx - m_last_state.lRx ) * 0.02 / 1000;
		m_args->m_state.y_rot = 0.0;
		m_args->m_state.z_rot = ( m_cur_state.lRz - m_last_state.lRz ) * 0.02 / 1000;

		NORMALIZE_RADIAN(m_args->m_state.x_rot);
		NORMALIZE_RADIAN(m_args->m_state.z_rot);
		
		memcpy( (void*)m_args->m_state.stick_buttons, (void*)m_cur_state.rgbButtons, 128 );

		memcpy( (void*)&m_last_state, (void*)&m_cur_state, sizeof(joystick_state) );	
	}

	// refresh
	bool mouse_3d_device_impl::refresh( void )
	{
		//return true;
		collect_mouse_info();

		return true;
	}	

	//-----------------------------------------------------------------------------
	// Name: EnumObjectsCallback()
	// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
	//       joystick. This function enables user interface elements for objects
	//       that are found to exist, and scales axes min/max values.
	//-----------------------------------------------------------------------------
	BOOL mouse_3d_device_impl::enum_objects_callback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
	{
		HWND hDlg = (HWND)pContext;

		static int nSliderCount = 0;  // Number of returned slider controls
		static int nPOVCount = 0;     // Number of returned POV controls

		mouse_3d_device_impl* v_joystick = (mouse_3d_device_impl*)pContext;

		// For axes that are returned, set the DIPROP_RANGE property for the
		// enumerated axis in order to scale min/max values.
		if( pdidoi->dwType & DIDFT_AXIS )
		{
			DIPROPRANGE diprg; 
			diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
			diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
			diprg.diph.dwHow        = DIPH_BYID; 
			diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
			diprg.lMin              = -1000; 
			diprg.lMax              = +1000; 

			// Set the range for the axis
			if( FAILED( v_joystick->m_direct_mouse->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
				return DIENUM_STOP;

		}
		return DIENUM_CONTINUE;
	}
}