
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_hardware_mesh_buffer.cpp
//  Author  : Uniscope Team
//  Modifier: Uniscope Team
//  Created :
//  Purpose : d3d9_hardware_mesh_buffer class
//	Reference :
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_hardware_mesh_buffer.h"

namespace uniscope_globe
{
d3d9_hardware_mesh_buffer::d3d9_hardware_mesh_buffer( void )
    {
    m_buffer_is_dirty		= false;
    m_32bit_index			= false;
    
    // vertex
    m_vertex_buffer			= NULL;
    // index
    m_index_buffer			= NULL;
    // attribute
    m_attribute_buffer		= NULL;
    
    m_num_of_vertices		= 0;
    m_vertex_buffer_size	= 0;
    m_num_of_triangles		= 0;
    m_index_buffer_size		= 0;
    m_attribute_buffer_size = 0;
    
    m_path					= L"";
    }
    
d3d9_hardware_mesh_buffer::~d3d9_hardware_mesh_buffer( void )
    {
    }
    
void d3d9_hardware_mesh_buffer::lock()
    {
    
    }
    
void d3d9_hardware_mesh_buffer::unlock()
    {
    
    }
    
void d3d9_hardware_mesh_buffer::clear_buffer( void )
    {
    }
    
bool d3d9_hardware_mesh_buffer::set_vector3( int offset, int index, vector_3f new_val )
    {
    if( offset == -1 ) { return false; }
    
    float* p_vert = ( float* )( ( byte* )m_vertex_buffer + offset + index * m_vertex_stride );
    *( p_vert ) = new_val.x;
    *( p_vert + 1 ) = new_val.y;
    *( p_vert + 2 ) = new_val.z;
    return true;
    }
    
bool d3d9_hardware_mesh_buffer::set_vector3( int offset, int index, vector_3f* new_val )
    {
    if( offset == -1 ) { return false; }
    
    float* p_vert = ( float* )( ( byte* )m_vertex_buffer + offset + index * m_vertex_stride );
    *( p_vert ) = new_val->x;
    *( p_vert + 1 ) = new_val->y;
    *( p_vert + 2 ) = new_val->z;
    return true;
    }
    
bool d3d9_hardware_mesh_buffer::set_vector3_rh( int offset, int index, vector_3f* new_val )
    {
    if( offset == -1 ) { return false; }
    
    float* p_vert = ( float* )( ( byte* )m_vertex_buffer + offset + index * m_vertex_stride );
    *( p_vert ) = new_val->x;
    *( p_vert + 1 ) = new_val->z;
    *( p_vert + 2 ) = new_val->y;
    return true;
    }
    
vector_3f d3d9_hardware_mesh_buffer::get_vector3( int offset, int index )
    {
    if( offset == -1 ) { return vector_3f( 0, 0, 0 ); }
    float* p_vert = ( float* )( ( byte* )m_vertex_buffer + offset + index * m_vertex_stride );
    return vector_3f( *p_vert, *( p_vert + 1 ), *( p_vert + 2 ) );
    }
    
bool d3d9_hardware_mesh_buffer::set_vector2( int offset, int index, vector_2f new_val )
    {
    if( offset == -1 ) { return false; }
    float* p_vert = ( float* )( ( byte* )m_vertex_buffer + offset + index * m_vertex_stride );
    *( p_vert ) = new_val.x;
    *( p_vert + 1 ) = new_val.y;
    return true;
    }
    
bool d3d9_hardware_mesh_buffer::set_vector2( int offset, int index, vector_2f* new_val )
    {
    if( offset == -1 ) { return false; }
    float* p_vert = ( float* )( ( byte* )m_vertex_buffer + offset + index * m_vertex_stride );
    *( p_vert ) = new_val->x;
    *( p_vert + 1 ) = new_val->y;
    return true;
    }
    
vector_2f d3d9_hardware_mesh_buffer::get_vector2( int offset, int index )
    {
    if( offset == -1 ) { return vector_2f( 0, 0 ); }
    float* p_vert = ( float* )( ( byte* )m_vertex_buffer + offset + index * m_vertex_stride );
    return vector_2f( *p_vert, *( p_vert + 1 ) );
    }
    
bool d3d9_hardware_mesh_buffer::set_float( int offset, int index, float new_val )
    {
    if( offset == -1 ) { return false; }
    float* p_vert = ( float* )( ( byte* )m_vertex_buffer + offset + index * m_vertex_stride );
    *( p_vert ) = new_val;
    return true;
    }
    
float d3d9_hardware_mesh_buffer::get_float( int offset, int index )
    {
    if( offset == -1 ) { return 0; }
    float* p_vert = ( float* )( ( byte* )m_vertex_buffer + offset + index * m_vertex_stride );
    return *p_vert;
    }
    
bool d3d9_hardware_mesh_buffer::set_ulong( int offset, int index, ulong new_val )
    {
    if( offset == -1 ) { return false; }
    ulong* p_vert = ( ulong* )( ( byte* )m_vertex_buffer + offset + index * m_vertex_stride );
    *( p_vert ) = new_val;
    return true;
    }
    
ulong d3d9_hardware_mesh_buffer::get_ulong( int offset, int index )
    {
    if( offset == -1 ) { return 0; }
    ulong* p_vert = ( ulong* )( ( byte* )m_vertex_buffer + offset + index * m_vertex_stride );
    return *p_vert;
    return true;
    }
    
bool d3d9_hardware_mesh_buffer::get_vector3_array( int offset, std::vector<vector_3f>& v_array )
    {
    if( offset == -1 ) { return false; }
    return true;
    }
    
bool d3d9_hardware_mesh_buffer::get_vector2_array( int offset, std::vector<vector_2f>& v_array	)
    {
    if( offset == -1 ) { return false; }
    return true;
    }
    
bool d3d9_hardware_mesh_buffer::set_vector3_array( int offset, std::vector<vector_3f>& v_array )
    {
    if( offset == -1 ) { return false; }
    return true;
    }
    
bool d3d9_hardware_mesh_buffer::set_vector2_array( int offset, std::vector<vector_2f>& v_array	)
    {
    if( offset == -1 ) { return false; }
    return true;
    }
    
byte* d3d9_hardware_mesh_buffer::get_fat_vertex( int index )
    {
    return ( ( byte* )m_vertex_buffer + index * m_vertex_stride );
    }
    
int d3d9_hardware_mesh_buffer::get_offset( int type )
    {
    switch( type )
        {
        case USFVF_XYZ:
            return 0;
            break;
        case USFVF_NORMAL:
            return 12;
            break;
        case USFVF_DIFFUSE:
            {
            if ( m_fvf & USFVF_NORMAL )
                {
                return 24;
                }
            return 12;
            }
        break;
        case USFVF_TEX0:
        case USFVF_TEX1:
        case USFVF_TEX2:
        case USFVF_TEX3:
        case USFVF_TEX4:
        case USFVF_TEX5:
        case USFVF_TEX6:
        case USFVF_TEX7:
        case USFVF_TEX8:
            {
            int v_off = 12 + ( ( type & USFVF_TEXCOUNT_MASK ) >> 5 );
            if ( m_fvf & USFVF_NORMAL )
                {
                v_off += 12;
                }
            if ( m_fvf & USFVF_DIFFUSE )
                {
                v_off += 4;
                }
            return v_off;
            }
        break;
        }
        
    return -1;
    }
    
void d3d9_hardware_mesh_buffer::attach_buffer_from( d3d9_hardware_mesh_buffer* in_src_buffer )
    {
    m_type				= in_src_buffer->m_type;
    m_path				= in_src_buffer->m_path;
    m_mesh_aabb				= in_src_buffer->m_mesh_aabb;
    m_fvf				= in_src_buffer->m_fvf;
    m_vertex_stride		= in_src_buffer->m_vertex_stride;
    m_32bit_index		= in_src_buffer->m_32bit_index;
    
    m_num_of_vertices		= in_src_buffer->m_num_of_vertices;
    m_vertex_buffer_size	= in_src_buffer->m_vertex_buffer_size;
    m_vertex_buffer			= in_src_buffer->m_vertex_buffer;
    
    m_num_of_triangles		= in_src_buffer->m_num_of_triangles;
    m_index_buffer_size		= in_src_buffer->m_index_buffer_size;
    m_index_buffer			= in_src_buffer->m_index_buffer;
    
    m_attribute_buffer_size	= in_src_buffer->m_attribute_buffer_size;
    m_attribute_buffer		= in_src_buffer->m_attribute_buffer;
    }
    
void d3d9_hardware_mesh_buffer::detach_buffer( void )
    {
    m_32bit_index			= false;
    
    // vertex
    m_vertex_buffer			= NULL;
    // index
    m_index_buffer			= NULL;
    // attribute
    m_attribute_buffer		= NULL;
    
    m_vertex_buffer_size	= 0;
    m_num_of_triangles		= 0;
    m_index_buffer_size		= 0;
    m_attribute_buffer_size = 0;
    
    m_path					= L"";
    }
    
void d3d9_hardware_mesh_buffer::copy_buffer_from( d3d9_hardware_mesh_buffer* in_src_buffer )
    {
    m_type				= in_src_buffer->m_type;
    m_path				= in_src_buffer->m_path;
    m_mesh_aabb				= in_src_buffer->m_mesh_aabb;
    m_fvf				= in_src_buffer->m_fvf;
    m_vertex_stride		= in_src_buffer->m_vertex_stride;
    m_32bit_index		= in_src_buffer->m_32bit_index;
    
    m_num_of_vertices		= in_src_buffer->m_num_of_vertices;
    m_vertex_buffer_size	= in_src_buffer->m_vertex_buffer_size;
    m_vertex_buffer			= new unsigned char[ m_vertex_buffer_size ];
    memcpy( m_vertex_buffer, in_src_buffer->m_vertex_buffer, m_vertex_buffer_size );
    
    m_num_of_triangles		= in_src_buffer->m_num_of_triangles ;
    m_index_buffer_size		= in_src_buffer->m_index_buffer_size;
    m_index_buffer			= new unsigned char[ m_index_buffer_size ];
    memcpy( m_index_buffer, in_src_buffer->m_index_buffer, m_index_buffer_size );
    
    m_attribute_buffer_size	= in_src_buffer->m_attribute_buffer_size;
    m_attribute_buffer		= new unsigned char[ m_attribute_buffer_size ];
    memcpy( m_attribute_buffer, in_src_buffer->m_attribute_buffer, m_attribute_buffer_size );
    
    
    m_edges = in_src_buffer->m_edges;
    m_face_normals = in_src_buffer->m_face_normals;
    }
    
void d3d9_hardware_mesh_buffer::transform_vertex_buffer( matrix_4f& v_mat )
    {
    if ( m_vertex_buffer == NULL ) { return; }
    if ( m_num_of_vertices == 0 ) { return; }
    
    byte* v_vertex_buf = ( byte* )m_vertex_buffer;
    for( ulong ni = 0; ni < m_num_of_vertices; ni++ )
        {
        vector_3f* p_vertex = ( vector_3f* )( v_vertex_buf + ni * m_vertex_stride );
        *p_vertex = ( *p_vertex ) * v_mat;
        }
    }
    
void d3d9_hardware_mesh_buffer::transform_vertex_buffer( matrix_4d& v_mat )
    {
    if ( m_vertex_buffer == NULL ) { return; }
    if ( m_num_of_vertices == 0 ) { return; }
    
    byte* v_vertex_buf = ( byte* )m_vertex_buffer;
    for( ulong ni = 0; ni < m_num_of_vertices; ni++ )
        {
        vector_3d v_vertex( ( float* )( v_vertex_buf + ni * m_vertex_stride ) );
        v_vertex = v_vertex * v_mat;
        
        vector_3f* p_vertex = ( vector_3f* )( v_vertex_buf + ni * m_vertex_stride );
        p_vertex[0].x = v_vertex.x;
        p_vertex[0].y = v_vertex.y;
        p_vertex[0].z = v_vertex.z;
        }
    }
    
//void d3d9_hardware_mesh_buffer::optimize( void )
//{
//	if ( m_32bit_index )
//	{
//		std::vector<vector3<int>> new_index_array;

//		ulong* v_faces = (ulong*)m_index_buffer;
//		for ( int ni = 0; ni < m_num_of_triangles; ni++ )
//		{
//			int k = ni * 3;
//			ulong a = v_faces[k];
//			ulong b = v_faces[k+1];
//			ulong c = v_faces[k+2];

//			vector_3f vec_a = ((vector_3f*)((byte*)m_vertex_buffer+a*m_vertex_stride))[0];
//			vector_3f vec_b = ((vector_3f*)((byte*)m_vertex_buffer+b*m_vertex_stride))[0];
//			vector_3f vec_c = ((vector_3f*)((byte*)m_vertex_buffer+c*m_vertex_stride))[0];
//
//			if ( vector_3f::normalize( vec_a - vec_b ) == vector_3f::s_zero ||
//				 vector_3f::normalize( vec_a - vec_c ) == vector_3f::s_zero ||
//				 vector_3f::normalize( vec_b - vec_c ) == vector_3f::s_zero )
//			{
//				continue;
//			}

//			new_index_array.push_back( vector3<int>( a, b, c ) );
//		}

//		m_num_of_triangles = new_index_array.size();
//		m_index_buffer_size = sizeof( ulong ) * 3 * m_num_of_triangles;

//		ulong * v_new_faces = new ulong[m_num_of_triangles*3];
//		memcpy( v_new_faces, new_index_array[0], m_index_buffer_size );

//		m_index_buffer = v_new_faces;

//		new_index_array.clear();
//	}
//	else
//	{
//		std::vector<vector3<ushort>> new_index_array;

//		ushort* v_faces = (ushort*)m_index_buffer;
//		for ( int ni = 0; ni < m_num_of_triangles; ni++ )
//		{
//			int k = ni * 3;
//			ushort a = v_faces[k];
//			ushort b = v_faces[k+1];
//			ushort c = v_faces[k+2];

//			vector_3f vec_a = ((vector_3f*)((byte*)m_vertex_buffer+a*m_vertex_stride))[0];
//			vector_3f vec_b = ((vector_3f*)((byte*)m_vertex_buffer+b*m_vertex_stride))[0];
//			vector_3f vec_c = ((vector_3f*)((byte*)m_vertex_buffer+c*m_vertex_stride))[0];

//			if ( vector_3f::normalize( vec_a - vec_b ) == vector_3f::s_zero ||
//				vector_3f::normalize( vec_a - vec_c ) == vector_3f::s_zero ||
//				vector_3f::normalize( vec_b - vec_c ) == vector_3f::s_zero )
//			{
//				continue;
//			}

//			new_index_array.push_back( vector3<ushort>( a, b, c ) );
//		}

//			m_num_of_triangles = new_index_array.size();
//			m_index_buffer_size = sizeof( ushort ) * 3 * m_num_of_triangles;

//			ulong * v_new_faces = new ulong[m_num_of_triangles*3];
//			memcpy( v_new_faces, new_index_array[0], m_index_buffer_size );

//			m_index_buffer = v_new_faces;

//			new_index_array.clear();
//
//	}
//}

void d3d9_hardware_mesh_buffer::optimize( void )
    {
    // 判定相同点
    std::vector<int> new_vertex_mapping;
    new_vertex_mapping.resize( m_num_of_vertices );
    for ( int ni = 0; ni < m_num_of_vertices; ni++ )
        {
        new_vertex_mapping[ni] = ni;
        }
        
    for ( int ni = 0; ni < m_num_of_vertices - 1; ni++ )
        {
        vector_3f vec_ii11 = ( ( vector_3f* )( ( byte* )m_vertex_buffer + ni * m_vertex_stride ) )[0];
        vector_3d vec_ii( ( float* )( &vec_ii11 ) );
        
        for ( int nj = ni + 1; nj < m_num_of_vertices; nj++ )
            {
            vector_3f vec_jj11 = ( ( vector_3f* )( ( byte* )m_vertex_buffer + nj * m_vertex_stride ) )[0];
            vector_3d vec_jj( ( float* )( &vec_jj11 ) );
            
            if ( ( vec_ii - vec_jj ).length() < 0.001 )
                {
                new_vertex_mapping[nj] = new_vertex_mapping[ni];
                }
            else
                {
                new_vertex_mapping[nj] = new_vertex_mapping[nj];
                }
            }
        }
        
    if ( m_32bit_index )
        {
        std::vector<vector3<int>> new_index_array;
        
        ulong* v_faces = ( ulong* )m_index_buffer;
        for ( int ni = 0; ni < m_num_of_triangles; ni++ )
            {
            int k = ni * 3;
            ulong a = v_faces[k];
            ulong b = v_faces[k + 1];
            ulong c = v_faces[k + 2];
            
            if ( new_vertex_mapping[a] == new_vertex_mapping[b] ||
                    new_vertex_mapping[a] == new_vertex_mapping[c] ||
                    new_vertex_mapping[b] == new_vertex_mapping[c] )
                {
                continue;
                }
                
            vector_3d vec_a( ( float* )( ( byte* )m_vertex_buffer + a * m_vertex_stride ) );
            vector_3d vec_b( ( float* )( ( byte* )m_vertex_buffer + b * m_vertex_stride ) );
            vector_3d vec_c( ( float* )( ( byte* )m_vertex_buffer + c * m_vertex_stride ) );
            
            vector_3d edge1( vec_b - vec_a );
            vector_3d edge2( vec_c - vec_a );
            edge1.normalize();
            edge2.normalize();
            
            if ( edge1 == edge2 ||
                    edge1 == -edge2 )
                {
                continue;
                }
                
            new_index_array.push_back( vector3<int>( new_vertex_mapping[a], new_vertex_mapping[b], new_vertex_mapping[c] ) );
            }
            
        m_num_of_triangles = new_index_array.size();
        m_index_buffer_size = sizeof( ulong ) * 3 * m_num_of_triangles;
        
        ulong* v_new_faces = new ulong[m_num_of_triangles * 3];
        assert( !new_index_array.empty() );
        memcpy( v_new_faces, &new_index_array[0], m_index_buffer_size );
        
        m_index_buffer = v_new_faces;
        
        new_index_array.clear();
        }
    else
        {
        std::vector<vector3<ushort>> new_index_array;
        
        ushort* v_faces = ( ushort* )m_index_buffer;
        for ( int ni = 0; ni < m_num_of_triangles; ni++ )
            {
            int k = ni * 3;
            ushort a = v_faces[k];
            ushort b = v_faces[k + 1];
            ushort c = v_faces[k + 2];
            
            if ( new_vertex_mapping[a] == new_vertex_mapping[b] ||
                    new_vertex_mapping[a] == new_vertex_mapping[c] ||
                    new_vertex_mapping[b] == new_vertex_mapping[c] )
                {
                continue;
                }
                
            vector_3f vec_a = ( ( vector_3f* )( ( byte* )m_vertex_buffer + a * m_vertex_stride ) )[0];
            vector_3f vec_b = ( ( vector_3f* )( ( byte* )m_vertex_buffer + b * m_vertex_stride ) )[0];
            vector_3f vec_c = ( ( vector_3f* )( ( byte* )m_vertex_buffer + c * m_vertex_stride ) )[0];
            
            if ( vector_3f::normalize( vec_a - vec_b ) == vector_3f::s_zero ||
                    vector_3f::normalize( vec_a - vec_c ) == vector_3f::s_zero ||
                    vector_3f::normalize( vec_b - vec_c ) == vector_3f::s_zero )
                {
                continue;
                }
                
            new_index_array.push_back( vector3<ushort>( new_vertex_mapping[a], new_vertex_mapping[b], new_vertex_mapping[c] ) );
            }
            
        m_num_of_triangles = new_index_array.size();
        m_index_buffer_size = sizeof( ushort ) * 3 * m_num_of_triangles;
        
        ulong* v_new_faces = new ulong[m_num_of_triangles * 3];
        assert( new_index_array.empty() );
        memcpy( v_new_faces, &new_index_array[0], m_index_buffer_size );
        
        m_index_buffer = v_new_faces;
        
        new_index_array.clear();
        
        }
    }
    
edge_array& d3d9_hardware_mesh_buffer::get_edge( void )
    {
    if( m_edges.size() > 0 )
        {
        return m_edges;
        }
        
    if ( m_index_buffer == NULL || m_vertex_buffer == NULL )
        {
        return m_edges;
        }
        
    bldg_line_map v_line_map;
    bldg_line_map::iterator itr;
    
    int v_off = 0;
    int v1 = 0, v2 = 0, v3 = 0;
    bldg_point_pair v_pair1, v_pair2;
    
    if ( m_32bit_index )
        {
        ulong* v_index_buffer = ( ulong* )m_index_buffer;
        
        for ( int i = 0 ; i < ( int )m_num_of_triangles; i++ )
            {
            v_off = i * 3;
            v1 = v_index_buffer[v_off];
            v2 = v_index_buffer[v_off + 1];
            v3 = v_index_buffer[v_off + 2];
            
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
                    m_edges.push_back( v_line );
                    
                    v_line_map[v_pair1] = m_edges.size() - 1;
                    }
                else
                    {
                    m_edges[itr->second].left_poly = i;
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
                    m_edges.push_back( v_line );
                    
                    v_line_map[v_pair1] = m_edges.size() - 1;
                    }
                else
                    {
                    m_edges[itr->second].left_poly = i;
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
                    m_edges.push_back( v_line );
                    
                    v_line_map[v_pair1] = m_edges.size() - 1;
                    }
                else
                    {
                    m_edges[itr->second].left_poly = i;
                    }
                }
                
            }
        }
    else
        {
        ushort* v_index_buffer = ( ushort* )m_index_buffer;
        
        for ( int i = 0 ; i < ( int )m_num_of_triangles; i++ )
            {
            v_off = i * 3;
            v1 = v_index_buffer[v_off];
            v2 = v_index_buffer[v_off + 1];
            v3 = v_index_buffer[v_off + 2];
            
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
                    m_edges.push_back( v_line );
                    
                    v_line_map[v_pair1] = m_edges.size() - 1;
                    }
                else
                    {
                    m_edges[itr->second].left_poly = i;
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
                    m_edges.push_back( v_line );
                    
                    v_line_map[v_pair1] = m_edges.size() - 1;
                    }
                else
                    {
                    m_edges[itr->second].left_poly = i;
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
                    m_edges.push_back( v_line );
                    
                    v_line_map[v_pair1] = m_edges.size() - 1;
                    }
                else
                    {
                    m_edges[itr->second].left_poly = i;
                    }
                }
                
            }
        }
        
    // release
    v_line_map.clear();
    
    
    return m_edges;
    }
    
