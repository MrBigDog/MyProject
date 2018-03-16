///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_geometry_mesh.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_geometry_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_GEOMETRY_MESH_H_
#define _US_RENDER_OBJECT_GEOMETRY_MESH_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_hardware_mesh_buffer.h>
#include <usGfx/us_d3d9_vertex_declear.h>
#include <usCore/us_render_object.h>
//#include <usCore/us_hardware_mesh.h>
#include <usCore/us_shadow_volume_creator.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class render_object_factory;

	class USGFX_EXPORT render_object_geometry_mesh
		: public render_object
		, public shadow_volume_creator
		//, public intersect_base
		, public d3d9_hardware_mesh_buffer
	{
	public:
		render_object_geometry_mesh( void );

		virtual ~render_object_geometry_mesh( void );

		friend class render_object_factory;
		friend class render_object_shadow_factory;
		friend class render_object_line_factory;
		friend class render_object_polygon_factory;

	public:
		static render_object_geometry_mesh* create_shared_instance();

		static render_object_geometry_mesh* create_shared_instance( d3d9_hardware_mesh_buffer* v_mesh_buffer, ulong v_color );

	// override from render_object
	public:
		virtual void draw( render_argument* args );
	
		virtual void set_world_matrix( matrix4<double>& v_mat ) { m_trans_matrix = v_mat; }

		virtual matrix4<double> get_world_matrix( void ) { return m_trans_matrix; }
		
	// override from intersect_base
	public:
		virtual bool intersect( const ray<double>& a_ray, intersect_result& result );

	public:
		void draw_figure( render_device* args, matrix_4f& v_mat );

		void draw_figure( render_argument* args );

		void draw_without_figure(  render_argument* args  );

		void clear();

		void set_collapse( matrix_4d& mat );

		// override from shadow_volume_creator
	public:
		virtual render_object* get_shadow_object( void );

		// override from d3d9_hardware_mesh_buffer
	public:
		virtual void lock();

		virtual void unlock();

	protected:
		bool generate_topological();

		bool generate_normals();

		face_normal create_normal_f( vector_3f& vec0, vector_3f& vec1, vector_3f& vec2 );

	public:
		std::vector<position_color> m_vertex_array;

		std::vector<ushort>			m_index_array;

		std::vector<ulong>			m_index_array_32;
						
		std::vector<position_color> m_frame_vertex_array;

		std::vector<ushort>			m_frame_index_array;

		matrix_4d					m_trans_matrix;

		matrix_4d					m_collapse_matrix;

		vector3<double>				m_center;

		ulong						m_fvf;
	};
}

#endif // _US_RENDER_OBJECT_GEOMETRY_MESH_H_