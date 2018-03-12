
// us_usm_filter_7z.cpp

#include "stdafx.h"
#include "us_usm_filter_7z.h"

namespace uniscope_globe
{
	object_base* usm_filter_7z::parse( raw_buffer* v_buffer )
	{
		building_create_buffer* in_buffer = (building_create_buffer*)v_buffer;

		// new one
		byte* v_src_data = in_buffer->get_buffer();
		ulong v_src_size = in_buffer->get_size();
		//in_buffer->seek_to_begin();

		bldg_builder_head v_head( 0, 0 );
		in_buffer->read( &v_head, sizeof(v_head) );

		size_t v_data_size = 0;
		in_buffer->read( &v_data_size, sizeof(v_data_size) );

		LzmaProperties prop;
		in_buffer->read( &prop, sizeof( prop) );

		ulong v_compress_data_offset = sizeof(v_head) + sizeof(v_data_size) + sizeof( prop);

		BYTE* v_uncompress_data = new BYTE[v_data_size];
		//uncompress( v_uncompress_data, (unsigned long*)&v_data_size, v_src_data + v_compress_data_offset, v_src_size - v_compress_data_offset );

		int ret = LzmaUncompress( v_uncompress_data, &v_data_size, v_src_data + v_compress_data_offset, v_src_size - v_compress_data_offset, &prop );

		// 创建buffer
		building_create_buffer uncompress_buf( v_uncompress_data, v_data_size );
		uncompress_buf.m_height = in_buffer->m_height;

		model_lod_hardware_mesh* v_mesh = parse_model_lod_hardware_mesh( &uncompress_buf );

		AUTO_DELETE( v_uncompress_data );

		return v_mesh;
	}


	model_lod_hardware_mesh* usm_filter_7z::parse_model_lod_hardware_mesh( building_create_buffer* uncompress_buf )
	{
		model_lod_hardware_mesh* v_mesh = new model_lod_hardware_mesh();

		//	解析数据
		int v_data_size = uncompress_buf->get_size();

		m_build_normal = false;

		int v_readed_bytes = 0;
		int v_head_size = sizeof( bldg_builder_head );
		while( uncompress_buf->get_cursor() < v_data_size )
		{
			bldg_builder_head unknown_head( 0, 0 );
			v_readed_bytes += uncompress_buf->read( &unknown_head, v_head_size );

			switch( unknown_head.m_type )
			{
			case US_USC_VERTEX:
				v_readed_bytes += read_vertex( uncompress_buf, v_mesh );
				break;
			case US_USC_INDICE:
				v_readed_bytes += read_index_attribute( uncompress_buf, v_mesh );
				break;
			case US_USC_BOUNDBOX:
				v_readed_bytes += read_aabb( uncompress_buf, v_mesh );
				break;
			case US_USC_MATERIAL:
				v_readed_bytes += read_material( uncompress_buf, v_mesh );
				break;
			case US_USC_TOPOLOGIC:
				v_readed_bytes += read_topologic( uncompress_buf, v_mesh );
				break;
			case US_USC_FIGURE: 
				v_readed_bytes += read_figure( uncompress_buf, v_mesh );
				break;
			}
		}

		///////////////////////////////////////////////////////////////////////////////
		if(m_build_normal)
		{
			v_mesh->generate_vertex_normal();
		}
		///////////////////////////////////////////////////////////////////////////////

		return v_mesh;
	}

