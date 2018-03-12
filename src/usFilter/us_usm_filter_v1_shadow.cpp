///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usm_filter_v1_shadow.cpp
//  Author  : Uniscope Team
//  Modifier: Uniscope Team
//  Created :
//  Purpose : usm_filter_v1_shadow class
//	Reference :
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_usm_filter_v1_shadow.h"

namespace uniscope_globe
{
usm_filter_v1_shadow::usm_filter_v1_shadow( void )
    {
    
    }
    
usm_filter_v1_shadow::~usm_filter_v1_shadow( void )
    {
    
    }
    
void* usm_filter_v1_shadow::parse( raw_buffer* v_buffer )
    {
    raw_buffer* compress_buf = v_buffer;
    
    // 读取数据头
    bldg_builder_head v_head( 0, 0 );
    compress_buf->read( &v_head, sizeof( v_head ) );
    
    ulong v_data_size = 0;
    compress_buf->read( &v_data_size, sizeof( v_data_size ) );
    
    // 分配解压缩数据内存
    BYTE* v_uncompress_data = new BYTE[v_data_size];
    
    // 原始数据
    ulong v_compress_data_offset = sizeof( v_head ) + sizeof( v_data_size );
    byte* v_src_data = compress_buf->get_buffer();
    ulong v_src_size = compress_buf->get_size();
    
    // 解压
    uncompress( v_uncompress_data, ( unsigned long* )&v_data_size, v_src_data + v_compress_data_offset, v_src_size - v_compress_data_offset );
    
    // 创建buffer
    managed_buffer uncompress_buf( v_uncompress_data, v_data_size );
    
    d3d9_hardware_mesh_buffer* v_mesh = parse_d3d9_hardware_mesh( &uncompress_buf );
    
    AUTO_DELETE( v_uncompress_data );
    
    return v_mesh;
    }
    
d3d9_hardware_mesh_buffer* usm_filter_v1_shadow::parse_d3d9_hardware_mesh( managed_buffer* uncompress_buf )
    {
    d3d9_hardware_mesh_buffer* v_mesh = new d3d9_hardware_mesh_buffer();
    
    //	解析数据
    int v_data_size = uncompress_buf->get_size();
    
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
        
    return v_mesh;
    }
    
int usm_filter_v1_shadow::read_vertex( managed_buffer* v_stream, d3d9_hardware_mesh_buffer* v_mesh )
    {
    ulong v_des_index = 0, v_src_index = 0;
    
    int v_readed_bytes		= 0;
    int v_head_size			= sizeof( bldg_builder_head );
    int v_section_size		= sizeof( bldg_builder_vertex_head );
    
    usc_vertex_head v_vertex_head;
    v_readed_bytes += v_stream->read( &v_vertex_head, sizeof( v_vertex_head ) );
    
    v_mesh->set_fvf( position_normal::fvf );
    v_mesh->set_stride( position_normal::stride );
    v_mesh->set_num_of_vertices( v_vertex_head.m_num );
    v_mesh->set_vertex_buffer_size( v_mesh->m_vertex_stride * v_mesh->m_num_of_vertices );
    
    v_mesh->m_vertex_buffer = ( void* )( new byte[v_mesh->m_vertex_buffer_size] );
    memset( v_mesh->m_vertex_buffer, 0, v_mesh->m_vertex_buffer_size );
    
    ulong v_mesh_stride = v_mesh->m_vertex_stride / 4;
    ulong v_stride = 0;
    while ( v_stride < v_vertex_head.m_stride )
        {
        bldg_builder_head unknwon_head( 0 , 0 );
        v_readed_bytes += v_stream->read( &unknwon_head, v_head_size );
        
        switch( unknwon_head.m_type )
            {
            case US_BLDG_BUILDER_XYZ:
                {
                if ( unknwon_head.m_version == US_BUILDING_RAW )
                    {
                    bldg_builder_vertex_head v_meta;
                    v_stream->read( &v_meta, v_section_size );
                    
                    float* v_xyz = ( float* )( v_stream->get_buffer() + v_stream->get_cursor() );
                    float* v_vertex = ( float* )v_mesh->m_vertex_buffer;
                    
                    for ( int i = 0; i < ( int )v_vertex_head.m_num; i++ )
                        {
                        v_des_index = i * v_mesh_stride;
                        v_src_index = i * 3;
                        v_vertex[v_des_index] = v_xyz[v_src_index];
                        v_vertex[v_des_index + 1] = v_xyz[v_src_index + 1];
                        v_vertex[v_des_index + 2] = v_xyz[v_src_index + 2];
                        }
                        
                    v_stride += 3;
                    v_stream->seek_to( v_stream->get_cursor() + v_meta.m_size );
                    }
                else
                    {
                    bldg_builder_vertex_head v_meta;
                    v_stream->read( &v_meta, v_section_size );
                    
                    short* v_xyz = ( short* )( v_stream->get_buffer() + v_stream->get_cursor() );
                    float* v_vertex = ( float* )v_mesh->m_vertex_buffer;
                    
                    for ( int i = 0; i < ( int )v_vertex_head.m_num; i++ )
                        {
                        v_des_index = i * v_mesh_stride;
                        v_src_index = i * 3;
                        v_vertex[v_des_index] = v_xyz[v_src_index] * 0.02f;
                        v_vertex[v_des_index + 1] = v_xyz[v_src_index + 1] * 0.02f;
                        v_vertex[v_des_index + 2] = v_xyz[v_src_index + 2] * 0.02f;
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
                
                short* v_normal = ( short* )( v_stream->get_buffer() + v_stream->get_cursor() );
                float* v_vertex = ( float* )v_mesh->m_vertex_buffer;
                
                for ( int i = 0; i < ( int )v_vertex_head.m_num; i++ )
                    {
                    v_des_index = i * v_mesh_stride + 3;
                    v_src_index = i * 3;
                    v_vertex[v_des_index] = v_normal[v_src_index] * 0.0001f;
                    v_vertex[v_des_index + 1] = v_normal[v_src_index + 1] * 0.0001f;
                    v_vertex[v_des_index + 2] = v_normal[v_src_index + 2] * 0.0001f;
                    }
                    
                v_stride += 3;
                v_stream->seek_to( v_stream->get_cursor() + v_meta.m_size );
                }
            break;
            case US_BLDG_BUILDER_DIFFUSE:
                {
                bldg_builder_vertex_head v_meta;
                v_stream->read( &v_meta, v_section_size );
                
                v_stride += 1;
                
                v_stream->seek_to( v_stream->get_cursor() + v_meta.m_size );
                }
            break;
            case US_BLDG_BUILDER_TEX0:
                {
                bldg_builder_vertex_head v_meta;
                v_stream->read( &v_meta, v_section_size );
                
                int v_text_levels = ( v_vertex_head.m_fvf & USFVF_TEXCOUNT_MASK ) >> 8;
                long v_text_size = v_text_levels * 2 * sizeof( float ) * v_meta.m_num;
                
                v_stride += ( v_text_levels * 2 ) ;
                
                v_stream->seek_to( v_stream->get_cursor() + v_text_size );
                }
            break;
            }
        }
        
    return v_readed_bytes;
    }
    
int usm_filter_v1_shadow::read_index_attribute( managed_buffer* v_stream, d3d9_hardware_mesh_buffer* v_mesh )
    {
    int v_readed_bytes = 0;
    
    int v_head_size			= sizeof( bldg_builder_head );
    int v_meta_size			= sizeof( bldg_builder_vertex_head );
    
    // read face head
    usc_index_head v_face_head;
    v_readed_bytes += v_stream->read( &v_face_head, sizeof( v_face_head ) );
    v_mesh->set_32bit_index( v_face_head.m_bit_count == 32 );
    v_mesh->set_num_of_triangles( v_face_head.m_face_num );
    
    // read index
    bldg_builder_head v_index_head( 0, 0 );
    v_readed_bytes += v_stream->read( &v_index_head, sizeof( v_index_head ) );
    
    bldg_builder_vertex_head v_index_meta( 0, 0 );
    v_readed_bytes += v_stream->read( &v_index_meta, v_meta_size );
    v_mesh->set_index_buffer_size( v_index_meta.m_size );
    
	// 当顶点个数超过两个字节能表示时，就得到4个字节了。 [9/18/2017 mahc]
	if (v_face_head.m_bit_count == 32)
	{
		int* v_index_buffer = new int[v_face_head.m_face_num * 3];
		v_readed_bytes += v_stream->read( v_index_buffer, v_index_meta.m_size );
		v_mesh->set_index_buffer( v_index_buffer );
	}
	else
	{
		ushort* v_index_buffer = new ushort[v_face_head.m_face_num * 3];
		v_readed_bytes += v_stream->read( v_index_buffer, v_index_meta.m_size );
		v_mesh->set_index_buffer( v_index_buffer );
	}
    
    // read attribute
    bldg_builder_head v_attr_head( 0 , 0 );
    v_readed_bytes += v_stream->read( &v_attr_head, v_head_size );
    
    bldg_builder_vertex_head v_attr_meta( 0, 0 );
    v_readed_bytes += v_stream->read( &v_attr_meta, v_meta_size );
    
    v_stream->seek_to( v_stream->get_cursor() + v_attr_meta.m_size );
    
    return v_readed_bytes;
    }
    
int usm_filter_v1_shadow::read_figure( managed_buffer* v_stream, d3d9_hardware_mesh_buffer* v_mesh )
    {
    int v_readed_bytes = 0;
    
    // buffer
    usc_figure_head v_figure_buf_head;
    v_readed_bytes += v_stream->read( &v_figure_buf_head, sizeof( v_figure_buf_head ) );
    
    v_stream->seek_to( v_stream->get_cursor() + v_figure_buf_head.m_bit_count );
    
    // index
    usc_figure_head v_figure_index_head;
    v_readed_bytes += v_stream->read( &v_figure_index_head, sizeof( v_figure_index_head ) );
    
    v_stream->seek_to( v_stream->get_cursor() + v_figure_index_head.m_bit_count );
    
    return v_readed_bytes;
    }
    
int usm_filter_v1_shadow::read_topologic( managed_buffer* v_stream, d3d9_hardware_mesh_buffer* v_mesh )
    {
    int v_readed_bytes = 0;
    
    // edge
    bldg_builder_head v_edge_head( 0, 0 );
    v_readed_bytes += v_stream->read( &v_edge_head, sizeof( v_edge_head ) );
    
    bldg_builder_vertex_head v_edge_meta( 0, 0 );
    v_readed_bytes += v_stream->read( &v_edge_meta, sizeof( v_edge_meta ) );
    
    v_mesh->m_edges.resize( v_edge_meta.m_num );
    v_readed_bytes += v_stream->read( &( v_mesh->m_edges[0] ), v_edge_meta.m_size );
    
    // face normal
    bldg_builder_head v_face_normal_head( 0, 0 );
    v_readed_bytes += v_stream->read( &v_face_normal_head, sizeof( v_face_normal_head ) );
    
    bldg_builder_vertex_head v_face_normal_meta( 0, 0 );
    v_readed_bytes += v_stream->read( &v_face_normal_meta, sizeof( v_face_normal_meta ) );
    
    v_mesh->m_face_normals.resize( v_face_normal_meta.m_num );
    v_readed_bytes += v_stream->read( &( v_mesh->m_face_normals[0] ), v_face_normal_meta.m_size );
    
    return v_readed_bytes;
    }
    
int usm_filter_v1_shadow::read_aabb( managed_buffer* v_stream, d3d9_hardware_mesh_buffer* v_mesh )
    {
    int v_readed_bytes = 0, v_box_num = 0;
    
    v_readed_bytes += v_stream->read( &v_box_num, sizeof( v_box_num ) );
    
    axis_aligned_box_d v_aabb;
    
    vector_3d vec_min, vec_max;
    v_readed_bytes += v_stream->read( &vec_min, sizeof( vec_min ) );
    v_readed_bytes += v_stream->read( &vec_max, sizeof( vec_max ) );
    v_aabb.set_extent( vec_min, vec_max );
    
    return v_readed_bytes;
    }
    
int usm_filter_v1_shadow::read_material( managed_buffer* v_stream, d3d9_hardware_mesh_buffer* v_mesh )
    {
    int v_readed_bytes = 0;
    
    int v_color_size = sizeof( color_value );
    
    usd_material_head v_material_head;
    v_readed_bytes += v_stream->read( &v_material_head, sizeof( v_material_head ) );
    
    std::vector<int> v_materials_segment;
    v_materials_segment.resize( v_material_head.m_material_segment );
    v_readed_bytes += v_stream->read( &v_materials_segment[0], sizeof( int ) * v_material_head.m_material_segment );
    
    std::vector<int> v_materials_type;
    v_materials_type.resize( v_material_head.m_num );
    for ( int i = 0; i < ( int )v_material_head.m_num; i++ )
        {
        model_material_buffer v_mat_buf;
        
        // material
        v_readed_bytes += v_stream->read( &v_mat_buf.m_material.m_diffuse_rgba, v_color_size );
        v_readed_bytes += v_stream->read( &v_mat_buf.m_material.m_ambient_rgb, v_color_size );
        v_readed_bytes += v_stream->read( &v_mat_buf.m_material.m_specular_rgb, v_color_size );
        v_readed_bytes += v_stream->read( &v_mat_buf.m_material.m_emissive_rgb, v_color_size );
        v_readed_bytes += v_stream->read( &v_mat_buf.m_material.m_power, sizeof( v_mat_buf.m_material.m_power ) );
        
        for ( int j = 0; j < ( int )v_material_head.m_texture_stage_count; j++ )
            {
            int v_type = 0;
            v_readed_bytes += v_stream->read( &v_type, sizeof( v_type ) );
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
                    v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_diffuse_texture );
                    }
                break;
                case 1:
                    {
                    v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_self_illuminant_texture );
                    }
                break;
                case 2:
                    {
                    v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_specular_texture );
                    }
                break;
                case 3:
                    {
                    v_readed_bytes += v_stream->read( v_mat_buf.m_texture_file.m_bump_texture );
                    }
                break;
                default:
                    {
                    ustring str_file;
                    v_readed_bytes += v_stream->read( str_file );
                    }
                }
            }
        }
        
    return v_readed_bytes;
    }
    
    
}