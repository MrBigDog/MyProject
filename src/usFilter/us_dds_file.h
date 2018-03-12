///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_dds_file.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : dds_file class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_DDS_FILE_H_
#define _US_DDS_FILE_H_

#include <usFilter\Export.h>

namespace uniscope_globe
{
	class USFILTER_EXPORT dds_file
	{
	public:
		dds_file( void );

		~dds_file( void );

	protected:
		int calculate_levl(int src_col);

		int calculate_size(int in_row, int in_col, int mip_level);

	public:
		void create_dxt1( byte* dib_buffer, long width, long height, long mipmap_level, bool file_header );

		void create_dxt5( byte* dib_buffer, long width, long height, long mipmap_level, bool file_header );

		bool create_argb( byte* dib_buffer, long width, long height, long mipmap_level, bool file_header );

		raw_buffer* get_data( void ) { return m_dxt_buffer; }

	public:
		texture_info m_tex_info;

		long m_memory_size;

		long m_mipmap_level;

		//long m_width;

		//long m_height;

		raw_buffer* m_dxt_buffer;
		 
	};
}

#endif // _US_DDS_FILE_H_