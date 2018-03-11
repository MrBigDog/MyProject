///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usm_filter_v1_shadow.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usm_filter_v1_shadow class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USM_FILTER_V1_SHADOW_H_
#define _US_USM_FILTER_V1_SHADOW_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class usm_filter_v1_shadow
	{
	public:
		usm_filter_v1_shadow( void );

		virtual ~usm_filter_v1_shadow( void );

	public:
		virtual void* parse( raw_buffer* in_buffer );

	public:
		d3d9_hardware_mesh_buffer* parse_d3d9_hardware_mesh( managed_buffer* uncompress_buf );

	protected:
		int read_vertex( managed_buffer* v_stream, d3d9_hardware_mesh_buffer* v_mesh );

		int read_index_attribute( managed_buffer* v_stream, d3d9_hardware_mesh_buffer* v_mesh );

		int read_aabb( managed_buffer* v_stream, d3d9_hardware_mesh_buffer* v_mesh );

		int read_material( managed_buffer* v_stream, d3d9_hardware_mesh_buffer* v_mesh );

		int read_figure( managed_buffer* v_stream, d3d9_hardware_mesh_buffer* v_mesh );

		int read_topologic( managed_buffer* v_stream, d3d9_hardware_mesh_buffer* v_mesh );
		 
	};
}

#endif // _US_USM_FILTER_V1_SHADOW_H_ 