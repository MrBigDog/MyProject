///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_file_format_declear.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose :  
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_FILE_FORMAT_DECLEAR_H_
#define _US_FILE_FORMAT_DECLEAR_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	typedef struct USFILTER_EXPORT _VG_DataBlockHead
	{
		short nVersion;
		short nType;
		short nGridSize;
	}VG_DataBlockHead;


	struct USFILTER_EXPORT us_square_head
	{
		short version;
		short square_type;		// us_square_type
	};

	struct USFILTER_EXPORT us_common_segment
	{
		short			segment_type;		// us_segment_type
		unsigned long	data_size;
	};

	struct USFILTER_EXPORT us_zipped_segment
	{
		short			segment_type;		// us_segment_type
		unsigned long	unzipped_data_size;
		unsigned long	zipped_data_size;
	};

	struct USFILTER_EXPORT us_grid_segment_ex
	{
		unsigned short invalid_value;
		short divide_number;
		float elevation_max;				// vertex3d segment use this extent head
		float elevation_min;
	};

	struct USFILTER_EXPORT us_vertex_segment_ex
	{
		short			divide_number;
		float			elevation_max;
		float			elevation_min;
	};

	struct USFILTER_EXPORT us_vertex_segment
	{
		short			segment_type;
		unsigned long	data_size;
	};

	struct USFILTER_EXPORT us_index_segment
	{
		short			segment_type;
		unsigned long	data_size;
	};

	typedef struct USFILTER_EXPORT _us_grid_segment_head
	{
		short			segment_type;		// us_segment_type
		short			grid_size;
		long			data_length;
		float			max_height;
		float			min_height;
		long			interval_count;			
	} us_grid_segment_head;

	typedef struct USFILTER_EXPORT _us_zipped_grid_segment_head
	{
		short			segment_type;		// us_segment_type
		short			grid_size;
		float			max_height;
		float			min_height;
		long			interval_count;	
		long			data_length;
	} us_zipped_grid_segment_head;

	typedef struct USFILTER_EXPORT _us_vertex3d 
	{
		short x;
		short y;
		short h;
	} us_vertex3d;

	typedef struct USFILTER_EXPORT _us_zipped_segment_head 
	{
		short segment_type;		// us_segment_type
		unsigned long  unzipped_data_size;
		unsigned long  zipped_data_size;
	} us_zipped_segment_head;

	typedef struct USFILTER_EXPORT _us_zipped_tin_segment_head
	{
		short			segment_type;		// us_segment_type
		float			max_height;
		float			min_height;
		long			data_length;
	} us_zipped_tin_segment_head;

	typedef struct USFILTER_EXPORT _us_zipped_vertex_segment_head
	{		
		long  segment_type;			// =1
		long  vertex_count;			
		long  zip_data_size;		
		float max_height;			
		float min_height;			
		long  interval_count;
	} us_zipped_vertex_segment_head;

	typedef struct USFILTER_EXPORT _us_zipped_index_segment_head
	{		
		long  segment_type;			// =3
		long  face_count;			
		long  zip_data_size;		
	} us_zipped_index_segment_head;

	typedef struct USFILTER_EXPORT _us_jpeg_segment_head
	{
		short		segment_type;
		long		reverse_flags;
		long		zip_data_size;
	} us_jpeg_segment_head;

	typedef struct USFILTER_EXPORT _us_alpha_1bit_segment_head
	{
		short		segment_type;
		long		zip_data_size;
		long		unzip_data_size;
		
	} us_alpha_1bit_segment_head;


}

#endif // _US_FILE_FORMAT_DECLEAR_H_