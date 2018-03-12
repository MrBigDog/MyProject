///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_tin_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : tin_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TIN_FILTER_H_
#define _US_TIN_FILTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class LIB_FILTER_API tin_filter
		: public filter_base
	{
	public:
		tin_filter( void ) {}

		virtual ~tin_filter( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );

	private:
		object_base* parse_from_compressed_stream( raw_buffer* stream, interior_index index );
		 
	};


	class LIB_FILTER_API tin_filter_v2
		: public filter_base
	{
	public:
		tin_filter_v2( void ) {}

		virtual ~tin_filter_v2( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );

	private:
		object_base* parse_from_compressed_stream( raw_buffer* stream, interior_index index );

	};

	class LIB_FILTER_API tin_filter_v4
		: public filter_base
	{
	public:
		tin_filter_v4( void ) {}

		virtual ~tin_filter_v4( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );

	private:
		object_base* parse_from_compressed_stream( raw_buffer* stream, interior_index index );

	};


	class LIB_FILTER_API dem_mesh_filter
		: public filter_base
	{
	public:
		dem_mesh_filter( void ) {}

		virtual ~dem_mesh_filter( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );

	private:
		object_base* parse_from_compressed_stream( raw_buffer* stream, interior_index index );

	};


	struct segment_head_aux
	{
		short  segment_type;		// us_segment_type
		short  segment_child_count;
		segment_head_aux()
		{
			segment_type = -1;
			segment_child_count = 0;
		}
	};

	struct segment_head_node
	{
		short  segment_type;		// us_segment_type
		short  segment_version;
		segment_head_node()
		{
			segment_type = -1;
			segment_version = 0;
		}
	};

	struct segment_head_mesh_data
	{
		ulong  segment_type;		// us_segment_type
		ulong  data_length;
		segment_head_mesh_data()
		{
			segment_type = -1;
			data_length = 0;
		}
	};

	struct segment_head_texture_data
	{
		ulong  segment_type;		// us_segment_type
		ulong  data_length;
		segment_head_texture_data()
		{
			segment_type = -1;
			data_length = 0;
		}
	};



	class LIB_FILTER_API dem_mesh_filter_v2
		: public filter_base
	{
	public:
		dem_mesh_filter_v2( void ) {}

		virtual ~dem_mesh_filter_v2( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );

	private:
		object_base* parse_from_compressed_stream( raw_buffer* stream, interior_index index );

	};


	class LIB_FILTER_API dem_mesh_filter_v3
		: public filter_base
	{
	public:
		dem_mesh_filter_v3( void ) {}

		virtual ~dem_mesh_filter_v3( void ) {}

	public:
		virtual object_base* parse( raw_buffer* in_buffer );

	private:
		object_base* parse_from_compressed_stream( raw_buffer* stream, interior_index index );

	};

	class LIB_FILTER_API dem_mesh_filter_v4
		: public filter_base
	{
	public:
		virtual object_base* parse( raw_buffer* in_buffer );

	private:
		object_base* parse_from_compressed_stream( raw_buffer* stream, interior_index index );

	};
}

#endif // _US_TIN_FILTER_H_




