
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_ust_filter_v2.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : ust_filter_v2 class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_ust_filter_v2.h"

namespace uniscope_globe
{
	object_base* ust_filter_v2::parse( raw_buffer* in_buffer )
	{
		square_buffer* v_square_buffer = (square_buffer*)in_buffer;
		ustring v_path = v_square_buffer->get_path();

		//ust_buffer* v_ust_buffer = new ust_buffer( v_path.c_str(), in_buffer );		
		//v_ust_buffer->initialize();

		d3d9_hardware_mesh* v_out_hardware_mesh = new d3d9_hardware_mesh();

		v_out_hardware_mesh->set_name( L"ust_mesh_no_name" );
		v_out_hardware_mesh->m_path = v_path;

		in_buffer->seek_to_begin();
		bldg_builder_head v_usc_head;
		in_buffer->read( &v_usc_head, sizeof(bldg_builder_head) );

		bldg_builder_head v_unknown_head;
		while( in_buffer->get_cursor() < in_buffer->get_size() )
		{
			//if(m_buffer->read( &v_unknown_head, sizeof(bldg_builder_head) ) == 0) break;
			in_buffer->read( &v_unknown_head, sizeof(bldg_builder_head) );
			switch( v_unknown_head.m_type )
			{
			case US_USC_VERTEX:
				read_vertex_buffer( in_buffer, v_out_hardware_mesh );
				break;
			case US_USC_INDICE:
				read_face_buffer( in_buffer, v_out_hardware_mesh );
				break;
			case US_USC_BOUNDBOX:
				read_box( in_buffer, v_out_hardware_mesh );
				break;
			case US_USC_MATERIAL:
				read_material( v_square_buffer->get_document(), in_buffer, v_out_hardware_mesh );
				break;
			case US_USC_FIGURE:
				read_figure( in_buffer, v_out_hardware_mesh );
				break;
			}
		}

		return v_out_hardware_mesh;

		//return new d3d9_hardware_mesh( v_ust_buffer );
	}	

	void ust_filter_v2::read_figure( raw_buffer* in_buffer, d3d9_hardware_mesh* v_out_mesh )
	{
		//int v_readed_bytes = 0;

		//// buffer
		//v_readed_bytes += in_buffer->read( &m_figure_buf_head, sizeof(m_figure_buf_head) );

		//m_figure_buf = new position_color[m_figure_buf_head.m_num];
		//v_readed_bytes += in_buffer->read( m_figure_buf, m_figure_buf_head.m_bit_count );

		//// index
		//v_readed_bytes += in_buffer->read( &m_figure_index_head, sizeof(m_figure_index_head) );

		//m_figure_index = new ulong[m_figure_index_head.m_num*2];
		//v_readed_bytes += in_buffer->read( m_figure_index, m_figure_index_head.m_bit_count );
	}

