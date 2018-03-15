///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_exporter_assistant.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_exporter_assistant class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_EXPORTER_ASSISTANT_H_
#define _US_USX_EXPORTER_ASSISTANT_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class usx_exporter_assistant
	{
	public:
		usx_exporter_assistant( void );

		virtual ~usx_exporter_assistant( void );

	public:
		bool save_usx( hardware_mesh* mesh, cpstr str_path );

	private:
		static ustring export_resource(cpstr src_path, cpstr dest_directory, cpstr dest_file_name );

	private:
		bool export_single_mesh( d3d9_hardware_mesh* v_mesh, cpstr str_path );

		bool export_complex_hardware_mesh( d3d9_complex_hardware_mesh* v_mesh, cpstr str_path );

		bool export_skinning_hardware_mesh( d3d9_skinning_hardware_mesh* v_mesh, cpstr str_path );

		bool export_mesh( ID3DXFileSaveObject* save_obj, d3d9_hardware_mesh* v_mesh, cpstr str_mesh_path );

	};



	class USFILTER_EXPORT singleton_usx_exporter_assistant 
		: public singleton<usx_exporter_assistant>
	{
	public:
		singleton_usx_exporter_assistant( void )
		{
		}

		virtual ~singleton_usx_exporter_assistant( void )
		{
		}
	};
}

#endif // _US_USX_EXPORTER_ASSISTANT_H_ 