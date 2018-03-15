///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_mesh_buffer_factory.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_mesh_buffer_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_hardware_mesh_buffer_factory.h"

namespace uniscope_globe
{
	hardware_mesh_buffer_factory::hardware_mesh_buffer_factory( void )
	{

	}

	hardware_mesh_buffer_factory::~hardware_mesh_buffer_factory( void )
	{

	}

	hardware_mesh_buffer* hardware_mesh_buffer_factory::create( usx_mesh* in_mesh, usx_frame* in_root_frame, cpstr in_path )
	{
		if( in_mesh->m_fvf == position_weights4_bone_texture::fvf )
		{
			hardware_mesh_skinned_buffer* v_buffer = new hardware_mesh_skinned_buffer();

			v_buffer->m_path = in_path;
			v_buffer->m_root_frame = in_root_frame;

			copy( v_buffer, in_mesh );

			return v_buffer;
		}
		else
		{
			hardware_mesh_buffer* v_buffer = new hardware_mesh_buffer();

			v_buffer->m_path = in_path;

			copy( v_buffer, in_mesh );

			//if( system_environment::s_enable_shadow )
			//{
			//	
			//}

			return v_buffer;
		}

		return NULL;

	}

	void hardware_mesh_buffer_factory::copy( hardware_mesh_buffer* in_buffer, usx_mesh* in_mesh )
	{
		in_buffer->m_name = in_mesh->m_name;		

		in_buffer->m_fvf = in_mesh->m_fvf;
		in_buffer->m_stride = in_mesh->m_vertex_stride;

		vector3<float> v_min = in_mesh->m_bbox.get_min();
		vector3<float> v_max = in_mesh->m_bbox.get_max();
		in_buffer->m_aabb.set_extent( vector3<double>( (float*)&v_min.x ), vector3<double>( (float*)&v_max.x ) );


		in_buffer->m_num_of_vertices = in_mesh->m_number_of_vertices;
		in_buffer->m_vertex_buffer_size =  in_buffer->m_stride * in_buffer->m_num_of_vertices;
		in_buffer->m_vertex_buffer = new unsigned char[in_buffer->m_vertex_buffer_size];

		// vertex
		int vertex_offset = 0;
		for ( ulong i = 0; i < in_mesh->m_number_of_vertices; i++ )
		{
			in_buffer->set_vector3( vertex_offset, i, in_mesh->m_vertex_data[i] );
		}	

		// texture coordinates
		float* v_texture_coord = NULL;
		ulong v_texture_coord_num = in_mesh->m_vertex_texture_coords.size();
		if ( v_texture_coord_num > 0 )
		{
			vertex_offset += 3 * sizeof(float);
			v_texture_coord = in_mesh->m_vertex_texture_coords[0];
			for ( int i = 0; i < (int)in_mesh->m_number_of_vertices; i++ )
			{
				in_buffer->set_vector2( vertex_offset, i, vector2<float>( v_texture_coord[(i<<1)+0], v_texture_coord[(i<<1)+1]) );
			}

			if ( v_texture_coord_num > 1 )
			{
				vertex_offset += 2 * sizeof(float);
				v_texture_coord = in_mesh->m_vertex_texture_coords[1];
				for ( int i = 0; i < (int)in_mesh->m_number_of_vertices; i++ )
				{
					in_buffer->set_vector2( vertex_offset, i,  vector2<float>(v_texture_coord[(i<<1)+0], v_texture_coord[(i<<1)+1]) );
				}
			}
		}

		in_buffer->m_num_of_triangles = in_mesh->m_number_of_faces;
		if ( in_mesh->m_number_of_vertices > 1<<16 )
		{
			in_buffer->m_32bit_index = true;
			in_buffer->m_index_buffer = new ulong[ in_buffer->m_num_of_triangles * 3 ];
			in_buffer->m_index_buffer_size = sizeof( ulong ) * in_buffer->m_num_of_triangles * 3;
			memcpy( in_buffer->m_index_buffer, in_mesh->m_face_index, in_buffer->m_index_buffer_size );
		}
		else
		{			
			in_buffer->m_index_buffer = new ushort[ in_buffer->m_num_of_triangles * 3 ];
			ushort* v_buffer = (ushort*)in_buffer->m_index_buffer;
			in_buffer->m_index_buffer_size = sizeof(ushort) * in_buffer->m_num_of_triangles * 3;
			int v_off = 0;
			for ( int i = 0; i < (int)in_buffer->m_num_of_triangles; i++ )
			{
				v_off = i * 3;
				v_buffer[v_off]   = in_mesh->m_face_index[i].a;
				v_buffer[v_off+1] = in_mesh->m_face_index[i].b;
				v_buffer[v_off+2] = in_mesh->m_face_index[i].c;
			}			
		}


		if( in_mesh->m_attribute_data != NULL )
		{
			in_buffer->m_attribute_buffer = new ulong[in_buffer->m_num_of_triangles];
			in_buffer->m_attribute_buffer_size = sizeof( ulong ) * in_buffer->m_num_of_triangles;
			memcpy( in_buffer->m_attribute_buffer, in_mesh->m_attribute_data, in_buffer->m_attribute_buffer_size );
		}

		in_buffer->m_material_set = new material_set();

		for ( int i = 0; i < in_mesh->m_materials.size(); i++ )
		{
			usx_material* v_mat = in_mesh->m_materials[i];

			material v_new_mat;
			memcpy( (void*)&v_new_mat.m_ambient_rgb, (void*)&v_mat->m_ambient_color_rgb, sizeof( color_value ) );
			memcpy( (void*)&v_new_mat.m_diffuse_rgba, (void*)&v_mat->m_diffuse_color_rgba, sizeof( color_value ) );
			memcpy( (void*)&v_new_mat.m_specular_rgb, (void*)&v_mat->m_ambient_color_rgb, sizeof( color_value ) );
			memcpy( (void*)&v_new_mat.m_emissive_rgb, (void*)&v_mat->m_emissive_color_rgb, sizeof( color_value ) );			
			v_new_mat.m_power = v_mat->m_power;
			v_new_mat.m_name = v_mat->m_name;

			texture_file v_txt_file;

			for ( int j = 0; j < v_mat->m_texture_files.size(); j++ )
			{				
				usx_texture_file& v_txt = v_mat->m_texture_files[j];

				if( v_txt.v_type == 0 && v_txt.m_texture_file.size() > 0 )
				{
					v_txt_file.m_diffuse_texture = v_txt.m_texture_file;
					US_STRING_LOWER( v_txt_file.m_diffuse_texture );
				}
				else if( v_txt.v_type == 1 && v_txt.m_texture_file.size() > 0 )
				{
					v_txt_file.m_self_illuminant_texture = v_txt.m_texture_file;
					US_STRING_LOWER( v_txt_file.m_self_illuminant_texture );
				}
				else if( v_txt.v_type == 2 && v_txt.m_texture_file.size() > 0 ) 
				{
					v_txt_file.m_specular_texture = v_txt.m_texture_file;
					US_STRING_LOWER( v_txt_file.m_specular_texture );
				}
				else if( v_txt.v_type == 3 && v_txt.m_texture_file.size() > 0 )
				{
					v_txt_file.m_bump_texture = v_txt.m_texture_file;
					US_STRING_LOWER( v_txt_file.m_bump_texture );
				}
			}

			in_buffer->m_material_set->m_materials.push_back( v_new_mat );
			in_buffer->m_material_set->m_texture_files.push_back( v_txt_file );
		}
	}

