///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_input_impl.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : input_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_input_impl.h"
#include "lib_core/lib_core.h"

#ifndef USE_STATIC_LIB
extern HMODULE g_hModule;
#endif

namespace uniscope_globe
{
	input_impl* singleton_input_impl::s_ins = NULL;

#ifdef WINM64
	///add by felix
	DWORD tls_singleton_input_impl::m_tls_index = TlsAlloc();
#endif

	input_impl::input_impl( void )
	{
		//m_keyboard_device = NULL;
		//m_mouse_device    = NULL;
		//m_direct_input    = NULL;
		//m_inited        = false;
		m_ref_count       = 0;

		m_pending_refresh = false;
	}

	input_impl::~input_impl()
	{
		destroy();
	}

	void input_impl::create( HWND hwnd, HWND hwnd_parent, HINSTANCE hinstance )
	{
		m_ref_count++;
		if( m_ref_count > 1 ) return;
		//if( m_inited )
		//	return;

		m_windows_handle		= hwnd;
		m_parent_window_handle	= hwnd_parent;
		m_proc_instance			= hinstance;

		HRESULT hr;   

		if( FAILED( hr = DirectInput8Create(	m_proc_instance, 
												DIRECTINPUT_VERSION,
												IID_IDirectInput8,
												(VOID**)&m_direct_input,
												NULL ) ) )
		{
			return;
		}		

		m_pending_refresh = true;
		
		// create hook
#ifndef USE_STATIC_LIB
		m_hook_handle = SetWindowsHookEx( WH_GETMESSAGE, get_windows_procedure, g_hModule, 0 );
#else
		m_hook_handle = SetWindowsHookEx( WH_GETMESSAGE, get_windows_procedure, m_proc_instance, 0/*::GetCurrentThreadId()*/ );
#endif

	}

	void input_impl::destroy( void )
	{
		m_ref_count--;
		if( m_ref_count >= 1 ) return;

		//m_inited = false;

		//
		// 释放钩子
		//
		if ( NULL != m_hook_handle ) {
			UnhookWindowsHookEx( m_hook_handle );
			m_hook_handle = NULL;
		}

		unregister_all_device();

		AUTO_RELEASE( m_direct_input );
	}

	void input_impl::probe_device( void )
	{
		probe_keyboard();

		probe_mouse();

		probe_joystick();
	}

	void input_impl::probe_keyboard()
	{
		IDirectInputDevice8* v_direct_keyboard = NULL;

		if( FAILED( m_direct_input->CreateDevice( GUID_SysKeyboard, &v_direct_keyboard, NULL ) ) )
			return;

		keyboard_device_impl* v_keyboard_device = new keyboard_device_impl(v_direct_keyboard);
		if( v_keyboard_device->create( &m_device_argument ) )
		{
			register_input_device( L"keyboard", v_keyboard_device );
		}
		else
		{
			AUTO_DELETE( v_keyboard_device );
		}
	}

	void input_impl::probe_mouse()
	{
		IDirectInputDevice8* v_direct_mouse = NULL;

		if ( FAILED( m_direct_input->CreateDevice( GUID_SysMouse, &v_direct_mouse, NULL ) ) )
			return;

		mouse_device_impl* v_mouse_device = new mouse_device_impl(v_direct_mouse);
		if( v_mouse_device->create( &m_mouse_argument ) )
		{
			register_input_device( L"mouse", v_mouse_device );
		}
		else
		{
			AUTO_DELETE( v_mouse_device );
		}
		

	}

	void input_impl::probe_joystick()
	{	
		if( FAILED( m_direct_input->EnumDevices( DI8DEVCLASS_GAMECTRL, enum_joysticks_callback, this, DIEDFL_ATTACHEDONLY ) ) )
			return;
	}

	BOOL input_impl::enum_joysticks_callback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
	{

		//
		// 判断设备是否已经存在
		//
		input_impl* v_input_impl = (input_impl*)pContext;

		UUID id = pdidInstance->guidInstance;
		RPC_WSTR wsGuid = NULL;
		UuidToString( &id, &wsGuid );
		
		if( v_input_impl->is_exist( (cpstr)wsGuid ) )
		{
			RpcStringFree( &wsGuid );
			return DIENUM_CONTINUE;
		}				

		IDirectInputDevice8* v_direct_joystick = NULL;
		if ( FAILED( v_input_impl->m_direct_input->CreateDevice( pdidInstance->guidInstance, &v_direct_joystick, NULL ) ) )
		{				
			return DIENUM_CONTINUE;
		}

		DIDEVICEINSTANCE devInfo = { 0 };
		devInfo.dwSize = sizeof( DIDEVICEINSTANCE );

		v_direct_joystick->GetDeviceInfo( &devInfo );

		if ( devInfo.dwDevType == ( DIDEVTYPE_HID | DI8DEVTYPEJOYSTICK_STANDARD << 8 | DI8DEVTYPE_JOYSTICK ) )
		{
			// joystick
			joystick_device_impl* v_joystick_device = new joystick_device_impl( v_direct_joystick );
			bool ret = false;
			if( v_joystick_device->create( &v_input_impl->m_device_argument ) )
			{
				v_input_impl->register_input_device( (const LPCTSTR)wsGuid, v_joystick_device );				
			}
			else
			{
				AUTO_DELETE( v_joystick_device );
				AUTO_RELEASE( v_direct_joystick );
			}
		}
		else if( devInfo.dwDevType == ( DIDEVTYPE_HID | DI8DEVTYPE1STPERSON_LIMITED << 8 | DI8DEVTYPE_1STPERSON ) )
		{
			mouse_3d_device_impl* v_mouse_3d_device = new mouse_3d_device_impl( v_direct_joystick );
			if( v_mouse_3d_device->create( &v_input_impl->m_device_argument ) )
			{
				v_input_impl->register_input_device( (const LPCTSTR)wsGuid, v_mouse_3d_device );
			}
			else
			{
				AUTO_DELETE( v_mouse_3d_device );
				AUTO_RELEASE( v_direct_joystick );
			}
		}

		RpcStringFree( &wsGuid );

		//*(GUID*) pContext = pdidInstance->guidInstance;
		return DIENUM_CONTINUE; 
		/*
		DI_ENUM_CONTEXT* pEnumContext = (DI_ENUM_CONTEXT*) pContext;
		HRESULT hr;

		// Skip anything other than the perferred joystick device as defined by the control panel.  
		// Instead you could store all the enumerated joysticks and let the user pick.
		if( pEnumContext->bPreferredJoyCfgValid && !IsEqualGUID( pdidInstance->guidInstance, pEnumContext->pPreferredJoyCfg->guidInstance ) )
		return DIENUM_CONTINUE;

		// Obtain an interface to the enumerated joystick.
		if( FAILED(v_joystick->m_direct_input->CreateDevice( pdidInstance->guidInstance, &v_joystick->m_direct_joystick, NULL )))
		{
		return DIENUM_CONTINUE;
		}

		// Stop enumeration. Note: we're just taking the first joystick we get. You
		// could store all the enumerated joysticks and let the user pick.
		return DIENUM_STOP;

		//*/
	}

