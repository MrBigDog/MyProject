///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_parser_assistant.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_parser_assistant class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_PARSER_ASSISTANT_H_
#define _US_USX_PARSER_ASSISTANT_H_

#include <usDotx/Export.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_singleton.h>
#include <map>
#include <wtypes.h>

struct ID3DXFileData;

namespace uniscope_globe
{
	class usx_parser;
	class usx_parser_object_container;

	class USDOTX_EXPORT usx_parser_assistant
	{
	public:
		usx_parser_assistant(void);

		virtual ~usx_parser_assistant(void);

		typedef std::map<ustring, usx_parser*> dotx_file_parser_map;

	public:
		usx_parser* get_usx_parser(const LPCTSTR strType);

		bool parse_x_file(char *file_name, usx_parser_object_container* in_container);

		bool parse_x_buffer(unsigned char *pBuffer, ulong nSize, usx_parser_object_container* in_container);

		bool parse_simple_object(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container, bool is_reference);

		ustring get_guid(ID3DXFileData* dotx_file_data);

		ustring get_name(ID3DXFileData* dotx_file_data);

	private:
		bool parse_mesh_normals(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container, bool is_reference);

		bool parse_material_list(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container, bool is_reference);

		bool parse_mesh_texture_coords(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container, bool is_reference);

		bool parse_material(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container, bool is_reference);

		bool parse_vertex_colors(ID3DXFileData* dotx_file_data, ID3DXFileData* parent_dotx_file_data, ulong depth, void** data, usx_parser_object_container* in_container, bool is_reference);

	protected:
		dotx_file_parser_map m_dotx_file_parser_map;
	};


	class USDOTX_EXPORT singleton_usx_parser_assistant
		: public singleton<usx_parser_assistant>
	{
	public:
		singleton_usx_parser_assistant(void)
		{
		}

		virtual ~singleton_usx_parser_assistant(void)
		{
		}
	};

}

#endif // _US_USX_PARSER_ASSISTANT_H_