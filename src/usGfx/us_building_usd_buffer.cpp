///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_building_usd_buffer.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : building_usd_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_building_usd_buffer.h"

namespace uniscope_globe
{
	building_usd_buffer::building_usd_buffer( raw_buffer* v_stream )
	{
		m_valid = false;
		m_lod_level = 1;
		m_vertex_buffer_size = 0;
		m_index_buffer_size = 0;
		m_attribute_buffer_size = 0;
		m_alpha_state = 0;

		m_vertex_buffer = NULL;
		m_index_buffer = NULL;
		m_attribute_buffer = NULL;
		m_material_blend = false;
		m_mateiral_animation = false;
		m_materials = new material_set;

		m_figure_buf = NULL;
		m_figure_index = NULL;

		parse_raw_buffer( v_stream );
	}

	building_usd_buffer::~building_usd_buffer( void )
	{
		AUTO_DELETE_ARRAY( m_vertex_buffer );
		AUTO_DELETE_ARRAY( m_index_buffer );
		AUTO_DELETE( m_attribute_buffer );
		AUTO_DELETE( m_materials );
		AUTO_DELETE_ARRAY( m_figure_buf );
		AUTO_DELETE( m_figure_index );
		m_indice_segment.clear();
		m_lines.clear();
		m_normals.clear();
		m_materials_type.clear();
		m_text_index.clear();
		m_materials_segment.clear();
	}

	void* building_usd_buffer::get_vertex_buffer( usc_vertex_head& vert_head )
	{
		void* ret_vertex_buffer = NULL;

		if( vert_head.m_fvf == position_texture::fvf )
		{
			ret_vertex_buffer = new position_texture[vert_head.m_num];
			m_vertex_buffer_size = sizeof( position_texture ) * vert_head.m_num;
		}
		else if ( vert_head.m_fvf == position_texture2::fvf )
		{
			ret_vertex_buffer = new position_texture2[vert_head.m_num];
			m_vertex_buffer_size = sizeof( position_texture2 ) * vert_head.m_num;
		}
		else if ( vert_head.m_fvf == position_color_texture::fvf )
		{
			ret_vertex_buffer = new position_color_texture[vert_head.m_num];
			m_vertex_buffer_size = sizeof( position_color_texture ) * vert_head.m_num;
		}
		else if ( vert_head.m_fvf == position_normal_color_texture::fvf )
		{
			ret_vertex_buffer = new position_normal_color_texture[vert_head.m_num];
			m_vertex_buffer_size = sizeof( position_normal_color_texture ) * vert_head.m_num;
		}
		else if (  vert_head.m_fvf == position_normal_texture::fvf )
		{
			ret_vertex_buffer = new position_normal_texture[vert_head.m_num];
			m_vertex_buffer_size = sizeof( position_normal_texture ) * vert_head.m_num;
		}

		return ret_vertex_buffer;
	}

	bool building_usd_buffer::parse_raw_buffer( raw_buffer* v_stream )
	{
		int v_readed_bytes = 0;
		bool v_ret = false;

		ulong v_size = v_stream->get_size();
		int v_head_size = sizeof( bldg_builder_head );

		//	read
		while( v_stream->get_cursor() < v_size )
		{
			bldg_builder_head unknown_head( 0, 0 );
			v_readed_bytes += v_stream->read( &unknown_head, v_head_size );

			switch( unknown_head.m_type )
			{
			case US_USC_VERTEX:
				if ( !read_vertex( v_stream ) )
					return false;
				break;
			case US_USC_INDICE:
				if ( !read_indice( v_stream ) )
					return false;
				break;
			case US_USC_BOUNDBOX:
				if ( !read_aabb( v_stream ) )
					return false;
				break;
			case US_USC_MATERIAL:
				if ( !read_material( v_stream ) )
					return false;
				break;
			case US_USC_TOPOLOGIC:
				if ( !read_topologic( v_stream ) )
					return false;
				break;
			case US_USC_FIGURE: 
				if( !read_figure( v_stream ) )
					return false;
				break;
			}

		}

		m_valid	= true;

		return m_valid;
	}

