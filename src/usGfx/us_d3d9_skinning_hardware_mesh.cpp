///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_skinning_hardware_mesh.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_skinning_hardware_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_skinning_hardware_mesh.h"

namespace uniscope_globe
{
	d3d9_skinning_hardware_mesh::d3d9_skinning_hardware_mesh()
	{
		m_rtti = US_RTTI_D3D9_SKINNING_HARDWARE_MESH;

		m_root_frame = NULL;

		m_num_of_bones = 0;

		m_frame_array.resize( 48 );
		memset( (void*)&m_frame_array[0], 0, 48 * 4 );

		m_final_matrix_array.resize(48);
		memset( (void*)&m_final_matrix_array[0], 0, 48 * sizeof(matrix4<float>) );

		m_offset_matrix_array.resize( 48 );
		memset( (void*)&m_offset_matrix_array[0], 0, 48 * sizeof(matrix4<double>) );

		//
		m_original_matrix_array.resize( 48 );
		memset( (void*)&m_original_matrix_array[0], 0, 48 * sizeof(matrix4<float>) );		
	}

	//d3d9_skinning_hardware_mesh::d3d9_skinning_hardware_mesh( d3d9_hardware_mesh_buffer* in_buffer )
	//	: d3d9_hardware_mesh( in_buffer )
	//{
	//	m_rtti = US_RTTI_D3D9_SKINNING_HARDWARE_MESH;

	//	hardware_mesh_skinned_buffer* v_buffer = (hardware_mesh_skinned_buffer*)in_buffer;

	//	m_num_of_bones = v_buffer->m_num_of_bones;

	//	m_offset_matrix_array.resize( v_buffer->m_offset_matrix_array.size() );
	//	std::copy( v_buffer->m_offset_matrix_array.begin(), v_buffer->m_offset_matrix_array.end(), m_offset_matrix_array.begin() );

	//	m_frame_array.resize( v_buffer->m_frame_array.size() );		
	//	std::copy( v_buffer->m_frame_array.begin(), v_buffer->m_frame_array.end(), m_frame_array.begin() );

	//	m_final_matrix_array.resize( m_offset_matrix_array.size() );
	//}

	d3d9_skinning_hardware_mesh::~d3d9_skinning_hardware_mesh( void )
	{

	}

	hardware_mesh* d3d9_skinning_hardware_mesh::clone( void )
	{
		d3d9_skinning_hardware_mesh* v_new_mesh = new d3d9_skinning_hardware_mesh();

		v_new_mesh->copy_buffer_from( this );

		v_new_mesh->m_root_frame		= this->m_root_frame->clone();
		v_new_mesh->m_num_of_bones		= this->m_num_of_bones;

		v_new_mesh->m_frame_array.resize( 48 );
		memset( &v_new_mesh->m_frame_array[0], 0, 48 * 4 );

		for ( int i = 0; i < this->m_frame_array.size(); i++ )
		{
			v_new_mesh->m_frame_array[i] = v_new_mesh->m_root_frame->find_frame( this->m_frame_array[i]->m_name.c_str() );
		}

		v_new_mesh->m_original_matrix_array.resize( 48 );
		memset( &v_new_mesh->m_original_matrix_array[0], 0, 48 * sizeof( matrix4<float> ) );

		v_new_mesh->m_offset_matrix_array.resize( 48 );
		memset( &v_new_mesh->m_offset_matrix_array[0], 0, 48 * sizeof( matrix4<double> ) );
		
		v_new_mesh->m_original_matrix_array = this->m_original_matrix_array;
		v_new_mesh->m_offset_matrix_array   = this->m_offset_matrix_array;

		return NULL;
	}

