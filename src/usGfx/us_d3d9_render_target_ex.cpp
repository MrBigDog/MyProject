
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_target_ex.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_target_ex class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_render_target_ex.h"
#include <usGfx/us_d3d9_render_device_impl.h>
#include <usCore/us_render_device.h>
#include <usUtil/us_file_directory.h>

namespace uniscope_globe
{
#define REFL_REFR_DETAIL 512

	d3d9_render_target_ex::d3d9_render_target_ex(render_device* rd)
	{
		m_device = rd;
		m_d3d_device = (LPDIRECT3DDEVICE9)(rd->get_device());
		m_device->m_event_reset_device += event_handle(this, &d3d9_render_target_ex::on_reset_device);
		m_device->m_event_lost_device += event_handle(this, &d3d9_render_target_ex::on_lost_device);
		m_old_backbuffer_surface = NULL;
		m_old_depth_surface = NULL;
		m_depth_surface = NULL;
		m_backbuffer_texture = NULL;
	}

	d3d9_render_target_ex::~d3d9_render_target_ex(void)
	{
		m_device->m_event_reset_device -= event_handle(this, &d3d9_render_target_ex::on_reset_device);
		m_device->m_event_lost_device -= event_handle(this, &d3d9_render_target_ex::on_lost_device);
		destroy();

	}

	bool d3d9_render_target_ex::create(uint width, uint height, int format, int multi_sample_type, ulong multi_sample_quality, bool lockable)
	{
		AUTO_RELEASE(m_depth_surface);
		AUTO_RELEASE(m_backbuffer_texture);

		if (FAILED(m_d3d_device->CreateTexture(REFL_REFR_DETAIL, REFL_REFR_DETAIL, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_backbuffer_texture, NULL)))
		{
			return false;
		}

		if (FAILED(m_d3d_device->CreateDepthStencilSurface(REFL_REFR_DETAIL, REFL_REFR_DETAIL, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, true, &m_depth_surface, NULL)))
		{
			return false;
		}
		return true;
	}

	void d3d9_render_target_ex::destroy(void)
	{
		AUTO_RELEASE(m_depth_surface);
		AUTO_RELEASE(m_backbuffer_texture);
	}

	void d3d9_render_target_ex::on_lost_device(event_argument* args)
	{
		this->destroy();
	}

	void d3d9_render_target_ex::on_reset_device(event_argument* args)
	{
		d3d9_render_device_impl* v_device = (d3d9_render_device_impl*)m_device;
		this->create(v_device->get_backbuffer_width(), v_device->get_backbuffer_height(), v_device->get_backbuffer_format(), v_device->get_backbuffer_multisampletype(), v_device->get_backbuffer_multisamplequality());
	}

	void d3d9_render_target_ex::begin(void)
	{
		m_d3d_device->GetRenderTarget(0, &m_old_backbuffer_surface);
		m_d3d_device->GetDepthStencilSurface(&m_old_depth_surface);


		LPDIRECT3DSURFACE9 v_backbuffer_surface;
		m_backbuffer_texture->GetSurfaceLevel(0, &v_backbuffer_surface);			//Link Texture to Surface;
		m_d3d_device->SetRenderTarget(0, v_backbuffer_surface);					//Set render target to Surface "lpsurfTarget" 
		m_d3d_device->SetDepthStencilSurface(m_depth_surface);
		AUTO_RELEASE(v_backbuffer_surface);

		//AUTO_RELEASE( v_surf_target );
	}

	void d3d9_render_target_ex::end(void)
	{
		//LPDIRECT3DSURFACE9	v_target = NULL;
		//m_texture->GetSurfaceLevel(0, &v_target);
		//m_d3d_device->StretchRect( m_surface, NULL, v_target, NULL, D3DTEXF_NONE );
		m_d3d_device->SetRenderTarget(0, m_old_backbuffer_surface);
		m_d3d_device->SetDepthStencilSurface(m_old_depth_surface);
		AUTO_RELEASE(m_old_backbuffer_surface);
		AUTO_RELEASE(m_old_depth_surface);
	}

	ULONG_PTR d3d9_render_target_ex::get_texture(void)
	{
		return (ULONG_PTR)m_backbuffer_texture;
	}

	void d3d9_render_target_ex::draw_texture(int stage)
	{
		m_d3d_device->SetTexture(stage, m_backbuffer_texture);
	}

	void d3d9_render_target_ex::save_file(cpstr str_path, int width, int height)
	{
		int v_file_type = D3DXIFF_JPG;
		ustring str_ext = file_directory::get_file_ext(str_path);
		US_STRING_LOWER(str_ext);
		if (str_ext == L"jpg")
		{
			v_file_type = D3DXIFF_JPG;
		}
		if (str_ext == L"bmp")
		{
			v_file_type = D3DXIFF_BMP;
		}
		if (str_ext == L"png")
		{
			v_file_type = D3DXIFF_PNG;
		}

		D3DXSaveTextureToFile(str_path, D3DXIMAGE_FILEFORMAT(v_file_type), m_backbuffer_texture, NULL);
	}


}