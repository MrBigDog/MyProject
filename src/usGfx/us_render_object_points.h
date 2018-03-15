///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_points.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_points class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_POINTS_H_
#define _US_RENDER_OBJECT_POINTS_H_

namespace uniscope_globe
{
	class LIB_GFX_API render_object_points
		: public render_object
	{
	public:
		render_object_points( void );

		virtual ~render_object_points( void );

		// override from render_object
	public:
		virtual void draw( render_argument* args );

	public:
		void set_matrix( matrix_4f& mat );

		void create_from_point_list( vector_3d* v_src, int v_size, ulong v_color );

	protected:
		std::vector<position_color> m_vertex_array;

		matrix_4f m_mat;
		 
	};
}

#endif // _US_RENDER_OBJECT_POINTS_H_