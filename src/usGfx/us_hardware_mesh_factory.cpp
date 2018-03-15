
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_mesh_factory.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_mesh_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_hardware_mesh_factory.h"

namespace uniscope_globe
{
	//hardware_mesh* hardware_mesh_factory::create( usx_mesh* in_mesh, usx_frame* in_root_frame, cpstr in_path )
	//{
	//	if( in_mesh->m_fvf == position_weights4_bone_texture::fvf )
	//	{
	//		d3d9_skinning_hardware_mesh* v_new_mesh = new d3d9_skinning_hardware_mesh();

	//		v_new_mesh->m_path = in_path;
	//		v_new_mesh->m_root_frame = in_root_frame;

	//		copy_skinning_mesh( v_new_mesh, in_mesh );

	//		return v_new_mesh;
	//	}
	//	else
	//	{
	//		d3d9_hardware_mesh* v_new_mesh = new d3d9_hardware_mesh();

	//		v_new_mesh->m_path = in_path;

	//		copy_single_mesh( v_new_mesh, in_mesh );

	//		//if( system_environment::s_enable_shadow )
	//		//{
	//		//	
	//		//}

	//		return v_new_mesh;
	//	}

	//	return NULL;

	//}

	hardware_mesh* hardware_mesh_factory::create_complex_mesh( document_base* in_doc, usx_parser_object_container* v_container )
	{
		d3d9_complex_hardware_mesh* v_new_mesh = new d3d9_complex_hardware_mesh();
		v_new_mesh->m_is_texture_ready = true;
		v_new_mesh->m_root_frame = v_container->m_root_frame;


		// 创建simple_animation_player
		if( v_new_mesh->m_root_frame != NULL )
		{			
			v_new_mesh->m_root_frame->reset();
			v_new_mesh->m_root_frame->update_hierarchy( matrix4<double>::s_identity );

			v_container->m_root_frame = NULL;
		}

		if( v_new_mesh->m_root_frame != NULL )
		{
			for( int i = 0; i < (int)v_container->m_animation_set_array.size(); i++ )
			{
				usx_animation_set* anim_set = v_container->m_animation_set_array[i];	

				simple_animation_player* v_player = new simple_animation_player( anim_set );
				v_player->attach( v_new_mesh->m_root_frame );

				v_new_mesh->m_animation_player_map.insert( make_pair(anim_set->m_name, v_player ) );
			}		

			v_container->m_animation_set_array.clear();
		}

		// 创建 submesh
		usx_mesh_frame_map::iterator itr = v_container->m_mesh_frame_map.begin();
		for ( ; itr != v_container->m_mesh_frame_map.end(); itr ++ )
		{
			usx_mesh* v_usx_mesh = itr->first;
			usx_frame* v_usx_frame = itr->second;	
			v_new_mesh->m_fvf = v_usx_mesh->m_fvf;

			d3d9_hardware_mesh* v_sub_mesh = create_single_mesh( in_doc, v_usx_mesh, v_container );

			v_new_mesh->m_bound_box.combine( v_sub_mesh->m_mesh_aabb );
			v_new_mesh->m_mesh_frame_map.insert( make_pair( v_sub_mesh,  v_usx_frame ) );
		}

		return dynamic_cast<hardware_mesh*>(v_new_mesh);
	}

	d3d9_hardware_mesh* hardware_mesh_factory::create_single_mesh( document_base* in_doc, usx_mesh* in_mesh, usx_parser_object_container* v_container )
	{
		d3d9_hardware_mesh* v_mesh = NULL;
		if( in_mesh->m_fvf & D3DFVF_LASTBETA_D3DCOLOR )
		{
			d3d9_skinning_hardware_mesh* v_skinning_mesh = new d3d9_skinning_hardware_mesh();
			v_skinning_mesh->m_path = v_container->get_path();
			v_skinning_mesh->m_root_frame = v_container->m_root_frame;
			copy_skinning_mesh( in_doc, v_skinning_mesh, in_mesh );

			v_mesh = v_skinning_mesh;

			v_container->m_root_frame = NULL;
		}
		else
		{
			d3d9_hardware_mesh* v_sub_mesh = new d3d9_hardware_mesh();
			v_sub_mesh->m_path = v_container->get_path();
			copy_single_mesh( in_doc, v_sub_mesh, in_mesh );

			v_mesh = v_sub_mesh;
			//if( system_environment::s_enable_shadow )
			//{
			//	
			//}
		}

		return v_mesh;
	}


	d3d9_hardware_mesh* hardware_mesh_factory::create_single_mesh_from_polygon( document_base* in_doc, vector_3d* in_pos_array, ulong v_size, ulong line_color, ulong fill_color )

