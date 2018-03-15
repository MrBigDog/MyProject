
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_shinning_shadow_4s.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_shinning_shadow_4s class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_SHINNING_SHADOW_4S_H_
#define _US_RENDER_OBJECT_SHINNING_SHADOW_4S_H_

namespace uniscope_globe
{
	class LIB_GFX_API render_object_shinning_shadow_4s
						: public render_object
						, public hardware_mesh
						, public shadow_volume_creator
	{
	//public:
	//	typedef std::vector< vector3<float> > vertex_array;

	//	typedef std::vector< position_normal > position_normal_array;

	//	typedef std::vector< ushort > index_array;

	//	typedef std::vector< vector3<int>> index_array_ex;

	//	typedef std::vector< byte > facing_array;

	//	typedef std::map< int, int > reorder_map;

	public:
		typedef std::pair<ulong, ulong> edge;

		typedef std::map< edge, degenerate_quad > edge_map;

	public:
		render_object_shinning_shadow_4s(){ }

	public:
		render_object_shinning_shadow_4s( d3d9_hardware_mesh_buffer* v_mesh, int mat_count );

		virtual ~render_object_shinning_shadow_4s();

	public:
		static render_object_shinning_shadow_4s* create_shared_instance( d3d9_hardware_mesh_buffer* v_mesh, int mat_count );

	public:
		virtual long create( render_device* device );

		virtual bool destroy( void );

		virtual bool refresh( void );		

	public:
		virtual void draw( render_argument* args );

		virtual void set_world_matrix( matrix4<double>& mat ) { m_world_matrix = mat; }

		virtual matrix4<double> get_world_matrix( void ) { return m_world_matrix; }

	private:
		ulong generate_shadow_mesh( render_device* in_device );

		vector3<float> compute_normal( d3d9_hardware_mesh_buffer* v_buffer, ulong a, ulong b, ulong c );

		vector3<float> compute_normal( vector3<float>* vec1, vector3<float>* vec2, vector3<float>* vec3 );

		position_normal_texture2 read_vertex( d3d9_hardware_mesh_buffer* v_buffer ,ulong index );

		void insert_triangle(	ulong o1, ulong o2, ulong n1, ulong n2, edge_map& in_edge_map, 
								std::vector<usx_tri_face_index>& in_index_array,
								std::vector<ulong>& in_attribute_array, ulong in_material_count );

		void patching(	d3d9_hardware_mesh_buffer* v_buffer,
						edge_map& in_edge_map,		
						std::vector<position_normal_texture2>& in_new_vertex_array, 
						std::vector<usx_tri_face_index>& in_new_face_index, 
						std::vector<ulong>& in_new_attribute_array,
						ulong in_material_count );

		// override from shadow volume creator
	public:	
		virtual bool create_shadow_object( render_argument* args  );

		virtual void clear_shadow_object( void ) { }

		virtual int get_pass1( void ) { return 0; }

		virtual int get_pass2( void ) { return 1; }

	protected:
		LPD3DXMESH					m_d3d_mesh;

		matrix4<double>				m_world_matrix;

		bool						m_valid;

		d3d9_hardware_mesh_buffer*  m_parent_mesh_buffer;

		int							m_mat_count;
	};
}

#endif // _US_RENDER_OBJECT_SHINNING_SHADOW_4S_H_