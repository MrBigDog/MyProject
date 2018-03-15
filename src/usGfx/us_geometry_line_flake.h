
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_line_flake.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_line_flake class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_LINE_FLAKE_H_
#define _US_GEOMETRY_LINE_FLAKE_H_

namespace uniscope_globe
{
	class LIB_GFX_API geometry_line_flake
		: public render_object
		, public intersect_base
	{
	public:
		geometry_line_flake( void );

		virtual ~geometry_line_flake( void );

		friend class geometry_line_factory;

	public:
		static geometry_line_flake* create_shared_instance( void );

		// override from render_object
	public:
		virtual void draw( render_argument* args );

		virtual void clear( void ) { m_vertex_array.clear(); }

		// override from intersect_base
	public:		
		//virtual bool intersect( const point& pt, intersect_result& out_result );

	public:
		virtual void set_world_matrix( matrix4<double>& mat ) { m_trans_matrix = mat; }

		virtual void set_collapse_matrix( matrix_4d& mat ) { m_collapse_matrix = m_collapse_matrix * mat; }

		virtual void set_line_width( float line_width ) { m_line_width = line_width; }

		virtual float get_line_width( void ) { return m_line_width; }

		virtual void set_line_color( ulong line_color ) { m_line_color = line_color;}

		virtual ulong get_line_color( void ) { return m_line_color; }

	private:
		vector_3f project(vector_3f& vec, view_port& viewport, matrix_4f& wvp);

		vector_3f project2(vector_3f& vec, view_port& viewport, matrix_4f& wvp);

		int project_line( std::vector<transformed_color>& out_vec_array, std::vector<position_normal_color>& in_vec_array, view_port& viewport, matrix_4f& proj_mat, matrix_4f& view_mat, matrix_4f& world_mat, float near_plane );

	protected:
		std::vector<position_normal_color> m_vertex_array;

		float		m_line_width;
		ulong		m_line_color;

		matrix_4d	m_trans_matrix;
		matrix_4d	m_collapse_matrix;
		
	};
}

#endif // _US_GEOMETRY_LINE_FLAKE_H_ 