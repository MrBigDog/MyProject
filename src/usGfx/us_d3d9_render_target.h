///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_target.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_target class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_RENDER_TARGET_H_
#define _US_D3D9_RENDER_TARGET_H_
#include <usGfx/Export.h>
#include <usCore/us_render_target.h>
#include <usUtil/us_common_file.h>

#include <d3d9.h>

namespace uniscope_globe
{
	class render_device;

	class USGFX_EXPORT d3d9_render_target
		: public render_target
	{
	protected:
		d3d9_render_target(void);

	public:
		d3d9_render_target(render_device* rd);

		virtual ~d3d9_render_target(void);


	public:
		bool create(uint width, uint height, int format, int multi_sample_type, ulong multi_sample_quality, bool lockable = false);

		//bool create_offline( uint width, uint height, int format, int multi_sample_type, ulong multi_sample_quality, bool lockable );

		void destroy(void);

	public:
		virtual void on_lost_device(event_argument* args);

		virtual void on_reset_device(event_argument* args);

		virtual void begin(void);

		virtual void end(void);

		virtual ULONG_PTR get_texture(void);

		virtual void draw_texture(int stage);

		virtual void save_file(cpstr str_path, int width, int height);

	public:
		LPDIRECT3DTEXTURE9		m_texture;

		LPDIRECT3DSURFACE9		m_surface;

		LPDIRECT3DDEVICE9		m_d3d_device;

		LPDIRECT3DSURFACE9		m_old_target_surface;

		render_device*			m_device;

	};
}

#endif // _US_D3D9_RENDER_TARGET_H_ 