///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_billboard.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_billboard class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_BILLBOARD_H_
#define _US_GEOMETRY_BILLBOARD_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_vertex_declear.h>
#include <usCore/us_render_object.h>
#include <usCore/us_intersect_base.h>
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	class hardware_texture;

	class USGFX_EXPORT geometry_billboard
		: public render_object
		, public intersect_base
	{
	public:
		geometry_billboard(void);

		virtual ~geometry_billboard(void);

		// override from render_object
	public:
		virtual void draw(render_argument* args);

	public:
		void set_matrix(matrix_4d& mat);

		void set_texture(hardware_texture* v_texture);

		void set_alpha_blend(bool v_alpha_blend)
		{
			m_alpha_blend = v_alpha_blend;
		}

	public:
		// 创建一个billboard
		void create_billboard(int bb_type, vector_3d v_locate, double v_width, double v_height);

	public:
		virtual bool intersect(const ray<double>& a_ray, intersect_result& result);

	protected:
		std::vector<position_texture> m_vertex_array;

		std::vector<ushort>			m_index_array;

		matrix_4d					m_mat;

		vector_3d					m_center;

		hardware_texture*			m_texture;

		bool						m_alpha_blend;

	};
}

#endif // _US_GEOMETRY_BILLBOARD_H_