	int usm_filter_7z::read_vertex( building_create_buffer* v_stream, model_lod_hardware_mesh* v_mesh )
	{
		ulong v_des_index = 0, v_src_index = 0;

		int v_readed_bytes		= 0;
		int v_head_size			= sizeof( bldg_builder_head );
		int v_section_size		= sizeof( bldg_builder_vertex_head );

		usc_vertex_head v_vertex_head;
		v_readed_bytes += v_stream->read( &v_vertex_head, sizeof(v_vertex_head) );

		int v_dest_vertex_xyz_stride = 3;
		int v_dest_vertex_stride =  v_vertex_head.m_stride;
		/////////////////////////////////////////////////////////////////////////////
		if( (!(v_vertex_head.m_fvf & D3DFVF_NORMAL)) && system_environment::s_preprocess_normal )
		{
			v_dest_vertex_xyz_stride = 6;
			v_vertex_head.m_fvf |= D3DFVF_NORMAL;
			v_dest_vertex_stride += 3;
			m_build_normal = true;
		}

		/////////////////////////////////////////////////////////////////////////////
		v_mesh->set_fvf( v_vertex_head.m_fvf );
		v_mesh->set_stride( v_dest_vertex_stride * 4 );
		v_mesh->set_num_of_vertices( v_vertex_head.m_num );
		v_mesh->set_vertex_buffer_size( v_mesh->m_vertex_stride * v_mesh->m_num_of_vertices );
		v_mesh->m_vertex_buffer = (void*)(new byte[v_mesh->m_vertex_buffer_size]);
		memset( v_mesh->m_vertex_buffer, 0, v_mesh->m_vertex_buffer_size );

		ulong v_src_stride = 0;
		ulong v_dest_stride = 0;
		int v_zip_type = US_BUILDING_RAW;
		while ( v_src_stride < v_vertex_head.m_stride )
		{
			bldg_builder_head unknwon_head( 0 , 0 );
			v_readed_bytes += v_stream->read( &unknwon_head, v_head_size );

			switch( unknwon_head.m_type )
			{
			case US_BLDG_BUILDER_XYZ:
				{
					building_create_buffer* cbuf = (building_create_buffer*)v_stream;

					v_zip_type = unknwon_head.m_version;
					if ( unknwon_head.m_version == US_BUILDING_RAW)
					{
						bldg_builder_vertex_head v_meta;
						v_stream->read( &v_meta, v_section_size );

						float* v_xyz = (float*)( v_stream->get_buffer() + v_stream->get_cursor() );
						float* v_vertex = (float*)v_mesh->m_vertex_buffer;

						for ( int i = 0; i < (int)v_vertex_head.m_num; i++ )
						{
							v_des_index = i * v_dest_vertex_stride + v_dest_stride;
							v_src_index = i * 3;
							v_vertex[v_des_index] = v_xyz[v_src_index];
							v_vertex[v_des_index+1] = v_xyz[v_src_index+1];
							v_vertex[v_des_index+2] = v_xyz[v_src_index+2];
						}

						v_src_stride += 3;
						v_dest_stride += v_dest_vertex_xyz_stride;
						v_stream->seek_to( v_stream->get_cursor() + v_meta.m_size );
					}
					else if ( unknwon_head.m_version == US_BUILDING_ZIP )
					{
						bldg_builder_vertex_head v_meta;
						v_stream->read( &v_meta, v_section_size );

						short* v_xyz = (short*)( v_stream->get_buffer() + v_stream->get_cursor() );
						float* v_vertex = (float*)v_mesh->m_vertex_buffer;

						for ( int i = 0; i < (int)v_vertex_head.m_num; i++ )
						{
							v_des_index = i * v_dest_vertex_stride + v_dest_stride;
							v_src_index = i * 3;
							v_vertex[v_des_index] = v_xyz[v_src_index] * 0.02f;
							v_vertex[v_des_index+1] = v_xyz[v_src_index+1] * 0.02f;
							v_vertex[v_des_index+2] = v_xyz[v_src_index+2] * 0.02f;
						}

						v_src_stride += 3;
						v_dest_stride += v_dest_vertex_xyz_stride;
						v_stream->seek_to( v_stream->get_cursor() + v_meta.m_size );
					}
					else if ( unknwon_head.m_version == US_BUILDING_7Z )
					{
						bldg_builder_vertex_head v_meta;
						v_stream->read( &v_meta, v_section_size );

						short* v_xyz = (short*)( v_stream->get_buffer() + v_stream->get_cursor() );
						float* v_vertex = (float*)v_mesh->m_vertex_buffer;

						v_stream->seek_to( v_stream->get_cursor() + v_meta.m_size );

						int v_big_coord_count = 0;
						v_stream->read( &v_big_coord_count, sizeof(v_big_coord_count) );
						float* v_big_coords = (float*)(v_stream->get_buffer() + v_stream->get_cursor());

						int big_xyz_coord_off = 0;
						for ( int i = 0; i < (int)v_vertex_head.m_num; i++ )
						{
							v_des_index = i * v_dest_vertex_stride + v_dest_stride;
							v_src_index = i * 3;

							if ( v_xyz[v_src_index] == 32765 )
							{
								v_vertex[v_des_index] = v_big_coords[big_xyz_coord_off++];
							}
							else
							{
								v_vertex[v_des_index] = v_xyz[v_src_index] * 0.02f;
							}

							if ( v_xyz[v_src_index+1] == 32765 )
							{
								v_vertex[v_des_index+1] = v_big_coords[big_xyz_coord_off++];
							}
							else
							{
								v_vertex[v_des_index+1] = v_xyz[v_src_index+1] * 0.02f;
							}

							if ( v_xyz[v_src_index+2] == 32765 )
							{
								v_vertex[v_des_index+2] = v_big_coords[big_xyz_coord_off++];
							}
							else
							{
								v_vertex[v_des_index+2] = v_xyz[v_src_index+2] * 0.02f;
							}
						}

						v_src_stride += 3;
						v_dest_stride += v_dest_vertex_xyz_stride;
						v_stream->seek_to( v_stream->get_cursor() + v_big_coord_count * sizeof( float ) );
					}
				}
				break;
			case US_BLDG_BUILDER_NORMAL:
				{
					bldg_builder_vertex_head v_meta;
					v_stream->read( &v_meta, v_section_size );

					short* v_normal = (short*)( v_stream->get_buffer() + v_stream->get_cursor() );
					float* v_vertex = (float*)v_mesh->m_vertex_buffer;

					for ( int i = 0; i < (int)v_vertex_head.m_num; i++ )
					{
						v_des_index = i * v_dest_vertex_stride + v_dest_stride;
						v_src_index = i * 3;
						v_vertex[v_des_index] = v_normal[v_src_index] * 0.0001f;
						v_vertex[v_des_index+1] = v_normal[v_src_index+1] * 0.0001f;
						v_vertex[v_des_index+2] = v_normal[v_src_index+2] * 0.0001f;
					}

					v_src_stride += 3;
					v_dest_stride += 3;
					v_stream->seek_to( v_stream->get_cursor() + v_meta.m_size );
				}
				break;
			case US_BLDG_BUILDER_DIFFUSE:
				{
					bldg_builder_vertex_head v_meta;
					v_stream->read( &v_meta, v_section_size );

					DWORD* v_diffuse = (DWORD*)( v_stream->get_buffer() + v_stream->get_cursor() );
					DWORD* v_vertex = (DWORD*)v_mesh->m_vertex_buffer;

					for ( int i = 0; i < (int)v_vertex_head.m_num; i++ )
					{
						v_des_index = i * v_dest_vertex_stride + v_dest_stride;
						v_vertex[v_des_index] = v_diffuse[i];
					}

					v_src_stride += 1;
					v_dest_stride += 1;
					v_stream->seek_to( v_stream->get_cursor() + v_meta.m_size );
				}
				break;
			case US_BLDG_BUILDER_TEX0:
				{
					int v_text_levels = ( v_vertex_head.m_fvf & USFVF_TEXCOUNT_MASK ) >> 8;

					bldg_builder_vertex_head v_meta;
					v_stream->read( &v_meta, v_section_size );

					if ( v_zip_type == US_BUILDING_7Z )
					{
						long v_text_size = v_text_levels * 2 * sizeof( short ) * v_meta.m_num;

						float* v_vertex = (float*)v_mesh->m_vertex_buffer;
						short* v_text_coord = (short*)( v_stream->get_buffer() + v_stream->get_cursor() );


						v_stream->seek_to( v_stream->get_cursor() + v_text_size );
						int v_big_coord_count = 0;
						v_stream->read( &v_big_coord_count, sizeof(v_big_coord_count) );
						float* v_big_coords = (float*)(v_stream->get_buffer() + v_stream->get_cursor());

						int big_tex_coord_off = 0;
						for ( int i = 0; i < v_text_levels; i++ )
						{
							for ( int j = 0; j < (int)v_vertex_head.m_num; j++ )
							{
								v_des_index = j * v_dest_vertex_stride + v_dest_stride;
								v_src_index = ( j * v_text_levels << 1 ) + ( i << 1 );

								if ( v_text_coord[v_src_index] == 32765 )
								{
									v_vertex[v_des_index] = v_big_coords[big_tex_coord_off++];
								}
								else 
								{
									v_vertex[v_des_index] = (float)(v_text_coord[v_src_index]) / 2000.0;
								}
								if ( v_text_coord[v_src_index+1] == 32765 )
								{
									v_vertex[v_des_index+1] = v_big_coords[big_tex_coord_off++];
								}
								else 
								{
									v_vertex[v_des_index+1] = (float)(v_text_coord[v_src_index+1]) / 2000.0;
								}
							}

							v_src_stride += 2;
							v_dest_stride += 2;
						}

						v_stream->seek_to( v_stream->get_cursor() + v_big_coord_count * sizeof( float ) );
					}
					else
					{
						long v_text_size = v_text_levels * 2 * sizeof( float ) * v_meta.m_num;

						float* v_text_coord = (float*)( v_stream->get_buffer() + v_stream->get_cursor() );
						float* v_vertex = (float*)v_mesh->m_vertex_buffer;

						for ( int i = 0; i < v_text_levels; i++ )
						{
							for ( int j = 0; j < (int)v_vertex_head.m_num; j++ )
							{
								v_des_index = j * v_dest_vertex_stride + v_dest_stride;
								v_src_index = ( j * v_text_levels << 1 ) + ( i << 1 );
								v_vertex[v_des_index] = v_text_coord[v_src_index];
								v_vertex[v_des_index+1] = v_text_coord[v_src_index+1];
							}

							v_src_stride += 2;
							v_dest_stride += 2;
						}

						v_stream->seek_to( v_stream->get_cursor() + v_text_size );
					}
				}
				break;
			}
		}

		return v_readed_bytes;
	}

