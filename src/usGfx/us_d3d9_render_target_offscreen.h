
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_target_offscreen.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_target_offscreen class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_RENDER_TARGET_OFFSCREEN_H_
#define _US_D3D9_RENDER_TARGET_OFFSCREEN_H_
#include <usGfx/Export.h>
#include <usCore/us_render_target.h>
#include <usUtil/us_common_file.h>

#include <d3d9.h>

namespace uniscope_globe
{
	class render_device;

	class USGFX_EXPORT d3d9_render_target_offscreen
		: public render_target
	{
	protected:
		d3d9_render_target_offscreen(void);

	public:
		d3d9_render_target_offscreen(render_device* rd);

		virtual ~d3d9_render_target_offscreen(void);

	public:
		bool create(uint width, uint height, int format, int multi_sample_type, ulong multi_sample_quality, bool lockable = false);

		void destroy(void);

	public:
		virtual void on_lost_device(event_argument* args);

		virtual void on_reset_device(event_argument* args);

		virtual void begin(void);

		virtual void end(void);

		virtual ULONG_PTR get_texture(void);

		virtual void draw_texture(int stage);

		virtual void get_surface_bits(LPVOID pBits);

		virtual void save_file(cpstr str_path, int width, int height);

	protected:
		// Returns BitsPerPixel form D3DFORMAT
		inline int BPPFromD3DFormat(D3DFORMAT fmt)
		{
			switch (fmt)
			{
			case D3DFMT_A2R10G10B10: return 32;
			case D3DFMT_A8R8G8B8: return 32;
			case D3DFMT_X8R8G8B8: return 32;
			case D3DFMT_R8G8B8:	return 24;
			case D3DFMT_R5G6B5: return 16;
			case D3DFMT_X1R5G5B5: return 16;
			case D3DFMT_A1R5G5B5: return 16;
			case D3DFMT_A4R4G4B4: return 16;
			case D3DFMT_X4R4G4B4: return 16;
			default: return 0;
			}
		}

	public:
		LPDIRECT3DDEVICE9		m_d3d_device;

		render_device*			m_device;

		LPDIRECT3DSURFACE9		m_render_target_depth_buffer;
		LPDIRECT3DSURFACE9		m_render_target;			//The Surface Onto which the scene would be Blit
		LPDIRECT3DTEXTURE9		m_render_target_texture;		//The Render Target is a Surface of this Texture
		LPDIRECT3DSURFACE9		m_offscreen_plain_surface;	//This is useful to get the render target data 

		LPDIRECT3DSURFACE9		m_old_back_buffer;			//Original back Buffer
		LPDIRECT3DSURFACE9		m_old_depth_buffer;			//Original Depth Buffer

		int						m_width;
		int						m_height;
		int				m_format;



	};
}

#endif // _US_D3D9_RENDER_TARGET_OFFSCREEN_H_ 