///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_mesh_skinned_buffer.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_mesh_skinned_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_hardware_mesh_skinned_buffer.h"

namespace uniscope_globe
{
	hardware_mesh_skinned_buffer::hardware_mesh_skinned_buffer( void )
	{
		m_type = US_HARDWARE_BUFFER_SKINNING;

		m_root_frame = NULL;

		m_frame_array.resize( 48 );
		memset( &m_frame_array[0], 0, 48 );

		m_offset_matrix_array.resize( 48 );
		memset( &m_offset_matrix_array[0], 0, 48 );

		m_original_matrix_array.resize( 48 );
		memset( &m_original_matrix_array[0], 0, 48 );

		m_num_of_bones = 0;
	}

	hardware_mesh_skinned_buffer::~hardware_mesh_skinned_buffer( void )
	{
	}

	//void hardware_mesh_skinned_buffer::initialize( void )
	//{
	//	m_fvf = position_weights4_bone_texture::fvf;

	//	m_stride = position_weights4_bone_texture::stride;

	//	m_num_of_vertices = m_usx_mesh->m_number_of_vertices;
	//	m_vertex_buffer_size =  m_stride * m_num_of_vertices;

	//	m_vertex_buffer = new position_weights4_bone_texture[m_num_of_vertices];
	//	position_weights4_bone_texture* v_vertex_buffer = (position_weights4_bone_texture*)m_vertex_buffer;
	//	memset( (void*)v_vertex_buffer, 0, sizeof( position_weights4_bone_texture ) * m_num_of_vertices );

	//	m_root_frame->update_hierarchy( matrix4<double>::s_identity );

	//	// øΩ±¥√…∆§–≈œ¢
	//	std::vector<int> weights_count_array;
	//	weights_count_array.resize( m_num_of_vertices );
	//	memset( &weights_count_array[0], 0, m_num_of_vertices * sizeof(int) );
	//	for( int j = 0; j < (int)m_usx_mesh->m_skin_info->m_weights_array.size(); j++ )
	//	{
	//		usx_weights_info* weights = m_usx_mesh->m_skin_info->m_weights_array[j];
	//		for ( int i = 0; i < (int)weights->m_indices.size(); i++ )
	//		{
	//			int sequence = weights_count_array[weights->m_indices[i]];

	//			v_vertex_buffer[weights->m_indices[i]].weight[sequence] = weights->m_weights[i];
	//			v_vertex_buffer[weights->m_indices[i]].bone_index |= j << (8 * sequence);

	//			weights_count_array[weights->m_indices[i]] ++;
	//		}

	//		usx_frame* v_frame = m_root_frame->find_frame( weights->m_frame_name.c_str() );
	//		m_frame_array[j] = v_frame;			
	//		m_offset_matrix_array[j] = weights->m_offset_matrix;
	//		m_original_matrix_array[j] = matrix4<float>( ( weights->m_offset_matrix * v_frame->m_combined_mat ).m ) ;
	//	}

	//	// øΩ±¥∂•µ„
	//	m_aabb.make_invalid();
	//	for ( int i = 0; i < (int)m_num_of_vertices; i++ )
	//	{
	//		//m_aabb.combine( vector_3d( m_usx_mesh->m_vertex_data[i].x, m_usx_mesh->m_vertex_data[i].y, m_usx_mesh->m_vertex_data[i].z ) );
	//		v_vertex_buffer[i].x = ( m_usx_mesh->m_vertex_data[i] ).x;
	//		v_vertex_buffer[i].y = ( m_usx_mesh->m_vertex_data[i] ).y;
	//		v_vertex_buffer[i].z = ( m_usx_mesh->m_vertex_data[i] ).z;
	//		//v_vertex_buffer[i].weight[0] = 0.0f;
	//		//v_vertex_buffer[i].weight[1] = 0.0f;
	//		//v_vertex_buffer[i].weight[2] = 0.0f;
	//		//v_vertex_buffer[i].weight[3] = 0.0f;
	//		//v_vertex_buffer[i].bone_index = 0;			
	//		v_vertex_buffer[i].tu1 = 0.0f;
	//		v_vertex_buffer[i].tv1 = 0.0f;

	//		m_aabb.combine( get_blended_vertex( v_vertex_buffer[i] ) );
	//	}

	//	// øΩ±¥Œ∆¿Ì◊¯±Í
	//	float* v_texture_coord = NULL;
	//	ulong v_texture_coord_num = m_usx_mesh->m_vertex_texture_coords.size();
	//	if ( v_texture_coord_num > 0 )
	//	{
	//		v_texture_coord = m_usx_mesh->m_vertex_texture_coords[0];
	//		for ( int i = 0; i < (int)m_num_of_vertices; i++ )
	//		{
	//			v_vertex_buffer[i].tu1 = v_texture_coord[(i<<1)+0];
	//			v_vertex_buffer[i].tv1 = v_texture_coord[(i<<1)+1];
	//		}
	//	}