	{
		d3d9_hardware_mesh* v_mesh = new d3d9_hardware_mesh();

		// 三角化
		int max_ni = 0;
		double max_z = -math_d::s_max_real;
		std::vector<vector_2d> tri_array;
		for ( int ni = 0; ni < (int)v_size; ni ++ )
		{
			tri_array.push_back( vector_2d( in_pos_array[ni].x, in_pos_array[ni].z ) );
			if ( in_pos_array[ni].z > max_z )
			{
				max_ni = ni;
				max_z = in_pos_array[ni].z;
			}

		}

		int in_triangle_start_mode = 0;
		vector_2d prev_pos, next_pos, curr_pos;
		curr_pos = tri_array[max_ni];
		if (max_ni == 0 || max_ni == v_size - 1)
		{
			prev_pos = tri_array[v_size - 2];
			next_pos = tri_array[1];
		}
		else
		{
			prev_pos = tri_array[max_ni - 1];
			next_pos = tri_array[max_ni + 1];
		}
		prev_pos = prev_pos - curr_pos;
		prev_pos.normalize();
		prev_pos = prev_pos + curr_pos;
		next_pos = next_pos - curr_pos;
		next_pos.normalize();
		next_pos = next_pos + curr_pos;


		// 		int in_triangle_start_mode = 0;
		// 		vector_2d prev_pos = tri_array[ (max_ni - 1 + v_size ) % v_size ];
		// 		vector_2d next_pos = tri_array[ (max_ni + 1 + v_size ) % v_size ];
		if( prev_pos.x < next_pos.x )
		{
			in_triangle_start_mode = 1;
		}
		else
		{
			in_triangle_start_mode = 0;
		}

		// 三角化
		triangulation<double> v_triangulation_algorithm;
		std::vector<ushort> v_temp_index_array;
		v_temp_index_array.resize(( v_size - 2 ) * 3);

		assert(!tri_array.empty());
		assert(!v_temp_index_array.empty());

		v_triangulation_algorithm.triangulation_polygon( &tri_array[0], v_size, &v_temp_index_array[0] );

		// 三角面索引
		v_mesh->m_num_of_triangles = v_size * 2 - 4;
		v_mesh->m_index_buffer_size = v_mesh->m_num_of_triangles * 3 * sizeof(ushort);
		v_mesh->m_index_buffer = new byte[v_mesh->m_index_buffer_size];
		ushort* v_index_buffer = (ushort*)v_mesh->m_index_buffer;

		int index_curr = 0;
		for ( int i = 0; i < (int)v_temp_index_array.size(); i += 3 )
		{
			if ( in_triangle_start_mode == 0 )
			{
				// CW
				v_index_buffer[index_curr++] = v_temp_index_array[i];
				v_index_buffer[index_curr++] = v_temp_index_array[i + 2];
				v_index_buffer[index_curr++] = v_temp_index_array[i + 1];
			}
			else if ( in_triangle_start_mode == 1 )
			{
				// CCW
				v_index_buffer[index_curr++] = v_temp_index_array[i];
				v_index_buffer[index_curr++] = v_temp_index_array[i + 1];
				v_index_buffer[index_curr++] = v_temp_index_array[i + 2];
			}			
		}
		for ( int i = 0; i < (int)v_temp_index_array.size(); i += 3 )
		{
			v_index_buffer[index_curr++] = v_index_buffer[i];
			v_index_buffer[index_curr++] = v_index_buffer[i + 2];
			v_index_buffer[index_curr++] = v_index_buffer[i + 1];
		}


		// 生成顶点集合
		vector_3d min_vec(  FLT_MAX,  FLT_MAX,  FLT_MAX );
		vector_3d max_vec( -FLT_MAX, -FLT_MAX, -FLT_MAX );
		for ( int ni = 0 ; ni < (int)v_size; ni++ )
		{
			min_vec.x = min( min_vec.x, in_pos_array[ni].x );
			max_vec.x = max( max_vec.x, in_pos_array[ni].x );

			min_vec.y = min( min_vec.y, in_pos_array[ni].y );
			max_vec.y = max( max_vec.y, in_pos_array[ni].y );

			min_vec.z = min( min_vec.z, in_pos_array[ni].z );
			max_vec.z = max( max_vec.z, in_pos_array[ni].z );
		}
		int v_crsr = 0;
		vector_3d vec;
		float v_x_scale = 1 / (max_vec.x - min_vec.x);
		float v_z_scale = 1 / (max_vec.z - min_vec.z);

		v_mesh->m_num_of_vertices = v_size * 4;
		v_mesh->m_vertex_buffer_size = v_mesh->m_num_of_vertices * sizeof(position_color_texture);
		v_mesh->m_vertex_buffer = new byte[v_mesh->m_vertex_buffer_size];
		position_color_texture* v_ver_buffer = (position_color_texture*)v_mesh->m_vertex_buffer;

		for ( int i = 0 ; i < (int)v_size; i++ )
		{
			vec = in_pos_array[i];

			v_ver_buffer[v_crsr].x = vec.x;
			v_ver_buffer[v_crsr].y = vec.y;
			v_ver_buffer[v_crsr].z = vec.z;
			v_ver_buffer[v_crsr].tu1 = 0 + (vec.x - min_vec.x) * v_x_scale;
			v_ver_buffer[v_crsr].tv1 = 0 + (max_vec.z - vec.z) * v_z_scale;
			v_ver_buffer[v_crsr].color = fill_color;
			v_mesh->m_mesh_aabb.combine(vec);
			v_crsr++;
		}

		// 添加attribute
		v_mesh->m_attribute_buffer_size = v_mesh->m_num_of_triangles * sizeof(ulong);
		v_mesh->m_attribute_buffer = new byte[v_mesh->m_attribute_buffer_size];
		ulong* v_ptr_attri = (ulong*)v_mesh->m_attribute_buffer;

		for (int i = 0; i < v_mesh->m_num_of_triangles; i ++)
		{
			v_ptr_attri[i] = 0;
		}

		resource_manager<ustring>* v_texture_manager = in_doc->get_common_manager_group()->get_texture_manager( L"district_texture" );
		v_mesh->set_texture_manager(v_texture_manager);
		// 纹理
		v_mesh->m_material_entry_array.resize(1);
		v_mesh->m_texture_array.resize(1);

		material_entry v_entry;
		USCOLOR_ULONG_TO_COLORVALUE(v_entry.m_diffuse_color, fill_color);
		v_entry.m_diffuse = NULL;
		v_mesh->m_material_entry_array[0] = v_entry;

		v_mesh->set_fvf(position_color_texture::fvf);
		v_mesh->set_stride(position_color_texture::stride);

		return v_mesh;
	}


