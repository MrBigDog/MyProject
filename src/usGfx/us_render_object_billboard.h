///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_billboard.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_billboard class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_OBJECT_BILLBOARD_H_
#define _US_RENDER_OBJECT_BILLBOARD_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_vertex_declear.h>
#include <usCore/us_render_object.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class hardware_texture;

	class USGFX_EXPORT render_object_billboard
		: public render_object
	{
	public:
		render_object_billboard(void);

		virtual ~render_object_billboard(void);

		// override from render_object
	public:
		virtual void draw(render_argument* args);

	public:
		void set_matrix(matrix_4d& mat);

		void set_texture(hardware_texture* v_texture);

	public:
		// 创建一个billboard
		void create_billboard(int bb_type, vector_3d v_locate, double v_width, double v_height);

	public:
		virtual bool intersect(const ray<double>& a_ray, intersect_result& result);

	protected:
		std::vector<position_texture> m_vertex_array;

		matrix_4d					m_mat;

		vector_3d					m_center;

		hardware_texture*			m_texture;
	};
}

#endif // _US_RENDER_OBJECT_BILLBOARD_H_