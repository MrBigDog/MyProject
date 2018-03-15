
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_mesh_buffer.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_hardware_mesh_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_HARDWARE_MESH_BUFFER_H_
#define _US_HARDWARE_MESH_BUFFER_H_

namespace uniscope_globe
{
#define US_HARDWARE_BUFFER_NONE 0
#define US_HARDWARE_BUFFER_PT2 1
#define US_HARDWARE_BUFFER_PNT2 2
#define US_HARDWARE_BUFFER_SKINNING 3
#define US_HARDWARE_BUFFER_UST 4

	class LIB_GFX_API d3d9_hardware_mesh_buffer		
	{
	public:
		d3d9_hardware_mesh_buffer();

		virtual ~d3d9_hardware_mesh_buffer( void );

	public:
		virtual void lock();

		virtual void unlock();

		virtual void set_buffer_is_dirty( void ) { m_buffer_is_dirty = true;; }

	public:
		virtual int get_type( void ) { return m_type; }

		virtual void set_fvf( ulong fvf ) { m_fvf = fvf; }

		virtual ulong get_fvf( void ) { return m_fvf; }

		virtual void set_stride( ulong v_stride ) { m_vertex_stride = v_stride; }

		virtual ulong get_stride( void ) { return m_vertex_stride; }

		virtual bool is_32bit_index ( void ) { return m_32bit_index; }	

		virtual void set_32bit_index ( bool v_32bit ) { m_32bit_index = v_32bit; }

	public:
		virtual bool set_vector3(int offset, int index, vector_3f new_val);

		virtual bool set_vector3(int offset, int index, vector_3f* new_val);
		virtual bool set_vector3_rh(int offset, int index, vector_3f* new_val);

		virtual vector_3f get_vector3(int offset, int index);

		virtual bool set_vector2(int offset, int index, vector_2f new_val);

		virtual bool set_vector2(int offset, int index, vector_2f* new_val);

		virtual vector_2f get_vector2(int offset, int inde );

		virtual bool set_float(int offset, int index, float new_val);

		virtual float get_float(int offset, int inde );

		virtual bool set_ulong(int offset, int index, ulong new_val);

		virtual ulong get_ulong(int offset, int inde );

		virtual bool set_vector3_array( int offset, std::vector<vector_3f>& v_array );

		virtual bool get_vector3_array( int offset, std::vector<vector_3f>& v_array );

		virtual bool set_vector2_array( int offset, std::vector<vector_2f>& v_array );

		virtual bool get_vector2_array( int offset, std::vector<vector_2f>& v_array );

		virtual int get_offset( int type );

	public:
		virtual byte* get_fat_vertex( int index );

	public:
		inline void  set_vertex_buffer( void* v_buf ) { m_vertex_buffer = v_buf; }
		inline void* get_vertex_buffer( void )	{ return m_vertex_buffer; }

		inline void  set_index_buffer( void* v_buf ) { m_index_buffer = v_buf; }
		inline void* get_index_buffer( void )	{ return m_index_buffer; }

		inline void	 set_attribute_buffer( void* v_buf ) { m_attribute_buffer = v_buf; }
		inline void* get_attribute_buffer( void ) { return m_attribute_buffer; }

		inline void  set_num_of_vertices( ulong v_num ) { m_num_of_vertices = v_num; }
		inline ulong get_num_of_vertices( void ) { return m_num_of_vertices; }

		inline void  set_vertex_buffer_size( ulong v_size ) { m_vertex_buffer_size = v_size; }
		inline ulong get_vertex_buffer_size( void ) { return m_vertex_buffer_size; }

		inline void  set_num_of_triangles( ulong v_num ) { m_num_of_triangles = v_num; }
		inline ulong get_num_of_triangles( void ) { return m_num_of_triangles; }

		inline void  set_index_buffer_size( ulong v_size ) { m_index_buffer_size = v_size; }
		inline ulong get_index_buffer_size( void ) { return m_index_buffer_size; }

		inline void  set_attribute_buffer_size( ulong v_size ) { m_attribute_buffer_size = v_size; }
		inline ulong get_attribute_buffer_size( void ) { return m_attribute_buffer_size; }

	public:
		virtual void clear_buffer( void );

		void copy_buffer_from( d3d9_hardware_mesh_buffer* in_src_buffer );

		void transform_vertex_buffer( matrix_4f& v_mat );

		void transform_vertex_buffer( matrix_4d& v_mat );

		void attach_buffer_from( d3d9_hardware_mesh_buffer* in_src_buffer );

		void detach_buffer( void );
		
	public:
		virtual void optimize( void );

		virtual edge_array& get_edge( void );

		virtual face_normal_array& get_face_normal( void );

		void generate_vertex_normal( void );

	protected:
		face_normal create_normal_f( vector_3f& vec0, vector_3f& vec1, vector_3f& vec2 );

		void create_normal(vector_3f& vec_out, vector_3f& vec0, vector_3f& vec1, vector_3f& vec2 );

	public:
		virtual void generate_adjacency( float v_eclipse, ulong* v_adjacency );

		virtual void generate_point_rep( ulong* v_adj_info, ulong* v_point_rep );

	public:
		static double calculate_perimeter( void* vertex_buffer, int vertex_stride, int vertex_count );

	public:
		int				m_type;
		ustring			m_path;

		bool			m_buffer_is_dirty;
		//ulong			m_mem_size;

	public:
		axis_aligned_box<double> m_mesh_aabb;

		ulong			m_fvf;
		ulong			m_vertex_stride;
		bool			m_32bit_index;

		// vertex
		void*			m_vertex_buffer;
		void*			m_index_buffer;
		void*			m_attribute_buffer;

		ulong			m_num_of_vertices;
		ulong			m_vertex_buffer_size;

		// index		
		ulong			m_num_of_triangles;
		ulong			m_index_buffer_size;

		// attribute
		ulong			m_attribute_buffer_size;

	public:
		edge_array			m_edges;

		face_normal_array	m_face_normals;

	public:
		ulong*			m_point_reps;

		ulong*			m_adjacency;

	};

	typedef std::map< d3d9_hardware_mesh_buffer*, usx_frame* > hardware_mesh_buffer_map;
}

#endif // _US_HARDWARE_MESH_BUFFER_H_ 