///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_polygon.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_polygon class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_POLYGON_H_
#define _US_RENDER_OBJECT_POLYGON_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_vertex_declear.h>
#include <usCore/us_render_object.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class USGFX_EXPORT render_object_polygon
		: public render_object
	{
	public:
		render_object_polygon(void);

		virtual ~render_object_polygon(void);

		friend class render_object_factory;
		friend class render_object_polygon_factory;

	public:
		static render_object_polygon* create_shared_instance(void);

		// override from render_object
	public:
		virtual void draw(render_argument* args);

	public:
		virtual void set_world_matrix(matrix4<double>& v_mat) { m_trans_matrix = v_mat; }

		virtual matrix4<double> get_world_matrix(void) { return m_trans_matrix; }

		void set_collapse(matrix_4d& mat);

		void clear(void);

	public:
		std::vector<position_color> m_vertex_array;
		std::vector<ushort> m_triangle_index;

		std::vector<position_color> m_line_array;

		vector_3d m_center;
		matrix_4d m_trans_matrix;
		matrix_4d m_collapse_matrix;

	};
}

#endif // _US_RENDER_OBJECT_POLYGON_H_