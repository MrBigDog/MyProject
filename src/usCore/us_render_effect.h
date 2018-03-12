//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_effect.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_effect class
//	Reference : 
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_EFFECT_H_
#define _US_RENDER_EFFECT_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT render_effect
	{
	protected:
		render_effect(void) {}

	public:
		virtual ~render_effect(void) {}

	public:
		virtual bool load_from_file(const LPCTSTR technique_file) = 0;

		virtual bool load_from_module_resource(ULONG_PTR resource_handle) = 0;

		virtual long on_lost_device(void) = 0;

		virtual long on_reset_device(void) = 0;

		virtual bool begin(uint* pass, ulong flags) = 0;

		virtual bool end(void) = 0;

		virtual bool begin_pass(uint pass) = 0;

		virtual bool end_pass(void) = 0;

		virtual bool set_technique(void) = 0;

		virtual bool commit_changes(void) = 0;

		render_device* get_device(void) { return m_device; }

	protected:
		render_device* m_device;
	};
}

#endif // _US_RENDER_EFFECT_H_
