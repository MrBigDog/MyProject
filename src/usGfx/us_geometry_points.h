///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_points.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_points class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_POINTS_H_
#define _US_GEOMETRY_POINTS_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_vertex_declear.h>
#include <usCore/us_render_object.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class USGFX_EXPORT geometry_points
		: public render_object
	{
	public:
		geometry_points(void);

		virtual ~geometry_points(void);

		// override from render_object
	public:
		virtual void draw(render_argument* args);

	public:
		void set_matrix(matrix_4f& mat);

		void create_from_point_list(vector_3d* v_src, int v_size, ulong v_color);

	protected:
		std::vector<position_color> m_vertex_array;

		matrix_4f m_mat;

	};
}

#endif // _US_GEOMETRY_POINTS_H_