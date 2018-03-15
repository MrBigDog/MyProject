///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_polygon.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_polygon class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_POLYGON_H_
#define _US_GEOMETRY_POLYGON_H_

namespace uniscope_globe
{
	class LIB_GFX_API geometry_polygon
		: public render_object
	{
	public:
		geometry_polygon( void );

		virtual ~geometry_polygon( void );

		friend class geometry_factory;

	public:
		static geometry_polygon* create_shared_instance( void );

		// override from render_object
	public:
		virtual void draw( render_argument* args );

	public:
		void set_matrix( matrix_4d& mat );

		void set_collapse( matrix_4d& mat );

		void clear( void );

		//void create_from_line_ring( vector_3d* v_src, int v_size, ulong v_line_color, ulong v_fill_color );

		//void create_from_line_ring_cart( vector_3d* v_src, int v_size, ulong v_line_color, ulong v_fill_color );

	private:
		//void create_triangle_list( vector_3d* v_src, int v_size );

	protected:
		std::vector<position_color> m_vertex_array;
		std::vector<ushort> m_triangle_index;

		std::vector<position_color> m_line_array;

		vector_3d m_center;
		matrix_4d m_trans_matrix;
		matrix_4d m_collapse_matrix;
		 
	};
}

#endif // _US_GEOMETRY_POLYGON_H_