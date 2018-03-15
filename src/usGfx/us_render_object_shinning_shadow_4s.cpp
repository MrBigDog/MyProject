///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_shinning_shadow_4s.cpp
//  Author  : Uniscope Team
//  Modifier: Uniscope Team
//  Created :
//  Purpose : render_object_shinning_shadow_4s class
//	Reference :
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_render_object_shinning_shadow_4s.h"

namespace uniscope_globe
{
render_object_shinning_shadow_4s::render_object_shinning_shadow_4s( d3d9_hardware_mesh_buffer* v_mesh_buffer, int mat_count )
    {
    m_d3d_mesh = NULL;
    m_valid = false;
    m_time_stamp = 0;
    m_parent_mesh_buffer = v_mesh_buffer;
    
    m_mat_count = mat_count;
    }
    
render_object_shinning_shadow_4s::~render_object_shinning_shadow_4s()
    {
    destroy();
    }
    
render_object_shinning_shadow_4s* render_object_shinning_shadow_4s::create_shared_instance( d3d9_hardware_mesh_buffer* v_mesh,  int mat_count )
    {
    render_object_shinning_shadow_4s* v_ro = new render_object_shinning_shadow_4s( v_mesh, mat_count );
    v_ro->add_ref();
    return v_ro;
    }
    
long render_object_shinning_shadow_4s::create( render_device* device )
    {
    if( m_d3d_mesh != NULL ) { return false; }
    
    if ( m_parent_mesh_buffer == NULL  ) { return false; }
    
    generate_shadow_mesh( device );
    
    m_shadow_object = this;
    
    return m_mem_size;
    }
    
bool render_object_shinning_shadow_4s::destroy( void )
    {
    AUTO_RELEASE( m_d3d_mesh );
    
    m_valid = false;
    
    return true;
    }
    
bool render_object_shinning_shadow_4s::create_shadow_object( render_argument* args )
    {
    if( m_d3d_mesh != NULL ) { return false; }
    
    if ( m_parent_mesh_buffer == NULL  ) { return false; }
    
    generate_shadow_mesh( args->m_device );
    
    m_shadow_object = this;
    
    return true;
    }
    
    
bool render_object_shinning_shadow_4s::refresh( void )
    {
    if( !m_valid )
        { singleton_common_manager_group::instance().get_hardware_creator()->insert_mesh( this ); }
    return true;
    }
    
void render_object_shinning_shadow_4s::draw( render_argument* args )
    {
    if( !m_d3d_mesh ) { return; }
    
    d3d9_effect_common_mesh* v_mesh_render = ( d3d9_effect_common_mesh* )args->m_render;
    
    matrix_4d v_mat = m_world_matrix;
    v_mat.m41 -= cartesian_coords::s_reference_position_geo.x;
    v_mat.m42 -= cartesian_coords::s_reference_position_geo.y;
    v_mat.m43 -= cartesian_coords::s_reference_position_geo.z;
    
    v_mesh_render->push_transform();
    v_mesh_render->multiply_transform(  matrix_4f( ( double* )v_mat.m ) );
    v_mesh_render->draw( ( void* )m_d3d_mesh, 0 );
    v_mesh_render->pop_transform();
    }
    
////////////////////////////////////////////////////////////////////////////////////////
/////
////////////////////////////////////////////////////////////////////////////////////////
//ulong render_object_shinning_shadow_4s::generate_shadow_mesh( render_device* in_device )
//{
//	d3d9_hardware_mesh_buffer* v_mesh_buffer = dynamic_cast<d3d9_hardware_mesh_buffer*>(m_parent_mesh_buffer);
//	v_mesh_buffer->lock();

//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	//
//	// 创建阴影渲染对象
//	//
//	ulong* v_adj_info  = new ulong[3 * v_mesh_buffer->get_num_of_triangles()];
//	ulong* v_point_rep = new ulong[v_mesh_buffer->get_num_of_vertices()];

//	v_mesh_buffer->generate_adjacency( 0.000001f, v_adj_info );
//	v_mesh_buffer->generate_point_rep( v_adj_info, v_point_rep );

//	int v_material_count = m_mat_count;
//	//
//	// 添加退化四边形
//	//
//	std::vector<usx_tri_face_index> v_new_face_index;
//	std::vector<position_normal_texture2> v_new_vertex_array;
//	std::vector<ulong> v_new_attribute_array;

//	edge_map v_edge_map;
//	//edge_triangle_map v_edge_triangle_map;
//	//triangle_normal_map v_triangle_normal_map;


//	int src_vertex_stride = v_mesh_buffer->get_stride();;
//	int dest_vertex_stride = v_mesh_buffer->get_stride();;
//	if(!(v_mesh_buffer->get_fvf() & D3DFVF_NORMAL))
//	{
//		dest_vertex_stride += sizeof(float) * 3;
//	}



//	for ( int i = 0; i < (int)v_mesh_buffer->get_num_of_triangles(); i++ )
//	{
//		//usx_tri_face_index v_face = v_mesh_buffer->m_index_buffer[i * 3];
//		ulong a, b, c;
//		if(v_mesh_buffer->is_32bit_index())
//		{
//			a = ((ulong*)v_mesh_buffer->m_index_buffer)[i * 3];
//			b = ((ulong*)v_mesh_buffer->m_index_buffer)[i * 3 + 1];
//			c = ((ulong*)v_mesh_buffer->m_index_buffer)[i * 3 + 2];
//		}
//		else
//		{
//			a = ((ushort*)v_mesh_buffer->m_index_buffer)[i * 3];
//			b = ((ushort*)v_mesh_buffer->m_index_buffer)[i * 3 + 1];
//			c = ((ushort*)v_mesh_buffer->m_index_buffer)[i * 3 + 2];
//		}

//		//byte* v_fat_vertex =v_mesh_buffer->get_fat_vertex(i);
//		vector3<float> v_normal = compute_normal(	(vector3<float>*)v_mesh_buffer->get_fat_vertex(a),
//			(vector3<float>*)v_mesh_buffer->get_fat_vertex(b),
//			(vector3<float>*)v_mesh_buffer->get_fat_vertex(c) );

//		// 计算法矢
//		//vector3<float> v_normal = compute_normal( a, b, c );

//		// 增加顶点

//		//managed_buffer v_buffer;
//		//v_buffer.write((void*)v_mesh_buffer->get_fat_vertex(a), sizeof(vector3<float>));
//		//v_buffer.write((void*)&v_normal, sizeof(vector3<float>));
//		//v_buffer.write((void*)v_mesh_buffer->get_fat_vertex(a) + sizeof(vector3<float>), src_vertex_stride - sizeof(vector3<float>));


//		position_normal_texture2 v1,v2,v3;
//		v1 = read_vertex(v_mesh_buffer, a );
//		v1.nx = v_normal.x;v1.ny = v_normal.y;v1.nz = v_normal.z;
//		v2 = read_vertex(v_mesh_buffer, b );
//		v2.nx = v_normal.x;v2.ny = v_normal.y;v2.nz = v_normal.z;
//		v3 = read_vertex(v_mesh_buffer, c );
//		v3.nx = v_normal.x;v3.ny = v_normal.y;v3.nz = v_normal.z;

//		// 增加三角形
//		usx_tri_face_index v_new_face;
//		v_new_face.a = v_new_vertex_array.size();
//		v_new_face.b = v_new_face.a + 1;
//		v_new_face.c = v_new_face.b + 1;
//		v_new_face_index.push_back( v_new_face );
//		v_new_attribute_array.push_back( ((ulong*)v_mesh_buffer->m_attribute_buffer)[i] );
//		//ulong v_index_count = v_new_face_index.size() - 1;

//		v_new_vertex_array.push_back( v1 );
//		v_new_vertex_array.push_back( v2 );
//		v_new_vertex_array.push_back( v3 );

//		//// [三角形-法矢]
//		//v_triangle_normal_map.insert( make_pair( v_index_count, v_normal ) );

//		//// [边-三角形]
//		//v_edge_triangle_map.insert( make_pair( edge( v_new_face.a, v_new_face.b ), v_index_count ) );
//		//v_edge_triangle_map.insert( make_pair( edge( v_new_face.b, v_new_face.c ), v_index_count ) );
//		//v_edge_triangle_map.insert( make_pair( edge( v_new_face.c, v_new_face.a ), v_index_count ) );

//		insert_triangle( v_point_rep[a], v_point_rep[b],  v_new_face.a, v_new_face.b, v_edge_map, v_new_face_index, v_new_attribute_array, v_material_count );
//		insert_triangle( v_point_rep[b], v_point_rep[c],  v_new_face.b, v_new_face.c, v_edge_map, v_new_face_index, v_new_attribute_array, v_material_count );
//		insert_triangle( v_point_rep[c], v_point_rep[a],  v_new_face.c, v_new_face.a, v_edge_map, v_new_face_index, v_new_attribute_array, v_material_count );
//	}

//	// 缝合
//	patching( v_mesh_buffer, v_edge_map, v_new_vertex_array, v_new_face_index, v_new_attribute_array, v_material_count );
//	//insert_triangle( v_edge_map, v_new_vertex_array, v_new_face_index, v_new_attribute_array );

//	//generate_mesh( v_point_rep );

//	v_mesh_buffer->unlock();

//	AUTO_DELETE( v_adj_info );
//	AUTO_DELETE( v_point_rep );

//	//////////////////////////////////////////////////////////////////////////
//	//
//	//////////////////////////////////////////////////////////////////////////
//	D3DVERTEXELEMENT9 decl[] =
//	{
//		{ 0, 0,     D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
//		{ 0, 3*4,   D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,    0 },
//		{ 0, 6*4,   D3DDECLTYPE_FLOAT2,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  0 },
//		{ 0, 8*4,   D3DDECLTYPE_FLOAT2,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  1 },
//		D3DDECL_END()
//	};

//	HRESULT hr = 0;
//	if ( FAILED( hr = D3DXCreateMesh(	v_new_face_index.size(),
//		v_new_vertex_array.size(),
//		D3DXMESH_MANAGED | D3DXMESH_32BIT,
//		decl,
//		(LPDIRECT3DDEVICE9)(in_device->get_device()),
//		&m_d3d_mesh) ) )
//	{
//		return m_mem_size;
//	}

//	void* v_vertices = NULL;
//	hr = m_d3d_mesh->LockVertexBuffer( 0, (void**)&v_vertices );
//	{
//		memcpy( (byte*)v_vertices, (void*)v_new_vertex_array[0], v_new_vertex_array.size() * sizeof( position_normal_texture2 ) );
//	}
//	hr = m_d3d_mesh->UnlockVertexBuffer();
//	m_mem_size += v_new_vertex_array.size() * sizeof( position_normal_texture2 );

//	void* v_indices = NULL;
//	hr = m_d3d_mesh->LockIndexBuffer( 0, (void**)&v_indices );
//	{
//		memcpy( (byte*)v_indices, (void*)v_new_face_index[0], v_new_face_index.size() * sizeof(ulong) * 3);
//	}
//	hr = m_d3d_mesh->UnlockIndexBuffer();
//	m_mem_size += sizeof( ulong ) * v_mesh_buffer->get_num_of_triangles();

//	if( v_mesh_buffer->m_attribute_buffer != NULL )
//	{
//		DWORD* v_attribute;
//		hr = m_d3d_mesh->LockAttributeBuffer( 0, &v_attribute );
//		{
//			memcpy( (byte*)v_attribute, (void*)v_new_attribute_array[0], sizeof( ulong ) * v_mesh_buffer->get_num_of_triangles() );
//		}
//		hr = m_d3d_mesh->UnlockAttributeBuffer();
//		m_mem_size += sizeof( ulong ) * v_mesh_buffer->get_num_of_triangles();
//	}

//	m_d3d_mesh->OptimizeInplace( D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL );


//}

//////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////
ulong render_object_shinning_shadow_4s::generate_shadow_mesh( render_device* in_device )
    {
    d3d9_hardware_mesh_buffer* v_mesh_buffer = dynamic_cast<d3d9_hardware_mesh_buffer*>( m_parent_mesh_buffer );
    v_mesh_buffer->lock();
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // 创建阴影渲染对象
    //
    
    std::vector<usx_tri_face_index> v_new_face_index;
    std::vector<position_normal_texture2> v_new_vertex_array;
    std::vector<ulong> v_new_attribute_array;
    
    int src_vertex_stride = v_mesh_buffer->get_stride();;
    int dest_vertex_stride = v_mesh_buffer->get_stride();;
    if( !( v_mesh_buffer->get_fvf() & D3DFVF_NORMAL ) )
        {
        dest_vertex_stride += sizeof( float ) * 3;
        }
        
    for ( int i = 0; i < ( int )v_mesh_buffer->get_num_of_triangles(); i++ )
        {
        ulong a, b, c;
        if( v_mesh_buffer->is_32bit_index() )
            {
            a = ( ( ulong* )v_mesh_buffer->m_index_buffer )[i * 3];
            b = ( ( ulong* )v_mesh_buffer->m_index_buffer )[i * 3 + 1];
            c = ( ( ulong* )v_mesh_buffer->m_index_buffer )[i * 3 + 2];
            }
        else
            {
            a = ( ( ushort* )v_mesh_buffer->m_index_buffer )[i * 3];
            b = ( ( ushort* )v_mesh_buffer->m_index_buffer )[i * 3 + 1];
            c = ( ( ushort* )v_mesh_buffer->m_index_buffer )[i * 3 + 2];
            }
            
        // 计算法向量
        vector3<float> v_normal = compute_normal(	( vector3<float>* )v_mesh_buffer->get_fat_vertex( a ),
                                  ( vector3<float>* )v_mesh_buffer->get_fat_vertex( b ),
                                  ( vector3<float>* )v_mesh_buffer->get_fat_vertex( c ) );
                                  
        position_normal_texture2 v1, v2, v3;
        
        // 增加三角形正面
        v1 = read_vertex( v_mesh_buffer, a );
        v1.nx = v_normal.x;
        v1.ny = v_normal.y;
        v1.nz = v_normal.z;
        v2 = read_vertex( v_mesh_buffer, b );
        v2.nx = v_normal.x;
        v2.ny = v_normal.y;
        v2.nz = v_normal.z;
        v3 = read_vertex( v_mesh_buffer, c );
        v3.nx = v_normal.x;
        v3.ny = v_normal.y;
        v3.nz = v_normal.z;
        
        usx_tri_face_index v_new_face1;
        v_new_face1.a = v_new_vertex_array.size();
        v_new_face1.b = v_new_face1.a + 1;
        v_new_face1.c = v_new_face1.b + 1;
        v_new_face_index.push_back( v_new_face1 );
        v_new_attribute_array.push_back( ( ( ulong* )v_mesh_buffer->m_attribute_buffer )[i] );
        
        v_new_vertex_array.push_back( v1 );
        v_new_vertex_array.push_back( v2 );
        v_new_vertex_array.push_back( v3 );
        
        // 增加三角形反面
        v1.nx = -v1.nx;
        v1.ny = -v1.ny;
        v1.nz = -v1.nz;
        v2.nx = -v2.nx;
        v2.ny = -v2.ny;
        v2.nz = -v2.nz;
        v3.nx = -v3.nx;
        v3.ny = -v3.ny;
        v3.nz = -v3.nz;
        
        usx_tri_face_index v_new_face2;
        v_new_face2.a = v_new_vertex_array.size();
        v_new_face2.b = v_new_face2.a + 1;
        v_new_face2.c = v_new_face2.b + 1;
        v_new_face_index.push_back( v_new_face2 );
        v_new_attribute_array.push_back( ( ( ulong* )v_mesh_buffer->m_attribute_buffer )[i] );
        
        v_new_vertex_array.push_back( v1 );
        v_new_vertex_array.push_back( v3 );
        v_new_vertex_array.push_back( v2 );
        
        //
        // 添加退化三角形
        //
        usx_tri_face_index tri1;
        tri1.a = v_new_face1.a;
        tri1.b = v_new_face2.a;
        tri1.c = v_new_face1.b;
        v_new_face_index.push_back( tri1 );
        v_new_attribute_array.push_back( 0 );
        
        usx_tri_face_index tri2;
        tri2.a = v_new_face1.b;
        tri2.b = v_new_face2.a;
        tri2.c = v_new_face2.c;
        v_new_face_index.push_back( tri2 );
        v_new_attribute_array.push_back( 0 );
        
        usx_tri_face_index tri3;
        tri3.a = v_new_face1.b;
        tri3.b = v_new_face2.c;
        tri3.c = v_new_face1.c;
        v_new_face_index.push_back( tri3 );
        v_new_attribute_array.push_back( 0 );
        
        usx_tri_face_index tri4;
        tri4.a = v_new_face1.c;
        tri4.b = v_new_face2.c;
        tri4.c = v_new_face2.b;
        v_new_face_index.push_back( tri4 );
        v_new_attribute_array.push_back( 0 );
        
        usx_tri_face_index tri5;
        tri5.a = v_new_face1.c;
        tri5.b = v_new_face2.b;
        tri5.c = v_new_face1.a;
        v_new_face_index.push_back( tri5 );
        v_new_attribute_array.push_back( 0 );
        
        usx_tri_face_index tri6;
        tri6.a = v_new_face1.a;
        tri6.b = v_new_face2.b;
        tri6.c = v_new_face2.a;
        v_new_face_index.push_back( tri6 );
        v_new_attribute_array.push_back( 0 );
        }
        
    v_mesh_buffer->unlock();
    
    //////////////////////////////////////////////////////////////////////////
    //
    //////////////////////////////////////////////////////////////////////////
    D3DVERTEXELEMENT9 decl[] =
        {
            { 0, 0,     D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION,  0 },
            { 0, 3 * 4,   D3DDECLTYPE_FLOAT3,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,    0 },
            { 0, 6 * 4,   D3DDECLTYPE_FLOAT2,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  0 },
            { 0, 8 * 4,   D3DDECLTYPE_FLOAT2,     D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD,  1 },
        D3DDECL_END()
        };
        
    HRESULT hr = 0;
    if ( FAILED( hr = D3DXCreateMesh(	v_new_face_index.size(),
                                        v_new_vertex_array.size(),
                                        D3DXMESH_MANAGED | D3DXMESH_32BIT,
                                        decl,
                                        ( LPDIRECT3DDEVICE9 )( in_device->get_device() ),
                                        &m_d3d_mesh ) ) )
        {
        return m_mem_size;
        }
        
    void* v_vertices = NULL;
    hr = m_d3d_mesh->LockVertexBuffer( 0, ( void** )&v_vertices );
        {
        assert( !v_new_vertex_array.empty() );        
        memcpy( ( byte* )v_vertices, ( void* )&v_new_vertex_array[0], v_new_vertex_array.size() * sizeof( position_normal_texture2 ) );
        }

    hr = m_d3d_mesh->UnlockVertexBuffer();
    m_mem_size += v_new_vertex_array.size() * sizeof( position_normal_texture2 );
    
    void* v_indices = NULL;
    hr = m_d3d_mesh->LockIndexBuffer( 0, ( void** )&v_indices );
        {
        assert( !v_new_face_index.empty() );        
        memcpy( ( byte* )v_indices, ( void* )&v_new_face_index[0], v_new_face_index.size() * sizeof( ulong ) * 3 );
        }

    hr = m_d3d_mesh->UnlockIndexBuffer();
    m_mem_size += sizeof( ulong ) * v_mesh_buffer->get_num_of_triangles();
    
    if( v_mesh_buffer->m_attribute_buffer != NULL )
        {
        DWORD* v_attribute;
        hr = m_d3d_mesh->LockAttributeBuffer( 0, &v_attribute );
            {
            memcpy( ( byte* )v_attribute, ( void* )v_new_attribute_array[0], sizeof( ulong ) * v_mesh_buffer->get_num_of_triangles() );
            }
        hr = m_d3d_mesh->UnlockAttributeBuffer();
        m_mem_size += sizeof( ulong ) * v_mesh_buffer->get_num_of_triangles();
        }
        
    m_d3d_mesh->OptimizeInplace( D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL );
    
    
    }
    
    
    
position_normal_texture2 render_object_shinning_shadow_4s::read_vertex( d3d9_hardware_mesh_buffer* v_buffer, ulong index )
    {
    int n_offset = 0;
    position_normal_texture2 v;
    
    vector3<float> vec1 = v_buffer->get_vector3( 0, index );
    v.x = vec1.x;
    v.y = vec1.y;
    v.z = vec1.z;
    
    n_offset += 12;
    
    float* v_float_ptr = NULL;
    vector2<float> vec2;
    if ( v_buffer->get_fvf() & D3DFVF_DIFFUSE )
        {
        n_offset += 4;
        }
        
    int v_texture_count = ( ( int )( v_buffer->get_fvf() ) & D3DFVF_TEXCOUNT_MASK ) >> 8;
    if( v_texture_count > 0 )
        {
        vec2 = v_buffer->get_vector2( n_offset, index );
        v.tu1 = vec2.x;
        v.tv1 = vec2.y;
        
        n_offset += 8;
        
        if( v_texture_count > 1 )
            {
            vec2 = v_buffer->get_vector2( n_offset, index );
            v.tu2 = vec2.x;
            v.tv2 = vec2.y;
            }
        }
        
    return v;
    }
    
vector3<float> render_object_shinning_shadow_4s::compute_normal( vector3<float>* vec1, vector3<float>* vec2, vector3<float>* vec3 )
    {
    //vector3<float> vec1, vec2, vec3;
    //vec1 = this->get_vector3(0, a);
    //vec2 = this->get_vector3(0, b);
    //vec3 = this->get_vector3(0, c);
    
    vector3<float> tmp1 = vector_3f::normalize( *vec2 - *vec1 );
    vector3<float> tmp2 = vector_3f::normalize( *vec3 - *vec1 );
    vector3<float> normal = vector3<float>::cross( tmp1, tmp2 );
    normal.normalize();
    
    return normal;
    }
    
vector3<float> render_object_shinning_shadow_4s::compute_normal( d3d9_hardware_mesh_buffer* v_buffer, ulong a, ulong b, ulong c )
    {
    vector3<float> vec1, vec2, vec3;
    vec1 = v_buffer->get_vector3( 0, a );
    vec2 = v_buffer->get_vector3( 0, b );
    vec3 = v_buffer->get_vector3( 0, c );
    
    vector3<float> tmp1 = vector3<float>::normalize( vec2 - vec1 );
    vector3<float> tmp2 = vector3<float>::normalize( vec3 - vec1 );
    vector3<float> normal = vector3<float>::cross( tmp1, tmp2 );
    normal.normalize();
    
    return normal;
    }
    
void render_object_shinning_shadow_4s::insert_triangle(	ulong o1, ulong o2, ulong n1, ulong n2, edge_map& in_edge_map,
        std::vector<usx_tri_face_index>& in_index_array,
        std::vector<ulong>& in_attribute_array, ulong in_material_count )
    {
    ulong v_material_count = in_material_count;
    
    edge_map::iterator itr = in_edge_map.find( make_pair( o2, o1 ) );
    if( itr != in_edge_map.end() )
        {
        degenerate_quad& quad = itr->second;
        usx_tri_face_index v_new_face;
        v_new_face.a = n1;
        v_new_face.b = quad.m_v2;
        v_new_face.c = quad.m_v1;
        in_index_array.push_back( v_new_face );
        in_attribute_array.push_back( v_material_count );
        
        v_new_face.a = n1;
        v_new_face.b = quad.m_v1;
        v_new_face.c = n2;
        in_index_array.push_back( v_new_face );
        in_attribute_array.push_back( v_material_count );
        
        in_edge_map.erase( itr );
        }
    else
        {
        degenerate_quad quad;
        quad.m_v1 = n1;
        quad.m_v2 = n2;
        in_edge_map.insert( make_pair( make_pair( o1, o2 ), quad ) );
        }
    }
    
void render_object_shinning_shadow_4s::patching(	d3d9_hardware_mesh_buffer* v_buffer,
        edge_map& in_edge_map,
        std::vector<position_normal_texture2>& in_new_vertex_array,
        std::vector<usx_tri_face_index>& in_new_face_index,
        std::vector<ulong>& in_new_attribute_array,
        ulong in_material_count )
    {
    ulong v_material_count = in_material_count;
    
    while ( in_edge_map.size() >= 2 )
        {
        edge_map::iterator itr1 = in_edge_map.begin();
        edge_map::iterator itr2 = in_edge_map.begin();
        itr2++;
        bool v_alone = true;
        for ( ; itr2 != in_edge_map.end(); itr2++ )
            {
            if(	( itr1->second.m_v3 == 0xffffffff || itr1->second.m_v4 == 0xffffffff ) &&
                    ( itr2->second.m_v3 == 0xffffffff  || itr2->second.m_v4 == 0xffffffff ) )
                {
                int shared_vertex = 0;
                
                ulong o1, o2, o3;
                ulong n1, n2, n3, n4;
                if( itr2->first.first == itr1->first.second )
                    {
                    shared_vertex++;
                    o1 = itr1->first.first;
                    o2 = itr1->first.second;
                    o3 = itr2->first.second;
                    
                    n1 = itr1->second.m_v1;
                    n2 = itr1->second.m_v2;
                    n3 = itr2->second.m_v1;
                    n4 = itr2->second.m_v2;
                    }
                    
                if( itr2->first.second == itr1->first.first )
                    {
                    shared_vertex++;
                    o1 = itr2->first.first;
                    o2 = itr2->first.second;
                    o3 = itr1->first.second;
                    
                    n1 = itr2->second.m_v1;
                    n2 = itr2->second.m_v2;
                    n3 = itr1->second.m_v1;
                    n4 = itr1->second.m_v2;
                    }
                    
                if( shared_vertex == 2 )
                    {
                    usx_tri_face_index tri1;
                    tri1.a = itr1->second.m_v1;
                    tri1.b = itr2->second.m_v2;
                    tri1.c = itr2->second.m_v1;
                    in_new_face_index.push_back( tri1 );
                    in_new_attribute_array.push_back( v_material_count );
                    
                    usx_tri_face_index tri2;
                    tri2.a = itr1->second.m_v1;
                    tri2.b = itr2->second.m_v1;
                    tri2.c = itr1->second.m_v2;
                    in_new_face_index.push_back( tri2 );
                    in_new_attribute_array.push_back( v_material_count );
                    
                    in_edge_map.erase( itr1 );
                    in_edge_map.erase( itr2 );
                    
                    v_alone = false;
                    
                    break;
                    }
                else if( shared_vertex == 1 )
                    {
                    // 计算法矢
                    vector3<float> v_normal = compute_normal( v_buffer, o1, o3, o2 );
                    
                    // 增加顶点
                    position_normal_texture2 v1, v2, v3;
                    v1 = in_new_vertex_array[n1];
                    v1.nx = v_normal.x;
                    v1.ny = v_normal.y;
                    v1.nz = v_normal.z;
                    v2 = in_new_vertex_array[n4];
                    v2.nx = v_normal.x;
                    v2.ny = v_normal.y;
                    v2.nz = v_normal.z;
                    v3 = in_new_vertex_array[n3];
                    v3.nx = v_normal.x;
                    v3.ny = v_normal.y;
                    v3.nz = v_normal.z;
                    
                    // 增加三角形
                    usx_tri_face_index v_new_face;
                    v_new_face.a = in_new_vertex_array.size();
                    v_new_face.b = v_new_face.a + 1;
                    v_new_face.c = v_new_face.b + 1;
                    in_new_face_index.push_back( v_new_face );
                    in_new_attribute_array.push_back( v_material_count );
                    
                    in_new_vertex_array.push_back( v1 );
                    in_new_vertex_array.push_back( v2 );
                    in_new_vertex_array.push_back( v3 );
                    
                    // 增加退化四边形
                    // first quad
                    usx_tri_face_index tri1;
                    tri1.a = n1;
                    tri1.b = v_new_face.a;
                    tri1.c = v_new_face.c;
                    in_new_face_index.push_back( tri1 );
                    in_new_attribute_array.push_back( v_material_count );
                    
                    usx_tri_face_index tri2;
                    tri2.a = n1;
                    tri2.b = v_new_face.c;
                    tri2.c = n2;
                    in_new_face_index.push_back( tri2 );
                    in_new_attribute_array.push_back( v_material_count );
                    
                    // second quad
                    usx_tri_face_index tri3;
                    tri3.a = n3;
                    tri3.b = v_new_face.c;
                    tri3.c = v_new_face.b;
                    in_new_face_index.push_back( tri3 );
                    in_new_attribute_array.push_back( v_material_count );
                    
                    usx_tri_face_index tri4;
                    tri4.a = n3;
                    tri4.b = v_new_face.b;
                    tri4.c = n4;
                    in_new_face_index.push_back( tri4 );
                    in_new_attribute_array.push_back( v_material_count );
                    
                    degenerate_quad v_new_quad;
                    v_new_quad.m_v1 = v_new_face.a;
                    v_new_quad.m_v2 = v_new_face.b;
                    in_edge_map.insert( make_pair( make_pair( o1, o3 ), v_new_quad ) );
                    in_edge_map.erase( itr1 );
                    in_edge_map.erase( itr2 );
                    
                    v_alone = false;
                    
                    break;
                    }
                }
                
            }// for
            
        if( v_alone == true )
            {
            in_edge_map.erase( itr1 );
            }
        }
    }
    
    
    
}



























