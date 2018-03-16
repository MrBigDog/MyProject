///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_target.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_target class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_render_target.h"
#include <usGfx/us_d3d9_render_device_impl.h>
#include <usCore/us_render_device.h>
#include <usUtil/us_file_directory.h>

namespace uniscope_globe
{
	d3d9_render_target::d3d9_render_target(render_device* rd)
	{
		m_device = rd;
		m_d3d_device = (LPDIRECT3DDEVICE9)(rd->get_device());
		//m_device->m_event_reset_device += event_handle(this, &d3d9_render_target::on_reset_device );
		//m_device->m_event_lost_device += event_handle(this, &d3d9_render_target::on_lost_device );
		m_old_target_surface = NULL;
		m_surface = NULL;
		m_texture = NULL;
	}

	d3d9_render_target::~d3d9_render_target(void)
	{
		m_device->m_event_reset_device -= event_handle(this, &d3d9_render_target::on_reset_device);
		m_device->m_event_lost_device -= event_handle(this, &d3d9_render_target::on_lost_device);
		destroy();

	}

	bool d3d9_render_target::create(uint width, uint height, int format, int multi_sample_type, ulong multi_sample_quality, bool lockable)
	{
		AUTO_RELEASE(m_surface);
		AUTO_RELEASE(m_texture);
		HRESULT hr;
		if (FAILED(hr = m_d3d_device->CreateRenderTarget(width, height, (D3DFORMAT)format, (D3DMULTISAMPLE_TYPE)multi_sample_type, multi_sample_quality, lockable, &m_surface, NULL)))
		{
			return false;
		}

		if (FAILED(D3DXCreateTexture(m_d3d_device, width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_texture)))
		{
			return false;
		}
		return true;
	}

	void d3d9_render_target::destroy(void)
	{
		AUTO_RELEASE(m_surface);
		AUTO_RELEASE(m_texture);
	}

	void d3d9_render_target::on_lost_device(event_argument* args)
	{
		this->destroy();
	}

	void d3d9_render_target::on_reset_device(event_argument* args)
	{
		d3d9_render_device_impl* v_device = (d3d9_render_device_impl*)m_device;
		this->create(v_device->get_backbuffer_width(), v_device->get_backbuffer_height(), v_device->get_backbuffer_format(), v_device->get_backbuffer_multisampletype(), v_device->get_backbuffer_multisamplequality());
	}

	void d3d9_render_target::begin(void)
	{
		m_d3d_device->GetRenderTarget(0, &m_old_target_surface);
		m_d3d_device->SetRenderTarget(0, m_surface);
	}

	void d3d9_render_target::end(void)
	{
		LPDIRECT3DSURFACE9	v_target = NULL;
		m_texture->GetSurfaceLevel(0, &v_target);
		m_d3d_device->StretchRect(m_surface, NULL, v_target, NULL, D3DTEXF_NONE);
		m_d3d_device->SetRenderTarget(0, m_old_target_surface);
		AUTO_RELEASE(v_target);
		AUTO_RELEASE(m_old_target_surface);
	}

	ULONG_PTR d3d9_render_target::get_texture(void)
	{
		return (ULONG_PTR)m_texture;
	}

	void d3d9_render_target::draw_texture(int stage)
	{
		m_d3d_device->SetTexture(stage, m_texture);
	}

	void d3d9_render_target::save_file(cpstr str_path, int width, int height)
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

		D3DXSaveTextureToFile(str_path, D3DXIMAGE_FILEFORMAT(v_file_type), m_texture, NULL);
	}


}