	//int usm_filter_7z::read_index_attribute( building_create_buffer* v_stream, model_lod_hardware_mesh* v_mesh )
	//{
	//	int v_readed_bytes = 0;

	//	int v_head_size			= sizeof( bldg_builder_head );
	//	int v_meta_size			= sizeof( bldg_builder_vertex_head );

	//	// read face head
	//	usc_index_head v_face_head;
	//	v_readed_bytes += v_stream->read( &v_face_head, sizeof( v_face_head ) );
	//	v_mesh->set_32bit_index( false );
	//	v_mesh->set_num_of_triangles( v_face_head.m_face_num );

	//	// read index
	//	bldg_builder_head v_index_head( 0, 0 );
	//	v_readed_bytes += v_stream->read( &v_index_head, sizeof(v_index_head) );

	//	bldg_builder_vertex_head v_index_meta( 0, 0 );
	//	v_readed_bytes += v_stream->read( &v_index_meta, v_meta_size );
	//	v_mesh->set_index_buffer_size( v_index_meta.m_size );

	//	ushort* v_index_buffer = new ushort[v_face_head.m_face_num*3];
	//	v_readed_bytes += v_stream->read( v_index_buffer, v_index_meta.m_size );
	//	v_mesh->set_index_buffer( v_index_buffer );