	//d3d9_hardware_mesh* hardware_mesh_factory::create_from_buffer( hardware_mesh_buffer* in_buffer )
	//{
	//	//if( in_buffer->m_type == US_HARDWARE_BUFFER_SKINNING )
	//	//{
	//	//	d3d9_skinning_hardware_mesh* v_mesh = new d3d9_skinning_hardware_mesh( in_buffer );
	//	//	return v_mesh;
	//	//}
	//	//else
	//	//{
	//	//	d3d9_hardware_mesh* v_mesh = new d3d9_hardware_mesh( in_buffer );
	//	//	return v_mesh;
	//	//}

	//	return NULL;
	//}

	//hardware_mesh* hardware_mesh_factory::create_complex_mesh( hardware_mesh_buffer_map& in_buffer_map, usx_frame* in_root_frame, usx_animation_set_array& in_animation_set_array )
	//{
	//	d3d9_complex_hardware_mesh* v_new_mesh = new d3d9_complex_hardware_mesh();

	//	v_new_mesh->m_is_texture_ready = true;

	//	v_new_mesh->m_root_frame = in_root_frame;

	//	if( v_new_mesh->m_root_frame != NULL )
	//	{			
	//		v_new_mesh->m_root_frame->reset();
	//		v_new_mesh->m_root_frame->update_hierarchy( matrix4<double>::s_identity );
	//	}

	//	if( v_new_mesh->m_root_frame != NULL )
	//	{
	//		for( int i = 0; i < (int)in_animation_set_array.size(); i++ )
	//		{
	//			usx_animation_set* anim_set = in_animation_set_array[i];	

	//			simple_animation_player* v_player = new simple_animation_player( anim_set );
	//			v_player->attach( v_new_mesh->m_root_frame );

	//			v_new_mesh->m_animation_player_map.insert( make_pair(anim_set->m_name, v_player ) );
	//		}			
	//	}

	//	v_new_mesh->m_bound_box.make_invalid();
	//	hardware_mesh_buffer_map::iterator itr = in_buffer_map.begin();
	//	for( ; itr != in_buffer_map.end(); itr ++ )
	//	{
	//		hardware_mesh_buffer* v_buffer = itr->first;
	//		usx_frame* v_frame = itr->second;

	//		if ( v_buffer->m_num_of_vertices == 0 )
	//			continue;

	//		v_new_mesh->m_fvf = v_buffer->m_fvf;
	//		d3d9_hardware_mesh* v_mesh = dynamic_cast<d3d9_hardware_mesh*>( v_buffer );

	//		v_new_mesh->m_bound_box.combine( v_buffer->m_aabb );
	//		v_new_mesh->m_mesh_frame_map.insert( make_pair( v_mesh,  v_frame ) );
	//	}
	//	return dynamic_cast<hardware_mesh*>(v_new_mesh);
	//}

	bool be_equal( usx_material* src_mat, usx_material* des_mat )
	{
		if ( src_mat->m_power != des_mat->m_power )
			return false;

		if ( src_mat->m_diffuse_color_rgba != des_mat->m_diffuse_color_rgba )
			return false;

		if ( src_mat->m_ambient_color_rgb != des_mat->m_ambient_color_rgb )
			return false;

		if ( src_mat->m_emissive_color_rgb != des_mat->m_emissive_color_rgb )
			return false;

		if ( src_mat->m_specular_color_rgb != des_mat->m_specular_color_rgb )
			return false;

		if ( src_mat->m_texture_files.size() == 0 )
			return true;

		if ( src_mat->m_texture_files.size() != des_mat->m_texture_files.size() )
			return false;

		if ( src_mat->m_texture_files[0].m_texture_file != des_mat->m_texture_files[0].m_texture_file )
			return false;

		return true;
	}