	//	m_num_of_triangles = m_usx_mesh->m_number_of_faces;
	//	if ( m_num_of_vertices > 1<<16 )
	//	{
	//		m_32bit_index = true;
	//		m_index_buffer = new DWORD[ m_num_of_triangles * 3 ];
	//		m_index_buffer_size = sizeof( DWORD ) * m_num_of_triangles * 3;
	//		memcpy( m_index_buffer, m_usx_mesh->m_face_index, m_index_buffer_size );
	//	}
	//	else
	//	{			
	//		m_index_buffer = new ushort[ m_num_of_triangles * 3 ];
	//		ushort* v_buffer = (ushort*)m_index_buffer;
	//		m_index_buffer_size = sizeof(ushort) * m_num_of_triangles * 3;
	//		int v_off = 0;
	//		for ( int i = 0; i < (int)m_num_of_triangles; i++ )
	//		{
	//			v_off = i * 3;
	//			v_buffer[v_off]   = m_usx_mesh->m_face_index[i].a;
	//			v_buffer[v_off+1] = m_usx_mesh->m_face_index[i].b;
	//			v_buffer[v_off+2] = m_usx_mesh->m_face_index[i].c;
	//		}			
	//	}

	//	m_attribute_buffer = new ulong[m_num_of_triangles];
	//	m_attribute_buffer_size = sizeof( ulong ) * m_num_of_triangles;
	//	memcpy( m_attribute_buffer, m_usx_mesh->m_attribute_data, m_attribute_buffer_size );

	//	m_material_set = new material_set();

	//	for ( int i = 0; i < m_usx_mesh->m_materials.size(); i++ )
	//	{
	//		usx_material* v_mat = m_usx_mesh->m_materials[i];

	//		material v_new_mat;
	//		memcpy( (void*)&v_new_mat.m_ambient_rgb, (void*)&v_mat->m_ambient_color_rgb, sizeof( color_value ) );
	//		memcpy( (void*)&v_new_mat.m_diffuse_rgba, (void*)&v_mat->m_diffuse_color_rgba, sizeof( color_value ) );
	//		memcpy( (void*)&v_new_mat.m_specular_rgb, (void*)&v_mat->m_ambient_color_rgb, sizeof( color_value ) );
	//		memcpy( (void*)&v_new_mat.m_emissive_rgb, (void*)&v_mat->m_emissive_color_rgb, sizeof( color_value ) );			
	//		v_new_mat.m_power = v_mat->m_power;
	//		v_new_mat.m_name = v_mat->m_name;

	//		texture_file v_txt_file;

	//		for ( int j = 0; j < v_mat->m_texture_files.size(); j++ )
	//		{				
	//			usx_texture_file& v_txt = v_mat->m_texture_files[j];

	//			if( v_txt.v_type == 0 && v_txt.m_texture_file.size() > 0 )
	//			{
	//				v_txt_file.m_diffuse_texture = v_txt.m_texture_file + m_ude_suffix;
	//				US_STRING_LOWER( v_txt_file.m_diffuse_texture );
	//			}
	//			else if( v_txt.v_type == 1 && v_txt.m_texture_file.size() > 0 )
	//			{
	//				v_txt_file.m_self_illuminant_texture = v_txt.m_texture_file + m_ude_suffix;
	//				US_STRING_LOWER( v_txt_file.m_self_illuminant_texture );
	//			}
	//			else if( v_txt.v_type == 2 && v_txt.m_texture_file.size() > 0 ) 
	//			{
	//				v_txt_file.m_specular_texture = v_txt.m_texture_file + m_ude_suffix;
	//				US_STRING_LOWER( v_txt_file.m_specular_texture );
	//			}
	//			else if( v_txt.v_type == 3 && v_txt.m_texture_file.size() > 0 )
	//			{
	//				v_txt_file.m_bump_texture = v_txt.m_texture_file + m_ude_suffix;
	//				US_STRING_LOWER( v_txt_file.m_bump_texture );
	//			}
	//		}

	//		m_material_set->m_materials.push_back( v_new_mat );
	//		m_material_set->m_texture_files.push_back( v_txt_file );
	//	}
	//}

	//vector3<double> hardware_mesh_skinned_buffer::get_blended_vertex( position_weights4_bone_texture& in_pos )
	//{
	//	vector3<float> input_vec;
	//	input_vec.x = in_pos.x;
	//	input_vec.y = in_pos.y;
	//	input_vec.z = in_pos.z;

	//	float lastweight = 0.0f;
	//	vector3<float> output_vec;
	//	for ( ulong bi = 0; bi < 3; bi ++ )
	//	{
	//		int index = ( in_pos.bone_index & ( 0x000000ff << (bi * 8) ) ) >> (bi * 8);
	//		float weight = in_pos.weight[bi];

	//		lastweight += weight;

	//		output_vec = output_vec + weight * ( input_vec * m_original_matrix_array[index] );
	//	}

