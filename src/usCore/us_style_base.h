///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_style.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_STYLE_H_
#define _US_RENDER_STYLE_H_

#include <vector>

#include <usCore\Export.h>
#include <usUtil\us_object_base.h>
#include <usCore\us_event_base.h>

namespace uniscope_globe
{
	enum style_type
	{
		mark_style = 0x00,
		line_style = 0x01,
		fill_style = 0x02,
		mat_style = 0x03
	};

	class styling_object;

	class USCORE_EXPORT style_base : public object_base
	{
	public:
		style_base(void);
		~style_base(void);

		typedef std::vector<style_base*> style_base_array;

	public:
		event_base event_on_style_changed;

	public:
		virtual void Initialize(styling_object* v_styling_obj);

	public:
		style_base_array m_style_array;

		style_type m_style_type;
	};
}

#endif // _US_RENDER_STYLE_H_