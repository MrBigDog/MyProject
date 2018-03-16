///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_hardware_vertex_buffer.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_hardware_vertex_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_hardware_vertex_buffer.h"
#include <usCore/us_render_device.h>
#include <usUtil/us_raw_buffer.h>

namespace uniscope_globe
{
	d3d9_hardware_vertex_buffer::d3d9_hardware_vertex_buffer(void)
	{
		m_vertex_buffer = NULL;
	}

	d3d9_hardware_vertex_buffer::~d3d9_hardware_vertex_buffer(void)
	{
		AUTO_RELEASE(m_vertex_buffer);
	}

	bool d3d9_hardware_vertex_buffer::create(render_device* device, raw_buffer* vertices_buffer, ulong fvf)
	{
		AUTO_RELEASE(m_vertex_buffer);

		if (!device->create_vertex_buffer(vertices_buffer->get_size(), D3DUSAGE_WRITEONLY, fvf, D3DPOOL_MANAGED, (void**)&m_vertex_buffer, NULL))
			return false;

		void* data = NULL;
		m_vertex_buffer->Lock(0, 0, &data, 0);
		memcpy(data, (void*)vertices_buffer->get_buffer(), vertices_buffer->get_size());
		m_vertex_buffer->Unlock();

		return true;
	}

	void* d3d9_hardware_vertex_buffer::data(void)
	{
		return (void*)m_vertex_buffer;
	}

}