	//	// read attribute
	//	bldg_builder_head v_attr_head( 0 , 0 );
	//	v_readed_bytes += v_stream->read( &v_attr_head, v_head_size );

	//	bldg_builder_vertex_head v_attr_meta( 0, 0 );
	//	v_readed_bytes += v_stream->read( &v_attr_meta, v_meta_size );
	//	v_mesh->set_attribute_buffer_size( v_attr_meta.m_size );
	//	
	//	ulong* v_attribute_buffer = new ulong[v_face_head.m_face_num];
	//	v_readed_bytes += v_stream->read( v_attribute_buffer, v_attr_meta.m_size );
	//	v_mesh->set_attribute_buffer( v_attribute_buffer );
	//	
	//	return v_readed_bytes;
	//}

	int usm_filter_7z::read_index_attribute( building_create_buffer* v_stream, model_lod_hardware_mesh* v_mesh )
	{
		int v_readed_bytes = 0;

		int v_head_size			= sizeof( bldg_builder_head );
		int v_meta_size			= sizeof( bldg_builder_vertex_head );

		// read face head
		usc_index_head v_face_head;
		v_readed_bytes += v_stream->read( &v_face_head, sizeof( v_face_head ) );
		v_mesh->set_num_of_triangles( v_face_head.m_face_num );

		// read index
		bldg_builder_head v_index_head( 0, 0 );
		v_readed_bytes += v_stream->read( &v_index_head, sizeof(v_index_head) );

		if ( v_index_head.m_version == 0 )
		{
			v_mesh->set_32bit_index( false );
		}
		else
		{
			v_mesh->set_32bit_index( true );
		}

		bldg_builder_vertex_head v_index_meta( 0, 0 );
		v_readed_bytes += v_stream->read( &v_index_meta, v_meta_size );
		v_mesh->set_index_buffer_size( v_index_meta.m_size );

		if ( v_index_head.m_version == 0 )
		{
			ushort* v_index_buffer = new ushort[v_face_head.m_face_num*3];
			v_readed_bytes += v_stream->read( v_index_buffer, v_index_meta.m_size );
			v_mesh->set_index_buffer( v_index_buffer );
		}
		else
		{
			ulong* v_index_buffer = new ulong[v_face_head.m_face_num*3];
			v_readed_bytes += v_stream->read( v_index_buffer, v_index_meta.m_size );
			v_mesh->set_index_buffer( v_index_buffer );
		}

		// read attribute
		bldg_builder_head v_attr_head( 0 , 0 );
		v_readed_bytes += v_stream->read( &v_attr_head, v_head_size );

		bldg_builder_vertex_head v_attr_meta( 0, 0 );
		v_readed_bytes += v_stream->read( &v_attr_meta, v_meta_size );

		if ( v_attr_meta.m_size == v_attr_meta.m_num * sizeof(short) )
		{
			ulong* v_attribute_buffer = new ulong[v_face_head.m_face_num];

			unsigned short* tmp_buffer = new unsigned short[v_face_head.m_face_num];
			v_readed_bytes += v_stream->read( tmp_buffer, v_attr_meta.m_size );

			for ( int ni = 0; ni < v_attr_meta.m_num; ni++ )
			{
				v_attribute_buffer[ni] = tmp_buffer[ni];
			}
			AUTO_DELETE( tmp_buffer );

			v_mesh->set_attribute_buffer_size( v_attr_meta.m_num * sizeof(ulong) );
			v_mesh->set_attribute_buffer( v_attribute_buffer );
		}
		else
		{
			v_mesh->set_attribute_buffer_size( v_attr_meta.m_size );
			ulong* v_attribute_buffer = new ulong[v_face_head.m_face_num];
			v_readed_bytes += v_stream->read( v_attribute_buffer, v_attr_meta.m_size );
			v_mesh->set_attribute_buffer( v_attribute_buffer );
		}
		
		return v_readed_bytes;
	}