	void hardware_mesh_factory::copy_single_mesh( document_base* in_doc, d3d9_hardware_mesh* in_hardware_mesh, usx_mesh* in_usx_mesh )
	{
		in_hardware_mesh->set_name( in_usx_mesh->m_name.c_str() );

		in_hardware_mesh->m_fvf = in_usx_mesh->m_fvf;
		in_hardware_mesh->m_vertex_stride = in_usx_mesh->m_vertex_stride;

		vector3<float> v_min = in_usx_mesh->m_bbox.get_min();
		vector3<float> v_max = in_usx_mesh->m_bbox.get_max();
		in_hardware_mesh->m_mesh_aabb.set_extent( vector3<double>( (float*)&v_min.x ), vector3<double>( (float*)&v_max.x ) );
		in_hardware_mesh->set_bound_box( in_hardware_mesh->m_mesh_aabb );

		in_hardware_mesh->m_num_of_vertices = in_usx_mesh->m_number_of_vertices;
		in_hardware_mesh->m_vertex_buffer_size =  in_hardware_mesh->m_vertex_stride * in_hardware_mesh->m_num_of_vertices;
		in_hardware_mesh->m_vertex_buffer = new unsigned char[in_hardware_mesh->m_vertex_buffer_size];

		// vertex
		int vertex_offset = 0;
		for ( ulong i = 0; i < in_usx_mesh->m_number_of_vertices; i++ )
		{
			in_hardware_mesh->set_vector3( vertex_offset, i, in_usx_mesh->m_vertex_data[i] );
		}	

		// normal
		//if(in_usx_mesh->m_normals != NULL)
		if ( in_hardware_mesh->m_fvf & USFVF_NORMAL )
		{
			vertex_offset += 3 * sizeof(float);
			for ( ulong i = 0; i < in_usx_mesh->m_number_of_vertices; i++ )
			{
				in_hardware_mesh->set_vector3( vertex_offset, i, in_usx_mesh->m_normals[i] );
			}
		}

		// texture coordinates
		float* v_texture_coord = NULL;
		ulong v_texture_coord_num = in_usx_mesh->m_vertex_texture_coords.size();
		if ( v_texture_coord_num > 0 )
		{
			vertex_offset += 3 * sizeof(float);
			v_texture_coord = in_usx_mesh->m_vertex_texture_coords[0];
			for ( int i = 0; i < (int)in_usx_mesh->m_number_of_vertices; i++ )
			{
				in_hardware_mesh->set_vector2( vertex_offset, i, vector2<float>( v_texture_coord[(i<<1)+0], v_texture_coord[(i<<1)+1]) );
			}

			if ( v_texture_coord_num > 1 )
			{
				vertex_offset += 2 * sizeof(float);
				v_texture_coord = in_usx_mesh->m_vertex_texture_coords[1];
				for ( int i = 0; i < (int)in_usx_mesh->m_number_of_vertices; i++ )
				{
					in_hardware_mesh->set_vector2( vertex_offset, i,  vector2<float>(v_texture_coord[(i<<1)+0], v_texture_coord[(i<<1)+1]) );
				}
			}
		}

		in_hardware_mesh->m_num_of_triangles = in_usx_mesh->m_number_of_faces;
		if ( in_usx_mesh->m_number_of_vertices > 1<<15 )
		{
			in_hardware_mesh->m_32bit_index = true;
			in_hardware_mesh->m_index_buffer = new ulong[ in_hardware_mesh->m_num_of_triangles * 3 ];
			in_hardware_mesh->m_index_buffer_size = sizeof( ulong ) * in_hardware_mesh->m_num_of_triangles * 3;
			memcpy( in_hardware_mesh->m_index_buffer, in_usx_mesh->m_face_index, in_hardware_mesh->m_index_buffer_size );
		}
		else
		{			
			in_hardware_mesh->m_index_buffer = new ushort[ in_hardware_mesh->m_num_of_triangles * 3 ];
			ushort* v_buffer = (ushort*)in_hardware_mesh->m_index_buffer;
			in_hardware_mesh->m_index_buffer_size = sizeof(ushort) * in_hardware_mesh->m_num_of_triangles * 3;
			int v_off = 0;
			for ( int i = 0; i < (int)in_hardware_mesh->m_num_of_triangles; i++ )
			{
				v_off = i * 3;
				v_buffer[v_off]   = in_usx_mesh->m_face_index[i].a;
				v_buffer[v_off+1] = in_usx_mesh->m_face_index[i].b;
				v_buffer[v_off+2] = in_usx_mesh->m_face_index[i].c;
			}			
		}


		if( in_usx_mesh->m_attribute_data != NULL )
		{
			in_hardware_mesh->m_attribute_buffer = new ulong[in_hardware_mesh->m_num_of_triangles];
			in_hardware_mesh->m_attribute_buffer_size = sizeof( ulong ) * in_hardware_mesh->m_num_of_triangles;
			memcpy( in_hardware_mesh->m_attribute_buffer, in_usx_mesh->m_attribute_data, in_hardware_mesh->m_attribute_buffer_size );
		}


		resource_manager<ustring>* v_texture_manager = in_doc->get_common_manager_group()->get_texture_manager( L"district_texture" );
		in_hardware_mesh->set_texture_manager(v_texture_manager);

		usx_mesh::usx_material_array* src_mat = &(in_usx_mesh->m_materials);

		usx_mesh::usx_material_array valid_materials;
		if ( (in_hardware_mesh->m_fvf & USFVF_NORMAL) && in_usx_mesh->m_materials.size() > 0 )
		{
			std::vector<int> mat_map_array;
			mat_map_array.resize( in_usx_mesh->m_materials.size() );

			for ( int ni = 0; ni < mat_map_array.size(); ni++ )
			{
				mat_map_array[ni] = -1;
			}

			for ( int nn = 0; nn < in_usx_mesh->m_materials.size(); nn++ )
			{
				usx_material* src_mat = in_usx_mesh->m_materials[nn];
				if ( mat_map_array[nn] != -1 )
					continue;
							
				valid_materials.push_back( src_mat );
				mat_map_array[nn] = nn;
				for ( int nj = nn + 1; nj < in_usx_mesh->m_materials.size(); nj++ )
				{
					usx_material* des_mat = in_usx_mesh->m_materials[nj];

					bool b_equal = be_equal( src_mat, des_mat );
					if ( b_equal )
					{
						mat_map_array[nj] = mat_map_array[nn];
					}
				}
			}

			int cur_material_index = 0;
			for ( int ni = 0; ni < mat_map_array.size(); ni++ )
			{
				if ( mat_map_array[ni] > cur_material_index )	
				{
					cur_material_index++;

					int tmp = mat_map_array[ni];
					mat_map_array[ni] = cur_material_index;

					for ( int nnk = ni+1; nnk < mat_map_array.size(); nnk++ )
					{
						if ( mat_map_array[nnk] == tmp )
							mat_map_array[nnk] = cur_material_index;
					}

					continue;
				}

				//mat_map_array[ni] = cur_material_index;
			}

			if( in_usx_mesh->m_attribute_data != NULL )
			{
				for ( int ni = 0; ni < in_hardware_mesh->m_num_of_triangles; ni++ )
				{
					((int*)in_hardware_mesh->m_attribute_buffer)[ni] = mat_map_array[ ((int*)in_hardware_mesh->m_attribute_buffer)[ni] ];
				}
			}
			
			src_mat = &valid_materials;
		}

		in_hardware_mesh->m_material_entry_array.resize(src_mat->size());
		in_hardware_mesh->m_texture_array.resize( src_mat->size() );
		for ( int i = 0; i < src_mat->size(); i++ )
		{
			usx_material* v_mat = src_mat->at(i);

			material_entry v_new_mat;

			v_new_mat.m_name = v_mat->m_name;
			v_new_mat.m_power = v_mat->m_power;

			memcpy( (void*)&v_new_mat.m_ambient_color, (void*)&v_mat->m_ambient_color_rgb, sizeof( color_value ) );
			memcpy( (void*)&v_new_mat.m_diffuse_color, (void*)&v_mat->m_diffuse_color_rgba, sizeof( color_value ) );
			memcpy( (void*)&v_new_mat.m_specular_color, (void*)&v_mat->m_ambient_color_rgb, sizeof( color_value ) );
			memcpy( (void*)&v_new_mat.m_emissive_color, (void*)&v_mat->m_emissive_color_rgb, sizeof( color_value ) );			
		

			for ( int j = 0; j < v_mat->m_texture_files.size(); j++ )
			{				
				usx_texture_file& v_txt = v_mat->m_texture_files[j];

				if( v_txt.m_texture_file.size() == 0 ) continue;

				ustring v_texture_file = v_txt.m_texture_file;
//				US_STRING_LOWER( v_texture_file );

				//texture_resource_container* v_texture_container = (texture_resource_container*)v_texture_manager->get_resource( v_texture_file );
				//if ( v_texture_container == NULL )
				//{
				//	v_texture_container = new texture_resource_container( v_texture_file, v_texture_file.c_str(), US_DOWNLOAD_IN_HEAP );
				//	v_texture_container->add_ref();
				//	v_texture_manager->add_resource( v_texture_file, v_texture_container );
				//}


				resource_container<ustring>* v_texture_container = (resource_container<ustring>*)v_texture_manager->get_resource( v_texture_file );
				if ( v_texture_container == NULL )
				{
					ustring v_file_ext = file_directory::get_file_ext(v_texture_file.c_str());
					if( v_file_ext == L"swf")
					{
						v_texture_container = new flash_resource_container( in_doc, v_texture_file, v_texture_file.c_str(), US_DOWNLOAD_IN_HEAP );
						v_texture_container->add_ref();
						v_texture_manager->add_resource( v_texture_file, v_texture_container );
					}
					else
					{
						v_texture_container = new texture_resource_container( in_doc, v_texture_file, v_texture_file.c_str(), US_DOWNLOAD_IN_HEAP );
						v_texture_container->add_ref();
						v_texture_manager->add_resource( v_texture_file, v_texture_container );
					}
				}


				if( v_txt.v_type == 0 )
				{
					v_new_mat.m_diffuse = v_texture_container;
				}
				else if( v_txt.v_type == 1 )
				{
					v_new_mat.m_self_illuminant = v_texture_container;
				}
				else if( v_txt.v_type == 2 ) 
				{
					v_new_mat.m_specular = v_texture_container;
				}
				else if( v_txt.v_type == 3 )
				{
					v_new_mat.m_bump = v_texture_container;
				}
			}

			in_hardware_mesh->m_material_entry_array[i] = v_new_mat;
		}
	}

