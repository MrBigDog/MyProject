///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_decl_data_parser.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_decl_data_parser class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_DECL_DATA_PARSER_H_
#define _US_USX_DECL_DATA_PARSER_H_
#include <usDotx/Export.h>
#include <usDotx/us_usx_parser.h>
#include <usDotx/us_usx_data_struct.h>
#include <usUtil/us_common_file.h>

struct ID3DXFileData;

namespace uniscope_globe
{
	class usx_mesh;
	class usx_parser_object_container;

	class USDOTX_EXPORT usx_decl_data_parser
		: public usx_parser
	{
	public:
		usx_decl_data_parser(void);

		virtual ~usx_decl_data_parser(void);

	public:
		virtual bool parse_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container);

	private:
		int get_element_number(ulong element_type);

		void parse_decl_texture_coord(ulong& offset, ulong* decl_data, usx_vertex_element& vertex_element, usx_mesh* mesh_data);

	};
}

#endif // _US_USX_DECL_DATA_PARSER_H_