	int usm_filter_7z::read_figure( building_create_buffer* v_stream, model_lod_hardware_mesh* v_mesh )
	{
		int v_readed_bytes = 0;

		// buffer
		v_readed_bytes += v_stream->read( &v_mesh->m_figure_buf_head, sizeof(v_mesh->m_figure_buf_head) );

		v_mesh->m_figure_buf = new position_color[v_mesh->m_figure_buf_head.m_num];
		v_readed_bytes += v_stream->read( v_mesh->m_figure_buf, v_mesh->m_figure_buf_head.m_bit_count );
		for ( int ni = 0; ni < v_mesh->m_figure_buf_head.m_num; ni++ )
		{
			v_mesh->m_figure_buf[ni].y += v_stream->m_height;
		}

		// index
		v_readed_bytes += v_stream->read( &v_mesh->m_figure_index_head, sizeof(v_mesh->m_figure_index_head) );

		v_mesh->m_figure_index = new ushort[v_mesh->m_figure_index_head.m_num*2];
		v_readed_bytes += v_stream->read( v_mesh->m_figure_index, v_mesh->m_figure_index_head.m_bit_count );

		return v_readed_bytes;
	}

	int usm_filter_7z::read_topologic( building_create_buffer* v_stream, model_lod_hardware_mesh* v_mesh )
	{
		int v_readed_bytes = 0;
		if(system_environment::s_generate_topologic == false ) return v_readed_bytes;

		// edge
		bldg_builder_head v_edge_head( 0, 0 );
		v_readed_bytes += v_stream->read( &v_edge_head, sizeof(v_edge_head) );

		bldg_builder_vertex_head v_edge_meta( 0, 0 );
		v_readed_bytes += v_stream->read( &v_edge_meta, sizeof(v_edge_meta) );

		v_mesh->m_edges.resize(v_edge_meta.m_num);
		v_readed_bytes += v_stream->read( &(v_mesh->m_edges[0]), v_edge_meta.m_size );

		// face normal
		bldg_builder_head v_face_normal_head( 0, 0 );
		v_readed_bytes += v_stream->read( &v_face_normal_head, sizeof(v_face_normal_head) );

		bldg_builder_vertex_head v_face_normal_meta( 0, 0 );
		v_readed_bytes += v_stream->read( &v_face_normal_meta, sizeof(v_face_normal_meta) );

		v_mesh->m_face_normals.resize( v_face_normal_meta.m_num );
		v_readed_bytes += v_stream->read( &(v_mesh->m_face_normals[0]), v_face_normal_meta.m_size );

		return v_readed_bytes;
	}