	void ust_filter_v2::read_vertex_buffer( raw_buffer* in_buffer, d3d9_hardware_mesh* v_out_mesh  )
	{
		ulong v_off = 0;

		int v_meta_size = sizeof( bldg_builder_vertex_head );

		usc_vertex_head v_vertex_head;
		in_buffer->read( &v_vertex_head, sizeof( usc_vertex_head ) );

		v_out_mesh->m_fvf = v_vertex_head.m_fvf;
		v_out_mesh->m_vertex_stride = v_vertex_head.m_stride;

		v_out_mesh->m_num_of_vertices = v_vertex_head.m_num;
		v_out_mesh->m_vertex_buffer_size = sizeof(float) * v_out_mesh->m_vertex_stride * v_out_mesh->m_num_of_vertices;
		v_out_mesh->m_vertex_buffer = new unsigned char[ v_out_mesh->m_vertex_buffer_size ];
		memset( v_out_mesh->m_vertex_buffer, 0, v_out_mesh->m_vertex_buffer_size );

		while ( v_off < v_vertex_head.m_stride )
		{
			bldg_builder_head unknwon_head( 0 , 0 );
			in_buffer->read( &unknwon_head, sizeof( bldg_builder_head ) );
			switch( unknwon_head.m_type )
			{
			case US_BLDG_BUILDER_XYZ:
				{					
					bldg_builder_vertex_head v_xyz_meta;
					in_buffer->read( &v_xyz_meta, v_meta_size );

					float* v_xyz = (float*)( in_buffer->get_buffer() + in_buffer->get_cursor() );			

					for ( int i = 0; i < (int)v_xyz_meta.m_num; i++ )
					{
						((float*)v_out_mesh->m_vertex_buffer)[i * v_out_mesh->m_vertex_stride + v_off] = v_xyz[i * 3];
						((float*)v_out_mesh->m_vertex_buffer)[i * v_out_mesh->m_vertex_stride + v_off + 1] = v_xyz[i * 3 + 1];
						((float*)v_out_mesh->m_vertex_buffer)[i * v_out_mesh->m_vertex_stride + v_off + 2] = v_xyz[i * 3 + 2];
					}

					v_off += 3;
					in_buffer->seek_to( in_buffer->get_cursor() + v_xyz_meta.m_size );					
				}
				break;
			case US_BLDG_BUILDER_NORMAL:
				{					
					bldg_builder_vertex_head v_normal_meta;
					in_buffer->read( &v_normal_meta, v_meta_size );

					float* v_normal = (float*)( in_buffer->get_buffer() + in_buffer->get_cursor() );

					for ( int i = 0; i < (int)v_normal_meta.m_num; i++ )
					{
						((float*)v_out_mesh->m_vertex_buffer)[i * v_out_mesh->m_vertex_stride + v_off] = v_normal[i * 3];
						((float*)v_out_mesh->m_vertex_buffer)[i * v_out_mesh->m_vertex_stride + v_off + 1] = v_normal[i * 3 + 1];
						((float*)v_out_mesh->m_vertex_buffer)[i * v_out_mesh->m_vertex_stride + v_off + 2] = v_normal[i * 3 + 2];
					}

					v_off += 3;
					in_buffer->seek_to( in_buffer->get_cursor() + v_normal_meta.m_size );					
				}
				break;
			case US_BLDG_BUILDER_DIFFUSE:
				{
					bldg_builder_vertex_head v_diffuse_meta;
					in_buffer->read( &v_diffuse_meta, v_meta_size );

					DWORD* v_diffuse = (DWORD*)( in_buffer->get_buffer() + in_buffer->get_cursor() );
					DWORD* v_vertex = (DWORD*)v_out_mesh->m_vertex_buffer;

					for ( int i = 0; i < (int)v_diffuse_meta.m_num; i++ )
					{
						((DWORD*)v_out_mesh->m_vertex_buffer)[i * v_out_mesh->m_vertex_stride + v_off] = v_diffuse[i];
					}

					v_off += 1;
					in_buffer->seek_to( in_buffer->get_cursor() + v_diffuse_meta.m_size );					
				}
				break;
			case US_BLDG_BUILDER_TEX0:
				{
					int v_text_levels = ( v_vertex_head.m_fvf & USFVF_TEXCOUNT_MASK ) >> 8;					

					bldg_builder_vertex_head v_text_coord_meta;
					in_buffer->read( &v_text_coord_meta, v_meta_size );

					float* v_text_coord = (float*)( in_buffer->get_buffer() + in_buffer->get_cursor() );
					for ( int i = 0; i < v_text_levels; i++ )
					{
						int src_off = v_text_coord_meta.m_num * 2 * i;
						for ( int j = 0; j < (int)v_vertex_head.m_num; j++ )
						{
							((float*)v_out_mesh->m_vertex_buffer)[j * v_out_mesh->m_vertex_stride + v_off] = v_text_coord[src_off + (j<<1)];
							((float*)v_out_mesh->m_vertex_buffer)[j * v_out_mesh->m_vertex_stride + v_off + 1] = v_text_coord[src_off + (j<<1) + 1];
						}

						v_off += 2;
					}

					in_buffer->seek_to( in_buffer->get_cursor() + v_text_coord_meta.m_num * sizeof(float) * v_text_levels * 2 );					
				}
				break;
			}
		}

	}

	void ust_filter_v2::read_face_buffer( raw_buffer* in_buffer, d3d9_hardware_mesh* v_out_mesh  )
	{
		int v_head_size = sizeof( bldg_builder_head );
		int v_indice_head_size = sizeof( usc_index_head );
		int v_meta_size = sizeof( bldg_builder_vertex_head );

		// read face head
		usc_index_head v_indice_section_head;
		in_buffer->read( &v_indice_section_head, v_indice_head_size );

		// read indice
		bldg_builder_head v_indice_head( 0 , 0 );
		in_buffer->read( &v_indice_head, v_head_size );

		bldg_builder_vertex_head v_indice_meta;
		in_buffer->read( &v_indice_meta, v_meta_size );

		usx_tri_face_index* v_indice_buffer = new usx_tri_face_index[v_indice_meta.m_num];
		in_buffer->read( v_indice_buffer, v_indice_meta.m_size );

		v_out_mesh->m_num_of_triangles = v_indice_meta.m_num;

		if( v_out_mesh->m_num_of_vertices > 1<<15 )
		{
			v_out_mesh->m_32bit_index = true;
			v_out_mesh->m_index_buffer = new DWORD[ v_out_mesh->m_num_of_triangles * 3 ];
			v_out_mesh->m_index_buffer_size = sizeof( DWORD ) * v_out_mesh->m_num_of_triangles * 3;
			memcpy( v_out_mesh->m_index_buffer, v_indice_buffer,v_out_mesh->m_index_buffer_size );
		}
		else
		{
			v_out_mesh->m_index_buffer_size = v_out_mesh->m_num_of_triangles * 3 * sizeof(ushort);
			v_out_mesh->m_index_buffer = new unsigned char[v_out_mesh->m_index_buffer_size];

			for( int i = 0; i < (int)v_out_mesh->m_num_of_triangles; i++ )
			{
				((ushort*)v_out_mesh->m_index_buffer)[i*3] = v_indice_buffer[i].a;
				((ushort*)v_out_mesh->m_index_buffer)[i*3 + 1] = v_indice_buffer[i].b;
				((ushort*)v_out_mesh->m_index_buffer)[i*3 + 2] = v_indice_buffer[i].c;
			}
		}

		AUTO_DELETE( v_indice_buffer );

		// read attribute
		bldg_builder_head v_attr_head( 0 , 0 );
		in_buffer->read( &v_attr_head, v_head_size );

		bldg_builder_vertex_head v_attribute_meta;
		in_buffer->read( &v_attribute_meta, v_meta_size );

		DWORD* v_attr_buffer = new DWORD[v_attribute_meta.m_num];
		in_buffer->read( v_attr_buffer, v_attribute_meta.m_size );

		v_out_mesh->m_attribute_buffer_size = v_attribute_meta.m_size;
		v_out_mesh->m_attribute_buffer = v_attr_buffer;
	}

