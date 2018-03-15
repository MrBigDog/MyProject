///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_frame_parser.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_frame_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_FRAME_PARSER_H_
#define _US_USX_FRAME_PARSER_H_
#include <usDotx/Export.h>
#include <usDotx/us_usx_parser.h>

#include <usUtil/us_common_file.h>

struct ID3DXFileData;

namespace uniscope_globe
{
	class usx_parser_object_container;

	class USDOTX_EXPORT usx_frame_parser
		: public usx_parser
	{
	public:
		usx_frame_parser(void);

		virtual ~usx_frame_parser(void);

	public:
		virtual bool parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container);

	protected:
		ulong m_frame_counter;

	};

	class usx_frame_transform_matrix_parser
		: public usx_parser
	{
	public:
		usx_frame_transform_matrix_parser(void);

		virtual ~usx_frame_transform_matrix_parser(void);

	public:
		virtual bool parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container);

	};
}

#endif // _US_USX_FRAME_PARSER_H_