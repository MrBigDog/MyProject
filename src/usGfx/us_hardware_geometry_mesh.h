///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_geometry_mesh.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_geometry_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_HARDWARE_GEOMETRY_MESH_H_
#define _US_HARDWARE_GEOMETRY_MESH_H_

namespace uniscope_globe
{
	class LIB_GFX_API hardware_geometry_mesh
		: public geometry_mesh
		, public hardware_base
	{
	protected:
		hardware_geometry_mesh( void ){}

	public:
		hardware_geometry_mesh( hardware_creator* in_creator );

		hardware_geometry_mesh( const hardware_geometry_mesh& v_mesh );

		virtual ~hardware_geometry_mesh( void );

		// override from hardware_base
	public:
		virtual long create( render_device* device );

		virtual bool destroy( void );

		// override from render_object
	public:
		virtual void draw( render_argument* args );

	protected:
		IDirect3DVertexBuffer9*			m_vertex_buffer;

		ulong							m_num_of_vertices;

		IDirect3DIndexBuffer9*			m_index_buffer;

		ulong							m_num_of_indices;		 

		hardware_creator*				m_mesh_creator;
	};
}

#endif // _US_HARDWARE_GEOMETRY_MESH_H_