	bool building_usd_buffer::read_vertex( raw_buffer* v_stream )
	{
		ulong v_des_index = 0, v_src_index = 0;
		ulong v_stride = 0;

		int v_readed_bytes		= 0;
		int v_head_size			= sizeof( bldg_builder_head );
		int v_section_size		= sizeof( bldg_builder_vertex_head );

		v_readed_bytes += v_stream->read( &m_vertex_head, sizeof(m_vertex_head) );
		m_vertex_buffer = get_vertex_buffer( m_vertex_head );
		if ( m_vertex_buffer == NULL )
			return false;

		while ( v_stride < m_vertex_head.m_stride )
		{
			bldg_builder_head unknwon_head( 0 , 0 );
			v_readed_bytes += v_stream->read( &unknwon_head, v_head_size );

			switch( unknwon_head.m_type )
			{
			case US_BLDG_BUILDER_XYZ:
				{
					building_create_buffer* cbuf = (building_create_buffer*)v_stream;

					if ( unknwon_head.m_version == US_BUILDING_RAW)
					{
						bldg_builder_vertex_head v_meta;
						v_stream->read( &v_meta, v_section_size );

						float* v_xyz = (float*)( v_stream->get_buffer() + v_stream->get_cursor() );
						float* v_vertex = (float*)m_vertex_buffer;

						for ( int i = 0; i < (int)m_vertex_head.m_num; i++ )
						{
							v_des_index = i * m_vertex_head.m_stride + v_stride;
							v_src_index = i * 3;
							v_vertex[v_des_index] = v_xyz[v_src_index];
							v_vertex[v_des_index+1] = v_xyz[v_src_index+1] + cbuf->m_height;
							v_vertex[v_des_index+2] = v_xyz[v_src_index+2];
						}

						v_stride += 3;
						v_stream->seek_to( v_stream->get_cursor() + v_meta.m_size );
					}
					else 
					{
						bldg_builder_vertex_head v_meta;
						v_stream->read( &v_meta, v_section_size );

						short* v_xyz = (short*)( v_stream->get_buffer() + v_stream->get_cursor() );
						float* v_vertex = (float*)m_vertex_buffer;

						for ( int i = 0; i < (int)m_vertex_head.m_num; i++ )
						{
							v_des_index = i * m_vertex_head.m_stride + v_stride;
							v_src_index = i * 3;
							v_vertex[v_des_index] = v_xyz[v_src_index] * 0.02f;
							v_vertex[v_des_index+1] = v_xyz[v_src_index+1] * 0.02f + cbuf->m_height;
							v_vertex[v_des_index+2] = v_xyz[v_src_index+2] * 0.02f;
						}

						v_stride += 3;
						v_stream->seek_to( v_stream->get_cursor() + v_meta.m_size );
					}
				}
				break;
			case US_BLDG_BUILDER_NORMAL:
				{
					bldg_builder_vertex_head v_meta;
					v_stream->read( &v_meta, v_section_size );

					float* v_normal = (float*)( v_stream->get_buffer() + v_stream->get_cursor() );
					float* v_vertex = (float*)m_vertex_buffer;

					for ( int i = 0; i < (int)m_vertex_head.m_num; i++ )
					{
						v_des_index = i * m_vertex_head.m_stride + v_stride;
						v_src_index = i * 3;
						v_vertex[v_des_index] = v_normal[v_src_index];
						v_vertex[v_des_index+1] = v_normal[v_src_index+1];
						v_vertex[v_des_index+2] = v_normal[v_src_index+2];
					}

					v_stride += 3;
					v_stream->seek_to( v_stream->get_cursor() + v_meta.m_size );
				}
				break;
			case US_BLDG_BUILDER_DIFFUSE:
				{
					bldg_builder_vertex_head v_meta;
					v_stream->read( &v_meta, v_section_size );

					DWORD* v_diffuse = (DWORD*)( v_stream->get_buffer() + v_stream->get_cursor() );
					DWORD* v_vertex = (DWORD*)m_vertex_buffer;

					for ( int i = 0; i < (int)m_vertex_head.m_num; i++ )
					{
						v_des_index = i * m_vertex_head.m_stride + v_stride;
						v_vertex[v_des_index] = v_diffuse[i];
					}

					v_stride += 1;
					v_stream->seek_to( v_stream->get_cursor() + v_meta.m_size );
				}
				break;
			case US_BLDG_BUILDER_TEX0:
				{
					int v_text_levels = ( m_vertex_head.m_fvf & USFVF_TEXCOUNT_MASK ) >> 8;

					bldg_builder_vertex_head v_meta;
					v_stream->read( &v_meta, v_section_size );
					
					m_lod_level = v_meta.m_size;
					
					long v_text_size = v_text_levels * 2 * sizeof( float ) * v_meta.m_num;

					float* v_text_coord = (float*)( v_stream->get_buffer() + v_stream->get_cursor() );
					float* v_vertex = (float*)m_vertex_buffer;

					for ( int i = 0; i < v_text_levels; i++ )
					{
						for ( int j = 0; j < (int)m_vertex_head.m_num; j++ )
						{
							v_des_index = j * m_vertex_head.m_stride + v_stride;
							v_src_index = ( j * v_text_levels << 1 ) + ( i << 1 );
							v_vertex[v_des_index] = v_text_coord[v_src_index];
							v_vertex[v_des_index+1] = v_text_coord[v_src_index+1];
						}

						v_stride += 2;
					}

					v_stream->seek_to( v_stream->get_cursor() + v_text_size );
				}
				break;
			}
		}

		m_vertex_head.m_stride *= 4;

		

		return true;
	}