	void input_impl::refresh_device( void )
	{
		m_pending_refresh = true;
	}

	bool input_impl::is_exist( cpstr device_name )
	{
		input_device_map::iterator itr = m_input_device_map.find( device_name );
		if( itr != m_input_device_map.end() )
			return true;

		return false;
	}

	bool input_impl::register_input_device( const LPCTSTR device_name, input_device_base* input_device )
	{
		input_device_map::iterator itr = m_input_device_map.find( device_name );
		if( itr != m_input_device_map.end() )
			return false;

		m_input_device_map.insert( make_pair( device_name, input_device) );

		return true;
	}

	void input_impl::unregister_input_device( const LPCTSTR device_name )
	{
		m_input_device_map.erase( device_name );
	}

	void input_impl::unregister_all_device( void )
	{
		input_device_map::iterator itr;
		for ( itr = m_input_device_map.begin(); itr != m_input_device_map.end(); itr++)
		{
			input_device_base *v_input_device = itr->second;

			AUTO_DELETE(v_input_device);
		}

		m_input_device_map.clear();
	}

	void input_impl::update( HWND hwnd )
	{
		//if( !m_inited ) return;
		if( m_ref_count < 1 ) return;

		if( m_pending_refresh )
		{
			unregister_all_device();

			probe_device();

			m_pending_refresh = false;
		}
		
		m_device_argument.reset();
		m_mouse_argument.reset();

		m_device_argument.m_hwnd = hwnd;
		m_mouse_argument.m_hwnd = hwnd;


		input_device_map::iterator itr;
		for ( itr = m_input_device_map.begin(); itr != m_input_device_map.end(); itr++)
		{
			input_device_base *v_input_device = itr->second;

			v_input_device->refresh();
		}

		//US_TRACE1( L"X Move: %lf\n", m_device_argument.m_state.x_move );
		//US_TRACE1( L"Y Move: %lf\n", m_device_argument.m_state.y_move );
		//US_TRACE1( L"Z Move: %lf\n", m_device_argument.m_state.z_move );
		//US_TRACE1( L"X rotation: %lf\n", m_device_argument.m_state.x_rot );
		//US_TRACE1( L"Y rotation: %lf\n", m_device_argument.m_state.y_rot );
		//US_TRACE1( L"Z rotation: %lf\n", m_device_argument.m_state.z_rot );


		m_input_event( (event_argument*)&m_device_argument );
		m_input_event( (event_argument*)&m_mouse_argument );
	}

	LRESULT CALLBACK input_impl::get_windows_procedure(int code, WPARAM w_param, LPARAM l_param )
	{
#ifdef WINM64
		
		input_impl* v_input = tls_singleton_input_impl::ptr();
#else
		input_impl* v_input = singleton_input_impl::ptr();
#endif
		
		MSG* msg = (MSG*)l_param;
		// do not process message 
		if ( code != HC_ACTION/* || v_input->m_windows_handle != msg->hwnd*/ ) 
		{
			return CallNextHookEx( v_input->m_hook_handle, code, w_param, l_param );	
		}


	


		if ( NULL != msg/* && msg->hwnd == v_input->m_windows_handle*/ && w_param == PM_REMOVE ) 
		{
			message_argument win_msg_args;

			win_msg_args.reset();
			win_msg_args.m_message	= msg->message;
			win_msg_args.m_w_param	= msg->wParam;
			win_msg_args.m_l_param	= msg->lParam;
			win_msg_args.m_hwnd		= msg->hwnd;
			win_msg_args.m_hook		= v_input->m_hook_handle;



			//if( win_msg_args.m_message == WM_LBUTTONUP )
			//{
			//	US_TRACE0( L"WM_LBUTTONUP\n" );
			//}

			if( win_msg_args.m_message == WM_DEVICECHANGE )
			{
				//US_TRACE1( L"Device changed: %d\n", win_msg_args.m_w_param );

				v_input->refresh_device();
			}

			v_input->m_input_event( (event_argument*)&win_msg_args );
			
		}

		return CallNextHookEx( v_input->m_hook_handle, code, w_param, l_param ); 
	}	
}