	void hardware_mesh_buffer_factory::copy( hardware_mesh_skinned_buffer* in_buffer, usx_mesh* in_mesh )
	{
		in_buffer->m_fvf = in_mesh->m_fvf;
		in_buffer->m_stride = in_mesh->m_vertex_stride;

		in_buffer->m_num_of_vertices = in_mesh->m_number_of_vertices;
		in_buffer->m_vertex_buffer_size =  in_buffer->m_stride * in_buffer->m_num_of_vertices;
		in_buffer->m_vertex_buffer = new unsigned char[in_buffer->m_vertex_buffer_size];
		memset( in_buffer->m_vertex_buffer, 0, in_buffer->m_vertex_buffer_size );

		in_buffer->m_root_frame->update_hierarchy( matrix4<double>::s_identity );

		int offset = 0;

		//
		// first we must copy weights info.
		//
		offset += 3 * sizeof( float );

		// øΩ±¥√…∆§–≈œ¢
		std::vector<int> weights_count_array;
		weights_count_array.resize( in_buffer->m_num_of_vertices );
		memset( &weights_count_array[0], 0, in_buffer->m_num_of_vertices * sizeof(int) );
		for( int j = 0; j < (int)in_mesh->m_skin_info->m_weights_array.size(); j++ )
		{
			usx_weights_info* weights = in_mesh->m_skin_info->m_weights_array[j];
			for ( int i = 0; i < (int)weights->m_indices.size(); i++ )
			{
				int sequence = weights_count_array[weights->m_indices[i]];

				in_buffer->set_float( offset + sequence * sizeof(float), weights->m_indices[i], weights->m_weights[i] );

				ulong bone_index = in_buffer->get_ulong( offset + 4 * sizeof(float), weights->m_indices[i] );
				bone_index |= j << (8 * sequence);

				in_buffer->set_ulong( offset + 4 * sizeof(float), weights->m_indices[i], bone_index );

				weights_count_array[weights->m_indices[i]] ++;
			}

			usx_frame* v_frame = in_buffer->m_root_frame->find_frame( weights->m_frame_name.c_str() );
			in_buffer->m_frame_array[j] = v_frame;			
			in_buffer->m_offset_matrix_array[j] = weights->m_offset_matrix;
			in_buffer->m_original_matrix_array[j] = matrix4<float>( ( weights->m_offset_matrix * v_frame->m_combined_mat ).m ) ;
		}

		in_buffer->m_num_of_bones = in_mesh->m_skin_info->m_weights_array.size();

		offset -= 3 * sizeof(float);

		// øΩ±¥∂•µ„
		in_buffer->m_aabb.make_invalid();
		for ( int i = 0; i < (int)in_buffer->m_num_of_vertices; i++ )
		{
			vector3<float> vec;
			vec.x = ( in_mesh->m_vertex_data[i] ).x;
			vec.y = ( in_mesh->m_vertex_data[i] ).y;
			vec.z = ( in_mesh->m_vertex_data[i] ).z;

			in_buffer->set_vector3( offset, i, vec );

			in_buffer->m_aabb.combine( get_blended_vertex( in_buffer, i ) );
		}

		// jump over 3 position coordinates
		offset += 3 * sizeof(float);

		// jump over 4 bone weights and 1 bone index.
		offset += 4 * sizeof( float ) + sizeof(float);

		// øΩ±¥Œ∆¿Ì◊¯±Í
		float* v_texture_coord = NULL;
		ulong v_texture_coord_num = in_mesh->m_vertex_texture_coords.size();
		if ( v_texture_coord_num > 0 )
		{
			v_texture_coord = in_mesh->m_vertex_texture_coords[0];
			for ( int i = 0; i < (int)in_buffer->m_num_of_vertices; i++ )
			{
				in_buffer->set_vector2( offset, i, vector2<float>( v_texture_coord[(i<<1)+0], v_texture_coord[(i<<1)+1] ) );
			}
		}

		in_buffer->m_num_of_triangles = in_mesh->m_number_of_faces;
		if ( in_buffer->m_num_of_vertices > 1<<16 )
		{
			in_buffer->m_32bit_index = true;
			in_buffer->m_index_buffer = new ulong[ in_buffer->m_num_of_triangles * 3 ];
			in_buffer->m_index_buffer_size = sizeof( ulong ) * in_buffer->m_num_of_triangles * 3;
			memcpy( in_buffer->m_index_buffer, in_mesh->m_face_index, in_buffer->m_index_buffer_size );
		}
		else
		{			
			in_buffer->m_index_buffer = new ushort[ in_buffer->m_num_of_triangles * 3 ];
			ushort* v_buffer = (ushort*)in_buffer->m_index_buffer;
			in_buffer->m_index_buffer_size = sizeof(ushort) * in_buffer->m_num_of_triangles * 3;
			int v_off = 0;
			for ( int i = 0; i < (int)in_buffer->m_num_of_triangles; i++ )
			{
				v_off = i * 3;
				v_buffer[v_off]   = in_mesh->m_face_index[i].a;
				v_buffer[v_off+1] = in_mesh->m_face_index[i].b;
				v_buffer[v_off+2] = in_mesh->m_face_index[i].c;
			}			
		}

		in_buffer->m_attribute_buffer = new ulong[in_buffer->m_num_of_triangles];
		in_buffer->m_attribute_buffer_size = sizeof( ulong ) * in_buffer->m_num_of_triangles;
		memcpy( in_buffer->m_attribute_buffer, in_mesh->m_attribute_data, in_buffer->m_attribute_buffer_size );

		in_buffer->m_material_set = new material_set();

		for ( int i = 0; i < in_mesh->m_materials.size(); i++ )
		{
			usx_material* v_mat = in_mesh->m_materials[i];

			material v_new_mat;
			memcpy( (void*)&v_new_mat.m_ambient_rgb, (void*)&v_mat->m_ambient_color_rgb, sizeof( color_value ) );
			memcpy( (void*)&v_new_mat.m_diffuse_rgba, (void*)&v_mat->m_diffuse_color_rgba, sizeof( color_value ) );
			memcpy( (void*)&v_new_mat.m_specular_rgb, (void*)&v_mat->m_ambient_color_rgb, sizeof( color_value ) );
			memcpy( (void*)&v_new_mat.m_emissive_rgb, (void*)&v_mat->m_emissive_color_rgb, sizeof( color_value ) );			
			v_new_mat.m_power = v_mat->m_power;
			v_new_mat.m_name = v_mat->m_name;

			texture_file v_txt_file;

			for ( int j = 0; j < v_mat->m_texture_files.size(); j++ )
			{				
				usx_texture_file& v_txt = v_mat->m_texture_files[j];

				if( v_txt.v_type == 0 && v_txt.m_texture_file.size() > 0 )
				{
					v_txt_file.m_diffuse_texture = v_txt.m_texture_file;
					US_STRING_LOWER( v_txt_file.m_diffuse_texture );
				}
				else if( v_txt.v_type == 1 && v_txt.m_texture_file.size() > 0 )
				{
					v_txt_file.m_self_illuminant_texture = v_txt.m_texture_file;
					US_STRING_LOWER( v_txt_file.m_self_illuminant_texture );
				}
				else if( v_txt.v_type == 2 && v_txt.m_texture_file.size() > 0 ) 
				{
					v_txt_file.m_specular_texture = v_txt.m_texture_file;
					US_STRING_LOWER( v_txt_file.m_specular_texture );
				}
				else if( v_txt.v_type == 3 && v_txt.m_texture_file.size() > 0 )
				{
					v_txt_file.m_bump_texture = v_txt.m_texture_file;
					US_STRING_LOWER( v_txt_file.m_bump_texture );
				}
			}

			in_buffer->m_material_set->m_materials.push_back( v_new_mat );
			in_buffer->m_material_set->m_texture_files.push_back( v_txt_file );
		}
	}

	vector3<double> hardware_mesh_buffer_factory::get_blended_vertex( hardware_mesh_skinned_buffer* in_buffer, ulong vert_index )
	{
		int pos_offset = 0;
		int weight_offset = 3 * sizeof(float);
		int bone_offset = 3 * sizeof(float) + 4 * sizeof(float);

		vector3<float> input_vec = in_buffer->get_vector3( pos_offset, vert_index );

		ulong bone_index = in_buffer->get_ulong( bone_offset, vert_index );

		float lastweight = 0.0f;
		vector3<float> output_vec;
		for ( ulong bi = 0; bi < 3; bi ++ )
		{
			int index = ( bone_index & ( 0x000000ff << (bi * 8) ) ) >> (bi * 8);			
			float weight = in_buffer->get_float( weight_offset + bi * sizeof(float), vert_index );

			lastweight += weight;

			output_vec = output_vec + weight * ( input_vec * in_buffer->m_original_matrix_array[index] );
		}

		lastweight = 1.0f - lastweight;
		output_vec = output_vec + lastweight * ( input_vec * in_buffer->m_original_matrix_array[( bone_index & ( 0x000000ff << 24 ) ) >> 24] );

		return vector3<double>( output_vec.x, output_vec.y, output_vec.z );
	}


}