	bool building_usd_buffer::read_indice( raw_buffer* v_stream )
	{
		int v_readed_bytes = 0;

		int v_head_size			= sizeof( bldg_builder_head );
		int v_indice_head_size	= sizeof( usc_index_head );
		int v_meta_size			= sizeof( bldg_builder_vertex_head );

		// read face head
		v_readed_bytes += v_stream->read( &m_index_head, v_indice_head_size );

		// read indice
		bldg_builder_head v_indice_head( 0 , 0 );
		v_readed_bytes += v_stream->read( &v_indice_head, v_head_size );

		bldg_builder_vertex_head v_indice_meta( 0, 0 );
		v_readed_bytes += v_stream->read( &v_indice_meta, v_meta_size );
		m_index_buffer_size = v_indice_meta.m_size;

		m_index_buffer = new ushort[m_index_head.m_face_num*3];
		v_readed_bytes += v_stream->read( m_index_buffer, v_indice_meta.m_size );

		// read attribute
		bldg_builder_head v_attr_head( 0 , 0 );
		v_readed_bytes += v_stream->read( &v_attr_head, v_head_size );

		bldg_builder_vertex_head v_attr_meta( 0, 0 );
		v_readed_bytes += v_stream->read( &v_attr_meta, v_meta_size );
		m_attribute_buffer_size = v_attr_meta.m_size;

		m_attribute_buffer = new ulong[m_index_head.m_face_num];
		v_readed_bytes += v_stream->read( m_attribute_buffer, v_attr_meta.m_size );

		m_index_head.m_bit_count = 16;

		return true;
	}

