
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_ust_filter_v2.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : ust_filter_v2 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_UST_FILTER_V2_H_
#define _US_UST_FILTER_V2_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class ust_filter_v2
	{
	public:
		ust_filter_v2( void ){}

		virtual ~ust_filter_v2( void ){}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );

	protected:
		void read_vertex_buffer( raw_buffer* in_buffer, d3d9_hardware_mesh* v_out_mesh );

		void read_face_buffer( raw_buffer* in_buffer, d3d9_hardware_mesh* v_out_mesh );

		void read_box( raw_buffer* in_buffer, d3d9_hardware_mesh* v_out_mesh );

		void read_material( document_base* in_doc, raw_buffer* in_buffer, d3d9_hardware_mesh* v_out_mesh );

		void read_figure( raw_buffer* in_buffer, d3d9_hardware_mesh* v_out_mesh );

	};
}

#endif // _US_UST_FILTER_V2_H_ 