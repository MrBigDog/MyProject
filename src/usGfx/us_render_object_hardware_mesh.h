///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_hardware_mesh.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_hardware_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_HARDWARE_MESH_H_
#define _US_RENDER_OBJECT_HARDWARE_MESH_H_

namespace uniscope_globe
{
	class LIB_GFX_API render_object_hardware_mesh
		: public render_object_geometry_mesh
		, public hardware_base
	{
	protected:
		render_object_hardware_mesh( void ){}

	public:
		render_object_hardware_mesh( hardware_creator* in_creator );

		render_object_hardware_mesh( const render_object_hardware_mesh& v_mesh );

		virtual ~render_object_hardware_mesh( void );

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

#endif // _US_RENDER_OBJECT_HARDWARE_MESH_H_