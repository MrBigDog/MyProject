///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_base.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_effect_base.h"
#include <usCore/us_render_device.h>

namespace uniscope_globe
{
	d3d9_effect_base::d3d9_effect_base(render_device* rd)
	{
		m_device = rd;
		m_d3dx_effect = NULL;
	}

	d3d9_effect_base::~d3d9_effect_base(void)
	{
		AUTO_RELEASE(m_d3dx_effect);
	}

	bool d3d9_effect_base::load_from_file(const LPCTSTR technique_file)
	{
		if (!m_device->create_effect_from_file(technique_file, (void**)&m_d3dx_effect))
		{
			//assert( m_d3dx_effect == NULL );
			return false;
		}

		return setup_handle();
	}

	bool d3d9_effect_base::load_from_module_resource(ULONG_PTR resource_handle)
	{
		if (!m_device->create_effect_from_resource(resource_handle, (void**)&m_d3dx_effect))
		{
			//assert( m_d3dx_effect == NULL );
			return false;
		}

		return setup_handle();
	}

	bool d3d9_effect_base::setup_handle(void)
	{
		return true;
	}

	long d3d9_effect_base::on_lost_device(void)
	{
		if (!m_d3dx_effect) return 0;
		return m_d3dx_effect->OnLostDevice();
	}

	long d3d9_effect_base::on_reset_device(void)
	{
		if (!m_d3dx_effect) return 0;
		return m_d3dx_effect->OnResetDevice();
	}

	bool d3d9_effect_base::begin(uint* pass, ulong flags)
	{
		m_d3dx_effect->Begin(pass, 0);
		return true;
	}

	bool d3d9_effect_base::end(void)
	{
		m_d3dx_effect->End();
		return true;
	}

	bool d3d9_effect_base::begin_pass(uint pass)
	{
		m_d3dx_effect->BeginPass(pass);
		return true;
	}

	bool d3d9_effect_base::end_pass(void)
	{
		m_d3dx_effect->EndPass();
		return true;
	}

	bool d3d9_effect_base::set_technique(void)
	{
		//assert( m_d3dx_effect != NULL );
		//assert( m_technique != NULL );

		if (m_d3dx_effect == NULL || m_technique == NULL)
			return false;

		m_d3dx_effect->SetTechnique(m_technique);

		return true;
	}

	bool d3d9_effect_base::commit_changes(void)
	{
		m_d3dx_effect->CommitChanges();
		return true;
	}



}