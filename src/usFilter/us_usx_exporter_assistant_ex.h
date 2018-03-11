///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_exporter_assistant_ex.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_exporter_assistant_ex class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_EXPORTER_ASSISTANT_EX_H_
#define _US_USX_EXPORTER_ASSISTANT_EX_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class usx_exporter_assistant_ex
	{
	public:
		usx_exporter_assistant_ex( void ) {}

		virtual ~usx_exporter_assistant_ex( void ) {}

	public:
		bool save_usx( hardware_mesh* mesh, cpstr str_request_path, ustring& out_final_path,  axis_aligned_box<float>& out_box );

	public:
		static ustring export_resource(cpstr src_path, cpstr dest_directory, cpstr dest_file_name );

	private:
		bool export_single_mesh( d3d9_hardware_mesh* v_mesh, cpstr str_path );

		bool export_complex_hardware_mesh( d3d9_complex_hardware_mesh* v_mesh, cpstr str_path );


		//bool export_mesh( ID3DXFileSaveObject* save_obj, d3d9_hardware_mesh* v_mesh, cpstr str_mesh_path );
		 
	};

	class LIB_FILTER_API singleton_usx_exporter_assistant_ex 
		: public singleton<usx_exporter_assistant_ex>
	{
	public:
		singleton_usx_exporter_assistant_ex( void )
		{
		}

		virtual ~singleton_usx_exporter_assistant_ex( void )
		{
		}
	};
}

#endif // _US_USX_EXPORTER_ASSISTANT_EX_H_ 