///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_effect.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_effect class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_render_effect.h"

namespace uniscope_globe
{
	d3d9_render_effect::d3d9_render_effect(  render_device* rd  )
	{
		m_render_device = rd;
		m_d3dx_effect = NULL;
	}

	d3d9_render_effect::~d3d9_render_effect( void )
	{
		AUTO_RELEASE(m_d3dx_effect);
	}

	bool d3d9_render_effect::load_form_file( const LPCTSTR technique_file )
	{
		if( !m_render_device->create_effect_from_file( technique_file, (void**)&m_d3dx_effect) )
		{
			assert( m_d3dx_effect == NULL );
			return false;
		}

		return setup_handle();
	}

	bool d3d9_render_effect::load_form_module_resource( long resource_handle )
	{
		if( !m_render_device->create_effect_from_resource( resource_handle, (void**)&m_d3dx_effect) )
		{
			assert( m_d3dx_effect == NULL );
			return false;
		}

		return setup_handle();
	}

	bool d3d9_render_effect::setup_handle( void )
	{
		return true;
	}

	long d3d9_render_effect::on_lost_device( void )
	{
		if (! m_d3dx_effect) return 0;
		return m_d3dx_effect->OnLostDevice();
	}

	long d3d9_render_effect::on_reset_device( void )
	{
		if (! m_d3dx_effect) return 0;
		return m_d3dx_effect->OnResetDevice();
	}

	bool d3d9_render_effect::begin( uint* pass, ulong flags )
	{
		m_d3dx_effect->Begin(pass, 0);
		return true;
	}

	bool d3d9_render_effect::end( void )
	{
		m_d3dx_effect->End();
		return true;
	}

	bool d3d9_render_effect::begin_pass( uint pass )
	{
		m_d3dx_effect->BeginPass( pass );
		return true;
	}

	bool d3d9_render_effect::end_pass(void)
	{
		m_d3dx_effect->EndPass();
		return true;
	}

	bool d3d9_render_effect::set_technique(void)
	{
		m_d3dx_effect->SetTechnique(m_technique);

		return true;
	}

	bool d3d9_render_effect::commit_changes(void)
	{
		m_d3dx_effect->CommitChanges();
		return true;
	}

	
}