face_normal_array& d3d9_hardware_mesh_buffer::get_face_normal( void )
    {
    if( m_face_normals.size() > 0 )
        {
        return m_face_normals;
        }
        
    if ( m_index_buffer == NULL || m_vertex_buffer == NULL )
        {
        return m_face_normals;
        }
        
    if ( m_32bit_index )
        {
        float* v_vertices = ( float* ) m_vertex_buffer;
        ulong* v_index_buffer = ( ulong* )m_index_buffer;
        int v_stride = m_vertex_stride >> 2;
        for ( int i = 0 ; i < m_num_of_triangles; i++ )
            {
            int v_off = i * 3;
            
            int a = v_index_buffer[v_off];
            int b = v_index_buffer[v_off + 1];
            int c = v_index_buffer[v_off + 2];
            
            m_face_normals.push_back( create_normal_f(
                                          *( ( vector_3f* )( v_vertices + a * v_stride ) ),
                                          *( ( vector_3f* )( v_vertices + b * v_stride ) ),
                                          *( ( vector_3f* )( v_vertices + c * v_stride ) ) )
                                    );
            }
        }
    else
        {
        float* v_vertices = ( float* ) m_vertex_buffer;
        ushort* v_index_buffer = ( ushort* )m_index_buffer;
        int v_stride = m_vertex_stride >> 2;
        for ( int i = 0 ; i < m_num_of_triangles; i++ )
            {
            int v_off = i * 3;
            
            int a = v_index_buffer[v_off];
            int b = v_index_buffer[v_off + 1];
            int c = v_index_buffer[v_off + 2];
            
            m_face_normals.push_back( create_normal_f(
                                          *( ( vector_3f* )( v_vertices + a * v_stride ) ),
                                          *( ( vector_3f* )( v_vertices + b * v_stride ) ),
                                          *( ( vector_3f* )( v_vertices + c * v_stride ) ) )
                                    );
            }
        }
        
    return m_face_normals;
    }
    
