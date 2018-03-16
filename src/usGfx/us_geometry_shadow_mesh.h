///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_shadow_mesh.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_shadow_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_SHADOW_MESH_H_
#define _US_GEOMETRY_SHADOW_MESH_H_

#include <usGfx/Export.h>
#include <usGfx/us_d3d9_vertex_declear.h>

#include <usCore/us_render_object.h>
#include <usUtil/us_common_file.h>


namespace uniscope_globe
{
	class USGFX_EXPORT geometry_shadow_mesh
		: public render_object
		, public intersect_base
	{
	public:
		geometry_shadow_mesh(void);

		virtual ~geometry_shadow_mesh(void);

		friend class geometry_shadow_factory;

	public:
		static geometry_shadow_mesh* create_shared_instance();

		// override from render_object
	public:
		virtual void draw(render_argument* args);

		virtual void set_world_matrix(matrix4<double>& v_mat) { m_trans_matrix = v_mat; }

		virtual matrix4<double> get_world_matrix(void) { return m_trans_matrix; }

		// override from intersect_base
	public:
		virtual bool intersect(const ray<double>& a_ray, intersect_result& result);

	public:
		void clear();

		void set_collapse(matrix_4d& mat);

	public:
		ulong m_fvf;
		ulong m_stride;

		std::vector<position_normal_color> m_vertex_array;
		std::vector<ulong> m_index_array;


		std::vector<position_color> m_frame_vertex_array;
		std::vector<ushort>			m_frame_index_array;

		vector_3d m_center;
		matrix_4d m_trans_matrix;
		matrix_4d m_collapse_matrix;
	};
}

#endif // _US_GEOMETRY_SHADOW_MESH_H_ 