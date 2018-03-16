///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_input_impl.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : input_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_INPUT_IMPL_H_
#define _US_INPUT_IMPL_H_
#include <usGfx/Export.h>
#include <usGfx/us_mouse_argument.h>
#include <usGfx/us_input_device_base.h>
#include <usCore/us_event_base.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_singleton.h>

#include <dinput.h>
#include <map>

namespace uniscope_globe
{
	class input_device_base;

	class USGFX_EXPORT input_impl
		//#ifdef WIN64
		//		: public input_base
		//#endif
	{
		typedef std::map<ustring, input_device_base*> input_device_map;
	public:
		input_impl(void);

		virtual ~input_impl(void);

	public:
		void create(HWND hwnd, HWND hwnd_parent, HINSTANCE hinstance);

		void destroy(void);

	public:
		static BOOL CALLBACK enum_joysticks_callback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);

	public:
		virtual void update(HWND hwnd);

		//virtual void set_exclusive_mode( bool enable );

		void refresh_device(void);

		bool is_exist(cpstr device_name);

		virtual bool register_input_device(const LPCTSTR device_name, input_device_base* input_device);

		virtual void unregister_input_device(const LPCTSTR device_name);

		void unregister_all_device(void);

		static LRESULT CALLBACK get_windows_procedure(int code, WPARAM w_param, LPARAM l_param);

	protected:
		void probe_device(void);

		void probe_keyboard(void);

		void probe_mouse(void);

		void probe_joystick(void);

	public:
		event_base	m_input_event;

	protected:
		HINSTANCE m_proc_instance;

		HWND	  m_windows_handle;

		HWND	  m_parent_window_handle;

		HHOOK     m_hook_handle;


		IDirectInput8*			m_direct_input;

		//keyboard_device_impl*	m_keyboard_device;

		//joystick_device_impl*	m_joystick_device;

		//mouse_device_impl*	m_mouse_device;

		input_device_map		m_input_device_map;

		//bool					m_inited;

		int						m_ref_count;

		input_device_argument m_device_argument;

		mouse_argument m_mouse_argument;

		bool  m_pending_refresh;
	};

	class USGFX_EXPORT singleton_input_impl
		: public singleton<input_impl>
	{
	public:
		singleton_input_impl(void)
		{
		}

		virtual ~singleton_input_impl(void)
		{
		}
	};

#ifdef WIN64
	///add by felix
	class USGFX_EXPORT tls_singleton_input_impl : public tls_singleton<input_impl>
	{
	};

#endif

}

#endif // _US_INPUT_IMPL_H_