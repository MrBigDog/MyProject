
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_complex_mesh_exporter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_complex_mesh_exporter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_COMPLEX_MESH_EXPORTER_H_
#define _US_USX_COMPLEX_MESH_EXPORTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class usx_complex_mesh_exporter
	{
	public:
		usx_complex_mesh_exporter( void ) {}

		virtual ~usx_complex_mesh_exporter( void ) {}

	public:
		static bool do_export( ID3DXFileSaveObject* save_obj, d3d9_complex_hardware_mesh* v_mesh, cpstr dest_path, axis_aligned_box<float>& out_box );

	private:
		//static bool export_frame( ID3DXFileSaveObject* save_obj, usx_frame* v_frame, cpstr dest_path, axis_aligned_box<float>& out_box, matrix4<float> in_mat );

		//static bool export_frame(ID3DXFileSaveData* save_obj, usx_frame* v_frame, cpstr dest_path, axis_aligned_box<float>& out_box, matrix4<float> in_mat );

		static bool export_mesh( ID3DXFileSaveObject* save_obj, usx_mesh* v_mesh, cpstr dest_path, axis_aligned_box<float>& out_box, matrix4<float> in_mat );

		static bool export_mesh( ID3DXFileSaveData* save_obj, usx_mesh* v_mesh, cpstr dest_path, axis_aligned_box<float>& out_box, matrix4<float> in_mat );

		static bool export_normal( ID3DXFileSaveData* save_data, usx_mesh* v_mesh );

		static bool export_texture_coords( ID3DXFileSaveData* save_data, usx_mesh* v_mesh );

		static bool export_color( ID3DXFileSaveData* save_data, usx_mesh* v_mesh );

		static bool export_dcls( ID3DXFileSaveData* save_data, usx_mesh* v_mesh );

		static bool export_attribute( ID3DXFileSaveData* save_data, usx_mesh* v_mesh, cpstr dest_path );

		static bool export_materials( ID3DXFileSaveData* save_data, usx_mesh* v_mesh, cpstr dest_path );
	
		static bool export_skin( ID3DXFileSaveData* save_data, usx_skin_info* in_skin );

		 
	};
}

#endif // _US_USX_COMPLEX_MESH_EXPORTER_H_ 