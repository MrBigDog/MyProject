///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mouse_device_impl.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mouse_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_mouse_device_impl.h"

namespace uniscope_globe
{
	mouse_device_impl::mouse_device_impl(IDirectInputDevice8* in_direct_mouse)
	{
		m_direct_mouse = in_direct_mouse;
	}

	mouse_device_impl::~mouse_device_impl(void)
	{
		destroy();
	}

	bool mouse_device_impl::create( mouse_argument* args )
	{
		HRESULT hr;

		if( FAILED( hr = m_direct_mouse->SetDataFormat( &c_dfDIMouse2 ) ) )
			return false;
		//if( FAILED( hr = m_direct_mouse->SetCooperativeLevel( m_hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
		//	return false;

		hr = m_direct_mouse->Acquire();

		memset( (void*)&m_cur_state, 0, sizeof(mouse_state) );

		m_args = args;

		return true;
	}

	void mouse_device_impl::destroy( void )
	{
		if (m_direct_mouse)
		{
			m_direct_mouse->Unacquire();
		}

		AUTO_RELEASE( m_direct_mouse );
	}

	void mouse_device_impl::collect_mouse_info( void )
	{
		HRESULT hr;
		hr = m_direct_mouse->GetDeviceState( sizeof(DIMOUSESTATE2), &m_cur_state );
		if( FAILED(hr) ) 
		{

			hr = m_direct_mouse->Acquire();
			while( hr == DIERR_INPUTLOST )
			{
				hr = m_direct_mouse->Acquire();
			}
			return ; 
		}
	}

	// refresh
	bool mouse_device_impl::refresh( void )
	{
		collect_mouse_info();

		memcpy( (void *)&m_args->m_state, (void* )&m_cur_state, sizeof( mouse_state ) );

		return true;
	}

	
}