	//	lastweight = 1.0f - lastweight;
	//	output_vec = output_vec + lastweight * ( input_vec * m_original_matrix_array[( in_pos.bone_index & ( 0x000000ff << 24 ) ) >> 24] );

	//	return vector3<double>( output_vec.x, output_vec.y, output_vec.z );
	//}

	hardware_mesh_buffer* hardware_mesh_skinned_buffer::clone( void )
	{
		hardware_mesh_skinned_buffer* v_buffer = new hardware_mesh_skinned_buffer();

		// name
		v_buffer->m_name = this->m_name;

		// path
		v_buffer->m_path = this->m_path;

		// fvf
		v_buffer->m_fvf = this->m_fvf;

		// stride
		v_buffer->m_stride = this->m_stride;

		//// usx_mesh
		//v_buffer->m_usx_mesh = this->m_usx_mesh;

		// vertex buffer
		if( this->m_vertex_buffer != NULL )
		{
			v_buffer->m_num_of_vertices = this->m_num_of_vertices;
			v_buffer->m_vertex_buffer_size = this->m_vertex_buffer_size;
			v_buffer->m_vertex_buffer = new unsigned char[v_buffer->m_vertex_buffer_size];
			memcpy( (void*)v_buffer->m_vertex_buffer, (void*)this->m_vertex_buffer, v_buffer->m_vertex_buffer_size );
		}

		// index buffer
		if( this->m_index_buffer != NULL )
		{
			v_buffer->m_num_of_triangles = this->m_num_of_triangles;
			v_buffer->m_index_buffer_size = this->m_index_buffer_size;
			v_buffer->m_index_buffer = new unsigned char[v_buffer->m_index_buffer_size];
			memcpy( (void*)v_buffer->m_index_buffer, (void*)this->m_index_buffer, v_buffer->m_index_buffer_size );
		}

		// attribute buffer
		if( this->m_attribute_buffer != NULL )
		{
			v_buffer->m_attribute_buffer_size = this->m_attribute_buffer_size;
			v_buffer->m_attribute_buffer = new unsigned char[v_buffer->m_attribute_buffer_size];
			memcpy( (void*)v_buffer->m_attribute_buffer, (void*)this->m_attribute_buffer, v_buffer->m_attribute_buffer_size );
		}

		// material
		v_buffer->m_material_set = new material_set();
		v_buffer->m_material_set->m_materials = this->m_material_set->m_materials;
		v_buffer->m_material_set->m_texture_files = this->m_material_set->m_texture_files;

		v_buffer->m_32bit_index = this->m_32bit_index;


		return v_buffer;
	}

	//long hardware_mesh_skinned_buffer::create( render_device* in_device )
	//{
	//	if( m_d3d9_mesh != NULL /*|| m_usx_mesh == NULL*/ ) return 0;

	//	//
	//	// create mesh		
	//	//
	//	D3DVERTEXELEMENT9 decl[] = 
	//	{
	//		{ 0, 0,     D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
	//		{ 0, 3*4,   D3DDECLTYPE_FLOAT4,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_BLENDWEIGHT,    0 },
	//		{ 0, 7*4,   D3DDECLTYPE_D3DCOLOR,   D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_BLENDINDICES,     0 },
	//		{ 0, 8*4,   D3DDECLTYPE_FLOAT2,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,     0 },
	//		D3DDECL_END()
	//	};

	//	HRESULT hr = D3DXCreateMesh(	m_num_of_triangles,
	//									m_num_of_vertices,
	//									D3DXMESH_MANAGED | (m_32bit_index?D3DXMESH_32BIT:0x000),
	//									decl,
	//									(LPDIRECT3DDEVICE9)in_device->get_device(),
	//									&m_d3d9_mesh );

	//	m_mem_size = 0;

	//	//vertex
	//	void* v_vertices = NULL;
	//	hr = m_d3d9_mesh->LockVertexBuffer( 0, (void**)&v_vertices );
	//	{
	//		memcpy( (byte*)v_vertices, m_vertex_buffer, m_vertex_buffer_size );			
	//	}
	//	hr = m_d3d9_mesh->UnlockVertexBuffer();
	//	m_mem_size += m_vertex_buffer_size;

	//	void* v_indices = NULL;
	//	hr = m_d3d9_mesh->LockIndexBuffer( 0, (void**)&v_indices );
	//	{
	//		memcpy( (byte*)v_indices, m_index_buffer, m_index_buffer_size );			
	//	}
	//	hr = m_d3d9_mesh->UnlockIndexBuffer();
	//	m_mem_size += m_index_buffer_size;


	//	DWORD* v_attribute;
	//	hr = m_d3d9_mesh->LockAttributeBuffer( 0, &v_attribute );
	//	{
	//		memcpy( (byte*)v_attribute, m_attribute_buffer, m_attribute_buffer_size );
	//	}
	//	hr = m_d3d9_mesh->UnlockAttributeBuffer();

	//	m_mem_size += m_attribute_buffer_size;

	//	m_d3d9_mesh->OptimizeInplace( D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL );

	//	return m_mem_size;
	//}

	
}