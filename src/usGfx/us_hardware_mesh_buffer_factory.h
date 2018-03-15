///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_mesh_buffer_factory.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_mesh_buffer_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_HARDWARE_MESH_BUFFER_FACTORY_H_
#define _US_HARDWARE_MESH_BUFFER_FACTORY_H_

namespace uniscope_globe
{
	class hardware_mesh_buffer_factory
	{
	public:
		hardware_mesh_buffer_factory( void );

		virtual ~hardware_mesh_buffer_factory( void );

	public:
		static hardware_mesh_buffer* create( usx_mesh* in_mesh, usx_frame* in_root_frame, cpstr in_path );

	protected:
		static void copy( hardware_mesh_buffer* in_buffer, usx_mesh* in_mesh );

		static void copy( hardware_mesh_skinned_buffer* in_buffer, usx_mesh* in_mesh );

		static vector3<double> get_blended_vertex( hardware_mesh_skinned_buffer* in_buffer, ulong vert_index );
	};
}

#endif // _US_HARDWARE_MESH_BUFFER_FACTORY_H_ 