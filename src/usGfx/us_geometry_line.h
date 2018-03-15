///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_line.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_line class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_LINE_H_
#define _US_GEOMETRY_LINE_H_

namespace uniscope_globe
{
	class LIB_GFX_API geometry_line
		: public render_object
	{
	public:
		geometry_line( void );

		virtual ~geometry_line( void );

		friend class geometry_factory;

		friend class geometry_line_factory;


	public:
		static geometry_line* create_shared_instance();

	// override from render_object
	public:
		virtual void draw( render_argument* args );

	public:
		virtual void set_world_matrix( matrix4<double>& v_mat );

		void set_collapse( matrix_4d& mat );

		void clear();

		void create_from_line_string( vector_3d* v_src, int v_size, ulong v_color );

	protected:
		std::vector<position_color> m_vertex_array;


		vector_3d m_center;
		matrix_4d m_trans_matrix;
		matrix_4d m_collapse_matrix;
	};
}

#endif // _US_GEOMETRY_LINE_H_