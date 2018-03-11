///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_hardware_mesh_exporter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_hardware_mesh_exporter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_HARDWARE_MESH_EXPORTER_H_
#define _US_USX_HARDWARE_MESH_EXPORTER_H_
#include <usFilter\Export.h>

namespace uniscope_globe
{
	class usx_hardware_mesh_exporter
	{
	public:
		usx_hardware_mesh_exporter( void ) {}

		virtual ~usx_hardware_mesh_exporter( void ) {}

	public:
		static bool do_export(ID3DXFileSaveObject* save_obj, d3d9_hardware_mesh* v_mesh, cpstr dest_path, axis_aligned_box<float>& out_box );
		 
	};
}

#endif // _US_USX_HARDWARE_MESH_EXPORTER_H_ 