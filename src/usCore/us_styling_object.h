///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_styling_object.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : styling_object class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_STYLING_OBJECT_H_
#define _US_STYLING_OBJECT_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT styling_object
	{
	public:
		styling_object( void );

		virtual ~styling_object( void );

	public:
		virtual void set_render_style( style_base* v_style );

		virtual style_base* get_render_style( void ) = 0;

		virtual void on_render_style_changed(style_base* v_style_base){}
		
	protected:
		style_base*	m_render_style;
	};
}

#endif // _US_STYLING_OBJECT_H_ 