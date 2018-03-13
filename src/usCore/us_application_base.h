///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_application_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : application_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_APPLICATION_BASE_H_
#define _US_APPLICATION_BASE_H_

#include <usCore\Export.h>
#include <usUtil\us_object_base.h>
#include <usCore\us_view_base.h>
#include <usCore\us_common_manager_group.h>
#include <usCore\us_render_manager_base.h>
#include <wtypes.h>

namespace uniscope_globe
{
	class document_base;

	class USCORE_EXPORT application_base
	{
	protected:
		application_base() {}

	public:
		application_base(HWND hwnd, HWND parent_hwnd, HINSTANCE hinstance);

		virtual ~application_base(void);

	public:
		virtual bool initialize(void) { return true; }

		virtual bool finalize(void) { return true; }

		HWND get_hwnd(void) { return m_hwnd; }

		HWND get_parent_hwnd(void) { return m_parent_hwnd; }

		common_manager_group* get_manager_group(void) { return m_manager_group; }

		render_manager_base* get_render_manager(void) { return m_view->get_render_manager(); }

		virtual object_base* get_animation_manager(void) { return NULL; }

	protected:
		document_base* m_document;

		view_base* m_view;

		HWND		m_hwnd;
		HWND		m_parent_hwnd;
		HINSTANCE	m_hinstance;
	public:
		common_manager_group* m_manager_group;
	};
}

#endif // _US_APPLICATION_BASE_H_