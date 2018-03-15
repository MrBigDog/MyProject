///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_geometry_mesh.cpp
//  Author  : Uniscope Team
//  Modifier: Uniscope Team
//  Created :
//  Purpose : render_object_geometry_mesh class
//	Reference :
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_render_object_geometry_mesh.h"

namespace uniscope_globe
{
render_object_geometry_mesh::render_object_geometry_mesh( void )
    {
    m_rtti = US_RTTI_GEOMETRY_MESH;
    m_center = vector_3d::s_zero;
    
    m_fvf = position_color::fvf;
    m_shadow_type = US_SHADOW_NONE;
    m_trans_matrix = matrix_4d::s_identity;
    m_collapse_matrix = matrix_4d::s_identity;
    }
    
render_object_geometry_mesh::~render_object_geometry_mesh( void )
    {
    m_vertex_array.clear();
    m_index_array_32.clear();
    m_index_array.clear();
    m_frame_vertex_array.clear();
    m_frame_index_array.clear();
    }
    
render_object_geometry_mesh* render_object_geometry_mesh::create_shared_instance()
    {
    render_object_geometry_mesh* v_geometry = new render_object_geometry_mesh();
    v_geometry->add_ref();
    return v_geometry;
    }
    
render_object_geometry_mesh* render_object_geometry_mesh::create_shared_instance( d3d9_hardware_mesh_buffer* v_mesh_buffer, ulong v_color )
    {
    render_object_geometry_mesh* v_geometry = new render_object_geometry_mesh();
    v_geometry->set_32bit_index( v_mesh_buffer->m_32bit_index );
    
    v_geometry->m_fvf = position_color::fvf;
    v_geometry->m_vertex_stride = position_color::stride;
    v_geometry->m_num_of_vertices = v_mesh_buffer->m_num_of_vertices;
    v_geometry->m_vertex_buffer_size = v_geometry->m_num_of_vertices * v_geometry->m_vertex_stride;
    
    v_geometry->m_vertex_array.resize( v_mesh_buffer->m_num_of_vertices );
    for ( int ni = 0; ni < v_mesh_buffer->m_num_of_vertices; ni++ )
        {
        vector_3f* vertex_buf = ( vector_3f* )( ( byte* )v_mesh_buffer->m_vertex_buffer + ni * v_mesh_buffer->m_vertex_stride );
        v_geometry->m_vertex_array[ni].x = vertex_buf[0].x;
        v_geometry->m_vertex_array[ni].y = vertex_buf[0].y;
        v_geometry->m_vertex_array[ni].z = vertex_buf[0].z;
        v_geometry->m_vertex_array[ni].color = v_color;
        }
        
    v_geometry->m_num_of_triangles = v_mesh_buffer->m_num_of_triangles;
    v_geometry->m_index_buffer_size = v_mesh_buffer->m_index_buffer_size;
    if ( v_mesh_buffer->m_32bit_index )
        {
        v_geometry->m_index_array_32.resize( v_mesh_buffer->m_num_of_triangles * 3 );
        memcpy( &v_geometry->m_index_array_32[0], v_mesh_buffer->m_index_buffer, v_mesh_buffer->m_index_buffer_size );
        }
    else
        {
        v_geometry->m_index_array.resize( v_mesh_buffer->m_num_of_triangles * 3 );
        memcpy( &v_geometry->m_index_array[0], v_mesh_buffer->m_index_buffer, v_mesh_buffer->m_index_buffer_size );
        }
        
    v_geometry->add_ref();
    
    return v_geometry;
    }
    
void render_object_geometry_mesh::draw_figure( render_device* device, matrix_4f& in_mat )
    {
    if ( m_32bit_index )
        { return; }
        
    if ( m_frame_index_array.size() > 0 && m_frame_vertex_array.size() > 0 )
        {
        device->set_texture( 0, NULL );
        
        matrix_4f v_mat = in_mat;
        v_mat.m41 = in_mat.m41 - cartesian_coords::s_reference_position_geo.x;
        v_mat.m42 = in_mat.m42 - cartesian_coords::s_reference_position_geo.y;
        v_mat.m43 = in_mat.m43 - cartesian_coords::s_reference_position_geo.z;
        
        device->set_vertex_declaration( position_color::fvf );
        device->push_transform();
        
        device->multiply_transform( v_mat );
        
        render_helper::draw_indexed_primitive_up (  device, D3DPT_LINELIST, 0,
                &m_frame_vertex_array[0], &m_frame_index_array[0],
                m_frame_vertex_array.size(), m_frame_index_array.size() / 2, position_color::stride );
                
        device->pop_transform();
        }
    }
    
void render_object_geometry_mesh::draw( render_argument* args )
    {
    d3d9_effect_common_mesh* v_mesh_render = ( d3d9_effect_common_mesh* )args->m_render;
    
    v_mesh_render->set_texture( 0, NULL );
    
    matrix_4d v_mat = m_collapse_matrix * m_trans_matrix;
    v_mat.m41 = v_mat.m41 - cartesian_coords::s_reference_position_geo.x;
    v_mat.m42 = v_mat.m42 - cartesian_coords::s_reference_position_geo.y;
    v_mat.m43 = v_mat.m43 - cartesian_coords::s_reference_position_geo.z;
    
    v_mesh_render->set_vertex_declaration( m_fvf );
    v_mesh_render->push_transform();
    v_mesh_render->multiply_transform( matrix_4f( v_mat.m ) );
    v_mesh_render->commit_changes();
    
    if ( m_32bit_index )
        {
        args->m_device->draw_indexed_triangle_list( D3DFMT_INDEX32, &m_vertex_array[0], m_vertex_array.size(),
                &m_index_array_32[0], m_index_array_32.size(), position_color::stride );
        }
    else
        {
        if ( m_vertex_array.size() > 0 )
            {
            args->m_device->draw_indexed_triangle_list( D3DFMT_INDEX16, &m_vertex_array[0], m_vertex_array.size(),
                    &m_index_array[0], m_index_array.size(), position_color::stride );
            }
            
        if ( m_frame_index_array.size() > 0 && m_frame_vertex_array.size() > 0 )
            {
            render_helper::draw_indexed_primitive_up (  args->m_device, D3DPT_LINELIST, 0,
                    &m_frame_vertex_array[0], &m_frame_index_array[0],
                    m_frame_vertex_array.size(), m_frame_index_array.size() / 2, position_color::stride );
            }
        }
        
    v_mesh_render->pop_transform();
    }
    
void render_object_geometry_mesh::draw_without_figure(  render_argument* args  )
    {
    if ( m_vertex_array.size() == 0 )
        { return; }
        
    d3d9_effect_common_mesh* v_mesh_render = ( d3d9_effect_common_mesh* )args->m_render;
    
    v_mesh_render->set_texture( 0, NULL );
    
    matrix_4d v_mat = m_collapse_matrix * m_trans_matrix;
    v_mat.m41 = v_mat.m41 - cartesian_coords::s_reference_position_geo.x;
    v_mat.m42 = v_mat.m42 - cartesian_coords::s_reference_position_geo.y;
    v_mat.m43 = v_mat.m43 - cartesian_coords::s_reference_position_geo.z;
    
    v_mesh_render->set_vertex_declaration( m_fvf );
    
    v_mesh_render->push_transform();
    
    v_mesh_render->multiply_transform( matrix_4f( v_mat.m ) );
    
    v_mesh_render->commit_changes();
    
    if ( m_32bit_index )
        {
        args->m_device->draw_indexed_triangle_list( D3DFMT_INDEX32, &m_vertex_array[0], m_vertex_array.size(),
                &m_index_array_32[0], m_index_array_32.size(), position_color::stride );
        }
    else
        {
        args->m_device->draw_indexed_triangle_list( D3DFMT_INDEX16, &m_vertex_array[0], m_vertex_array.size(),
                &m_index_array[0], m_index_array.size(), position_color::stride );
        }
        
    v_mesh_render->pop_transform();
    }
    
void render_object_geometry_mesh::draw_figure( render_argument* args )
    {
    if ( m_32bit_index )
        { return; }
        
    d3d9_effect_common_mesh* v_mesh_render = ( d3d9_effect_common_mesh* )args->m_render;
    
    v_mesh_render->set_texture( 0, NULL );
    
    matrix_4d v_mat = m_collapse_matrix * m_trans_matrix;
    v_mat.m41 = v_mat.m41 - cartesian_coords::s_reference_position_geo.x;
    v_mat.m42 = v_mat.m42 - cartesian_coords::s_reference_position_geo.y;
    v_mat.m43 = v_mat.m43 - cartesian_coords::s_reference_position_geo.z;
    
    v_mesh_render->set_vertex_declaration( m_fvf );
    
    v_mesh_render->push_transform();
    
    v_mesh_render->multiply_transform( matrix_4f( v_mat.m ) );
    
    v_mesh_render->commit_changes();
    
    if ( m_frame_index_array.size() > 0 && m_frame_vertex_array.size() > 0 )
        {
        render_helper::draw_indexed_primitive_up (  args->m_device, D3DPT_LINELIST, 0,
                &m_frame_vertex_array[0], &m_frame_index_array[0],
                m_frame_vertex_array.size(), m_frame_index_array.size() / 2, position_color::stride );
        }
        
    v_mesh_render->pop_transform();
    }
    
void render_object_geometry_mesh::clear()
    {
    m_vertex_array.clear();
    m_index_array.clear();
    m_index_array_32.clear();
    
    m_frame_vertex_array.clear();
    m_frame_index_array.clear();
    
    m_collapse_matrix = matrix_4d::s_identity;
    
    m_render_aabb.make_invalid();
    }
    
bool render_object_geometry_mesh::intersect( const ray<double>& a_ray, intersect_result& result )
    {
    bool b_ret = false;
    
    if ( m_32bit_index )
        {
        ray<double> in_ray = a_ray;
        
        vector_3d pick_point;
        double pick_length = 0.0;
        for ( int i = 0; i < m_index_array.size(); i += 3 )
            {
            vector_3d vec1( m_vertex_array[m_index_array_32[i]].x, m_vertex_array[m_index_array_32[i]].y, m_vertex_array[m_index_array_32[i]].z );
            vector_3d vec2( m_vertex_array[m_index_array_32[i + 1]].x, m_vertex_array[m_index_array_32[i + 1]].y, m_vertex_array[m_index_array_32[i + 1]].z );
            vector_3d vec3( m_vertex_array[m_index_array_32[i + 2]].x, m_vertex_array[m_index_array_32[i + 2]].y, m_vertex_array[m_index_array_32[i + 2]].z );
            
            triangle_d tri( vec1, vec2, vec3 );
            bool b_pick = triangle_d::intersect( in_ray, tri );
            if ( triangle_d::pick( in_ray, tri, pick_point, 10000000.0 ) )
                {
                pick_length = ( pick_point - in_ray.ray_origin ).length();
                if ( result.m_distance > pick_length )
                    {
                    result.m_distance = pick_length;
                    result.m_position = pick_point + m_center;
                    }
                    
                b_ret = true;
                }
            }
        }
    else
        {
        ray<double> in_ray = a_ray;
        
        vector_3d pick_point;
        double pick_length = 0.0;
        for ( int i = 0; i < m_index_array.size(); i += 3 )
            {
            vector_3d vec1( m_vertex_array[m_index_array[i]].x, m_vertex_array[m_index_array[i]].y, m_vertex_array[m_index_array[i]].z );
            vector_3d vec2( m_vertex_array[m_index_array[i + 1]].x, m_vertex_array[m_index_array[i + 1]].y, m_vertex_array[m_index_array[i + 1]].z );
            vector_3d vec3( m_vertex_array[m_index_array[i + 2]].x, m_vertex_array[m_index_array[i + 2]].y, m_vertex_array[m_index_array[i + 2]].z );
            
            triangle_d tri( vec1, vec2, vec3 );
            bool b_pick = triangle_d::intersect( in_ray, tri );
            if ( triangle_d::pick( in_ray, tri, pick_point, 10000000.0 ) )
                {
                pick_length = ( pick_point - in_ray.ray_origin ).length();
                if ( result.m_distance > pick_length )
                    {
                    result.m_distance = pick_length;
                    result.m_position = pick_point + m_center;
                    }
                    
                b_ret = true;
                }
            }
        }
        
    return b_ret;
    }
    
void render_object_geometry_mesh::set_collapse( matrix_4d& mat )
    {
    m_collapse_matrix = m_collapse_matrix * mat;
    }
    
render_object* render_object_geometry_mesh::get_shadow_object( void )
    {
    return this;
    }
    
void render_object_geometry_mesh::lock()
    {
    assert( m_vertex_array.empty() );
    assert( m_index_array_32.empty() );
    assert( m_index_array.empty() );
    if ( m_vertex_array.size() == 0 )
        { return; }
        
    if ( m_index_array.size() == 0 )
        { return; }
        
    m_vertex_stride = position_color::stride;
    
    m_num_of_vertices	= m_vertex_array.size();
    m_num_of_triangles	= m_index_array.size() / 3;
    
    m_vertex_buffer_size = m_num_of_vertices * m_vertex_stride;
    m_vertex_buffer = ( void* )( &m_vertex_array[0] );
    
    if ( m_32bit_index )
        {
        m_index_buffer_size = m_num_of_triangles * 3 * sizeof( ulong );
        m_index_buffer = ( void* )( &m_index_array_32[0] );
        }
    else
        {
        m_index_buffer_size = m_num_of_triangles * 3 * sizeof( ushort );
        m_index_buffer = ( void* )( &m_index_array[0]);
        }
    }
    
void render_object_geometry_mesh::unlock()
    {
    m_vertex_buffer		= NULL;
    m_index_buffer		= NULL;
    
    m_vertex_buffer_size	= 0;
    m_index_buffer_size		= 0;
    }
}