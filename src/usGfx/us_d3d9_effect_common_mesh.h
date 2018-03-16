///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_common_mesh.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_common_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_COMMON_MESH_H_
#define _US_D3D9_EFFECT_COMMON_MESH_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_effect_base.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_matrix4.h>
#include <usCore/us_texture_state.h>

#include <stack>

namespace uniscope_globe
{
	class hardware_mesh;

	class USGFX_EXPORT d3d9_effect_common_mesh
		: public d3d9_effect_base
	{
	protected:
		d3d9_effect_common_mesh(void) { }

	public:
		d3d9_effect_common_mesh(render_device* device);

		virtual ~d3d9_effect_common_mesh(void);

	public:
		virtual void set_vertex_declaration(uint decl);

		virtual void set_texture(ulong stage_index, texture_handle handle);

		virtual void set_blank_texture(ulong stage_index);

		virtual void set_material(ULONG_PTR material_handle);

		virtual void set_transparency_texture(ulong stage_index, ulong texture_idx = 0);

		virtual void set_world_matrix(matrix4<float>& in_world_mat);

		virtual void set_view_matrix(matrix4<float>& in_view_mat);

		virtual void set_projection_matrix(matrix4<float>& in_proj_mat);

		virtual void push_transform(void);

		virtual void multiply_transform(matrix4<float>& mat);

		virtual void pop_transform(void);

	public:
		virtual void draw(hardware_mesh* in_mesh);

		virtual void draw(void* mesh_pointer, uint pass_index);

	protected:
		std::stack<matrix4<float>> m_world_matrix_stack;

		matrix4<float> m_wvp_matrix;

		matrix4<float> m_world_matrix;

		matrix4<float> m_view_matrix;

		matrix4<float> m_world_view_matrix;

		matrix4<float> m_projection_matrix;
	};
}

#endif // _US_D3D9_EFFECT_COMMON_MESH_H_ 