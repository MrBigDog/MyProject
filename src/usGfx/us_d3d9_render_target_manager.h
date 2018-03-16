///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_target_manager.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_target_manager class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_RENDER_TARGET_MANAGER_H_
#define _US_D3D9_RENDER_TARGET_MANAGER_H_
#include <usGfx/Export.h>
#include <usUtil/us_common_file.h>

#include <vector>
#include <basetsd.h>

namespace uniscope_globe
{

#define US_RENDER_TARGET_TERRAIN				0
#define US_RENDER_TARGET_COLOR_ADJUSTMENT		1
#define US_RENDER_TARGET_POST_PROCESS			2
#define US_RENDER_TARGET_REFLECT				3
#define US_RENDER_TARGET_OFFSCREEN				4
#define US_RENDER_TARGET_REFRACT				5
#define US_RENDER_TARGET_COUNT					6

	class render_target;
	class render_device;

	class USGFX_EXPORT d3d9_render_target_manager
	{
	protected:
		typedef std::vector<render_target*> render_target_array;

	protected:
		d3d9_render_target_manager(void) {}

	public:
		d3d9_render_target_manager(render_device* rd);

		virtual ~d3d9_render_target_manager(void);

	public:
		ULONG_PTR get_render_target(long rt_handle);

		ulong add_render_target(render_target* v_render_target);

		long on_lost_device(void);

		long on_reset_device(void);

		void delete_render_target(ulong rt_handle);

	protected:
		render_target_array m_render_target_array;

		render_device* m_device;


	};


}

#endif // _US_D3D9_RENDER_TARGET_MANAGER_H_ 