	void hardware_mesh_factory::copy_skinning_mesh( document_base* in_doc, d3d9_skinning_hardware_mesh* in_hardware_mesh, usx_mesh* in_usx_mesh )
	{
		in_hardware_mesh->set_name( in_usx_mesh->m_name.c_str() );
		in_hardware_mesh->m_fvf = in_usx_mesh->m_fvf;
		in_hardware_mesh->m_vertex_stride = in_usx_mesh->m_vertex_stride;

		in_hardware_mesh->m_num_of_vertices = in_usx_mesh->m_number_of_vertices;
		in_hardware_mesh->m_vertex_buffer_size =  in_hardware_mesh->m_vertex_stride * in_hardware_mesh->m_num_of_vertices;
		in_hardware_mesh->m_vertex_buffer = new unsigned char[in_hardware_mesh->m_vertex_buffer_size];
		memset( in_hardware_mesh->m_vertex_buffer, 0, in_hardware_mesh->m_vertex_buffer_size );

		in_hardware_mesh->m_root_frame->update_hierarchy( matrix4<double>::s_identity );

		int offset = 0;

		//
		// first we must copy weights info.
		//
		offset += 3 * sizeof( float );

		// 拷贝蒙皮信息
		std::vector<int> weights_count_array;
		weights_count_array.resize( in_hardware_mesh->m_num_of_vertices );
		memset( &weights_count_array[0], 0, in_hardware_mesh->m_num_of_vertices * sizeof(int) );
		for( int j = 0; j < (int)in_usx_mesh->m_skin_info->m_weights_array.size(); j++ )
		{
			usx_weights_info* weights = in_usx_mesh->m_skin_info->m_weights_array[j];
			for ( int i = 0; i < (int)weights->m_indices.size(); i++ )
			{
				int sequence = weights_count_array[weights->m_indices[i]];

				if( sequence > 3 )
				{
					int k = 0;
				}
				in_hardware_mesh->set_float( offset + sequence * sizeof(float), weights->m_indices[i], weights->m_weights[i] );

				ulong bone_index = in_hardware_mesh->get_ulong( offset + 4 * sizeof(float), weights->m_indices[i] );
				bone_index |= j << (8 * sequence);

				in_hardware_mesh->set_ulong( offset + 4 * sizeof(float), weights->m_indices[i], bone_index );

				weights_count_array[weights->m_indices[i]] ++;
			}

			usx_frame* v_frame = in_hardware_mesh->m_root_frame->find_frame( weights->m_frame_name.c_str() );
			in_hardware_mesh->m_frame_array[j] = v_frame;			
			in_hardware_mesh->m_offset_matrix_array[j] = weights->m_offset_matrix;
			in_hardware_mesh->m_original_matrix_array[j] = matrix4<float>( ( weights->m_offset_matrix * v_frame->m_combined_mat ).m ) ;
		}

		in_hardware_mesh->m_num_of_bones = in_usx_mesh->m_skin_info->m_weights_array.size();

		offset -= 3 * sizeof(float);

		// 拷贝顶点
		in_hardware_mesh->m_mesh_aabb.make_invalid();
		for ( int i = 0; i < (int)in_hardware_mesh->m_num_of_vertices; i++ )
		{
			vector3<float> vec;
			vec.x = ( in_usx_mesh->m_vertex_data[i] ).x;
			vec.y = ( in_usx_mesh->m_vertex_data[i] ).y;
			vec.z = ( in_usx_mesh->m_vertex_data[i] ).z;

			in_hardware_mesh->set_vector3( offset, i, vec );

			in_hardware_mesh->m_mesh_aabb.combine( get_blended_vertex( in_hardware_mesh, i ) );
		}

		// jump over 3 position coordinates
		offset += 3 * sizeof(float);

		// jump over 4 bone weights and 1 bone index.
		offset += 4 * sizeof( float ) + sizeof(float);


		if ( in_hardware_mesh->m_fvf & USFVF_NORMAL )
		{
			for ( ulong i = 0; i < in_usx_mesh->m_number_of_vertices; i++ )
			{
				in_hardware_mesh->set_vector3( offset, i, in_usx_mesh->m_normals[i] );
			}
			offset += 3 * sizeof(float);
		}

		// 拷贝纹理坐标
		float* v_texture_coord = NULL;
		ulong v_texture_coord_num = in_usx_mesh->m_vertex_texture_coords.size();
		if ( v_texture_coord_num > 0 )
		{
			v_texture_coord = in_usx_mesh->m_vertex_texture_coords[0];
			for ( int i = 0; i < (int)in_hardware_mesh->m_num_of_vertices; i++ )
			{
				in_hardware_mesh->set_vector2( offset, i, vector2<float>( v_texture_coord[(i<<1)+0], v_texture_coord[(i<<1)+1] ) );
			}
		}

		in_hardware_mesh->m_num_of_triangles = in_usx_mesh->m_number_of_faces;
		in_hardware_mesh->m_texture_array.resize( in_usx_mesh->m_materials.size() );
		if ( in_hardware_mesh->m_num_of_vertices > 1<<15 )
		{
			in_hardware_mesh->m_32bit_index = true;
			in_hardware_mesh->m_index_buffer = new ulong[ in_hardware_mesh->m_num_of_triangles * 3 ];
			in_hardware_mesh->m_index_buffer_size = sizeof( ulong ) * in_hardware_mesh->m_num_of_triangles * 3;
			memcpy( in_hardware_mesh->m_index_buffer, in_usx_mesh->m_face_index, in_hardware_mesh->m_index_buffer_size );
		}
		else
		{			
			in_hardware_mesh->m_index_buffer = new ushort[ in_hardware_mesh->m_num_of_triangles * 3 ];
			ushort* v_buffer = (ushort*)in_hardware_mesh->m_index_buffer;
			in_hardware_mesh->m_index_buffer_size = sizeof(ushort) * in_hardware_mesh->m_num_of_triangles * 3;
			int v_off = 0;
			for ( int i = 0; i < (int)in_hardware_mesh->m_num_of_triangles; i++ )
			{
				v_off = i * 3;
				v_buffer[v_off]   = in_usx_mesh->m_face_index[i].a;
				v_buffer[v_off+1] = in_usx_mesh->m_face_index[i].b;
				v_buffer[v_off+2] = in_usx_mesh->m_face_index[i].c;
			}			
		}

		in_hardware_mesh->m_attribute_buffer = new ulong[in_hardware_mesh->m_num_of_triangles];
		in_hardware_mesh->m_attribute_buffer_size = sizeof( ulong ) * in_hardware_mesh->m_num_of_triangles;
		memcpy( in_hardware_mesh->m_attribute_buffer, in_usx_mesh->m_attribute_data, in_hardware_mesh->m_attribute_buffer_size );

		resource_manager<ustring>* v_texture_manager = in_doc->get_common_manager_group()->get_texture_manager( L"district_texture" );

		in_hardware_mesh->m_material_entry_array.resize(in_usx_mesh->m_materials.size());
		for ( int i = 0; i < in_usx_mesh->m_materials.size(); i++ )
		{
			usx_material* v_mat = in_usx_mesh->m_materials[i];

			material_entry v_new_mat;

			v_new_mat.m_name = v_mat->m_name;
			v_new_mat.m_power = v_mat->m_power;

			memcpy( (void*)&v_new_mat.m_ambient_color, (void*)&v_mat->m_ambient_color_rgb, sizeof( color_value ) );
			memcpy( (void*)&v_new_mat.m_diffuse_color, (void*)&v_mat->m_diffuse_color_rgba, sizeof( color_value ) );
			memcpy( (void*)&v_new_mat.m_specular_color, (void*)&v_mat->m_ambient_color_rgb, sizeof( color_value ) );
			memcpy( (void*)&v_new_mat.m_emissive_color, (void*)&v_mat->m_emissive_color_rgb, sizeof( color_value ) );			


			for ( int j = 0; j < v_mat->m_texture_files.size(); j++ )
			{				
				usx_texture_file& v_txt = v_mat->m_texture_files[j];

				if( v_txt.m_texture_file.size() == 0 ) continue;

				ustring v_texture_file = v_txt.m_texture_file;
				US_STRING_LOWER( v_texture_file );

				resource_container<ustring>* v_texture_container = (resource_container<ustring>*)v_texture_manager->get_resource( v_texture_file );
				if ( v_texture_container == NULL )
				{
					ustring v_file_ext = file_directory::get_file_ext(v_texture_file.c_str());
					if( v_file_ext == L"swf")
					{
						v_texture_container = new flash_resource_container( in_doc, v_texture_file, v_texture_file.c_str(), US_DOWNLOAD_IN_HEAP );						
						v_texture_container->add_ref();
						v_texture_manager->add_resource( v_texture_file, v_texture_container );
					}
					else
					{
						v_texture_container = new texture_resource_container( in_doc, v_texture_file, v_texture_file.c_str(), US_DOWNLOAD_IN_HEAP );
						v_texture_container->add_ref();
						v_texture_manager->add_resource( v_texture_file, v_texture_container );
					}
				}


				if( v_txt.v_type == 0 )
				{
					v_new_mat.m_diffuse = v_texture_container;
				}
				else if( v_txt.v_type == 1 )
				{
					v_new_mat.m_self_illuminant = v_texture_container;
				}
				else if( v_txt.v_type == 2 ) 
				{
					v_new_mat.m_specular = v_texture_container;
				}
				else if( v_txt.v_type == 3 )
				{
					v_new_mat.m_bump = v_texture_container;
				}
			}

			in_hardware_mesh->m_material_entry_array[i] = v_new_mat;
		}
	}