void d3d9_hardware_mesh_buffer::generate_vertex_normal( void )
    {
    if ( m_index_buffer == NULL || m_vertex_buffer == NULL )
        {
        return;
        }
        
    if ( m_32bit_index )
        {
        float* v_vertices = ( float* ) m_vertex_buffer;
        ulong* v_index_buffer = ( ulong* )m_index_buffer;
        int v_stride = m_vertex_stride >> 2;
        for ( int i = 0 ; i < m_num_of_triangles; i++ )
            {
            int v_off = i * 3;
            
            int a = v_index_buffer[v_off];
            int b = v_index_buffer[v_off + 1];
            int c = v_index_buffer[v_off + 2];
            
            
            vector_3f v_normal;
            create_normal( v_normal,
                           *( ( vector_3f* )( v_vertices + a * v_stride ) ),
                           *( ( vector_3f* )( v_vertices + b * v_stride ) ),
                           *( ( vector_3f* )( v_vertices + c * v_stride ) ) );
                           
            *( ( vector_3f* )( v_vertices + a * v_stride + 3 ) ) = v_normal;
            *( ( vector_3f* )( v_vertices + b * v_stride + 4 ) ) = v_normal;
            *( ( vector_3f* )( v_vertices + c * v_stride + 5 ) ) = v_normal;
            }
        }
    else
        {
        float* v_vertices = ( float* ) m_vertex_buffer;
        ushort* v_index_buffer = ( ushort* )m_index_buffer;
        int v_stride = m_vertex_stride >> 2;
        for ( int i = 0 ; i < m_num_of_triangles; i++ )
            {
            int v_off = i * 3;
            
            int a = v_index_buffer[v_off];
            int b = v_index_buffer[v_off + 1];
            int c = v_index_buffer[v_off + 2];
            
            vector_3f v_normal;
            create_normal( v_normal,
                           *( ( vector_3f* )( v_vertices + a * v_stride ) ),
                           *( ( vector_3f* )( v_vertices + b * v_stride ) ),
                           *( ( vector_3f* )( v_vertices + c * v_stride ) ) );
                           
            *( ( vector_3f* )( v_vertices + a * v_stride + 3 ) ) = v_normal;
            *( ( vector_3f* )( v_vertices + b * v_stride + 3 ) ) = v_normal;
            *( ( vector_3f* )( v_vertices + c * v_stride + 3 ) ) = v_normal;
            }
        }
    }
