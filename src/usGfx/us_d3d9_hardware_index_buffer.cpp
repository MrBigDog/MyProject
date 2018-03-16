///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_hardware_index_buffer.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_hardware_index_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_hardware_index_buffer.h"
#include <usUtil/us_raw_buffer.h>
#include <usCore/us_render_device.h>

namespace uniscope_globe
{
	d3d9_hardware_index_buffer::d3d9_hardware_index_buffer(void)
	{
		m_index_buffer = NULL;
	}

	d3d9_hardware_index_buffer::~d3d9_hardware_index_buffer(void)
	{
		AUTO_RELEASE(m_index_buffer);
	}

	bool d3d9_hardware_index_buffer::create(render_device* device, raw_buffer* vertices_buffer)
	{
		AUTO_RELEASE(m_index_buffer);

		if (!device->create_index_buffer(vertices_buffer->get_size(), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, (void**)&m_index_buffer, NULL))
			return false;

		return true;
	}

	void* d3d9_hardware_index_buffer::data(void)
	{
		return m_index_buffer;
	}
}