	long d3d9_skinning_hardware_mesh::create( render_device* in_device )
	{
		if ( m_valid )
			return 0;

		d3d9_hardware_mesh_buffer* v_buffer = dynamic_cast<d3d9_hardware_mesh_buffer*>(this);

		//
		// create mesh		
		//
		//D3DVERTEXELEMENT9 decl[] = 
		//{
		//	{ 0, 0,     D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
		//	{ 0, 3*4,   D3DDECLTYPE_FLOAT4,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_BLENDWEIGHT,    0 },
		//	{ 0, 7*4,   D3DDECLTYPE_D3DCOLOR,   D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_BLENDINDICES,     0 },
		//	{ 0, 8*4,   D3DDECLTYPE_FLOAT2,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,     0 },
		//	D3DDECL_END()
		//};

		HRESULT hr = D3DXCreateMeshFVF(	v_buffer->m_num_of_triangles,
										v_buffer->m_num_of_vertices,
										D3DXMESH_MANAGED | (v_buffer->m_32bit_index?D3DXMESH_32BIT:0x000),
										v_buffer->m_fvf,
										(LPDIRECT3DDEVICE9)in_device->get_device(),
										&m_d3d_mesh );

		m_mem_size = 0;

		//vertex
		void* v_vertices = NULL;
		hr = m_d3d_mesh->LockVertexBuffer( 0, (void**)&v_vertices );
		{
			memcpy( (byte*)v_vertices, v_buffer->m_vertex_buffer, v_buffer->m_vertex_buffer_size );			
		}
		hr = m_d3d_mesh->UnlockVertexBuffer();
		m_mem_size += v_buffer->m_vertex_buffer_size;

		void* v_indices = NULL;
		hr = m_d3d_mesh->LockIndexBuffer( 0, (void**)&v_indices );
		{
			memcpy( (byte*)v_indices, v_buffer->m_index_buffer, v_buffer->m_index_buffer_size );			
		}
		hr = m_d3d_mesh->UnlockIndexBuffer();
		m_mem_size += v_buffer->m_index_buffer_size;


		DWORD* v_attribute;
		hr = m_d3d_mesh->LockAttributeBuffer( 0, &v_attribute );
		{
			memcpy( (byte*)v_attribute, v_buffer->m_attribute_buffer, v_buffer->m_attribute_buffer_size );
		}
		hr = m_d3d_mesh->UnlockAttributeBuffer();

		m_mem_size += v_buffer->m_attribute_buffer_size;

		m_d3d_mesh->OptimizeInplace( D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL );

		//update_bound_box();

		m_bound_box = v_buffer->m_mesh_aabb;

		m_valid = true;

		return m_mem_size;
	}