	void ust_filter_v2::read_box( raw_buffer* in_buffer, d3d9_hardware_mesh* v_out_mesh  )
	{
		int v_box_num = 0;
		in_buffer->read( &v_box_num, sizeof(v_box_num) );

		vector_3d vec_min, vec_max;
		in_buffer->read( &vec_min, sizeof(vec_min) );
		in_buffer->read( &vec_max, sizeof(vec_max) );
		v_out_mesh->m_mesh_aabb.set_extent( vec_min, vec_max );
	}

	void ust_filter_v2::read_material( document_base* in_doc, raw_buffer* in_buffer, d3d9_hardware_mesh* v_out_mesh  )
	{
		square_buffer* v_square_buffer = (square_buffer*)in_buffer;
		ustring v_path = v_square_buffer->get_path();
		v_path = file_directory::get_directory( v_path.c_str() );

		int v_color_size = sizeof( color_value );

		usd_material_head v_material_head;
		in_buffer->read( &v_material_head, sizeof(v_material_head) );

		//long* v_data = new long[v_material_head.m_material_segment];
		//in_buffer->read( v_data, sizeof(long)*v_material_head.m_material_segment);
		in_buffer->seek_to( in_buffer->get_cursor() + sizeof(long)*v_material_head.m_material_segment );

		resource_manager<ustring>* v_texture_manager = in_doc->get_common_manager_group()->get_texture_manager( L"district_texture" );

		v_out_mesh->m_material_entry_array.resize( v_material_head.m_num );

		for ( int i = 0; i < (int)v_material_head.m_num; i++ )
		{
			material_entry v_mat;

			v_mat.m_name = L"mat";

			// material
			in_buffer->read( &v_mat.m_diffuse_color, v_color_size );
			in_buffer->read( &v_mat.m_ambient_color, v_color_size );
			in_buffer->read( &v_mat.m_specular_color, v_color_size );
			in_buffer->read( &v_mat.m_emissive_color, v_color_size );
			in_buffer->read( &v_mat.m_power, sizeof(v_mat.m_power) );

			for ( int i = 0; i < (int)v_material_head.m_texture_stage_count; i++ )
			{
				int v_type = 0;
				in_buffer->read( &v_type, sizeof(v_type) );

				ustring v_texture_file;
				in_buffer->read( v_texture_file );
				US_STRING_LOWER( v_texture_file );
				v_texture_file = v_path + v_texture_file;

				texture_resource_container* v_texture_container = (texture_resource_container*)v_texture_manager->get_resource( v_texture_file );
				if ( v_texture_container == NULL )
				{
					v_texture_container = new texture_resource_container( in_doc, v_texture_file, v_texture_file.c_str(), US_DOWNLOAD_IN_HEAP );
					v_texture_container->add_ref();
					v_texture_manager->add_resource( v_texture_file, v_texture_container );
				}

				
				switch ( v_type )
				{
				case US_TEXTURE_TYPE_NORMAL:
				case US_TEXTURE_TYPE_ANIMATION:
				case US_TEXTURE_TYPE_WATER:
				case US_TEXTURE_TYPE_ALPHA:
				case US_TEXTURE_TYPE_BLEND:
				case US_TEXTURE_TYPE_UNKNOWN:
				case US_TEXTURE_TYPE_ERROR:
					{
						v_mat.m_diffuse = v_texture_container;
					}
					break;
				case 1:
					{						
						v_mat.m_self_illuminant = v_texture_container;
					}
					break;
				case 2:
					{
						v_mat.m_specular = v_texture_container;
					}
					break;
				case 3:
					{
						v_mat.m_bump = v_texture_container;
					}
					break;
				}
			}				

			v_out_mesh->m_material_entry_array[i] = v_mat;////?????????????????????????
		}

	}


	
}