	vector3<double> hardware_mesh_factory::get_blended_vertex( d3d9_skinning_hardware_mesh* in_buffer, ulong vert_index )
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

	d3d9_hardware_mesh* hardware_mesh_factory::create_hardware_mesh_from_buffer( render_device* device, raw_buffer* verts_buf, raw_buffer* indices_buf, int verts_count, int indices_count, int fvf )
	{
		d3d9_hardware_mesh* new_mesh = new d3d9_hardware_mesh();
		new_mesh->set_mem_size( device->create_mesh_from_buffer( verts_buf, indices_buf, NULL, 
																verts_count, indices_count, fvf, 
																(void**)&(new_mesh->m_d3d_mesh) ) );
		return new_mesh;
	}




	d3d9_hardware_mesh* hardware_mesh_factory::create_single_mesh_from_element( document_base* in_doc, render_object_geometry_mesh* in_mesh )
	{
		d3d9_hardware_mesh* new_mesh = new d3d9_hardware_mesh();

		new_mesh->set_fvf( in_mesh->get_fvf() );
		new_mesh->set_stride( in_mesh->get_stride() );
		new_mesh->m_mesh_aabb.combine( in_mesh->get_render_aabb() );

		in_mesh->lock();
		// vertex_buffer
		new_mesh->m_num_of_vertices = in_mesh->m_num_of_vertices;
		new_mesh->m_vertex_buffer_size = in_mesh->m_vertex_buffer_size;
		new_mesh->m_vertex_buffer = new byte[new_mesh->m_vertex_buffer_size];
		memcpy(new_mesh->m_vertex_buffer, in_mesh->m_vertex_buffer, in_mesh->m_vertex_buffer_size);

		// index_buffer
		new_mesh->m_32bit_index = in_mesh->m_32bit_index;
		new_mesh->m_num_of_triangles = in_mesh->m_num_of_triangles;
		new_mesh->m_index_buffer_size = in_mesh->m_index_buffer_size;
		new_mesh->m_index_buffer = new byte[new_mesh->m_index_buffer_size];
		memcpy(new_mesh->m_index_buffer, in_mesh->m_index_buffer, in_mesh->m_index_buffer_size);


		// attribute
		new_mesh->m_attribute_buffer_size = new_mesh->m_num_of_triangles * sizeof(ulong);
		new_mesh->m_attribute_buffer = new byte[new_mesh->m_attribute_buffer_size];
		ulong* v_ptr_attri = (ulong*)new_mesh->m_attribute_buffer;

		for (int i = 0; i < new_mesh->m_num_of_triangles; i ++)
		{
			v_ptr_attri[i] = 0;
		}

		resource_manager<ustring>* v_texture_manager = in_doc->get_common_manager_group()->get_texture_manager( L"district_texture" );
		new_mesh->set_texture_manager(v_texture_manager);
		// material
		new_mesh->m_material_entry_array.resize(1);
		new_mesh->m_texture_array.resize(1);

		material_entry v_entry;
		v_entry.m_diffuse = NULL;
		new_mesh->m_material_entry_array[0] = v_entry;


		in_mesh->unlock();

		return new_mesh;
	}