	bool building_usd_buffer::read_figure( raw_buffer* v_stream )
	{
		int v_readed_bytes = 0;
		building_create_buffer* cbuf = (building_create_buffer*)v_stream;

		// buffer
		v_readed_bytes += v_stream->read( &m_figure_buf_head, sizeof(m_figure_buf_head) );

		m_figure_buf = new position_color[m_figure_buf_head.m_num];
		v_readed_bytes += v_stream->read( m_figure_buf, m_figure_buf_head.m_bit_count );
		for ( int ni = 0; ni < m_figure_buf_head.m_num; ni++ )
		{
			m_figure_buf[ni].y += cbuf->m_height;
		}
		
		// index
		v_readed_bytes += v_stream->read( &m_figure_index_head, sizeof(m_figure_index_head) );

		m_figure_index = new ushort[m_figure_index_head.m_num*2];
		v_readed_bytes += v_stream->read( m_figure_index, m_figure_index_head.m_bit_count );
		
		return true;
	}

	bool building_usd_buffer::read_topologic( raw_buffer* v_stream )
	{
		int v_readed_bytes = 0;

		// edge
		bldg_builder_head v_edge_head( 0, 0 );
		v_readed_bytes += v_stream->read( &v_edge_head, sizeof(v_edge_head) );

		bldg_builder_vertex_head v_edge_meta( 0, 0 );
		v_readed_bytes += v_stream->read( &v_edge_meta, sizeof(v_edge_meta) );

		m_lines.resize(v_edge_meta.m_num);
		assert(!m_lines.empty());
		v_readed_bytes += v_stream->read( &m_lines[0], v_edge_meta.m_size );

		// face normal
		bldg_builder_head v_face_normal_head( 0, 0 );
		v_readed_bytes += v_stream->read( &v_face_normal_head, sizeof(v_face_normal_head) );

		bldg_builder_vertex_head v_face_normal_meta( 0, 0 );
		v_readed_bytes += v_stream->read( &v_face_normal_meta, sizeof(v_face_normal_meta) );

		m_normals.resize( v_face_normal_meta.m_num );
		assert(!m_normals.empty());
		v_readed_bytes += v_stream->read( &m_normals[0], v_face_normal_meta.m_size );

		return true;
	}

	bool building_usd_buffer::read_aabb( raw_buffer* v_stream )
	{
		building_create_buffer* cbuf = (building_create_buffer*)v_stream;

		int v_readed_bytes = 0, v_box_num = 0;

		v_readed_bytes += v_stream->read( &v_box_num, sizeof(v_box_num) );

		vector_3d vec_min, vec_max;
		v_readed_bytes += v_stream->read( &vec_min, sizeof(vec_min) );
		v_readed_bytes += v_stream->read( &vec_max, sizeof(vec_max) );
		m_box.set_extent( vec_min, vec_max );
		m_box.translate( vector_3d(0.0,cbuf->m_height,0.0) );

		return true;
	}
	
