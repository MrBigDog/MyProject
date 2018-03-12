///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_exporter_assistant_ex.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_exporter_assistant_ex class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_usx_exporter_assistant_ex.h"

namespace uniscope_globe
{
	usx_exporter_assistant_ex* singleton_usx_exporter_assistant_ex::s_ins = NULL;

	bool usx_exporter_assistant_ex::save_usx( hardware_mesh* v_mesh, cpstr str_request_path, ustring& out_final_path, axis_aligned_box<float>& out_box )
	{
		if(v_mesh == NULL) return false;

		// 创建File
		CComPtr<ID3DXFile> d3dx_file;
		if ( FAILED( D3DXFileCreate(&d3dx_file) ) )
		{
			return false;
		}

		// 注册模版
		//static char const * str_extensions = XEXTENSIONS_TEMPLATES;
		if ( FAILED( d3dx_file->RegisterTemplates((void*)usx_template::get_default_template(), D3DRMXTEMPLATEBYTES) ) )
		{
			return false;
		}

		if( FAILED(d3dx_file->RegisterTemplates((void*)XEXTENSIONS_TEMPLATES, strlen(XEXTENSIONS_TEMPLATES)) ) )
		{
			return false;
		}

		if( FAILED( d3dx_file->RegisterTemplates((void*)XSKINEXP_TEMPLATES, strlen(XSKINEXP_TEMPLATES) ) ) )
		{
			return false;
		}

		// 使用二进制模式保存
		D3DXF_FILEFORMAT file_format = D3DXF_FILEFORMAT_TEXT;

		//bool use_binary_mode = false;
		//if ( use_binary_mode )
		//{
		//	file_format = D3DXF_FILEFORMAT_BINARY | D3DXF_FILEFORMAT_COMPRESSED;
		//}

		// delete existing file with same name
		string str_file = string_ext::from_wstring( str_request_path );
		DeleteFileA(str_file.c_str());


		// 创建对象
		CComPtr<ID3DXFileSaveObject> file_save_obj;
		if( FAILED( d3dx_file->CreateSaveObject( str_file.c_str(), D3DXF_FILESAVE_TOFILE, file_format, &file_save_obj) ) )
		{
			return false;
		}

		// export_mesh
		if( v_mesh->get_rtti() == US_RTTI_D3D9_COMPLEX_HARDWARE_MESH )
		{
			usx_complex_mesh_exporter::do_export( file_save_obj.p, dynamic_cast<d3d9_complex_hardware_mesh*>(v_mesh), str_request_path, out_box );
		}
		else
		{
			usx_hardware_mesh_exporter::do_export( file_save_obj.p, dynamic_cast<d3d9_hardware_mesh*>(v_mesh), str_request_path, out_box );
		}

		if ( FAILED( file_save_obj->Save() ) )
		{
			return false;
		}

		out_final_path = file_directory::get_file_name( str_request_path );

		return true;
	}

	ustring usx_exporter_assistant_ex::export_resource(cpstr src_path, cpstr dest_directory, cpstr dest_file_name )
	{
		ustring src_link = src_path;

// 		if( file_directory::is_local_file( src_path ))
		{
			ustring str_dest_file_path = dest_directory;
			str_dest_file_path += dest_file_name;
			data_loader v_loader;
			raw_buffer* v_buffer = v_loader.load(src_path);
			if(v_buffer != NULL )
			{
				// save_raw_buffer;
				file_buffer v_file_buffer;
				v_file_buffer.file_open(str_dest_file_path.c_str(), L"wb");
				v_file_buffer.write(v_buffer->get_buffer(), v_buffer->get_size());
				v_file_buffer.file_close();
			}

			return dest_file_name;
		}

		return src_link;
	}

	
}