void d3d9_hardware_mesh_buffer::create_normal( vector_3f& vec_out, vector_3f& vec0, vector_3f& vec1, vector_3f& vec2 )
    {
    vector_3f edge1 = vec1 - vec0;
    vector_3f edge2 = vec2 - vec0;
    edge1.normalize();
    edge2.normalize();
    vec_out = vector_3f::cross( edge1, edge2 );
    }
    
face_normal d3d9_hardware_mesh_buffer::create_normal_f( vector_3f& vec0, vector_3f& vec1, vector_3f& vec2 )
    {
    //vector_3f edge1 = vec1 - vec0;
    //vector_3f edge2 = vec2 - vec0;
    //edge1.normalize();
    //edge2.normalize();
    
    //face_normal v_ret;
    
    //vector_3f v_normal = vector_3f::cross( edge1, edge2 );
    //v_normal.normalize();
    //v_ret.a = v_normal.x;
    //v_ret.b = v_normal.y;
    //v_ret.c = v_normal.z;
    //v_ret.d = -v_normal.dot(vec0);
    
    vector_3d edge1( ( float* )( &( vec1 - vec0 ) ) );
    vector_3d edge2( ( float* )( &( vec2 - vec0 ) ) );
    edge1.normalize();
    edge2.normalize();
    
    face_normal v_ret;
    
    vector_3d v_normal = vector_3d::cross( edge1, edge2 );
    v_normal.normalize();
    v_ret.a = v_normal.x;
    v_ret.b = v_normal.y;
    v_ret.c = v_normal.z;
    v_ret.d = -v_normal.dot( vector_3d( ( float* )( &vec0 ) ) );
    
    return v_ret;
    }
    
void d3d9_hardware_mesh_buffer::generate_adjacency( float v_eclipse,  ulong* v_adjacency )
    {
    }
    
void d3d9_hardware_mesh_buffer::generate_point_rep( ulong* v_adj_info, ulong* v_point_rep )
    {
    
    }
    
double d3d9_hardware_mesh_buffer::calculate_perimeter( void* vertex_buffer, int vertex_stride, int vertex_count )
    {
    double v_total_length = 0;
    
    for ( int ni = 0; ni < vertex_count - 1; ni ++ )
        {
        vector_3f* pv1 = ( vector_3f* )( ( byte* )vertex_buffer + vertex_stride * ni );
        vector_3f* pv2 = ( vector_3f* )( ( byte* )vertex_buffer + vertex_stride * ( ni + 1 ) );
        
        v_total_length += ( ( *pv2 ) - ( *pv1 ) ).length();
        }
    vector_3f* pv1 = ( vector_3f* )( ( byte* )vertex_buffer + vertex_stride * ( vertex_count - 1 ) );
    vector_3f* pv2 = ( vector_3f* )( ( byte* )vertex_buffer );
    
    v_total_length += ( ( *pv2 ) - ( *pv1 ) ).length();
    
    return v_total_length;
    }
}