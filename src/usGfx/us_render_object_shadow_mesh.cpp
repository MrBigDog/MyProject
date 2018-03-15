///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_shadow_mesh.cpp
//  Author  : Uniscope Team
//  Modifier: Uniscope Team
//  Created :
//  Purpose : render_object_shadow_mesh class
//	Reference :
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_render_object_shadow_mesh.h"

namespace uniscope_globe
{
render_object_shadow_mesh::render_object_shadow_mesh( void )
    {
    m_fvf = position_normal_color::fvf;
    m_stride = position_normal_color::stride;
    
    m_center = vector_3d::s_zero;
    m_trans_matrix = matrix_4d::s_identity;
    m_collapse_matrix = matrix_4d::s_identity;
    }
    
render_object_shadow_mesh::~render_object_shadow_mesh( void )
    {
    clear();
    }
    
render_object_shadow_mesh* render_object_shadow_mesh::create_shared_instance()
    {
    render_object_shadow_mesh* v_geometry = new render_object_shadow_mesh();
    v_geometry->add_ref();
    return v_geometry;
    }
    
void render_object_shadow_mesh::draw( render_argument* args )
    {
    d3d9_effect_common_mesh* v_mesh_render = ( d3d9_effect_common_mesh* )args->m_render;
    
    v_mesh_render->set_blank_texture( 0 );
    
    matrix_4d v_mat = m_collapse_matrix * m_trans_matrix;
    v_mat.m41 = v_mat.m41 - cartesian_coords::s_reference_position_geo.x;
    v_mat.m42 = v_mat.m42 - cartesian_coords::s_reference_position_geo.y;
    v_mat.m43 = v_mat.m43 - cartesian_coords::s_reference_position_geo.z;
    
    v_mesh_render->push_transform();
    v_mesh_render->multiply_transform( matrix_4f( v_mat.m ) );
    v_mesh_render->commit_changes();
    
    args->m_device->set_vertex_declaration( m_fvf );
    assert( !m_vertex_array.empty() );
    assert( !m_index_array.empty() );
    
    args->m_device->draw_indexed_triangle_list( D3DFMT_INDEX32, &m_vertex_array[0], ( ulong )m_vertex_array.size(),
            &m_index_array[0], ( ulong )m_index_array.size(), m_stride );
            
    if ( m_frame_index_array.size() > 0 && m_frame_vertex_array.size() > 0 )
        {
        args->m_device->set_vertex_declaration( position_color::fvf );
        assert( !m_frame_vertex_array.empty() );
        assert( !m_frame_index_array.empty() );
        render_helper::draw_indexed_primitive_up (
            args->m_device, D3DPT_LINELIST, 0
            , &m_frame_vertex_array[0]
            , &m_frame_index_array[0]
            , m_frame_vertex_array.size()
            , m_frame_index_array.size() / 2, position_color::stride );
        }
        
    v_mesh_render->pop_transform();
    }
    
void render_object_shadow_mesh::clear()
    {
    m_vertex_array.clear();
    m_index_array.clear();
    
    m_frame_vertex_array.clear();
    m_frame_index_array.clear();
    
    m_collapse_matrix = matrix_4d::s_identity;
    m_trans_matrix = matrix_4d::s_identity;
    
    m_render_aabb.make_invalid();
    }
    
void render_object_shadow_mesh::set_collapse( matrix_4d& mat )
    {
    m_collapse_matrix = m_collapse_matrix * mat;
    }
    
bool render_object_shadow_mesh::intersect( const ray<double>& a_ray, intersect_result& result )
    {
    bool b_ret = false;
    ray<double> in_ray = a_ray;
    
    if ( !m_render_aabb.intersect( a_ray ) )
        {
        return false;
        }
    //in_ray.ray_origin -= m_center;
    //in_ray.ray_origin.x -= m_collapse_matrix.m41;
    //in_ray.ray_origin.y -= m_collapse_matrix.m42;
    //in_ray.ray_origin.z -= m_collapse_matrix.m43;
    
    vector_3d pick_point;
    double pick_length = 0.0;
    for ( int i = 0; i < ( int )m_index_array.size(); i += 3 )
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
        
    return b_ret;
    }
}