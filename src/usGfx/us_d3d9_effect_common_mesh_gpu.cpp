///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_common_mesh_gpu.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_common_mesh_gpu class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_effect_common_mesh_gpu.h"
#include <usGfx/resource.h>
#include <usCore/us_render_device.h>

namespace uniscope_globe
{

	d3d9_effect_common_mesh_gpu::d3d9_effect_common_mesh_gpu(render_device* device)
		:d3d9_effect_common_mesh(device)
	{
		load_from_module_resource(IDR_SHADER_COMMON_MESH);
		//load_from_file(L"d:\\uniscope\\common_mesh_gpu.fx");
	}

	d3d9_effect_common_mesh_gpu::~d3d9_effect_common_mesh_gpu(void)
	{

	}

	bool d3d9_effect_common_mesh_gpu::setup_handle(void)
	{
		if (m_d3dx_effect)
		{
			m_technique = m_d3dx_effect->GetTechniqueByName("CommonMesh");
			m_transform_handle = m_d3dx_effect->GetParameterByName(0, "g_matWorldViewProj");
			m_texture_handle[0] = m_d3dx_effect->GetParameterByName(0, "g_txTexture0");
			m_texture_handle[1] = m_d3dx_effect->GetParameterByName(0, "g_txTexture1");
		}
		return true;
	}

	void d3d9_effect_common_mesh_gpu::set_vertex_declaration(uint decl)
	{
		//m_device->set_vertex_declaration( decl );
	}

	void d3d9_effect_common_mesh_gpu::set_texture(ulong stage_index, texture_handle handle)
	{
		if (handle == NULL)
		{
			m_d3dx_effect->SetTexture(m_texture_handle[stage_index], NULL);
		}
		else
		{
			m_d3dx_effect->SetTexture(m_texture_handle[stage_index], (LPDIRECT3DBASETEXTURE9)handle);
		}
	}

	void d3d9_effect_common_mesh_gpu::set_blank_texture(ulong stage_index)
	{
		m_d3dx_effect->SetTexture(m_texture_handle[stage_index], (LPDIRECT3DBASETEXTURE9)m_device->get_blank_texture());
	}

	void d3d9_effect_common_mesh_gpu::set_transparency_texture(ulong stage_index, ulong texture_idx)
	{
		m_d3dx_effect->SetTexture(m_texture_handle[stage_index], (LPDIRECT3DBASETEXTURE9)m_device->get_transparency_texture(texture_idx));
	}

	void d3d9_effect_common_mesh_gpu::set_material(ULONG_PTR material_handle)
	{

	}

	void d3d9_effect_common_mesh_gpu::set_world_matrix(matrix4<float>& in_world_mat)
	{
		m_world_matrix = in_world_mat;
	}

	void d3d9_effect_common_mesh_gpu::set_view_matrix(matrix4<float>& in_view_mat)
	{
		m_view_matrix = in_view_mat;
	}

	void d3d9_effect_common_mesh_gpu::set_projection_matrix(matrix4<float>& in_proj_mat)
	{
		m_projection_matrix = in_proj_mat;
	}

	void d3d9_effect_common_mesh_gpu::push_transform(void)
	{
		m_world_matrix_stack.push(m_world_matrix);
	}

	void d3d9_effect_common_mesh_gpu::multiply_transform(matrix4<float>& mat)
	{
		m_world_matrix = mat * m_world_matrix;
	}

	void d3d9_effect_common_mesh_gpu::pop_transform(void)
	{
		if (m_world_matrix_stack.size() == 0) return;

		m_world_matrix = m_world_matrix_stack.top();

		m_world_matrix_stack.pop();
	}

	bool d3d9_effect_common_mesh_gpu::commit_changes(void)
	{
		m_wvp_matrix = m_world_matrix * m_view_matrix * m_projection_matrix;

		m_d3dx_effect->SetMatrix(m_transform_handle, (D3DXMATRIX *)&m_wvp_matrix);

		return d3d9_effect_common_mesh::commit_changes();
	}
}