	void d3d9_skinning_hardware_mesh::draw( render_argument* args )
	{
		// 更新矩阵列表		
		for ( int i = 0; i < (int)m_num_of_bones; i ++ )
		{
			//m_final_matrix_array[i] = matrix4<double>::s_identity;
			m_final_matrix_array[i] = matrix4<float>( ( m_offset_matrix_array[i] * m_frame_array[i]->m_combined_mat ).m ) ;
		}

		//////////////////////////////////////////////////////////////////////////
		// software
		//////////////////////////////////////////////////////////////////////////
		/*
		render_device* device = args->m_device;

		LPDIRECT3DDEVICE9 d3d_device = (LPDIRECT3DDEVICE9) device->get_device();

		// clone mesh
		D3DVERTEXELEMENT9 decl[] = 
		{
			{ 0, 0,     D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
			{ 0, 3*4,   D3DDECLTYPE_FLOAT4,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_BLENDWEIGHT,    0 },
			{ 0, 7*4,   D3DDECLTYPE_D3DCOLOR,   D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_BLENDINDICES,     0 },
			{ 0, 8*4,   D3DDECLTYPE_FLOAT2,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,     0 },
			D3DDECL_END()
		};

		LPD3DXMESH m_d3d_mesh_clone = NULL;
		m_d3d_mesh->CloneMesh(D3DXMESH_MANAGED, decl, d3d_device, &m_d3d_mesh_clone );

		void* v_vertices = NULL;
		m_d3d_mesh_clone->LockVertexBuffer( 0, (void**)&v_vertices );
		position_weights4_bone_texture* v_vertex_buffer = (position_weights4_bone_texture*)	v_vertices;
		ulong vertex_size = m_d3d_mesh_clone->GetNumVertices();
		for( ulong ti = 0; ti < vertex_size; ti++ )
		{
			vector3<float> input_vec;
			input_vec.x = v_vertex_buffer[ti].x;
			input_vec.y = v_vertex_buffer[ti].y;
			input_vec.z = v_vertex_buffer[ti].z;

			float lastweight = 0.0f;
			vector3<float> output_vec;
			for ( ulong bi = 0; bi < 3; bi ++ )
			{
				int index = ( v_vertex_buffer[ti].bone_index & ( 0x000000ff << (bi * 8) ) ) >> (bi * 8);
				float weight = v_vertex_buffer[ti].weight[bi];

				lastweight += weight;
				
				output_vec = output_vec + weight * ( input_vec * m_final_matrix_array[index] );
			}

			lastweight = 1.0f - lastweight;
			output_vec = output_vec + lastweight * ( input_vec * m_final_matrix_array[( v_vertex_buffer[ti].bone_index & ( 0x000000ff << 24 ) ) >> 24] );

			v_vertex_buffer[ti].x = output_vec.x;
			v_vertex_buffer[ti].y = output_vec.y;
			v_vertex_buffer[ti].z = output_vec.z;
		}
		m_d3d_mesh_clone->UnlockVertexBuffer();

		

		d3d9_effect_vertex_blend* effect = (d3d9_effect_vertex_blend* )device->get_effect(US_EFFECT_VERTEX_BLEND);

		effect->set_technique();

		matrix4<float> world_matrix;
		matrix4<float> view_matrix;
		matrix4<float> projection_matrix;
		matrix4<float> world_view_projection_matrix;

		world_matrix = device->get_world_matrix();
		view_matrix  = device->get_view_matrix();
		projection_matrix = device->get_projection_matrix();

		world_view_projection_matrix = world_matrix * view_matrix * projection_matrix;

		effect->set_transform( world_view_projection_matrix );

		uint number_of_passes = 0;
		effect->begin(&number_of_passes, 0);

		for( ulong i = 0; i < number_of_passes; ++i)
		{
			effect->begin_pass(i);
			{
				effect->set_matrix_array( m_final_matrix_array[0], m_final_matrix_array.size() );

				for ( ulong j = 0; j < m_texture_array.size() ; j++ )
				{
					if ( m_texture_array[j].m_diffuse )
					{
						if ( m_texture_array[j].m_diffuse->is_valid() )
						{
							effect->set_texture( m_texture_array[j].m_diffuse );
						}
						else 
						{
							effect->set_texture( NULL );
						}
					}
					else
					{
						effect->set_texture( NULL );
					}					

					effect->commit_changes();

					m_d3d_mesh_clone->DrawSubset( j );
				}

			}

			effect->end_pass();
		}

		effect->end();	

		AUTO_RELEASE( m_d3d_mesh_clone );

		//*/

		//////////////////////////////////////////////////////////////////////////
		// shader
		//////////////////////////////////////////////////////////////////////////
		///*
		d3d9_effect_vertex_blend* m_render = (d3d9_effect_vertex_blend*)args->m_render;
		//args->m_device->set_vertex_declaration( m_d3d_mesh->GetFVF())

		m_render->set_matrix_array( &m_final_matrix_array[0], m_final_matrix_array.size() );

		for ( int j = 0; j < (int)m_texture_array.size() ; j++ )
		{
			if ( m_texture_array[j].m_diffuse )
			{
				if ( m_texture_array[j].m_diffuse->is_valid() )
				{
					m_render->set_texture( 0, m_texture_array[j].m_diffuse->get_handle() );
				}
				else 
				{
					m_render->set_texture( 0, NULL );
				}
			}
			else
			{
				m_render->set_texture( 0, NULL );
			}					

			m_render->commit_changes();

			m_d3d_mesh->DrawSubset( j );
		}				
		
		//*/

	}

	bool d3d9_skinning_hardware_mesh::intersect( const ray<double>& in_ray, intersect_result& out_result )
	{
		return false;
	}
}