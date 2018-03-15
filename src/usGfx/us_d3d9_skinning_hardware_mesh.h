///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_skinning_hardware_mesh.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_skinning_hardware_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_SKINNING_HARDWARE_MESH_H_
#define _US_D3D9_SKINNING_HARDWARE_MESH_H_

namespace uniscope_globe
{
	class d3d9_skinning_hardware_mesh
		: public d3d9_hardware_mesh
	{
	public:
		d3d9_skinning_hardware_mesh( void );

	public:
		//d3d9_skinning_hardware_mesh( hardware_mesh_buffer* in_buffer );

		virtual ~d3d9_skinning_hardware_mesh( void );

	public:
		virtual long create( render_device* device );

		virtual void draw( render_argument* args );

		virtual bool intersect( const ray<double>& in_ray, intersect_result& out_result );

		// override hardware_mesh
	public:
		virtual hardware_mesh* clone( void );

	public:
		usx_frame* get_root_frame( void ) { return m_root_frame; }

		void set_root_frame( usx_frame* v_root_frame ) { m_root_frame = v_root_frame; }

	public:
		usx_frame* m_root_frame;

		std::vector<usx_frame*> m_frame_array;

		std::vector< matrix4<double> > m_offset_matrix_array;

		std::vector< matrix4<float> > m_final_matrix_array;

		// for updating bounding box
		std::vector< matrix4<float> > m_original_matrix_array;

		int m_num_of_bones;
	};
}

#endif // _US_D3D9_SKINNING_HARDWARE_MESH_H_