///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_device_impl.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_render_device_impl.h"
#include "resource.h"
#include <usGfx/us_d3d9_effect_manager.h>
#include <usGfx/us_d3d9_render_target_manager.h>
#include <usGfx/us_d3d9_hardware_occlusion_query.h>

#include <usCore/us_system_environment.h>
#include <usCore/us_extended_material.h>
#include <usCore/us_hardware_texture.h>

#include <usUtil/us_raw_buffer.h>
#include <usUtil/us_string_ext.h>
#include <usUtil/us_point.h>

//#ifndef USE_STATIC_LIB
extern HMODULE g_hModule;
//#endif

namespace uniscope_globe
{
#define MAX_OCCLUSION_COUNT 40;
	d3d9_render_device_impl::d3d9_render_device_impl(HWND hwnd)
	{
		m_hwnd = hwnd;

		m_d3d_object = NULL;
		m_d3d_device = NULL;
		m_d3d_swapchain = NULL;
		m_d3d_zbuffer = NULL;
		m_d3d_sprite = NULL;
		m_d3d_line = NULL;
		m_d3d_font = NULL;
		m_d3d_font_ext = NULL;
		m_blank_texture = NULL;
		m_transparency_texture_array[0] = NULL;
		m_transparency_texture_array[1] = NULL;
		m_occlusion_query = NULL;
		//m_render_target_array.resize(2);
		//m_render_target_array[0] = NULL;
		//m_render_target_array[1] = NULL;

		m_effect_manager = NULL;
		m_render_target_manager = NULL;
		m_world_matrix.identity();
		m_view_matrix.identity();
		m_project_matrix.identity();
		m_clip_matrix.identity();


		m_batch_count = 0;
		m_render_count = 0;
		m_is_windowed = true;

		m_font_size = 1.0;

		create();

	}

	d3d9_render_device_impl::~d3d9_render_device_impl(void)
	{
		//m_d3d_device->EvictManagedResources();
	}

	bool d3d9_render_device_impl::create(void)
	{
		if (!::IsWindow(m_hwnd)) return false;

		// calculate render rect size
		RECT win_rect;
		GetWindowRect(m_hwnd, &win_rect);

		m_d3d_display_mode.Width = win_rect.right - win_rect.left;
		if (m_d3d_display_mode.Width == 0)
		{
			m_d3d_display_mode.Width = 2;
		}
		m_d3d_display_mode.Height = win_rect.bottom - win_rect.top;
		if (m_d3d_display_mode.Height == 0)
		{
			m_d3d_display_mode.Height = 2;
		}

		// create d3d9 object
		m_d3d_object = Direct3DCreate9(D3D_SDK_VERSION);
		if (m_d3d_object == NULL) return false;

		// create d3d9 device
		if (!create_device_impl()) return false;

		// refresh swapchain
		if (!reset_swapchain()) return false;

		// create d3d font
		if (!create_font_impl()) return false;

		//m_d3d9_vertex_blend_effect = new d3d9_vertex_blend_effect( this );

		// create d3d sprite
		if (!create_sprite_impl()) return false;

		if (!create_line_impl()) return false;

		if (!create_blank_texture()) return false;

		if (!create_transparency_texture()) return false;

		if (!create_effect()) return false;

		if (!create_render_target()) return false;

		//m_occlusion_query = new d3d9_hardware_occlusion_query(m_d3d_device);

		return true;
	}

	bool d3d9_render_device_impl::create_device_impl(void)
	{
		memset(&m_d3d_present_param, 0, sizeof(m_d3d_present_param));
#ifdef _PUBLISH
		m_is_windowed = true;
#else
		m_is_windowed = !system_environment::s_debug_mode;
		//m_is_windowed = false;
#endif
		// check the device caps 
		uint vertex_process_caps;
		D3DCAPS9 d3d_capabilities;

		m_d3d_object->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3d_capabilities);
		if (d3d_capabilities.VertexProcessingCaps != 0)
		{
			vertex_process_caps = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		}
		else
		{
			vertex_process_caps = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		//m_d3d_display_mode.Width = 15;
		//m_d3d_display_mode.Height = 15;

		m_d3d_present_param.BackBufferCount = 1;
		m_d3d_present_param.BackBufferFormat = D3DFMT_A8R8G8B8;
		m_d3d_present_param.BackBufferWidth = m_d3d_display_mode.Width;
		m_d3d_present_param.BackBufferHeight = m_d3d_display_mode.Height;
		m_d3d_present_param.hDeviceWindow = m_hwnd;
		m_d3d_present_param.Windowed = true;
		m_d3d_present_param.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_d3d_present_param.AutoDepthStencilFormat = D3DFMT_D24S8;
		m_d3d_present_param.EnableAutoDepthStencil = TRUE;
		m_d3d_present_param.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		m_d3d_present_param.MultiSampleType = D3DMULTISAMPLE_NONE;

		ulong v_back_buffer_quality_levels = 0;
		ulong v_stencil_buffer_quality_levels = 0;
		D3DMULTISAMPLE_TYPE v_multi_sample_type = D3DMULTISAMPLE_TYPE(system_environment::s_multi_sample_type);
		if (SUCCEEDED(m_d3d_object->CheckDeviceMultiSampleType(d3d_capabilities.AdapterOrdinal,
			d3d_capabilities.DeviceType, m_d3d_present_param.BackBufferFormat,
			TRUE, v_multi_sample_type, &v_back_buffer_quality_levels)) &&
			SUCCEEDED(m_d3d_object->CheckDeviceMultiSampleType(d3d_capabilities.AdapterOrdinal,
				d3d_capabilities.DeviceType, m_d3d_present_param.AutoDepthStencilFormat,
				TRUE, v_multi_sample_type, &v_stencil_buffer_quality_levels)))
		{
			m_d3d_present_param.MultiSampleType = v_multi_sample_type;
			//m_d3d_present_param.MultiSampleQuality = v_back_buffer_quality_levels;
		}

		// create d3d device
		//if ( system_environment::s_ref_mode == 0 )
		//{
		//	if(FAILED(m_d3d_object->CreateDevice( D3DADAPTER_DEFAULT, 
		//		D3DDEVTYPE_HAL,
		//		m_hwnd,
		//		vertex_process_caps | D3DCREATE_FPU_PRESERVE,
		//		&m_d3d_present_param, 
		//		&m_d3d_device)))
		//	{
		//		return false;
		//	}
		//		
		//}
		//else if( system_environment::s_ref_mode == 1 )
		//{
		//	
		//										
		//}

		UINT nv_adapter_to_use = D3DADAPTER_DEFAULT;
		D3DDEVTYPE device_type = D3DDEVTYPE_HAL;

#ifndef _PUBLISH

		for (int nv_adapter = 0; nv_adapter < (int)m_d3d_object->GetAdapterCount(); nv_adapter++)
		{
			D3DADAPTER_IDENTIFIER9 v_identifier;
			m_d3d_object->GetAdapterIdentifier(nv_adapter, 0, &v_identifier);
			if (strstr(v_identifier.Description, "PerfHUD") != 0)
			{
				nv_adapter_to_use = nv_adapter;
				device_type = D3DDEVTYPE_REF;
				break;
			}
		}
#endif
		HRESULT hhhr = m_d3d_object->CreateDevice(nv_adapter_to_use,
			device_type,
			m_hwnd,
			vertex_process_caps | D3DCREATE_FPU_PRESERVE,
			&m_d3d_present_param,
			&m_d3d_device);
		if (FAILED(hhhr))
		{
			return false;
		}


		memset(&m_view_port, 0, sizeof(m_view_port));
		m_d3d_device->GetViewport((D3DVIEWPORT9*)&m_view_port);
		m_view_port.m_z_max = 1.0f;
		m_view_port.m_z_min = 0.0f;
		m_view_port.m_width = m_d3d_display_mode.Width;
		m_view_port.m_height = m_d3d_display_mode.Height;
		m_d3d_device->SetViewport((D3DVIEWPORT9*)&m_view_port);

		//m_clip_rect = rect(-m_view_port.m_width, -m_view_port.m_height, m_view_port.m_width, m_view_port.m_height);

		return true;
	}

	// refresh swapchain
	bool d3d9_render_device_impl::reset_swapchain(void)
	{
		if (m_is_windowed == false) return true;

		m_d3d_present_param.BackBufferWidth = m_d3d_display_mode.Width;
		m_d3d_present_param.BackBufferHeight = m_d3d_display_mode.Height;

		if (m_d3d_zbuffer)
		{
			int n_count = m_d3d_zbuffer->Release();
			m_d3d_zbuffer = NULL;
		}


		if (m_d3d_swapchain)
		{
			int n_count = m_d3d_swapchain->Release();
			m_d3d_swapchain = NULL;
		}

		AUTO_RELEASE(m_d3d_zbuffer);
		AUTO_RELEASE(m_d3d_swapchain);


		if (FAILED(m_d3d_device->CreateAdditionalSwapChain(&m_d3d_present_param, &m_d3d_swapchain)))
		{
			return false;
		}

		D3DPRESENT_PARAMETERS d3dpp;
		m_d3d_swapchain->GetPresentParameters(&d3dpp);

		D3DDISPLAYMODE ddpm;
		m_d3d_device->GetDisplayMode(0, &ddpm);

		// set back buffer to new swapchain
		LPDIRECT3DSURFACE9 back_buffer = NULL;

		m_d3d_swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &back_buffer);
		m_d3d_device->SetRenderTarget(0, back_buffer);


