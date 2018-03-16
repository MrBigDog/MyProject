
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_target_offscreen.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_target_offscreen class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_render_target_offscreen.h"
#include <usGfx/us_d3d9_render_device_impl.h>
#include <usCore/us_render_device.h>
#include <usUtil/us_file_directory.h>

namespace uniscope_globe
{
	d3d9_render_target_offscreen::d3d9_render_target_offscreen( render_device* rd )
	{
		m_device = rd;
		m_d3d_device = (LPDIRECT3DDEVICE9)(rd->get_device());
		//m_device->m_event_reset_device += event_handle(this, &d3d9_render_target_offscreen::on_reset_device );
		//m_device->m_event_lost_device += event_handle(this, &d3d9_render_target_offscreen::on_lost_device );
		m_render_target_depth_buffer = 0;
		m_render_target				 = 0;
		//m_render_target_texture		 = 0;
		//m_offscreen_plain_surface    = 0;

		m_old_back_buffer			 = 0;
		m_old_depth_buffer			 = 0;
	}

	d3d9_render_target_offscreen::~d3d9_render_target_offscreen( void )
	{
		m_device->m_event_reset_device -= event_handle(this, &d3d9_render_target_offscreen::on_reset_device );
		m_device->m_event_lost_device -= event_handle(this, &d3d9_render_target_offscreen::on_lost_device );
		destroy();

	}

	bool d3d9_render_target_offscreen::create( uint width, uint height, int format, int multi_sample_type, ulong multi_sample_quality, bool lockable )
	{
		AUTO_RELEASE(m_render_target_depth_buffer);
		AUTO_RELEASE(m_render_target);
		HRESULT hr;
		if(FAILED(hr = m_d3d_device->CreateRenderTarget( width, height,(D3DFORMAT)format,(D3DMULTISAMPLE_TYPE)multi_sample_type, multi_sample_quality, lockable, &m_render_target, NULL )))
		{
			return false;
		}

		if(FAILED(m_d3d_device->CreateDepthStencilSurface( width, height, D3DFMT_D24S8, (D3DMULTISAMPLE_TYPE)multi_sample_type, multi_sample_quality, TRUE, &m_render_target_depth_buffer, NULL )))
		{
			return false;
		}

		return true;
	}

	void d3d9_render_target_offscreen::destroy( void )
	{
		AUTO_RELEASE(m_render_target_depth_buffer);
		AUTO_RELEASE(m_render_target);
	}

	void d3d9_render_target_offscreen::on_lost_device( event_argument* args )
	{
		this->destroy();
	}

	void d3d9_render_target_offscreen::on_reset_device( event_argument* args )
	{
		d3d9_render_device_impl* v_device = (d3d9_render_device_impl*)m_device;
		this->create( v_device->get_backbuffer_width(), v_device->get_backbuffer_height(), v_device->get_backbuffer_format(), v_device->get_backbuffer_multisampletype(), v_device->get_backbuffer_multisamplequality());
	}

	void d3d9_render_target_offscreen::begin( void )
	{
		HRESULT hr;
		hr = m_d3d_device->GetRenderTarget(0, &m_old_back_buffer);
		hr = m_d3d_device->GetDepthStencilSurface( &m_old_depth_buffer );

		hr = m_d3d_device->SetRenderTarget( 0, m_render_target );
		hr = m_d3d_device->SetDepthStencilSurface( m_render_target_depth_buffer );
	}

	void d3d9_render_target_offscreen::end( void )
	{
		m_d3d_device->SetRenderTarget( 0, m_old_back_buffer );
		m_d3d_device->SetDepthStencilSurface( m_old_depth_buffer );

		if( m_old_back_buffer )
		{
			int n_count = m_old_back_buffer->Release();
			m_old_back_buffer = NULL;
		}
		AUTO_RELEASE(m_old_back_buffer);
		AUTO_RELEASE(m_old_depth_buffer);
	}

	ULONG_PTR d3d9_render_target_offscreen::get_texture( void )
	{
		return (ULONG_PTR)m_render_target_texture;
	}

	void d3d9_render_target_offscreen::draw_texture( int stage )
	{
		m_d3d_device->SetTexture(stage, m_render_target_texture);
	}

	void d3d9_render_target_offscreen::get_surface_bits( LPVOID pBits )
	{
		HRESULT hr;
		D3DLOCKED_RECT d3dSrcRect;        

		if(FAILED(hr = m_d3d_device->GetRenderTargetData(m_render_target, m_offscreen_plain_surface)))
		{
			return;
		}

		if(FAILED(hr = m_offscreen_plain_surface->LockRect(&d3dSrcRect,NULL,D3DLOCK_READONLY|D3DLOCK_NO_DIRTY_UPDATE)))			
		{
			return;
		}

		BYTE *pSrc	=	(BYTE*) d3dSrcRect.pBits;
		BYTE *pDest	=	(BYTE*)	pBits;
		int nBytesPerPixel = BPPFromD3DFormat((D3DFORMAT)m_format)/8;

		for(int i=0, nPitch=0, nRevPitch = (m_height-1) * d3dSrcRect.Pitch; i< m_height; i++, nPitch += d3dSrcRect.Pitch, nRevPitch -= d3dSrcRect.Pitch)
		memcpy(pDest + nRevPitch, pSrc + nPitch, m_width * nBytesPerPixel);

		m_offscreen_plain_surface->UnlockRect();
	}

	void d3d9_render_target_offscreen::save_file( cpstr str_path, int width, int height )
	{
		IDirect3DSurface9 *pBackSurface=NULL;
		IDirect3DSurface9 *pLockSurface=NULL;
		IDirect3DSurface9 *pRenderTarget=NULL;

		//m_d3d_device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackSurface);
		D3DSURFACE_DESC desc;
		HRESULT hr = m_render_target->GetDesc(&desc);

		if(FAILED(m_d3d_device->CreateRenderTarget( width, height, desc.Format ,D3DMULTISAMPLE_NONE, 0, false, &pRenderTarget, NULL )))
		{
			return;
		}

		if(FAILED(hr = m_d3d_device->StretchRect(m_render_target, NULL, pRenderTarget, NULL, D3DTEXF_NONE )))
		{
			return;
		}

		m_d3d_device->CreateOffscreenPlainSurface(width, height,desc.Format,D3DPOOL_SYSTEMMEM,&pLockSurface,NULL);
		m_d3d_device->GetRenderTargetData(pRenderTarget, pLockSurface);

		int v_file_type = D3DXIFF_JPG;
		D3DXSaveSurfaceToFile(str_path, D3DXIMAGE_FILEFORMAT(v_file_type), pLockSurface, NULL, NULL);

		if (pBackSurface) pBackSurface->Release();
		if (pLockSurface) pLockSurface->Release();
		if (pRenderTarget) pRenderTarget->Release();
	}


	
}