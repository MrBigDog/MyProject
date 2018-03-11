///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usm_filter_v1.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usm_filter_v1 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USM_FILTER_V1_H_
#define _US_USM_FILTER_V1_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class LIB_FILTER_API usm_filter_v1
		: public filter_base
	{
	public:
		usm_filter_v1( void ) { }

		virtual ~usm_filter_v1( void ) { }

	public:
		virtual object_base* parse( raw_buffer* in_buffer );

	public:
		model_lod_hardware_mesh* parse_model_lod_hardware_mesh( building_create_buffer* uncompress_buf );
		
	protected:
		int read_vertex( building_create_buffer* v_stream, model_lod_hardware_mesh* v_mesh );

		int read_index_attribute( building_create_buffer* v_stream, model_lod_hardware_mesh* v_mesh );

		int read_aabb( building_create_buffer* v_stream, model_lod_hardware_mesh* v_mesh );

		int read_material( building_create_buffer* v_stream, model_lod_hardware_mesh* v_mesh );

		int read_figure( building_create_buffer* v_stream, model_lod_hardware_mesh* v_mesh );

		int read_topologic( building_create_buffer* v_stream, model_lod_hardware_mesh* v_mesh );

	protected:
		bool m_build_normal;
	};
}

#endif // _US_USM_FILTER_V1_H_ 