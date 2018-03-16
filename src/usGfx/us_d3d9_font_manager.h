///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_font_manager.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_font_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_FONT_MANAGER_H_
#define _US_D3D9_FONT_MANAGER_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_render_font.h>
#include <usUtil/us_common_file.h>

#include <vector>
#include <basetsd.h>

namespace uniscope_globe
{
	class render_device;

	class USGFX_EXPORT d3d9_font_manager
	{
	protected:
		typedef std::vector<d3d9_render_font*> render_font_array;

	protected:
		d3d9_font_manager(void) {}

	public:
		d3d9_font_manager(render_device* rd);

		virtual ~d3d9_font_manager(void);

	public:
		ULONG_PTR get_font(long font_handle);

		long on_lost_device(void);

		long on_reset_device(void);

	protected:
		render_font_array m_font_array;
	};


}

#endif // _US_D3D9_FONT_MANAGER_H_