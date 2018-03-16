///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_hardware_occlusion_query.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_hardware_occlusion_query class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_hardware_occlusion_query.h"

namespace uniscope_globe
{
	d3d9_hardware_occlusion_query::d3d9_hardware_occlusion_query(IDirect3DDevice9* device)
		: m_device(device)
	{
		// Create the query
		m_device->CreateQuery(D3DQUERYTYPE_OCCLUSION, &m_query);

		// Get the display mode to obtain the format
		D3DDISPLAYMODE mode;
		m_device->GetDisplayMode(0, &mode);

		// Create the texture first, so we can get access to it's surface
		if (FAILED(D3DXCreateTexture(m_device, 320, 240, 1, D3DUSAGE_RENDERTARGET, mode.Format, D3DPOOL_DEFAULT, &m_occlusion_texture)))
		{
			return;
		}

		// Obtain the surface (what we really need)
		D3DSURFACE_DESC desc;
		m_occlusion_texture->GetSurfaceLevel(0, &m_occlusion_surface);
		m_occlusion_surface->GetDesc(&desc);

		// Create the render to surface
		if (FAILED(D3DXCreateRenderToSurface(m_device, desc.Width, desc.Height, desc.Format, TRUE, D3DFMT_D24S8, &m_occlusion_render)))
		{
			return;
		}

		return;
	}

	d3d9_hardware_occlusion_query::~d3d9_hardware_occlusion_query(void)
	{
		AUTO_RELEASE(m_query);
	}

	void d3d9_hardware_occlusion_query::begin_occlusion_query(void)
	{
		HRESULT hr = m_occlusion_render->BeginScene(m_occlusion_surface, NULL);
		if (SUCCEEDED(hr))
		{
			// Clear the occlusionRender's surface
			m_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(200, 200, 200), 1.0f, 0);
		}
	}

	void d3d9_hardware_occlusion_query::end_occlusion_query(void)
	{
		m_occlusion_render->EndScene(0);
	}

	void d3d9_hardware_occlusion_query::issue_begin(void)
	{
		m_query->Issue(D3DISSUE_BEGIN);
	}

	void d3d9_hardware_occlusion_query::issue_end(void)
	{
		m_query->Issue(D3DISSUE_END);
	}

	bool d3d9_hardware_occlusion_query::pull_occlusion_query(uint* num_of_pixel)
	{
		// Loop until the data becomes available
		DWORD pixels;
		const size_t dataSize = sizeof(DWORD);
		while (1)
		{
			const HRESULT hr = m_query->GetData((void *)&pixels, dataSize, D3DGETDATA_FLUSH);

			if (hr == S_FALSE) continue;
			if (hr == S_OK)
			{
				*num_of_pixel = pixels;
				break;
			}
			if (hr == D3DERR_DEVICELOST)
			{
				*num_of_pixel = 100000;
				m_device->CreateQuery(D3DQUERYTYPE_OCCLUSION, &m_query);
				break;
			}
		}
		return true;
	}
	//------------------------------------------------------------------
	bool d3d9_hardware_occlusion_query::is_still_outstanding(void)
	{
		// in case you already asked for this query
		if (!m_is_query_result_still_outstanding)
			return false;

		DWORD pixels;
		const HRESULT hr = m_query->GetData((void *)&pixels, sizeof(DWORD), D3DGETDATA_FLUSH);

		if (hr == S_FALSE)
			return true;

		if (hr == D3DERR_DEVICELOST)
		{
			m_pixel_count = 100000;
			m_device->CreateQuery(D3DQUERYTYPE_OCCLUSION, &m_query);
		}
		m_pixel_count = pixels;
		m_is_query_result_still_outstanding = false;
		return false;
	}

	long d3d9_hardware_occlusion_query::on_lost_device(void)
	{
		m_occlusion_render->OnLostDevice();

		return S_OK;
	}

	long d3d9_hardware_occlusion_query::on_reset_device(void)
	{
		m_occlusion_render->OnResetDevice();
		return S_OK;
	}
}