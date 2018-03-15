///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_target.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_target class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_TARGET_H_
#define _US_RENDER_TARGET_H_
#include <usCore\Export.h>
#include <usUtil\us_common_file.h>
#include <basetsd.h>
namespace uniscope_globe
{
	class event_argument;
	class USCORE_EXPORT render_target
	{
	public:
		render_target(void);

		virtual ~render_target(void);

	public:
		virtual void on_lost_device(event_argument* args) {}

		virtual void on_reset_device(event_argument* args) {}

		virtual void begin(void) = 0;

		virtual void end(void) = 0;

		virtual ULONG_PTR get_texture(void) = 0;

		virtual void draw_texture(int stage) = 0;

		virtual void save_file(cpstr str_path, int width, int height) {}

	};
}

#endif // _US_RENDER_TARGET_H_ 