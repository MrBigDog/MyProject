///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_font.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_font class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_RENDER_FONT_H_
#define _US_D3D9_RENDER_FONT_H_

#include <usGfx/Export.h>
//#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	class USGFX_EXPORT d3d9_render_font
	{
	public:
		d3d9_render_font(void);

		virtual ~d3d9_render_font(void);

	public:
		long on_lost_device(void);

		long on_reset_device(void);

	};
}

#endif // _US_D3D9_RENDER_FONT_H_