	int usm_filter_7z::read_aabb( building_create_buffer* v_stream, model_lod_hardware_mesh* v_mesh )
	{
		int v_readed_bytes = 0, v_box_num = 0;

		v_readed_bytes += v_stream->read( &v_box_num, sizeof(v_box_num) );

		axis_aligned_box_d v_aabb;

		vector_3d vec_min, vec_max;
		v_readed_bytes += v_stream->read( &vec_min, sizeof(vec_min) );
		v_readed_bytes += v_stream->read( &vec_max, sizeof(vec_max) );
		v_aabb.set_extent( vec_min, vec_max );

		v_mesh->set_bound_box( v_aabb );

		return v_readed_bytes;
	}

	int usm_filter_7z::read_material( building_create_buffer* v_stream, model_lod_hardware_mesh* v_mesh )
	{
		int v_readed_bytes = 0;

		int v_color_size = sizeof( color_value );

		usd_material_head v_material_head;
		v_readed_bytes += v_stream->read( &v_material_head, sizeof(v_material_head) );

		v_mesh->m_text_level = v_material_head.m_texture_stage_count;

		std::vector<int> v_materials_segment;
		v_materials_segment.resize( v_material_head.m_material_segment );
		v_readed_bytes += v_stream->read( &v_materials_segment[0], sizeof(int) * v_material_head.m_material_segment );

		v_mesh->m_material_set.resize( v_material_head.m_num );
		for ( int i = 0; i < (int)v_material_head.m_num; i++ )
		{
			model_material_buffer& v_mat_buf = v_mesh->m_material_set[i];

			// material
			v_readed_bytes += v_stream->read( &v_mat_buf.m_material.m_diffuse_rgba, v_color_size );
			v_readed_bytes += v_stream->read( &v_mat_buf.m_material.m_ambient_rgb, v_color_size );
			v_readed_bytes += v_stream->read( &v_mat_buf.m_material.m_specular_rgb, v_color_size );
			v_readed_bytes += v_stream->read( &v_mat_buf.m_material.m_emissive_rgb, v_color_size );
			v_readed_bytes += v_stream->read( &v_mat_buf.m_material.m_power, sizeof(v_mat_buf.m_material.m_power) );

			for ( int j = 0; j < (int)v_material_head.m_texture_stage_count; j++ )
			{
				int v_type = 0;
				v_readed_bytes += v_stream->read( &v_type, sizeof(v_type) );
				switch ( v_type )
				{
				case US_TEXTURE_TYPE_NORMAL:
				case US_TEXTURE_TYPE_ERROR:
				case US_TEXTURE_TYPE_UNKNOWN:
					{
						v_mat_buf.m_texture_type = US_MATERIAL_TYPE_NORMAL;
						v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_diffuse_texture );
						v_mat_buf.m_texture_index.m_diffuse = string_ext::wto_int64( v_mat_buf.m_texture_file.m_diffuse_texture );
					}
					break;
				case US_TEXTURE_TYPE_ANIMATION:
					{
						v_mat_buf.m_texture_type = US_MATERIAL_TYPE_ALPHA_BLEND;
						v_mesh->m_has_animation_material = true;
						v_mesh->m_alpha_state |= US_MESH_ALPHA_BLEND;

						v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_diffuse_texture );
						v_mat_buf.m_texture_index.m_diffuse = string_ext::wto_int64( v_mat_buf.m_texture_file.m_diffuse_texture );						
					}
					break;
				case US_TEXTURE_TYPE_WATER:
					{
						v_mat_buf.m_texture_type = US_MATERIAL_TYPE_WATER;
						v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_diffuse_texture );
						v_mat_buf.m_texture_index.m_diffuse = string_ext::wto_int64( v_mat_buf.m_texture_file.m_diffuse_texture );	
					}
					break;
				case US_TEXTURE_TYPE_DYNAMIC:
					{
						v_mat_buf.m_texture_type = US_MATERIAL_TYPE_DYNAMIC;
						v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_diffuse_texture );
						v_mat_buf.m_texture_index.m_diffuse = string_ext::wto_int64( v_mat_buf.m_texture_file.m_diffuse_texture );
					}
					break;
				case US_TEXTURE_TYPE_ALPHA:
					{
						v_mat_buf.m_texture_type = US_MATERIAL_TYPE_ALPHA_TEST;
						v_mesh->m_alpha_state |= US_MESH_ALPHA_TEST;

						v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_diffuse_texture );
						v_mat_buf.m_texture_index.m_diffuse = string_ext::wto_int64( v_mat_buf.m_texture_file.m_diffuse_texture );
					}
					break;
				case US_TEXTURE_TYPE_BLEND:
					{
						v_mat_buf.m_texture_type = US_MATERIAL_TYPE_ALPHA_BLEND;
						v_mesh->m_alpha_state |= US_MESH_ALPHA_BLEND;

						v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_diffuse_texture );
						v_mat_buf.m_texture_index.m_diffuse = string_ext::wto_int64( v_mat_buf.m_texture_file.m_diffuse_texture );
					}
					break;
				case US_TEXTURE_TYPE_SELF_ILLUMINATION:
					{
						v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_self_illuminant_texture );
						v_mat_buf.m_texture_index.m_self_illuminant = string_ext::wto_int64( v_mat_buf.m_texture_file.m_self_illuminant_texture );
					}
					break;
				case US_TEXTURE_TYPE_ACCURACY_SI:
					{
						v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_self_illuminant_texture );
						v_mat_buf.m_texture_index.m_self_illuminant = string_ext::wto_int64( v_mat_buf.m_texture_file.m_self_illuminant_texture );
					}
					break;
				case US_TEXTURE_TYPE_SELF_ILLUMINATION_ERROR:
					{
						v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_self_illuminant_texture );
						v_mat_buf.m_texture_index.m_self_illuminant = string_ext::wto_int64( v_mat_buf.m_texture_file.m_self_illuminant_texture );
					}
					break;
				case US_TEXTURE_TYPE_SPECILAR:
					{
						v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_specular_texture );
						v_mat_buf.m_texture_index.m_specular = string_ext::wto_int64( v_mat_buf.m_texture_file.m_specular_texture );
					}
					break;
				case US_TEXTURE_TYPE_BUMP:
					{
						v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_bump_texture );
						v_mat_buf.m_texture_index.m_bump = string_ext::wto_int64( v_mat_buf.m_texture_file.m_bump_texture );
					}
					break;
				default:
					{
						ustring str_unknown;
						v_readed_bytes += v_stream->read( str_unknown );
					}
					break;
				}

			}
		}

		return v_readed_bytes;
	}
	
} 