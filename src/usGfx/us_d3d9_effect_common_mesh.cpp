///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_common_mesh.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_common_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_effect_common_mesh.h"
#include <usGfx/us_d3d9_hardware_mesh.h>
#include <usGfx/us_d3d9_material_declear.h>
#include <usCore/us_hardware_texture.h>
#include <usCore/us_render_device.h>

namespace uniscope_globe
{
	d3d9_effect_common_mesh::d3d9_effect_common_mesh(render_device* device)
		: d3d9_effect_base(device)
	{
		m_wvp_matrix = matrix4<float>::s_identity;

		m_world_matrix = matrix4<float>::s_identity;

		m_view_matrix = matrix4<float>::s_identity;

		m_projection_matrix = matrix4<float>::s_identity;
	}

	d3d9_effect_common_mesh::~d3d9_effect_common_mesh(void)
	{

	}

	void d3d9_effect_common_mesh::set_vertex_declaration(uint decl)
	{

	}

	void d3d9_effect_common_mesh::set_texture(ulong stage_index, texture_handle handle)
	{

	}

	void d3d9_effect_common_mesh::set_transparency_texture(ulong stage_index, ulong texture_idx)
	{

	}

	void d3d9_effect_common_mesh::set_blank_texture(ulong stage_index)
	{

	}

	void d3d9_effect_common_mesh::set_material(ULONG_PTR material_handle)
	{

	}

	void d3d9_effect_common_mesh::set_world_matrix(matrix4<float>& in_world_mat)
	{

	}

	void d3d9_effect_common_mesh::set_view_matrix(matrix4<float>& in_view_mat)
	{

	}

	void d3d9_effect_common_mesh::set_projection_matrix(matrix4<float>& in_proj_mat)
	{

	}

	void d3d9_effect_common_mesh::push_transform(void)
	{

	}

	void d3d9_effect_common_mesh::multiply_transform(matrix4<float>& mat)
	{

	}

	void d3d9_effect_common_mesh::pop_transform(void)
	{

	}

	void d3d9_effect_common_mesh::draw(void* mesh_pointer, uint pass_index)
	{
		if (mesh_pointer == NULL) return;

		commit_changes();
		((LPD3DXMESH)mesh_pointer)->DrawSubset(pass_index);
	}

	void d3d9_effect_common_mesh::draw(hardware_mesh* in_mesh)
	{
		d3d9_hardware_mesh* v_mesh = (d3d9_hardware_mesh*)in_mesh;

		//D3DLIGHT9  light;
		//ZeroMemory( &light, sizeof(light) );
		//light.Type = D3DLIGHT_DIRECTIONAL;
		//m_device->set_render_state( D3DRS_LIGHTING, TRUE );

		int tex_cnt = (int)v_mesh->m_texture_array.size();

		if (tex_cnt == 0)
		{
			commit_changes();

			//if(v_mesh->m_use_material)
			//{
			//	this->set_material((long)&v_mesh->m_material_entry_array[0]);
			//}
			v_mesh->m_d3d_mesh->DrawSubset(0);

			return;
		}

		for (int i = 0; i < tex_cnt; i++)
		{
			if (v_mesh->m_texture_array[i].m_diffuse)
			{
				if (v_mesh->m_texture_array[i].m_diffuse->is_valid())
				{
					set_texture(0, v_mesh->m_texture_array[i].m_diffuse->get_handle());
					//m_texture_array[i].m_diffuse->draw( device, 0 );
				}
				else
					m_device->set_transparency_texture(0);
				//set_texture( 0, NULL );
			}
			else
			{
				//set_texture( 0, NULL );				
				m_device->set_transparency_texture(0);
			}

			if (v_mesh->m_texture_array[i].m_self_illuminant)
			{
				set_texture(1, v_mesh->m_texture_array[i].m_self_illuminant->get_handle());
				//m_texture_array[i].m_self_illuminant->draw( device, 1 );
			}
			else
			{
				//set_texture( 1, m_device->get_transparency_texture(0));
				//v_mesh_render->set_blank_texture( 1 );
				set_texture(1, NULL);
			}

			commit_changes();

			if (v_mesh->m_use_material)
			{
				this->set_material((ULONG_PTR)&v_mesh->m_material_entry_array[i]);
			}

			v_mesh->m_d3d_mesh->DrawSubset(i);
		}
	}

}