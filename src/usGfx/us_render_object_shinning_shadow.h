///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_shinning_shadow.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_shinning_shadow class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_SHINNING_SHADOW_H_
#define _US_RENDER_OBJECT_SHINNING_SHADOW_H_

namespace uniscope_globe
{
	class LIB_GFX_API render_object_shinning_shadow
		: public render_object
		, public hardware_mesh
		, public shadow_volume_creator
	{
	public:
		typedef std::vector< vector3<float> > vertex_array;

		typedef std::vector< position_normal > position_normal_array;

		typedef std::vector< ushort > index_array;

		typedef std::vector< ulong > index_array_32;

		typedef std::vector< vector3<int>> index_array_ex;

		typedef std::vector< byte > facing_array;

		typedef std::map< int, int > reorder_map;

	protected:
		render_object_shinning_shadow(){ }

	public:
		render_object_shinning_shadow( d3d9_hardware_mesh_buffer* v_mesh );

		virtual ~render_object_shinning_shadow();

	public:
		static render_object_shinning_shadow* create_shared_instance( d3d9_hardware_mesh_buffer* v_mesh );

	public:
		virtual long create( render_device* device );

		virtual bool destroy( void );

		virtual bool refresh( void );		

	public:
		virtual void draw( render_argument* args );

		//void create_from_mesh( d3d9_hardware_mesh_buffer* parent_mesh, vector4<double>& in_light, ulong v_stamp);

		virtual void set_world_matrix( matrix4<double>& mat ) { m_world_matrix = mat; }

		virtual matrix4<double> get_world_matrix( void ) { return m_world_matrix; }

		void set_height_offset( double v_height_offset ) { m_height_offset = v_height_offset; }

	protected:
		void generate_vertices( float* in_vertices, ulong num_of_vertices, long stride, vector4<double>& in_light );

		void generate_indices( face_normal_array& in_normal_array, edge_array& in_edge_array, void* v_triangle, ulong num_of_vertices, vector4<double>& in_light );

	private:
		int  get_new_index( int index );

		// override from shadow volume creator
	public:	
		virtual bool create_shadow_object( render_argument* args  );

		virtual void clear_shadow_object( void ) { }

		//virtual int get_pass1( void ) { return 6; }

	protected:
		position_normal_array m_vertex_array;

		index_array		m_index_array;

		index_array_32	m_index_array_32;

		bool			m_32bit_index;

		facing_array	m_facing_array;

		LPD3DXMESH		m_d3d_mesh;

		matrix4<double>	m_world_matrix;

		bool			m_valid;

		d3d9_hardware_mesh_buffer*  m_parent_mesh_buffer;

		double			m_height_offset;

	};

}

#endif // _US_RENDER_OBJECT_SHINNING_SHADOW_H_ 