	bool building_usd_buffer::read_material( raw_buffer* v_stream )
	{
		int v_readed_bytes = 0;
		int v_mat_head_size = sizeof( usd_material_head );
		int v_color_size = sizeof( color_value );

		v_readed_bytes += v_stream->read( &m_material_head, v_mat_head_size );

		m_materials_segment.resize( m_material_head.m_material_segment );
		assert(!m_materials_segment.empty());
		v_readed_bytes += v_stream->read( &m_materials_segment[0], sizeof(int) * m_material_head.m_material_segment );

		m_materials_type.resize( m_material_head.m_num );
		for ( int i = 0; i < (int)m_material_head.m_num; i++ )
		{
			material v_mat;
			texture_file v_text_file;
			texture_index v_text_index;

			// material
			v_readed_bytes += v_stream->read( &v_mat.m_diffuse_rgba, v_color_size );
			v_readed_bytes += v_stream->read( &v_mat.m_ambient_rgb, v_color_size );
			v_readed_bytes += v_stream->read( &v_mat.m_specular_rgb, v_color_size );
			v_readed_bytes += v_stream->read( &v_mat.m_emissive_rgb, v_color_size );
			v_readed_bytes += v_stream->read( &v_mat.m_power, sizeof(v_mat.m_power) );

			for ( int j = 0; j < (int)m_material_head.m_texture_stage_count; j++ )
			{
				int v_type = 0;
				v_readed_bytes += v_stream->read( &v_type, sizeof(v_type) );
				switch ( v_type )
				{
				case US_TEXTURE_TYPE_NORMAL:
					{
						m_materials_type[i] = US_MATERIAL_TYPE_NORMAL;
						v_readed_bytes += v_stream->read( v_text_file.m_diffuse_texture );
						v_text_index.m_diffuse = string_ext::wto_int64( v_text_file.m_diffuse_texture );
					}
					break;
				case US_TEXTURE_TYPE_ANIMATION:
					{
						m_materials_type[i] = US_MATERIAL_TYPE_NORMAL;
						v_readed_bytes += v_stream->read( v_text_file.m_diffuse_texture );
						v_text_index.m_diffuse = string_ext::wto_int64( v_text_file.m_diffuse_texture );

						m_mateiral_animation = true;
					}
					break;
				case US_TEXTURE_TYPE_WATER:
					{
						m_materials_type[i] = US_MATERIAL_TYPE_WATER;
						v_readed_bytes += v_stream->read( v_text_file.m_diffuse_texture );
						v_text_index.m_diffuse = string_ext::wto_int64( v_text_file.m_diffuse_texture );
					}
					break;
				case US_TEXTURE_TYPE_ALPHA:
					{
						m_alpha_state |= US_MESH_ALPHA_TEST;
						m_materials_type[i] = US_MATERIAL_TYPE_ALPHA_TEST;
						v_readed_bytes += v_stream->read( v_text_file.m_diffuse_texture );
						v_text_index.m_diffuse = string_ext::wto_int64( v_text_file.m_diffuse_texture );
					}
					break;
				case US_TEXTURE_TYPE_BLEND:
					{
						m_alpha_state |= US_MESH_ALPHA_BLEND;
						m_materials_type[i] = US_MATERIAL_TYPE_ALPHA_BLEND;
						v_readed_bytes += v_stream->read( v_text_file.m_diffuse_texture );
						v_text_index.m_diffuse = string_ext::wto_int64( v_text_file.m_diffuse_texture );

						m_material_blend = true;
					}
					break;
				case US_TEXTURE_TYPE_UNKNOWN:
					{
						m_materials_type[i] = US_MATERIAL_TYPE_NORMAL;
						v_readed_bytes += v_stream->read( v_text_file.m_diffuse_texture );
						v_text_index.m_diffuse = string_ext::wto_int64( v_text_file.m_diffuse_texture );
					}
					break;
				case US_TEXTURE_TYPE_ERROR:
					{
						m_materials_type[i] = US_MATERIAL_TYPE_NORMAL;
						v_readed_bytes += v_stream->read( v_text_file.m_diffuse_texture );
						v_text_index.m_diffuse = string_ext::wto_int64( v_text_file.m_diffuse_texture );
					}
					break;
				case 1:
					{
						v_readed_bytes += v_stream->read( v_text_file.m_self_illuminant_texture );
						v_text_index.m_self_illuminant = string_ext::wto_int64( v_text_file.m_self_illuminant_texture );
					}
					break;
				case 2:
					{
						v_readed_bytes += v_stream->read( v_text_file.m_specular_texture );
						v_text_index.m_specular = string_ext::wto_int64( v_text_file.m_specular_texture );
					}
					break;
				case 3:
					{
						v_readed_bytes += v_stream->read( v_text_file.m_bump_texture );
						v_text_index.m_bump = string_ext::wto_int64( v_text_file.m_bump_texture );
					}
					break;
				}

			}

			m_materials->m_materials.push_back( v_mat );
			m_materials->m_texture_files.push_back( v_text_file );
			m_text_index.push_back( v_text_index );
		}
		
		return true;
	}

