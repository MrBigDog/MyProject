///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_hardware_vertex_buffer.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_hardware_vertex_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_HARDWARE_VERTEX_BUFFER_H_
#define _US_D3D9_HARDWARE_VERTEX_BUFFER_H_

#include <usGfx/Export.h>
#include <usUtil/us_common_file.h>

#include <d3d9.h>

namespace uniscope_globe
{
	class render_device;
	class raw_buffer;

	class USGFX_EXPORT d3d9_hardware_vertex_buffer
	{
	public:
		d3d9_hardware_vertex_buffer(void);

		virtual ~d3d9_hardware_vertex_buffer(void);

		virtual bool create(render_device* device, raw_buffer* vertices_buffer, ulong fvf);

		virtual void* data(void);

	protected:
		LPDIRECT3DVERTEXBUFFER9	m_vertex_buffer;

	};
}

#endif // _US_D3D9_HARDWARE_VERTEX_BUFFER_H_