	d3d9_hardware_mesh* hardware_mesh_factory::create_single_mesh_from_element_textured( document_base* in_doc, render_object_geometry_textured_mesh* in_mesh )
	{
		d3d9_hardware_mesh* new_mesh = new d3d9_hardware_mesh();

		new_mesh->set_fvf( in_mesh->get_fvf() );
		new_mesh->set_stride( in_mesh->get_stride() );
		new_mesh->m_mesh_aabb.combine( in_mesh->get_render_aabb() );

		in_mesh->lock();
		// vertex_buffer
		new_mesh->m_num_of_vertices = in_mesh->m_num_of_vertices;
		new_mesh->m_vertex_buffer_size = in_mesh->m_vertex_buffer_size;
		new_mesh->m_vertex_buffer = new byte[new_mesh->m_vertex_buffer_size];
		memcpy(new_mesh->m_vertex_buffer, in_mesh->m_vertex_buffer, in_mesh->m_vertex_buffer_size);

		// index_buffer
		new_mesh->m_32bit_index = in_mesh->m_32bit_index;
		new_mesh->m_num_of_triangles = in_mesh->m_num_of_triangles;
		new_mesh->m_index_buffer_size = in_mesh->m_index_buffer_size;
		new_mesh->m_index_buffer = new byte[new_mesh->m_index_buffer_size];
		memcpy(new_mesh->m_index_buffer, in_mesh->m_index_buffer, in_mesh->m_index_buffer_size);

		// attribute
		new_mesh->m_attribute_buffer_size = new_mesh->m_num_of_triangles * sizeof(ulong);
		new_mesh->m_attribute_buffer = new byte[new_mesh->m_attribute_buffer_size];
		ulong* v_ptr_attri = (ulong*)new_mesh->m_attribute_buffer;


		// reset uv / reset attribute / set texture
		int uv_offset, uv_cnt;
		float* ptr_tutv;
		hardware_mesh_factory::get_uv_info_from_fvf( in_mesh->get_fvf(), uv_offset, uv_cnt );

		int mat_size = in_mesh->m_material_entry_array.size();
		new_mesh->m_material_entry_array.resize(mat_size);
		new_mesh->m_texture_array.resize(mat_size);

		int attri_index = 0;
		for (int i = 0; i < mat_size; i ++)
		{
			// uv
			geometry_mesh_subset* mat = in_mesh->m_material_entry_array[i];
			ptr_tutv = (float*)((byte*)new_mesh->m_vertex_buffer + new_mesh->get_stride() * i + uv_offset);
			vector_3f uv_vec;
			for (int j = 0; j < uv_cnt; j += 2)
			{
				uv_vec = vector_3f(ptr_tutv[j], ptr_tutv[j + 1], 0);
				matrix_4f tex_matrix = mat->get_texture_matrix();
				uv_vec = uv_vec * tex_matrix;
				ptr_tutv[j] = uv_vec.x;
				ptr_tutv[j + 1] = uv_vec.y;
			}

			// attributer
			pair<ulong, ulong> mat_info = in_mesh->m_subset_info_array[i];
			for (int j = 0; j < mat_info.second; j += 3)
			{
				v_ptr_attri[attri_index ++] = i;
			}

			// texture
			new_mesh->m_material_entry_array[i] = *(mat->get_material_entry());
		}

		in_mesh->unlock();

		return new_mesh;
	}

	void hardware_mesh_factory::compact_index( void* index_buffer, int index_cnt, bool m_is_32_bit, map<ulong, ulong>& compacted_map )
	{
		compacted_map.clear();

		if( m_is_32_bit )
		{
			ulong* v_buffer = (ulong*)index_buffer;
			for(int ni = 0; ni < index_cnt; ni++ )
			{
				compacted_map[v_buffer[ni]] = v_buffer[ni];
			}
		}
		else
		{
			ushort* v_buffer = (ushort*)index_buffer;
			for(int ni = 0; ni < index_cnt; ni++ )
			{
				compacted_map[v_buffer[ni]] = v_buffer[ni];
			}
		}
	}
	
	void hardware_mesh_factory::get_uv_info_from_fvf( ulong fvf, int& uv_offset, int& uv_cnt )
	{
		uv_offset = 0;

		if ( fvf & USFVF_XYZ ) uv_offset += 12;
		if ( fvf & USFVF_NORMAL ) uv_offset += 12;
		if ( fvf & USFVF_DIFFUSE ) uv_offset += 4;

		uv_cnt = (fvf * USFVF_TEXCOUNT_MASK) >> USFVF_TEXCOUNT_SHIFT * 2;
	}

}