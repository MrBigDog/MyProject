///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_view_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : view_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_VIEW_BASE_H_
#define _US_VIEW_BASE_H_

#include <usCore\Export.h>

namespace uniscope_globe
{
	class application_base;
	class render_device;
	class observer_base;
	class sound_device;

	class USCORE_EXPORT view_base
	{
	public:
		view_base(void);

		view_base(application_base* in_app);

		virtual ~view_base(void);

	public:
		virtual bool initialize(void) { return true; }

		virtual bool finalize(void) { return true; }

		virtual render_device* get_render_device(void) { return m_render_device; }

		virtual observer_base* get_active_observer(void) { return m_active_observer; }

		virtual void set_active_observer(observer_base* observer) { m_active_observer = observer; }

		render_manager_base* get_render_manager(void) { return m_render_manager; }

	protected:
		application_base* m_application;

		render_device* m_render_device;

		sound_device*  m_sound_device;

		observer_base*	m_active_observer;

		render_manager_base* m_render_manager;
	};
}

#endif // _US_VIEW_BASE_H_