		if (back_buffer)
		{
			int n_count = back_buffer->Release();
			back_buffer = NULL;
		}
		AUTO_RELEASE(back_buffer)


			// create depth stencil surface
			if (FAILED(m_d3d_device->CreateDepthStencilSurface(m_d3d_present_param.BackBufferWidth,
				m_d3d_present_param.BackBufferHeight,
				m_d3d_present_param.AutoDepthStencilFormat,
				m_d3d_present_param.MultiSampleType,
				m_d3d_present_param.MultiSampleQuality,
				TRUE,
				&m_d3d_zbuffer,
				NULL)))
			{
				return false;
			}

		m_d3d_device->SetDepthStencilSurface(m_d3d_zbuffer);

		// update viewport
		memset(&m_view_port, 0, sizeof(m_view_port));
		m_d3d_device->GetViewport((D3DVIEWPORT9*)&m_view_port);
		m_view_port.m_z_max = 1.0f;
		m_view_port.m_z_min = 0.0f;
		m_view_port.m_width = m_d3d_display_mode.Width;
		m_view_port.m_height = m_d3d_display_mode.Height;
		m_d3d_device->SetViewport((D3DVIEWPORT9*)&m_view_port);

		//m_clip_rect = rect(0, 0, m_view_port.m_width, m_view_port.m_height);

		//update_clip_matrix();

