///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_mesh.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_MESH_H_
#define _US_GEOMETRY_MESH_H_

namespace uniscope_globe
{
	class geometry_factory;

	class LIB_GFX_API geometry_mesh
		: public render_object
		, public shadow_volume_creator
		, public intersect_base
	{
	public:
		geometry_mesh( void );

		virtual ~geometry_mesh( void );

		friend class geometry_factory;

	public:
		static geometry_mesh* create_shared_instance();

	// override from render_object
	public:
		virtual void draw( render_argument* args );
	
		virtual void set_world_matrix( matrix4<double>& v_mat ) { m_trans_matrix = v_mat; }

		virtual matrix4<double> get_world_matrix( void ) { return m_trans_matrix; }

	// override from shadow_volume_creator
	public:
		virtual render_object* get_shadow_object( void );

	// override from intersect_base
	public:
		virtual bool intersect( const ray<double>& a_ray, intersect_result& result );

	public:
		void draw_figure( render_device* args, matrix_4f& v_mat );

		void clear();

		void set_collapse( matrix_4d& mat );

	//public:
	//	void create_from_line( vector_3d* v_src, int v_size, double v_width, ulong v_color, double v_height = 200);

	//	void create_from_multiple_line( vector3<double>* in_vertex_buffer, ulong in_vertex_size, ushort* in_index_buffer, ulong in_index_size, double in_width, ulong in_color, double in_height = 9 );

	//	void create_from_polygon( vector_3d* v_src, int v_size, ulong v_color, double v_height = 200 );

	//	void create_from_multiple_polygon( vector3<double>* in_vertex_buffer, ulong in_vertex_size, ushort* in_index_buffer, ulong in_index_size, ulong v_color, double v_height = 9 );

	//	void create_from_box( lon_lat_box& v_box, ulong line_color, ulong fill_color );

	//	void create_from_volume( vector_3d* in_pos_array, ulong v_size, double min_height, double max_height, ulong line_color, ulong fill_color );

	//	void create_from_volume_in_plane( vector_3d* in_pos_array, ulong v_size, double vol_height, ulong line_color, ulong fill_color );

	//	void create_from_cone( vector_3d in_center, ulong in_sides, double in_radius, double in_height,  ulong line_color, ulong fill_color );

	//	void create_from_sphere( vector_3d center_point, double v_radius,  ulong line_color, ulong fill_color );

	//	void create_from_vertex_and_index( vector_3d* v_vertex, int v__vertex_size, ulong v_color, vector3<ushort>* v_index, int v_index_num );

	//protected:
	//	void generate_index_from_line( ulong size );

	//	void generate_vertex_from_line( coordinates_array& in_array, ulong v_color );

	//	void generate_line( vector_3d* in_pos_array, ulong v_size, double in_width, double in_height, ulong v_color );

	//	void generate_line_ex( vector_3d* in_pos_array, ulong v_size, double v_width, double v_height, ulong v_color );

	//	void generate_polygon( vector_3d* in_pos_array, ulong v_size, double min_height, double max_height, ulong v_color );

	//	void generate_polygon_ex( vector_3d* in_pos_array, ulong v_size, double in_height, ulong v_color );

	//	void generate_polygon_in_plane( vector_3d* in_pt_array, ulong v_size, double in_height, ulong v_color );

	//	void generate_polygon_coarse( vector_3d* in_pos_array, ulong v_size, double in_height, ulong v_color );

	//	double compute_scaled_length( vector3<double> pos1, vector3<double> pos2, double v_height );

	public:
		std::vector<position_color> m_vertex_array;		

		std::vector<ushort>			m_index_array;
						
		std::vector<position_color> m_frame_vertex_array;

		std::vector<ushort>			m_frame_index_array;

		matrix_4d					m_trans_matrix;

		matrix_4d					m_collapse_matrix;

		vector3<double>				m_center;

		ulong						m_fvf;
	};
}

#endif // _US_GEOMETRY_MESH_H_