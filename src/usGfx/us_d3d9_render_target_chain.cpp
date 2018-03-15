
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_target_chain.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_target_chain class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_render_target_chain.h"

namespace uniscope_globe
{

#define WRAP_VALUE(curValue, minValue, maxValue) ( ((curValue)>(maxValue)?(minValue):(curValue))<(minValue)?(maxValue):(curValue))


	d3d9_render_target_chain::d3d9_render_target_chain( render_device* rd )
	{
		m_device = rd;
		m_d3d_device = (LPDIRECT3DDEVICE9)(rd->get_device());
		//m_device->m_event_reset_device += event_handle(this, &d3d9_render_target_chain::on_reset_device );
		//m_device->m_event_lost_device += event_handle(this, &d3d9_render_target_chain::on_lost_device );
		m_old_target_surface = NULL;
		m_chain_count = 2;
		m_texture_array.resize(m_chain_count);
		m_surface_array.resize(m_chain_count);
		m_cur_curser = 0;
		m_next_curser = WRAP_VALUE(m_cur_curser+1, 0, m_chain_count - 1);
	}

	d3d9_render_target_chain::~d3d9_render_target_chain( void )
	{
		m_device->m_event_reset_device -= event_handle(this, &d3d9_render_target_chain::on_reset_device );
		m_device->m_event_lost_device -= event_handle(this, &d3d9_render_target_chain::on_lost_device );
		destroy();

	}

	bool d3d9_render_target_chain::create( uint width, uint height, int format, int multi_sample_type, ulong multi_sample_quality, bool lockable )
	{
		destroy();
		for(int ni = 0; ni < m_surface_array.size(); ni++)
		{
			if(FAILED(m_d3d_device->CreateRenderTarget( width, height,(D3DFORMAT)format,(D3DMULTISAMPLE_TYPE)multi_sample_type, multi_sample_quality, lockable, &m_surface_array[ni], NULL )))
			{
				return false;
			}
		}

		for(int ni = 0; ni < m_texture_array.size(); ni++)
		{
			if(FAILED(D3DXCreateTexture(m_d3d_device, width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_texture_array[ni])))			
			{
				return false;
			}
		}

		return true;
	}

	void d3d9_render_target_chain::destroy( void )
	{
		for(int ni = 0; ni < m_surface_array.size(); ni++)
		{
			AUTO_RELEASE(m_surface_array[ni]);
		}

		for(int ni = 0; ni < m_texture_array.size(); ni++)
		{
			AUTO_RELEASE(m_texture_array[ni]);
		}
	}

	void d3d9_render_target_chain::on_lost_device( event_argument* args )
	{
		this->destroy();
	}

	void d3d9_render_target_chain::on_reset_device( event_argument* args )
	{
		d3d9_render_device_impl* v_device = (d3d9_render_device_impl*)m_device;
		this->create( v_device->get_backbuffer_width(), v_device->get_backbuffer_height(), v_device->get_backbuffer_format(), v_device->get_backbuffer_multisampletype(), v_device->get_backbuffer_multisamplequality());
	}

	void d3d9_render_target_chain::begin( void )
	{
		m_d3d_device->GetRenderTarget( 0, &m_old_target_surface );
	}

	void d3d9_render_target_chain::end( void )
	{
		m_d3d_device->SetRenderTarget( 0, m_old_target_surface );
		AUTO_RELEASE(m_old_target_surface);
	}

	ULONG_PTR d3d9_render_target_chain::get_texture( void )
	{
		return (ULONG_PTR)m_texture_array[m_next_curser];
	}

	void d3d9_render_target_chain::draw_texture( int stage )
	{
		m_d3d_device->SetTexture(stage, m_texture_array[m_cur_curser]);
	}

	int wrap_value(int curValue, int minValue, int maxValue)
	{
		curValue = curValue>maxValue?minValue:curValue;
		return curValue<minValue?maxValue:curValue;
	}

	void d3d9_render_target_chain::flip( void )
	{
		LPDIRECT3DSURFACE9	v_target = NULL;
		m_texture_array[m_next_curser]->GetSurfaceLevel(0, &v_target);
		m_d3d_device->StretchRect( m_surface_array[m_cur_curser], NULL, v_target, NULL, D3DTEXF_NONE );
		AUTO_RELEASE(v_target);

		//( ((curValue)>(maxValue)?(minValue):(curValue))<(minValue)?(maxValue):(curValue))


		m_cur_curser = wrap_value( m_cur_curser + 1, 0, m_chain_count - 1);
		m_next_curser = wrap_value( m_cur_curser + 1, 0, m_chain_count - 1);
	}

	void d3d9_render_target_chain::set_render_target( int n_stage )
	{
		m_d3d_device->SetRenderTarget( 0, m_surface_array[m_cur_curser] );
	}

	
}