	bool building_usd_buffer::generate_topological()
	{
		if( m_lines.size() > 0) return true;

		bldg_line_map v_line_map;
		bldg_line_map::iterator itr;

		int v_off = 0;
		int v1 = 0, v2 = 0, v3 = 0;
		bldg_point_pair v_pair1, v_pair2;

		ushort* v_index_buffer = ( ushort* )m_index_buffer;

		for ( int i = 0 ; i < (int)m_index_head.m_face_num; i++ )
		{
			v_off = i * 3;
			v1 = v_index_buffer[v_off];
			v2 = v_index_buffer[v_off+1];
			v3 = v_index_buffer[v_off+2];

			// the first line
			v_pair1 = make_pair( v1, v2 );
			itr = v_line_map.find( v_pair1 );
			if ( itr == v_line_map.end() )
			{
				v_pair2 = make_pair( v2, v1 );
				itr = v_line_map.find( v_pair2 );
				if ( itr == v_line_map.end() )
				{
					edge v_line;
					v_line.start_point = v1;
					v_line.end_point = v2;
					v_line.right_poly = i;
					m_lines.push_back( v_line );

					v_line_map[v_pair1] = m_lines.size() - 1;
				}
				else
				{
					m_lines[itr->second].left_poly = i;
				}
			}

			// the second line
			v_pair1 = make_pair( v2, v3 );
			itr = v_line_map.find( v_pair1 );
			if ( itr == v_line_map.end() )
			{
				v_pair2 = make_pair( v3, v2 );
				itr = v_line_map.find( v_pair2 );
				if ( itr == v_line_map.end() )
				{
					edge v_line;
					v_line.start_point = v2;
					v_line.end_point = v3;
					v_line.right_poly = i;
					m_lines.push_back( v_line );

					v_line_map[v_pair1] = m_lines.size() - 1;
				}
				else
				{
					m_lines[itr->second].left_poly = i;
				}
			}

			// the third line
			v_pair1 = make_pair( v3, v1 );
			itr = v_line_map.find( v_pair1 );
			if ( itr == v_line_map.end() )
			{
				v_pair2 = make_pair( v1, v3 );
				itr = v_line_map.find( v_pair2 );
				if ( itr == v_line_map.end() )
				{
					edge v_line;
					v_line.start_point = v3;
					v_line.end_point = v1;
					v_line.right_poly = i;
					m_lines.push_back( v_line );

					v_line_map[v_pair1] = m_lines.size() - 1;
				}
				else
				{
					m_lines[itr->second].left_poly = i;
				}
			}

		}

		// release
		v_line_map.clear();

		return true;
	}

	bool building_usd_buffer::generate_normals( void )
	{
		if( m_normals.size() > 0) return true;
		float* v_vertices = (float*) m_vertex_buffer;
		ushort* v_index_buffer = ( ushort* )m_index_buffer;
		int v_stride = m_vertex_head.m_stride >> 2;
		for ( int i = 0 ; i < (int)m_index_head.m_face_num; i++ )
		{
			int v_off = i * 3;

			int a = v_index_buffer[v_off];
			int b = v_index_buffer[v_off+1];
			int c = v_index_buffer[v_off+2];
			
			m_normals.push_back( create_normal_f(
				*((vector_3f*)(v_vertices + a*v_stride )),
				*((vector_3f*)(v_vertices + b*v_stride )),
				*((vector_3f*)(v_vertices + c*v_stride )) )
				);
		}

		return true;
	}

	face_normal building_usd_buffer::create_normal_f( vector_3f& vec0, vector_3f& vec1, vector_3f& vec2 )
	{
		vector_3f edge1 = vec1 - vec0;
		vector_3f edge2 = vec2 - vec0;
		edge1.normalize();
		edge2.normalize();

		face_normal v_ret;

		vector_3f v_normal = vector_3f::cross( edge1, edge2 );
		v_normal.normalize();
		v_ret.a = v_normal.x;
		v_ret.b = v_normal.y;
		v_ret.c = v_normal.z;
		v_ret.d = -v_normal.dot(vec0);

		return v_ret;
	}
}