		return true;
	}

	void d3d9_render_device_impl::update_clip_matrix(void)
	{
		float x, y, w, h;
		x = (float)m_clip_rect.m_left / m_view_port.m_width;
		y = (float)m_clip_rect.m_top / m_view_port.m_height;
		w = (float)(m_clip_rect.get_width()) / m_view_port.m_width;
		h = (float)(m_clip_rect.get_height()) / m_view_port.m_height;
		//h = w;

		//x = -1.0f;
		//y = 0.0f;
		//w = 1;
		//h = 1;

		m_clip_matrix.m11 = 2.0f / w;
		m_clip_matrix.m41 = -1.0f - 2 * x / w;
		m_clip_matrix.m22 = 2.0f / h;
		m_clip_matrix.m42 = -1.0f - 2 * y / h;

	}

	// create d3d font
	bool d3d9_render_device_impl::create_font_impl(void)
	{
		if (FAILED(D3DXCreateFont(m_d3d_device,
			14, 0, 600, 1,
			FALSE,
			DEFAULT_CHARSET,
			OUT_OUTLINE_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH,
			L"Arial",
			&m_d3d_font)))
		{
			return false;
		}

		return true;
	}

	bool d3d9_render_device_impl::can_create_hardware(void)
	{
		///modify by felix内存控制
		/*if ( system_environment::s_always_create_hardware == 1 )
		return true;

		if ( system_environment::s_reclaim_memory_size < singleton_resource_reclaimer::ptr()->get_memory_size() )
		return false;*/

		return true;
	}

	// create 2d sprite
	bool d3d9_render_device_impl::create_sprite_impl(void)
	{
		if (FAILED(D3DXCreateSprite(m_d3d_device, &m_d3d_sprite)))
		{
			return false;
		}

		return true;
	}

	bool d3d9_render_device_impl::create_blank_texture(void)
	{
		HRESULT hr = D3DXCreateTexture(m_d3d_device,
			256,
			256,
			1,
			D3DX_DEFAULT,
			D3DFMT_A1R5G5B5,
			D3DPOOL_MANAGED, &m_blank_texture);

		if (SUCCEEDED(hr))
		{
			D3DLOCKED_RECT rLocked;
			unsigned short* pData;
			m_blank_texture->LockRect(0, &rLocked, NULL, NULL);
			pData = (unsigned short*)rLocked.pBits;
			if (NULL != pData)
			{
				memset(pData, 0xff, 256 * 256 * 2);
			}
			m_blank_texture->UnlockRect(0);
			D3DXFilterTexture(m_blank_texture, NULL, 0, D3DX_DEFAULT);
		}

		return true;
	}

	bool d3d9_render_device_impl::create_transparency_texture(void)
	{
		m_transparency_texture_array[0] = (LPDIRECT3DTEXTURE9)create_transparency_texture(system_environment::s_transparent_color);

		m_transparency_texture_array[1] = (LPDIRECT3DTEXTURE9)create_transparency_texture(system_environment::s_transparent_color2);

		return true;
	}



	// for test
	bool d3d9_render_device_impl::create_line_impl(void)
	{
		if (FAILED(D3DXCreateLine(m_d3d_device, &m_d3d_line)))
		{
			return false;
		}

		return true;
	}

	bool d3d9_render_device_impl::create_effect(void)
	{
		m_effect_manager = new d3d9_effect_manager(this);
		return true;
	}

	bool d3d9_render_device_impl::create_render_target(void)
	{
		m_render_target_manager = new d3d9_render_target_manager(this);

		return true;
	}

	void d3d9_render_device_impl::destroy(void)
	{
		//m_d3d_device->EvictManagedResources();

		AUTO_RELEASE(m_transparency_texture_array[0]);
		AUTO_RELEASE(m_transparency_texture_array[1]);
		AUTO_RELEASE(m_blank_texture);
		AUTO_RELEASE(m_d3d_sprite);
		AUTO_RELEASE(m_d3d_font);
		AUTO_DELETE(m_effect_manager);
		AUTO_DELETE(m_render_target_manager);
		AUTO_DELETE(m_occlusion_query);
		AUTO_RELEASE(m_d3d_line);
		AUTO_RELEASE(m_d3d_zbuffer);
		AUTO_RELEASE(m_d3d_swapchain);
		//AUTO_RELEASE(m_d3d_device);

		if (m_d3d_device)
		{
			m_d3d_device->EvictManagedResources();
			int n_count = m_d3d_device->Release();
			m_d3d_device = NULL;

		}
		AUTO_RELEASE(m_d3d_object);
	}

	void d3d9_render_device_impl::reset(void)
	{
		while (1)
		{
			switch (m_d3d_device->TestCooperativeLevel())
			{
			case D3D_OK:
				return;
			case D3DERR_DEVICELOST:
				break;
			case D3DERR_DEVICENOTRESET:
				on_lost_device();
				on_reset_device();
				return;
			default:
				break;
			}
		}
	}

	void d3d9_render_device_impl::on_resize_device(uint device_width, uint device_height)
	{
		m_d3d_display_mode.Width = device_width;
		m_d3d_display_mode.Height = device_height;
		//refresh_swapchain_impl();
	}

	void d3d9_render_device_impl::on_lost_device(void)
	{
		AUTO_RELEASE(m_d3d_zbuffer);
		AUTO_RELEASE(m_d3d_swapchain);

		m_d3d_line->OnLostDevice();
		m_d3d_sprite->OnLostDevice();
		m_d3d_font->OnLostDevice();

		m_effect_manager->on_lost_device();
		m_render_target_manager->on_lost_device();
		//m_occlusion_query->on_lost_device();
		m_event_lost_device((event_argument*)NULL);
	}

	void d3d9_render_device_impl::on_reset_device(void)
	{
		m_d3d_device->Reset(&m_d3d_present_param);
		reset_swapchain();
		m_d3d_line->OnResetDevice();
		m_d3d_sprite->OnResetDevice();
		m_d3d_font->OnResetDevice();
		m_effect_manager->on_reset_device();
		m_render_target_manager->on_reset_device();
		///m_occlusion_query->on_reset_device();

		m_event_reset_device((event_argument*)NULL);

		// Sets the view transform matrix.
		m_d3d_device->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_view_matrix);
		// Sets the projection transform matrix.
		m_d3d_device->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&(m_project_matrix  * m_clip_matrix));
	}

	void d3d9_render_device_impl::begin_frame(void)
	{
		if (m_d3d_display_mode.Width != m_view_port.m_width || m_d3d_display_mode.Height != m_view_port.m_height || m_d3d_display_mode.Width != m_d3d_present_param.BackBufferWidth || m_d3d_display_mode.Height != m_d3d_present_param.BackBufferHeight)
		{
			if (system_environment::s_debug_mode)
			{
				reset_swapchain();
			}
			else
			{
				//refresh_swapchain_impl();
				on_lost_device();
				on_reset_device();
			}

		}

		m_d3d_device->BeginScene();
		m_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00000000, 1.0f, 2/*0x00000000*/);
		m_render_count = 0;
		m_batch_count = 0;
	}

	void d3d9_render_device_impl::end_frame(void)
	{
		m_d3d_device->SetStreamSource(0, 0, 0, 0);
		m_d3d_device->SetIndices(0);
		m_d3d_device->EndScene();

		if (m_is_windowed)
		{
			if (m_d3d_swapchain)
			{
				HRESULT hr = m_d3d_swapchain->Present(NULL, NULL, NULL, NULL, D3DPRESENT_DONOTWAIT);
				//wchar_t ch[80];
				//swprintf( ch, L"m_d3d_swapchain->Present:0x%x\n", hr );
				//OutputDebugString( ch );
				if (hr == D3DERR_DEVICELOST)
				{
					reset();
				}
			}
		}
		else
		{
			HRESULT hr = m_d3d_device->Present(NULL, NULL, NULL, NULL);
			if (hr == D3DERR_DEVICELOST)
			{
				reset();
			}
		}
	}

	//void d3d9_render_device_impl::begin_frame_offscreen( void )
	//{
	//	if ( m_d3d_display_mode.Width != m_view_port.m_width || m_d3d_display_mode.Height != m_view_port.m_height || m_d3d_display_mode.Width != m_d3d_present_param.BackBufferWidth || m_d3d_display_mode.Height != m_d3d_present_param.BackBufferHeight )
	//	{
	//		if ( system_environment::s_debug_mode )
	//		{
	//			reset_swapchain();
	//		}
	//		else
	//		{
	//			//refresh_swapchain_impl();
	//			on_lost_device();
	//			on_reset_device();
	//		}

	//	}

	//	m_d3d_device->BeginScene();
	//	m_d3d_device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0x00000000, 1.0f, 2/*0x00000000*/ ); 
	//	m_render_count = 0;
	//	m_batch_count = 0;
	//}

	//void d3d9_render_device_impl::end_frame_offscreen( void )
	//{
	//	m_d3d_device->SetStreamSource( 0, 0, 0, 0 );
	//	m_d3d_device->SetIndices( 0 );
	//	m_d3d_device->EndScene();

	//	if(m_is_windowed)
	//	{
	//		if(m_d3d_swapchain)
	//		{
	//			HRESULT hr = m_d3d_swapchain->Present( NULL, NULL, NULL, NULL, D3DPRESENT_DONOTWAIT );
	//			//wchar_t ch[80];
	//			//swprintf( ch, L"m_d3d_swapchain->Present:0x%x\n", hr );
	//			//OutputDebugString( ch );
	//			if ( hr == D3DERR_DEVICELOST )
	//			{
	//				reset();
	//			}
	//			
	//		}
	//	}
	//	else
	//	{
	//		HRESULT hr = m_d3d_device->Present( NULL, NULL, NULL, NULL );
	//		if ( hr == D3DERR_DEVICELOST )
	//		{
	//			reset();
	//		}
	//	}
	//}

	void d3d9_render_device_impl::clear(ulong color, float z_buffer, ulong stencil)
	{
		m_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, color, z_buffer, stencil);
	}

	void d3d9_render_device_impl::clear_target(ulong color)
	{
		m_d3d_device->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0x00000000);
	}

	void d3d9_render_device_impl::clear_zbuffer(float z_buffer)
	{
		m_d3d_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0x00000000, z_buffer, 0x00000000);
	}

	void d3d9_render_device_impl::clear_stencil(ulong stencil)
	{
		m_d3d_device->Clear(0, NULL, D3DCLEAR_STENCIL, 0x00000000, 1.0f, stencil);
	}

	void d3d9_render_device_impl::save_back_buffer(cpstr str_path, int width, int height)
	{
		IDirect3DSurface9 *pBackSurface = NULL;
		IDirect3DSurface9 *pLockSurface = NULL;
		IDirect3DSurface9 *pRenderTarget = NULL;

		m_d3d_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackSurface);
		D3DSURFACE_DESC desc;
		HRESULT hr = pBackSurface->GetDesc(&desc);

		if (FAILED(m_d3d_device->CreateRenderTarget(width, height, desc.Format, D3DMULTISAMPLE_NONE, 0, false, &pRenderTarget, NULL)))
		{
			return;
		}

		if (FAILED(hr = m_d3d_device->StretchRect(pBackSurface, NULL, pRenderTarget, NULL, D3DTEXF_NONE)))
		{
			return;
		}

		m_d3d_device->CreateOffscreenPlainSurface(width, height, desc.Format, D3DPOOL_SYSTEMMEM, &pLockSurface, NULL);
		m_d3d_device->GetRenderTargetData(pRenderTarget, pLockSurface);

		int v_file_type = D3DXIFF_JPG;
		D3DXSaveSurfaceToFile(str_path, D3DXIMAGE_FILEFORMAT(v_file_type), pLockSurface, NULL, NULL);

		if (pBackSurface) pBackSurface->Release();
		if (pLockSurface) pLockSurface->Release();
		if (pRenderTarget) pRenderTarget->Release();
	}

	void d3d9_render_device_impl::save_back_buffer(cpstr str_path, int width, int height, int stretch_type)
	{
		IDirect3DSurface9 *pBackSurface = NULL;
		IDirect3DSurface9 *pLockSurface = NULL;
		IDirect3DSurface9 *pRenderTarget = NULL;

		m_d3d_device->GetRenderTarget(0, &pBackSurface);
		D3DSURFACE_DESC desc;
		HRESULT hr = pBackSurface->GetDesc(&desc);


		if (FAILED(m_d3d_device->CreateRenderTarget(width, height, desc.Format, D3DMULTISAMPLE_NONE, 0, false, &pRenderTarget, NULL)))
		{
			return;
		}


		RECT* srcRect = NULL;
		if (stretch_type == 0) // USScreenShotOutputMode::StretchImage
		{
		}
		else if (stretch_type == 1) // USScreenShotOutputMode::ClipImage
		{
			double src_aspect = (double)m_view_port.m_width / (double)m_view_port.m_height;
			double dest_aspect = (double)width / (double)height;


			if (src_aspect > dest_aspect)
			{
				double srcWidth = (double)m_view_port.m_height * dest_aspect;
				double srcHeight = m_view_port.m_height;
				double srcX = ((double)m_view_port.m_width - srcWidth) / 2;
				double srcY = 0;

				srcRect = new RECT;
				srcRect->top = srcY;
				srcRect->left = srcX;
				srcRect->bottom = srcY + srcHeight;
				srcRect->right = srcX + srcWidth;
			}
			else if (src_aspect < dest_aspect)
			{
				double srcWidth = (double)m_view_port.m_width;
				double srcHeight = (double)m_view_port.m_width / dest_aspect;
				double srcX = 0;
				double srcY = ((double)m_view_port.m_height - srcHeight) / 2;

				srcRect = new RECT;
				srcRect->top = srcY;
				srcRect->left = srcX;
				srcRect->bottom = srcY + srcHeight;
				srcRect->right = srcX + srcWidth;
			}
			else
			{
				//state = v_output_graphics->DrawImage( v_temp_image, RectF(0,0, width, height) );
			}
		}
		else if (stretch_type == 2) //  USScreenShotOutputMode::ContainImage 
		{

		}

		if (FAILED(hr = m_d3d_device->StretchRect(pBackSurface, srcRect, pRenderTarget, NULL, D3DTEXF_LINEAR)))
		{
			return;
		}

		m_d3d_device->CreateOffscreenPlainSurface(width, height, desc.Format, D3DPOOL_SYSTEMMEM, &pLockSurface, NULL);
		m_d3d_device->GetRenderTargetData(pRenderTarget, pLockSurface);

		int v_file_type = D3DXIFF_JPG;
		hr = D3DXSaveSurfaceToFile(str_path, D3DXIMAGE_FILEFORMAT(v_file_type), pLockSurface, NULL, NULL);

		AUTO_DELETE(srcRect);

		if (pBackSurface) pBackSurface->Release();
		if (pLockSurface) pLockSurface->Release();
		if (pRenderTarget) pRenderTarget->Release();
	}

	//void d3d9_render_device_impl::save_back_buffer( cpstr str_path, int width, int height )
	//{
	//	IDirect3DSurface9 *pBackSurface=NULL;
	//	IDirect3DSurface9 *pLockSurface=NULL;
	//	IDirect3DSurface9 *pRenderTarget=NULL;

	//	m_d3d_swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackSurface);
	//	D3DSURFACE_DESC desc;
	//	HRESULT hr = pBackSurface->GetDesc(&desc);

	//	if(FAILED(m_d3d_device->CreateRenderTarget( width, height, desc.Format ,D3DMULTISAMPLE_NONE, 0, false, &pRenderTarget, NULL )))
	//	{
	//		return;
	//	}

	//	if(FAILED(hr = m_d3d_device->StretchRect(pBackSurface, NULL, pRenderTarget, NULL, D3DTEXF_NONE )))
	//	{
	//		return;
	//	}

	//	m_d3d_device->CreateOffscreenPlainSurface(width, height,desc.Format,D3DPOOL_SYSTEMMEM,&pLockSurface,NULL);
	//	m_d3d_device->GetRenderTargetData(pRenderTarget, pLockSurface);

	//	int v_file_type = D3DXIFF_JPG;
	//	D3DXSaveSurfaceToFile(str_path, D3DXIMAGE_FILEFORMAT(v_file_type), pLockSurface, NULL, NULL);

	//	if (pBackSurface) pBackSurface->Release();
	//	if (pLockSurface) pLockSurface->Release();
	//	if (pBackSurface) pRenderTarget->Release();
	//}



	void d3d9_render_device_impl::grab_back_buffer(raw_buffer* v_buffer, int width, int height, int stretch_type)
	{
		IDirect3DSurface9 *pBackSurface = NULL;
		IDirect3DSurface9 *pLockSurface = NULL;
		IDirect3DSurface9 *pRenderTarget = NULL;

		m_d3d_swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackSurface);
		D3DSURFACE_DESC desc;
		HRESULT hr = pBackSurface->GetDesc(&desc);

		if (FAILED(m_d3d_device->CreateRenderTarget(width, height, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, false, &pRenderTarget, NULL)))
		{
			return;
		}

		if (FAILED(hr = m_d3d_device->StretchRect(pBackSurface, NULL, pRenderTarget, NULL, D3DTEXF_NONE)))
		{
			return;
		}

		m_d3d_device->CreateOffscreenPlainSurface(width, height, desc.Format, D3DPOOL_SYSTEMMEM, &pLockSurface, NULL);
		m_d3d_device->GetRenderTargetData(pRenderTarget, pLockSurface);

		D3DLOCKED_RECT d3dlr;
		if (D3D_OK == pLockSurface->LockRect(&d3dlr, NULL, D3DLOCK_DONOTWAIT))//D3DLOCK_NO_DIRTY_UPDATE|D3DLOCK_DONOTWAIT|D3DLOCK_NOOVERWRITE|D3DLOCK_READONLY))
		{




			pLockSurface->UnlockRect();
		}

		if (pBackSurface) pBackSurface->Release();
		if (pLockSurface) pLockSurface->Release();
		if (pBackSurface) pRenderTarget->Release();
	}

	//void d3d9_render_device_impl::save_back_buffer(cpstr str_path, int width, int height)
	//{
	//	IDirect3DSurface9* pTargetSurface=NULL;
	//	HRESULT hr = m_d3d_swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pTargetSurface);
	//	//HRESULT hr = m_d3d_swapchain->GetRenderTarget(0,&pTargetSurface);
	//	time_value v_start_time = (double)timeGetTime();
	//	time_value v_render_time = (double)timeGetTime()- v_start_time;
	//	if(SUCCEEDED(hr))
	//	{
	//		D3DSURFACE_DESC desc;
	//		hr=pTargetSurface->GetDesc(&desc);
	//		if(SUCCEEDED(hr))
	//		{
	//			IDirect3DTexture9* pTempTexture=NULL;
	//			hr=m_d3d_device->CreateTexture(desc.Width,desc.Height,1,0,desc.Format,D3DPOOL_SYSTEMMEM,&pTempTexture,NULL);
	//			if(SUCCEEDED(hr))
	//			{
	//				IDirect3DSurface9* pTempSurface=NULL;
	//				hr=pTempTexture->GetSurfaceLevel(0,&pTempSurface);
	//				if(SUCCEEDED(hr))
	//				{
	//					hr=m_d3d_device->GetRenderTargetData(pTargetSurface,pTempSurface);
	//					if(SUCCEEDED(hr))
	//					{
	//						D3DXSaveTextureToFile(str_path, D3DXIFF_PNG, pTempTexture, NULL);
	//					}
	//					pTempSurface->Release();
	//				}
	//				pTempTexture->Release();
	//			}
	//		}
	//		pTargetSurface->Release();
	//	}
	//	time_value v_eclapse_time = (double)timeGetTime()- v_start_time;

	//	US_TRACE2(L"render time:%lf    save time:%lf\n", v_render_time, v_eclapse_time);
	//}

	//void d3d9_render_device_impl::save_back_buffer(cpstr str_path, int width, int height)
	//{
	//	IDirect3DSurface9* pTargetSurface=NULL;
	//	HRESULT hr = m_d3d_swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pTargetSurface);
	//	//HRESULT hr = m_d3d_swapchain->GetRenderTarget(0,&pTargetSurface);
	//	time_value v_start_time = (double)timeGetTime();
	//	time_value v_render_time = (double)timeGetTime()- v_start_time;
	//	if(SUCCEEDED(hr))
	//	{
	//		D3DSURFACE_DESC desc;
	//		hr=pTargetSurface->GetDesc(&desc);
	//		if(SUCCEEDED(hr))
	//		{
	//			IDirect3DTexture9* pTempTexture=NULL;
	//			hr=m_d3d_device->CreateTexture(desc.Width,desc.Height,1,0,desc.Format,D3DPOOL_SYSTEMMEM,&pTempTexture,NULL);
	//			if(SUCCEEDED(hr))
	//			{
	//				IDirect3DSurface9* pTempSurface=NULL;
	//				hr=pTempTexture->GetSurfaceLevel(0,&pTempSurface);
	//				if(SUCCEEDED(hr))
	//				{
	//					hr=m_d3d_device->GetRenderTargetData(pTargetSurface,pTempSurface);
	//					if(SUCCEEDED(hr))
	//					{
	//						D3DXSaveTextureToFile(str_path, D3DXIFF_PNG, pTempTexture, NULL);
	//					}
	//					pTempSurface->Release();
	//				}
	//				pTempTexture->Release();
	//			}
	//		}
	//		pTargetSurface->Release();
	//	}
	//	time_value v_eclapse_time = (double)timeGetTime()- v_start_time;

	//	US_TRACE2(L"render time:%lf    save time:%lf\n", v_render_time, v_eclapse_time);
	//}


	//void d3d9_render_device_impl::save_back_buffer( cpstr str_path, int width, int height )
	//{
	//	IDirect3DSurface9 *tmp = NULL;
	//	IDirect3DSurface9 *back = NULL;

	//	//生成固定颜色模式的离屏表面（Width和 Height为屏幕或窗口的宽高）
	//	m_d3d_device->CreateOffscreenPlainSurface(width, height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tmp, NULL);

	//	// 获得BackBuffer的D3D Surface
	//	m_d3d_swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &back);

	//	// Copy一下，，需要时转换颜色格式
	//	time_value v_start_time = (double)timeGetTime();
	//	D3DXLoadSurfaceFromSurface(tmp, NULL, NULL, back, NULL, NULL, D3DX_FILTER_NONE, 0);
	//	time_value v_render_time = (double)timeGetTime()- v_start_time;

	//	int v_file_type = D3DXIFF_JPG;
	//	ustring str_ext = file_directory::get_file_ext(str_path);
	//	US_STRING_LOWER(str_ext);
	//	if(str_ext == L"jpg" )
	//	{
	//		v_file_type = D3DXIFF_JPG;
	//	}
	//	if(str_ext == L"bmp" )
	//	{
	//		v_file_type = D3DXIFF_BMP;
	//	}
	//	if(str_ext == L"png" )
	//	{
	//		v_file_type = D3DXIFF_PNG;
	//	}

	//	// 保存成BMP格式
	//	D3DXSaveSurfaceToFile(str_path, D3DXIMAGE_FILEFORMAT(v_file_type), tmp, NULL, NULL);

	//	time_value v_eclapse_time = (double)timeGetTime()- v_start_time;

	//	US_TRACE2(L"render time:%lf    save time:%lf\n", v_render_time, v_eclapse_time);

	//	// 释放Surface，防止内存泄漏
	//	AUTO_RELEASE(tmp);
	//	AUTO_RELEASE(back);
	//}

	//void d3d9_render_device_impl::grab_back_buffer( raw_buffer* v_buffer, int width, int height  )
	//{
	//	IDirect3DSurface9 *tmp = NULL;
	//	IDirect3DSurface9 *back = NULL;

	//	//生成固定颜色模式的离屏表面（Width和 Height为屏幕或窗口的宽高）
	//	m_d3d_device->CreateOffscreenPlainSurface(width, height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tmp, NULL);

	//	// 获得BackBuffer的D3D Surface
	//	m_d3d_swapchain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &back);

	//	// Copy一下，，需要时转换颜色格式
	//	D3DXLoadSurfaceFromSurface(tmp, NULL, NULL, back, NULL, NULL, D3DX_FILTER_NONE, 0);

	//	int v_file_type = D3DXIFF_DIB;
	//
	//	// 保存成BMP格式
	//	LPD3DXBUFFER pBuffer = NULL;
	//	D3DXSaveSurfaceToFileInMemory(&pBuffer, D3DXIMAGE_FILEFORMAT(v_file_type), tmp, NULL, NULL);

	//	void* ptr = (pBuffer)->GetBufferPointer();
	//	UINT  len = (pBuffer)->GetBufferSize();

	//	// 释放Surface，防止内存泄漏
	//	AUTO_RELEASE(tmp);
	//	AUTO_RELEASE(back);
	//	AUTO_RELEASE(pBuffer);
	//}

	vector3<float> d3d9_render_device_impl::projection(vector3<float>& vector_in_world)
	{
		vector3<float> ret_vec;
		D3DXVec3Project((D3DXVECTOR3*)&ret_vec,
			(D3DXVECTOR3*)&vector_in_world,
			(D3DVIEWPORT9*)&m_view_port,
			(D3DXMATRIX*)&m_project_matrix,
			(D3DXMATRIX*)&m_view_matrix,
			(D3DXMATRIX*)&matrix4<float>::s_identity);
		return ret_vec;

	}

	vector3<float> d3d9_render_device_impl::unprojection(vector3<float>& vector_on_viewport)
	{
		vector3<float> ret_vec;
		D3DXVec3Unproject((D3DXVECTOR3*)&ret_vec,
			(D3DXVECTOR3*)&vector_on_viewport,
			(D3DVIEWPORT9*)&m_view_port,
			(D3DXMATRIX*)&m_project_matrix,
			(D3DXMATRIX*)&m_view_matrix,
			(D3DXMATRIX*)&matrix4<float>::s_identity);
		return ret_vec;
	}

	void d3d9_render_device_impl::set_clipping_plane(ulong index, float* in_plane)
	{
		m_d3d_device->SetClipPlane(index, in_plane);
	}

	void d3d9_render_device_impl::set_view_port(view_port& viewport)
	{
		m_view_port = viewport;
	}

	view_port d3d9_render_device_impl::get_view_port(void) const
	{
		return m_view_port;
	}

	render_transform* d3d9_render_device_impl::get_matrix_stack(void)
	{
		// not implement;
		return NULL;
	}

	render_state* d3d9_render_device_impl::get_render_state(void)
	{
		// not implement;
		return NULL;
	}

	texture_stage* d3d9_render_device_impl::get_texture_stage(ulong texture_stage_index)
	{
		// not implement;
		return NULL;
	}

	ULONG_PTR d3d9_render_device_impl::get_device(void)
	{
		return (ULONG_PTR)m_d3d_device;
	}

	ULONG_PTR d3d9_render_device_impl::get_sprite(void)
	{
		return (ULONG_PTR)m_d3d_sprite;
	}

	ULONG_PTR d3d9_render_device_impl::get_font(void)
	{
		return (ULONG_PTR)m_d3d_font;
	}

	ULONG_PTR d3d9_render_device_impl::get_line(void)
	{
		return (ULONG_PTR)m_d3d_line;
	}

	//ulong d3d9_render_device_impl::get_vertex_blend_effect( void )
	//{
	//	//return (ulong)m_d3d9_vertex_blend_effect;
	//}

	ULONG_PTR d3d9_render_device_impl::get_effect(long effect_handle)
	{
		return m_effect_manager->get_effect(effect_handle);
	}

	ULONG_PTR d3d9_render_device_impl::get_render_target(long rt_handle)
	{
		return m_render_target_manager->get_render_target(rt_handle);
	}

	void d3d9_render_device_impl::set_culling_mode(culling_mode mode)
	{
		m_d3d_device->SetRenderState(D3DRS_CULLMODE, mode);
	}

	culling_mode d3d9_render_device_impl::get_culling_mode(void) const
	{
		ulong cullmode = 0;
		m_d3d_device->GetRenderState(D3DRS_CULLMODE, &cullmode);
		return (culling_mode)cullmode;
	}

	void d3d9_render_device_impl::set_clip_rect(rect& clip_rect)
	{
		m_clip_rect = clip_rect;
		update_clip_matrix();
	}

	// x = -1, y = -1, w = 2, h = 2;
	void d3d9_render_device_impl::set_clip_rect(float x, float y, float w, float h)
	{
		//m_clip_rect = clip_rect;
		//update_clip_matrix();
		m_clip_matrix.m11 = 2.0f / w;
		m_clip_matrix.m41 = -1.0f - 2 * x / w;
		m_clip_matrix.m22 = 2.0f / h;
		m_clip_matrix.m42 = -1.0f - 2 * y / h;
	}

	// Sets the world transform matrix. 
	void d3d9_render_device_impl::set_world_matrix(const matrix4<float>& mat)
	{
		m_d3d_device->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&mat);
		m_world_matrix = mat;
	}

	// Get the world transform matrix.
	matrix4<float> d3d9_render_device_impl::get_world_matrix(void)
	{
		matrix4<float> mat;

		m_d3d_device->GetTransform(D3DTS_WORLD, (D3DXMATRIX*)&mat);

		return mat;
	}

	// Sets multiple world matrices (vertex blending).
	void d3d9_render_device_impl::set_world_matrices(const matrix4<float>* mats, ushort count)
	{
		for (int ni = 0; ni < (int)count; ni++)
		{
			m_d3d_device->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_WORLD1 + ni), (D3DXMATRIX*)&mats[ni]);
		}
	}

	// Sets the view transform matrix.
	void d3d9_render_device_impl::set_view_matrix(const matrix4<float>& mat)
	{
		m_view_matrix = mat;
		m_d3d_device->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&mat);

	}

	// Get the view transform matrix.
	matrix4<float> d3d9_render_device_impl::get_view_matrix(void)
	{
		matrix4<float> mat;

		m_d3d_device->GetTransform(D3DTS_VIEW, (D3DXMATRIX*)&mat);

		return mat;
	}

	// Sets the projection transform matrix.
	void d3d9_render_device_impl::set_projection_matrix(const matrix4<float>& mat)
	{
		m_project_matrix = mat;
		matrix4<float> v_mat = m_project_matrix  * m_clip_matrix;

		//matrix_4f matOrthoLH;
		//D3DXMatrixOrthoLH( (D3DXMATRIX*)&m_project_matrix, m_view_port.m_width, m_view_port.m_height, m_near_plane, m_far_plane );

		m_d3d_device->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&v_mat);

	}

	// Get the projection transform matrix.
	matrix4<float> d3d9_render_device_impl::get_projection_matrix(void)
	{
		matrix4<float> mat = m_project_matrix;

		//m_d3d_device->GetTransform( D3DTS_PROJECTION, (D3DXMATRIX*)&mat );

		return mat;
	}

	// Sets the clip transform matrix.
	void d3d9_render_device_impl::set_clip_matrix(const matrix4<float>& mat)
	{
		m_clip_matrix = mat;
	}

	// Get the clip transform matrix.
	matrix4<float> d3d9_render_device_impl::get_clip_matrix(void)
	{
		//matrix4<float> mat = m_project_matrix ;

		//m_d3d_device->GetTransform( D3DTS_PROJECTION, (D3DXMATRIX*)&mat );

		return m_clip_matrix;
	}

	// Sets the texture coordinate transformation matrix for a texture unit.
	// @param unit Texture unit to affect
	// @param xform The 4x4 matrix
	void d3d9_render_device_impl::set_texture_matrix(size_t unit, const matrix4<float>& mat)
	{
		m_d3d_device->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + unit), (D3DXMATRIX*)&mat);
	}

	// Sets the current vertex declaration, ie the source of vertex data.
	void d3d9_render_device_impl::set_vertex_declaration(vertex_declaration* decl)
	{
		// not implement;		
	}

	// Sets the current vertex declaration, ie the source of vertex data.
	void d3d9_render_device_impl::set_vertex_declaration(uint decl)
	{
		m_d3d_device->SetFVF(decl);
	}

	// Sets the current vertex buffer binding state.
	void d3d9_render_device_impl::set_vertex_buffer_binding(vertex_buffer_binding* binding)
	{
		// not implement;
	}

	// Create an object for performing hardware occlusion queries. 
	hardware_occlusion_query* d3d9_render_device_impl::create_hardware_occlusion_query(void)
	{
		// not implement;
		//if( !m_occlusion_query )
		//	m_occlusion_query = new d3d9_hardware_occlusion_query(m_d3d_device);

		return m_occlusion_query;
	}

	// Destroy a hardware occlusion query object. 
	void d3d9_render_device_impl::destroy_hardware_occlusion_query(hardware_occlusion_query* hq)
	{
		// not implement;
		AUTO_DELETE(hq);
	}

	void d3d9_render_device_impl::set_vertex_declear(ulong vertex_declear_type)
	{
		m_d3d_device->SetFVF(vertex_declear_type);
	}

	void d3d9_render_device_impl::set_texture(ulong stage_index, texture_handle handle)
	{
		m_d3d_device->SetTexture(stage_index, (LPDIRECT3DTEXTURE9)handle);
	}

	void d3d9_render_device_impl::set_blank_texture(ulong stage_index)
	{
		m_d3d_device->SetTexture(stage_index, m_blank_texture);
	}

	void d3d9_render_device_impl::set_transparency_texture(ulong stage_index)
	{
		m_d3d_device->SetTexture(stage_index, m_transparency_texture_array[0]);
	}

	void d3d9_render_device_impl::set_transparency_texture(ulong stage_index, ulong texture_index)
	{
		m_d3d_device->SetTexture(stage_index, m_transparency_texture_array[texture_index]);
	}

	ULONG_PTR d3d9_render_device_impl::get_blank_texture(void)
	{
		return (ULONG_PTR)m_blank_texture;
	}

	void d3d9_render_device_impl::set_material(ULONG_PTR material_handle)
	{
		m_d3d_device->SetMaterial((D3DMATERIAL9*)material_handle);
	}

	ULONG_PTR d3d9_render_device_impl::get_transparency_texture(ulong texture_index)
	{
		return (ULONG_PTR)m_transparency_texture_array[texture_index];
	}

	void d3d9_render_device_impl::push_render_state(render_state& state)
	{
		// not implement;
	}

	void d3d9_render_device_impl::pop_render_state(void)
	{
		// not implement;
	}

	void d3d9_render_device_impl::push_texture_state(texture_state& state)
	{
		// not implement;
	}

	void d3d9_render_device_impl::pop_texture_state(void)
	{
		// not implement;
	}

	void d3d9_render_device_impl::push_transform(void)
	{
		m_matrix_stack.push(m_world_matrix);
	}

	void d3d9_render_device_impl::multiply_transform(const matrix4<float>& mat)
	{
		//m_world_matrix = mat * m_world_matrix;
		//matrix4<float>* pMat = &mat;
		uniscope_globe::multiply_matrix_4f_sse2(&m_world_matrix, &mat, &m_world_matrix);
		m_d3d_device->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&m_world_matrix);
	}

	void d3d9_render_device_impl::pop_transform(void)
	{
		if (m_matrix_stack.size() == 0) return;
		m_world_matrix = m_matrix_stack.top();
		m_matrix_stack.pop();
	}

	void d3d9_render_device_impl::set_render_state(ulong state_type, ulong state_value)
	{
		m_d3d_device->SetRenderState((D3DRENDERSTATETYPE)state_type, state_value);
	}

	void d3d9_render_device_impl::get_render_state(ulong state_type, ulong* state_value)
	{
		m_d3d_device->GetRenderState((D3DRENDERSTATETYPE)state_type, state_value);
	}

	void d3d9_render_device_impl::set_texture_state(ulong stage_index, ulong state_type, ulong state_value)
	{
		m_d3d_device->SetTextureStageState(stage_index, (D3DTEXTURESTAGESTATETYPE)state_type, state_value);
	}

	void d3d9_render_device_impl::get_texture_state(ulong stage_index, ulong state_type, ulong* state_value)
	{
		m_d3d_device->GetTextureStageState(stage_index, (D3DTEXTURESTAGESTATETYPE)state_type, state_value);
	}

	void d3d9_render_device_impl::set_sampler_state(ulong stage_index, ulong state_type, ulong state_value)
	{
		m_d3d_device->SetSamplerState(stage_index, (D3DSAMPLERSTATETYPE)state_type, state_value);
	}

	void d3d9_render_device_impl::get_sampler_state(ulong stage_index, ulong state_type, ulong* state_value)
	{
		m_d3d_device->GetSamplerState(stage_index, (D3DSAMPLERSTATETYPE)state_type, state_value);
	}

	void d3d9_render_device_impl::draw_indexed_triangle_list(ulong opt, void* vertices_buffer,
		ulong vertices_count,
		void* indices_buffer,
		ulong indices_count,
		ulong stride_size)
	{
		if (opt == D3DFMT_INDEX16)
		{
			m_d3d_device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
				0,
				vertices_count,
				indices_count / 3,
				indices_buffer,
				D3DFMT_INDEX16,
				vertices_buffer,
				stride_size);
		}
		else
		{
			m_d3d_device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
				0,
				vertices_count,
				indices_count / 3,
				indices_buffer,
				D3DFMT_INDEX32,
				vertices_buffer,
				stride_size);
		}

	}

	void d3d9_render_device_impl::draw_indexed_triangle_fan(void* vertices_buffer,
		ulong vertices_count,
		ushort* indices_buffer,
		ulong indices_count,
		ulong stride_size)
	{
		m_d3d_device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLEFAN,
			0,
			vertices_count,
			vertices_count - 2,
			indices_buffer,
			D3DFMT_INDEX16,
			vertices_buffer,
			stride_size);
	}

	void d3d9_render_device_impl::draw_triangle_fan(void* vertices_buffer, ulong vertices_count, ulong stride_size)
	{
		m_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, vertices_count - 2, vertices_buffer, stride_size);
	}

	void d3d9_render_device_impl::draw_triangle_strip(void* vertices_buffer, ulong vertices_count, ulong strid_size)
	{
		m_d3d_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, vertices_count - 2, vertices_buffer, strid_size);
	}

	void d3d9_render_device_impl::draw_line_list(void* vertices_buffer, ulong vertices_count, ulong stride_size)
	{
		m_d3d_device->DrawPrimitiveUP(D3DPT_LINELIST, vertices_count / 2, vertices_buffer, stride_size);
	}

	void d3d9_render_device_impl::draw_line_strip(void* vertices_buffer, ulong vertices_count, ulong stride_size)
	{
		m_d3d_device->DrawPrimitiveUP(D3DPT_LINESTRIP, vertices_count - 1, vertices_buffer, stride_size);
	}

	void d3d9_render_device_impl::draw_point_list(void* vertices_buffer, ulong vertices_count, ulong stride_size)
	{
		m_d3d_device->DrawPrimitiveUP(D3DPT_POINTLIST, vertices_count, vertices_buffer, stride_size);
	}

	void d3d9_render_device_impl::draw_polygon(void* vertices_buffer, ulong vertices_count, ulong vertices_color)
	{
		//		m_d3d_device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, vertices_count, vertices_buffer, stride_size );
	}

	void d3d9_render_device_impl::draw_text(const LPCTSTR text_string, vector3<double>& position_in_geo, ulong text_color)
	{
		//vector3<float> screen_vec = projection( vector3<float>( (double*)&position_in_geo ) );

		//rect m_text_rect;
		//m_text_rect.set_rect( 0, 0, wcslen( text_string ) * 13, 14 );
		//m_text_rect.offset( screen_vec.x, screen_vec.y );

		//m_d3d_font->DrawText(NULL, text_string, -1, (RECT* )&m_text_rect, DT_CALCRECT, 0xffffffff);
	}

	void d3d9_render_device_impl::draw_mesh(void* mesh_pointer, uint pass_index)
	{
		if (mesh_pointer == NULL) return;
		((LPD3DXMESH)mesh_pointer)->DrawSubset(pass_index);
	}

	void d3d9_render_device_impl::draw_transformed_line_strip(void* point_buffer, ulong point_count, ulong point_color)
	{
		m_d3d_device->DrawPrimitiveUP(D3DPT_LINESTRIP, point_count, point_buffer, 12);
	}

	void d3d9_render_device_impl::draw_transformed_text(const LPCTSTR text_string, point point_transformed, ulong text_color)
	{
		D3DXMATRIX mat_trans;
		D3DXMatrixIdentity(&mat_trans);

		m_d3d_sprite->SetTransform(&mat_trans);

		m_d3d_sprite->Begin(D3DXSPRITE_SORT_DEPTH_BACKTOFRONT);
		{
			ulong mask = (0xff000000 & text_color);
			ulong bg_color = 0xff000000 & mask;

			//开始绘制。
			RECT rect = { 0, 0, 72, 64 };
			OffsetRect(&rect, point_transformed.x, point_transformed.y);

			HRESULT hr = m_d3d_font->DrawText(NULL, text_string, -1, &rect, DT_CALCRECT, 0x000000ff);

			float v_offset = 1;
			OffsetRect(&rect, 0, -v_offset);
			m_d3d_font->DrawText(NULL, text_string, -1, &rect, DT_LEFT | DT_VCENTER, bg_color);

			OffsetRect(&rect, -v_offset, v_offset);
			m_d3d_font->DrawText(NULL, text_string, -1, &rect, DT_LEFT | DT_VCENTER, bg_color);

			OffsetRect(&rect, v_offset, v_offset);
			m_d3d_font->DrawText(NULL, text_string, -1, &rect, DT_LEFT | DT_VCENTER, bg_color);

			OffsetRect(&rect, v_offset, -v_offset);
			m_d3d_font->DrawText(NULL, text_string, -1, &rect, DT_LEFT | DT_VCENTER, bg_color);

			OffsetRect(&rect, -v_offset, 0);
			m_d3d_font->DrawText(NULL, text_string, -1, &rect, DT_LEFT | DT_VCENTER, text_color);

			D3DXVECTOR3 v_center = D3DXVECTOR3((rect.right + rect.left) * 0.5f, (rect.bottom + rect.top) * 0.5f, 0);
			m_d3d_sprite->Draw(NULL, &rect, &v_center, NULL, 0xffffffff);
		}
		m_d3d_sprite->End();
	}


	void d3d9_render_device_impl::draw_transformed_text_ext(const LPCTSTR text_string, point point_transformed, ulong text_color, double size)
	{
		if (m_font_size != size)
		{
			if (FAILED(D3DXCreateFont(m_d3d_device,
				14 * size, 0, 600 * size, 1,
				FALSE,
				DEFAULT_CHARSET,
				OUT_OUTLINE_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH,
				L"Arial",
				&m_d3d_font_ext)))
			{
				return;
			}
			m_font_size = size;
		}

		if (m_d3d_font_ext == NULL)
		{
			return;
		}

		D3DXMATRIX mat_trans;
		D3DXMatrixIdentity(&mat_trans);

		m_d3d_sprite->SetTransform(&mat_trans);
		m_d3d_sprite->Begin(D3DXSPRITE_SORT_DEPTH_BACKTOFRONT);
		{
			ulong mask = (0xff000000 & text_color);
			ulong bg_color = 0xff000000 & mask;

			RECT rect = { 0, 0, 72, 64 };
			OffsetRect(&rect, point_transformed.x, point_transformed.y);

			HRESULT hr = m_d3d_font_ext->DrawText(NULL, text_string, -1, &rect, DT_CALCRECT, 0x000000ff);

			float v_offset = 1;
			OffsetRect(&rect, 0, -v_offset);
			m_d3d_font_ext->DrawText(NULL, text_string, -1, &rect, DT_LEFT | DT_VCENTER, bg_color);

			OffsetRect(&rect, -v_offset, v_offset);
			m_d3d_font_ext->DrawText(NULL, text_string, -1, &rect, DT_LEFT | DT_VCENTER, bg_color);

			OffsetRect(&rect, v_offset, v_offset);
			m_d3d_font_ext->DrawText(NULL, text_string, -1, &rect, DT_LEFT | DT_VCENTER, bg_color);

			OffsetRect(&rect, v_offset, -v_offset);
			m_d3d_font_ext->DrawText(NULL, text_string, -1, &rect, DT_LEFT | DT_VCENTER, bg_color);

			OffsetRect(&rect, -v_offset, 0);
			m_d3d_font_ext->DrawText(NULL, text_string, -1, &rect, DT_LEFT | DT_VCENTER, text_color);

			D3DXVECTOR3 v_center = D3DXVECTOR3((rect.right + rect.left) * 0.5f, (rect.bottom + rect.top) * 0.5f, 0);
			m_d3d_sprite->Draw(NULL, &rect, &v_center, NULL, 0xffffffff);
		}
		m_d3d_sprite->End();
	}

	long d3d9_render_device_impl::create_mesh_from_buffer(raw_buffer* vertices_buffer,
		raw_buffer* indices_buffer,
		raw_buffer* attribute_buffer,
		ulong vertices_count,
		ulong indices_count,
		ulong vertex_dcls,
		void** mesh_object)
	{
		LPD3DXMESH d3d_mesh = NULL;

		HRESULT hr = 0;
		if (FAILED(hr = D3DXCreateMeshFVF(indices_count / 3,
			vertices_count,
			D3DXMESH_MANAGED,
			vertex_dcls,
			m_d3d_device,
			&d3d_mesh)))
		{
			return 0;
		}

		int v_size = 0;

		//vertex
		void* v_vertices = NULL;
		hr = d3d_mesh->LockVertexBuffer(0, (void**)&v_vertices);
		{
			memcpy((byte*)v_vertices, vertices_buffer->get_buffer(), vertices_buffer->get_size());
		}
		hr = d3d_mesh->UnlockVertexBuffer();
		v_size += vertices_buffer->get_size();

		void* v_indices = NULL;
		hr = d3d_mesh->LockIndexBuffer(0, (void**)&v_indices);
		{
			memcpy((byte*)v_indices, indices_buffer->get_buffer(), indices_buffer->get_size());
		}
		hr = d3d_mesh->UnlockIndexBuffer();
		v_size += indices_buffer->get_size();

		if (attribute_buffer)
		{
			DWORD* v_attribute;

			hr = d3d_mesh->LockAttributeBuffer(0, &v_attribute);
			{
				memcpy((byte*)v_attribute, attribute_buffer->get_buffer(), attribute_buffer->get_size());
			}
			hr = d3d_mesh->UnlockAttributeBuffer();

			v_size += attribute_buffer->get_size();
		}

		d3d_mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);

		*mesh_object = (void*)d3d_mesh;

		return v_size;
	}


	long d3d9_render_device_impl::create_mesh_from_buffer(raw_buffer* vertices_buffer,
		raw_buffer* indices_buffer,
		raw_buffer* attribute_buffer,
		ulong vertices_count,
		ulong indices_count,
		ulong vertex_dcls,
		ulong options,
		void** mesh_object)
	{
		LPD3DXMESH d3d_mesh = NULL;

		HRESULT hr = 0;
		if (FAILED(hr = D3DXCreateMeshFVF(indices_count / 3,
			vertices_count,
			options,
			vertex_dcls,
			m_d3d_device,
			&d3d_mesh)))
		{
			return 0;
		}


		int v_size = 0;

		//vertex
		void* v_vertices = NULL;
		hr = d3d_mesh->LockVertexBuffer(0, (void**)&v_vertices);
		{
			memcpy((byte*)v_vertices, vertices_buffer->get_buffer(), vertices_buffer->get_size());
		}
		hr = d3d_mesh->UnlockVertexBuffer();
		v_size += vertices_buffer->get_size();

		void* v_indices = NULL;
		hr = d3d_mesh->LockIndexBuffer(0, (void**)&v_indices);
		{
			memcpy((byte*)v_indices, indices_buffer->get_buffer(), indices_buffer->get_size());
		}
		hr = d3d_mesh->UnlockIndexBuffer();
		v_size += indices_buffer->get_size();

		if (attribute_buffer)
		{
			DWORD* v_attribute;

			hr = d3d_mesh->LockAttributeBuffer(0, &v_attribute);
			{
				memcpy((byte*)v_attribute, attribute_buffer->get_buffer(), attribute_buffer->get_size());
			}
			hr = d3d_mesh->UnlockAttributeBuffer();

			v_size += attribute_buffer->get_size();
		}

		d3d_mesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL);

		*mesh_object = (void*)d3d_mesh;

		return v_size;
	}

	long d3d9_render_device_impl::create_mesh_from_file_in_memory(raw_buffer* in_buffer, material_set* mtl_buffer, void** mesh_object)
	{
		LPD3DXMESH v_mesh = (LPD3DXMESH)(*mesh_object);

		ID3DXBuffer* material_buffer;
		ID3DXBuffer* effect_buffer;
		ID3DXBuffer* face_buffer;
		ulong v_texture_count;
		if (FAILED(D3DXLoadMeshFromXInMemory(in_buffer->get_buffer(),
			in_buffer->get_size(),
			D3DXMESH_MANAGED,
			m_d3d_device,
			&face_buffer,
			&material_buffer,
			&effect_buffer,
			&v_texture_count,
			&v_mesh)))
		{
			return 0;
		}


		int v_index_size = 2;
		if (v_mesh->GetOptions() == D3DXMESH_32BIT)
			v_index_size = 4;
		long v_size = v_mesh->GetNumBytesPerVertex() * (v_mesh->GetNumVertices() + v_index_size) +
			v_mesh->GetNumFaces() * (v_index_size * 3 + 4);

		// fill the extenal_material
		D3DXMATERIAL* materials = (D3DXMATERIAL*)material_buffer->GetBufferPointer();
		for (int i = 0; i < (int)v_texture_count; i++)
		{
			texture_file tf;

			if (materials[i].pTextureFilename)
			{
#ifdef _UNICODE
				tf.m_diffuse_texture = string_ext::to_wstring(materials[i].pTextureFilename);
#else
				tf.m_diffuse_texture = materials[i].pTextureFilename;
#endif //_UNICODE
				US_STRING_LOWER(tf.m_diffuse_texture);
				//std::transform( tf.m_diffuse_texture.begin(), tf.m_diffuse_texture.end(), tf.m_diffuse_texture.begin(),::tolower );
			}

			mtl_buffer->m_texture_files.push_back(tf);

			material mat;
			mat.m_ambient_rgb = color_value((float*)&materials[i].MatD3D.Ambient);
			mat.m_diffuse_rgba = color_value((float*)&materials[i].MatD3D.Diffuse);
			mat.m_emissive_rgb = color_value((float*)&materials[i].MatD3D.Emissive);
			mat.m_power = materials[i].MatD3D.Power;
			mat.m_specular_rgb = color_value((float*)&materials[i].MatD3D.Specular);

			mtl_buffer->m_materials.push_back(mat);

		}

		*mesh_object = v_mesh;

		AUTO_RELEASE(face_buffer);
		AUTO_RELEASE(material_buffer);
		AUTO_RELEASE(effect_buffer);

		return v_size;
	}

	long d3d9_render_device_impl::create_texture_from_buffer(raw_buffer* in_buffer, texture_info* tex_info, void** tex_object)
	{
		long v_size = 0;

		LPDIRECT3DTEXTURE9 v_texture;
		HRESULT hr = D3DXCreateTexture(m_d3d_device,
			tex_info->width,
			tex_info->height,
			tex_info->mipmap,
			D3DX_DEFAULT,
			(D3DFORMAT)tex_info->format,
			D3DPOOL_MANAGED, &v_texture);

		if (SUCCEEDED(hr))
		{
			D3DLOCKED_RECT rLocked;
			unsigned short* pData;
			v_texture->LockRect(0, &rLocked, NULL, NULL);
			int nSize = rLocked.Pitch;
			pData = (unsigned short*)rLocked.pBits;
			if (NULL != pData)
			{
				memcpy(pData, in_buffer->get_buffer(), in_buffer->get_size());
			}
			v_texture->UnlockRect(0);
			//D3DXFilterTexture(v_texture, NULL, 0, D3DX_DEFAULT);

			*tex_object = (void*)v_texture;

			v_size = in_buffer->get_size();

			return v_size;
		}

		return v_size;
	}

	long d3d9_render_device_impl::create_texture_from_file(const LPCTSTR texture_path, void** tex_object)
	{
		HRESULT hr = D3DXCreateTextureFromFile(m_d3d_device, texture_path, (LPDIRECT3DTEXTURE9*)tex_object);
		if (SUCCEEDED(hr))
		{
			LPDIRECT3DTEXTURE9 v_text = (LPDIRECT3DTEXTURE9)(*tex_object);
			return 1;
		}

		return 0;
	}

	long d3d9_render_device_impl::create_texture_from_file_in_memory(raw_buffer* in_buffer, void** tex_object)
	{
		HRESULT hr = D3DXCreateTextureFromFileInMemory(m_d3d_device, in_buffer->get_buffer(), in_buffer->get_size(), (LPDIRECT3DTEXTURE9*)tex_object);
		if (SUCCEEDED(hr))
		{
			return in_buffer->get_size();
		}

		return 0;
	}

	long d3d9_render_device_impl::create_texture_from_resource(ULONG_PTR handle, void** tex_object)
	{
		//#ifndef USE_STATIC_LIB
		HRESULT hr = D3DXCreateTextureFromResource(m_d3d_device, g_hModule, MAKEINTRESOURCE(handle), (LPDIRECT3DTEXTURE9*)tex_object);
		if (SUCCEEDED(hr))
		{
			return 0;
		}
		//#endif
		return 0;
	}

	bool d3d9_render_device_impl::create_effect_from_resource(ULONG_PTR handle, void** effect)
	{
		//#ifndef USE_STATIC_LIB
		*effect = NULL;

		LPD3DXBUFFER d3dx_error_buffer = NULL;
		HRESULT hr = D3DXCreateEffectFromResourceEx(m_d3d_device,
			g_hModule,
			MAKEINTRESOURCE(handle),
			0,
			0,
			0,
			0,
			0,
			(LPD3DXEFFECT*)(effect),
			&d3dx_error_buffer);

		if (FAILED(hr))
		{
			if (d3dx_error_buffer)
			{
				ustring effect_log_path = singleton_system_environment::instance().m_application_path + L"shader.log";
				FILE* fp = _wfopen(effect_log_path.c_str(), L"at");
				fwrite(d3dx_error_buffer->GetBufferPointer(), d3dx_error_buffer->GetBufferSize(), 1, fp);
				fclose(fp);
				AUTO_RELEASE(d3dx_error_buffer);
			}
			//else
			//{
			//	ustring effect_log_path = singleton_system_environment::instance().m_application_path + L"shader.log";
			//	FILE* fp = _wfopen( effect_log_path.c_str(), L"at" );
			//	string str = string_ext::from_dword( (DWORD)hr ) + "_xx_" + string_ext::from_dword( (DWORD)g_hModule ) + "_xxxxxx\n";
			//	fwrite( str.c_str(), str.size(), 1, fp );
			//	fclose( fp );
			//	AUTO_RELEASE(d3dx_error_buffer);
			//}
			return false;
		}

		AUTO_RELEASE(d3dx_error_buffer);
		//#endif

		return true;
	}

	bool d3d9_render_device_impl::create_effect_from_file(const LPCTSTR effect_file, void** effect)
	{
		*effect = NULL;

		LPD3DXBUFFER d3dx_error_buffer = NULL;
		HRESULT hr = D3DXCreateEffectFromFileEx(
			m_d3d_device,
			effect_file,
			0,
			0,
			0,
			0,
			0,
			(LPD3DXEFFECT*)(effect),
			&d3dx_error_buffer);

		if (FAILED(hr))
		{
			if (d3dx_error_buffer)
			{
				ustring effect_log_path = singleton_system_environment::instance().m_application_path + L"shader.log";
				FILE* fp = _wfopen(effect_log_path.c_str(), L"at");
				fwrite(d3dx_error_buffer->GetBufferPointer(), d3dx_error_buffer->GetBufferSize(), 1, fp);
				fclose(fp);
				AUTO_RELEASE(d3dx_error_buffer);
			}
			return false;
		}

		AUTO_RELEASE(d3dx_error_buffer);

		return true;
	}

	bool d3d9_render_device_impl::create_vertex_buffer(ulong buffer_size, ulong usage, ulong fvf, ulong pool, void** vertex_buffer, void** shared_handle)
	{
		HRESULT hr = m_d3d_device->CreateVertexBuffer(buffer_size, usage, fvf, (D3DPOOL)pool, (IDirect3DVertexBuffer9**)vertex_buffer, (HANDLE*)shared_handle);

		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}

	bool d3d9_render_device_impl::create_index_buffer(ulong buffer_size, ulong usage, ulong d3d_format, ulong pool, void** index_buffer, void** shared_handle)
	{
		HRESULT hr = m_d3d_device->CreateIndexBuffer(buffer_size, usage, (D3DFORMAT)d3d_format, (D3DPOOL)pool, (IDirect3DIndexBuffer9**)index_buffer, (HANDLE*)shared_handle);

		if (FAILED(hr))
		{
			return false;
		}

		return true;
	}

	void* d3d9_render_device_impl::create_transparency_texture(ulong v_color)
	{
		LPDIRECT3DTEXTURE9 v_transparency_texture = NULL;

		HRESULT hr = D3DXCreateTexture(m_d3d_device,
			32,
			32,
			1,
			D3DX_DEFAULT,
			D3DFMT_A8B8G8R8,
			D3DPOOL_MANAGED, &v_transparency_texture);

		if (SUCCEEDED(hr))
		{
			D3DLOCKED_RECT rLocked;
			unsigned long* pData;
			v_transparency_texture->LockRect(0, &rLocked, NULL, NULL);
			pData = (unsigned long*)rLocked.pBits;
			if (NULL != pData)
			{
				for (int ni = 0; ni < 1024; ni++)
				{
					pData[ni] = v_color;
				}
			}
			v_transparency_texture->UnlockRect(0);
			D3DXFilterTexture(v_transparency_texture, NULL, 0, D3DX_DEFAULT);
		}

		return (void*)v_transparency_texture;
	}


	//void d3d9_render_device_impl::occlusion_test(axis_aligned_box* aabb, long* handle)
	//{
	//	long query_handle;
	//	if(m_occlusion_queries.empty())
	//	{
	//		for(int ni = 0;ni < MAX_OCCLUSION_COUNT; ni++)
	//		{
	//			m_d3d_device->CreateQuery(D3DQUERYTYPE_OCCLUSION,(IDirect3DQuery9**)&query_handle);
	//			m_occlusion_queries.push_back( )
	//		}
	//	}
	//	query_handle = m_occlusion_queries.front();
	//	m_occlusion_queries.pop_front();
	//	IDirect3DQuery9 * query = (IDirect3DQuery9*)query_handle;
	//	query->Issue(D3DISSUE_BEGIN);
	//	render_helper::draw_filled_box( device, aabb->get_corners(), 0xffffffff );
	//	query->Issue(D3DISSUE_END);
	//	*handle = query_handle;
	//	return S_OK;
	//}

	//long d3d9_render_device_impl::get_pixel_count( long handle )
	//{
	//	if(handle < 0) { return 1000000; }
	//	IDirect3DQuery9 * query = (IDirect3DQuery9*)handle;
	//	DWORD size = 0;
	//	while(query->GetData(&size,sizeof(size),D3DGETDATA_FLUSH) == S_FALSE);
	//	m_occlusion_queries.push_back(handle);
	//	long pixel_count = size & 0x7FFFFFFF;
	//	return pixel_count;
	//}



}
