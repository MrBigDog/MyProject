///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_textured_face.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_textured_face class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_TEXTURED_FACE_H_
#define _US_GEOMETRY_TEXTURED_FACE_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_vertex_declear.h>
#include <usCore/us_render_object.h>
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{

	class hardware_texture;

#define TEXFACE_TYPE_HORIZON	0x0001	// 面为水平方向；
#define TEXFACE_WIDTH_GRID		0x0002	// 宽度/高度方向纹理为平铺指定幅；
#define TEXFACE_LENGTH_GRID		0x0004	// 长度方向纹理为平铺指定幅；

	class USGFX_EXPORT geometry_textured_face
		: public render_object
	{
	public:
		geometry_textured_face(void);

		virtual ~geometry_textured_face(void);

		// override from render_object
	public:
		virtual void draw(render_argument* args);

	public:
		void set_matrix(matrix_4d& mat);

		void set_texture(hardware_texture* v_texture);
		// 		void set_location( vector_3d vec_center );

	public:
		void create_textured_face_vertical(std::vector<vector_3d> vers, double width, double width_grid, double length_scale, double length_begin);

	protected:
		std::vector<position_texture> m_vertex_array;
		// 		std::vector<ushort>			m_index_array;
		matrix_4d					m_mat;

		vector_3d					m_center;
		hardware_texture*			m_texture;

	};
}

#endif // _US_GEOMETRY_TEXTURED_FACE_H_