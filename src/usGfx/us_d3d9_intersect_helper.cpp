///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_intersect_helper.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_intersect_helper class
//	Reference : Microsoft DircetX SDK(December 2005)
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_intersect_helper.h"

namespace uniscope_globe
{
	d3d9_intersect_helper* singleton_d3d9_intersect_helper::s_ins = NULL;

	d3d9_intersect_helper::d3d9_intersect_helper( void )
	{

	}

	d3d9_intersect_helper::~d3d9_intersect_helper( void )
	{

	}

	bool d3d9_intersect_helper::hit_test( void* d3d_mesh, ray<double> in_ray, intersect_result& out_result )
	{
		if( d3d_mesh == NULL ) return false;
		bool ret_val = false;

		BOOL is_hit ;
		ulong face_index, hit_count;
		float u, v, v_distance;

		ID3DXBuffer* all_hit_buffer = NULL;
		D3DXVECTOR3 d3dvec_position( in_ray.ray_origin.x, in_ray.ray_origin.y, in_ray.ray_origin.z );
		D3DXVECTOR3 d3dvec_direction( in_ray.ray_direction.x, in_ray.ray_direction.y, in_ray.ray_direction.z );
		D3DXIntersect( (LPD3DXMESH)d3d_mesh, &d3dvec_position, &d3dvec_direction, &is_hit, &face_index, &u, &v, &v_distance, &all_hit_buffer, &hit_count);

		if( is_hit )
		{
			if( v_distance < out_result.m_distance )
			{
				out_result.m_distance = v_distance;
				out_result.m_face_index = face_index;
				out_result.m_hit_count = hit_count;
				out_result.m_tu = u;
				out_result.m_tv = v;

				ret_val = true;
			}
		}

		AUTO_RELEASE( all_hit_buffer );

		return ret_val;

	}

	//*
	bool d3d9_intersect_helper::intersect( void* d3d_mesh, ray<double> in_ray, intersect_result& out_result, int cull_mode )
	{
		if( d3d_mesh == NULL ) return false;
		bool ret_val = false;

		BOOL is_hit ;
		ulong face_index, hit_count;
		float u, v, v_distance;

		ID3DXBuffer* all_hit_buffer = NULL;
		D3DXVECTOR3 d3dvec_position( in_ray.ray_origin.x, in_ray.ray_origin.y, in_ray.ray_origin.z );
		D3DXVECTOR3 d3dvec_direction( in_ray.ray_direction.x, in_ray.ray_direction.y, in_ray.ray_direction.z );
		D3DXIntersect( (LPD3DXMESH)d3d_mesh, &d3dvec_position, &d3dvec_direction, &is_hit, &face_index, &u, &v, &v_distance, &all_hit_buffer, &hit_count);

		if( is_hit )
		{
			if( v_distance < out_result.m_distance )
			{
				vector3<double> v_face_normal;
				vector3<double> v_pos = calculate_intersect_point( d3d_mesh, face_index, u, v, v_face_normal );

				switch( cull_mode )
				{
					case 0: // none
						{
							out_result.m_distance = v_distance;
							out_result.m_position = v_pos;
							out_result.m_face_index = face_index;
							out_result.m_face_direction = vector3<double>::normalize( v_face_normal );
							ret_val = true;
						}
						break;
					case 1: // cw
						{
							if( vector3<double>::dot( in_ray.ray_direction, v_face_normal ) > 0 )
							{
								out_result.m_distance = v_distance;
								out_result.m_position = v_pos;
								out_result.m_face_index = face_index;
								out_result.m_face_direction = vector3<double>::normalize( v_face_normal );
								ret_val = true;
							}
						}
						break;
					case 2: //ccw
						{
							if( vector3<double>::dot( in_ray.ray_direction, v_face_normal ) < 0 )
							{
								out_result.m_distance = v_distance;
								out_result.m_position = v_pos;
								out_result.m_face_index = face_index;
								out_result.m_face_direction = vector3<double>::normalize( v_face_normal );
								ret_val = true;
							}
						}
						break;
				}
			}
		}

		AUTO_RELEASE( all_hit_buffer );

		return ret_val;
	}
	//*/

	bool d3d9_intersect_helper::get_intersect_info(void* d3d_mesh, ulong face_index, float u, float v, intersect_result& out_result )
	{
		LPD3DXMESH v_d3d_mesh = (LPD3DXMESH)d3d_mesh;

		LPDIRECT3DINDEXBUFFER9  v_d3d_index_buffer;
		LPDIRECT3DVERTEXBUFFER9 v_d3d_vertex_buffer;

		v_d3d_mesh->GetIndexBuffer( &v_d3d_index_buffer );
		v_d3d_mesh->GetVertexBuffer( &v_d3d_vertex_buffer );

		ulong v_face_number = v_d3d_mesh->GetNumFaces();
		ulong v_verts_number = v_d3d_mesh->GetNumVertices();

		D3DINDEXBUFFER_DESC index_desc;
		D3DVERTEXBUFFER_DESC vertex_desc;

		v_d3d_index_buffer->GetDesc(&index_desc);
		v_d3d_vertex_buffer->GetDesc(&vertex_desc);

		ulong v_index_size  = index_desc.Size  / v_face_number;
		//ulong v_vertex_size = vertex_desc.Size / v_verts_number;
		ulong v_vertex_size = v_d3d_mesh->GetNumBytesPerVertex();

		byte* v_indices = NULL;
		byte* v_vertices= NULL;

		v_d3d_index_buffer->Lock( 0, 0, (void**)&v_indices, 0 );
		v_d3d_vertex_buffer->Lock( 0, 0, (void**)&v_vertices, 0 );

		assert(v_indices);
		assert(v_vertices);

		if(index_desc.Format == D3DFMT_INDEX16)
		{
			ushort *p_v1_indx = (ushort*)(v_indices + v_index_size * face_index);
			ushort *p_v2_indx = (ushort*)(v_indices + v_index_size * face_index + 2);
			ushort *p_v3_indx = (ushort*)(v_indices + v_index_size * face_index + 4);

			float *p_vert = (float *)(v_vertices + (*p_v1_indx)*v_vertex_size); 
			vector3<float> v1(*p_vert,*(p_vert+1),*(p_vert+2));

			p_vert = (float *)(v_vertices + (*p_v2_indx)*v_vertex_size); 
			vector3<float> v2(*p_vert,*(p_vert+1),*(p_vert+2));

			p_vert = (float *)(v_vertices + (*p_v3_indx)*v_vertex_size); 
			vector3<float> v3(*p_vert,*(p_vert+1),*(p_vert+2));

			//V1 + U(V2 - V1) + V(V3 - V1).
			out_result.m_position.x = v1.x + u*(v2.x - v1.x) + v*(v3.x - v1.x);
			out_result.m_position.y = v1.y + u*(v2.y - v1.y) + v*(v3.y - v1.y);
			out_result.m_position.z = v1.z + u*(v2.z - v1.z) + v*(v3.z - v1.z);

			vector3<float> edge1 = v2 - v1;
			vector3<float> edge2 = v3 - v1;

			// Begin calculating determinant - also used to calculate U parameter
			out_result.m_face_direction = vector3<double>(((float*)&(edge1.cross(edge2))));
		}
		else if( index_desc.Format == D3DFMT_INDEX32 )
		{
			ulong* p_v1_indx = (ulong*)(v_indices + v_index_size * face_index);
			ulong* p_v2_indx = (ulong*)(v_indices + v_index_size * face_index + 4);
			ulong* p_v3_indx = (ulong*)(v_indices + v_index_size * face_index + 8);

			float *p_vert = (float *)(v_vertices + (*p_v1_indx)*v_vertex_size); 
			vector3<float> v1(*p_vert,*(p_vert+1),*(p_vert+2));

			p_vert = (float *)(v_vertices + (*p_v2_indx)*v_vertex_size); 
			vector3<float> v2(*p_vert,*(p_vert+1),*(p_vert+2));

			p_vert = (float *)(v_vertices + (*p_v3_indx)*v_vertex_size); 
			vector3<float> v3(*p_vert,*(p_vert+1),*(p_vert+2));

			//V1 + U(V2 - V1) + V(V3 - V1).
			out_result.m_position.x = v1.x + u*(v2.x - v1.x) + v*(v3.x - v1.x);
			out_result.m_position.y = v1.y + u*(v2.y - v1.y) + v*(v3.y - v1.y);
			out_result.m_position.z = v1.z + u*(v2.z - v1.z) + v*(v3.z - v1.z);

			vector3<float> edge1 = v2 - v1;
			vector3<float> edge2 = v3 - v1;

			// Begin calculating determinant - also used to calculate U parameter
			out_result.m_face_direction = vector3<double>(((float*)&(edge1.cross(edge2))));
		}

		v_d3d_index_buffer->Unlock();
		v_d3d_vertex_buffer->Unlock();

		AUTO_RELEASE(v_d3d_index_buffer);
		AUTO_RELEASE(v_d3d_vertex_buffer);

		return true;

	}

	bool d3d9_intersect_helper::get_material_index( void* d3d_mesh, ulong face_index, ulong& mat_index )
	{
		bool ret_val = false;
		if(d3d_mesh == NULL ) return ret_val;
		LPD3DXMESH v_d3d_mesh = (LPD3DXMESH)d3d_mesh;

		DWORD* v_attribute = NULL;
		v_d3d_mesh->LockAttributeBuffer(0, &v_attribute);

		mat_index = 0;

		if(v_attribute)
		{
			mat_index = v_attribute[face_index];		

			ret_val = true;
		}
		v_d3d_mesh->UnlockAttributeBuffer();

		return ret_val;
	}

	//bool d3d9_intersect_helper::select_surface(void* d3d_mesh, ulong face_index, surface_object& v_surface )
	//{
	//	if(d3d_mesh == NULL ) return false;
	//	LPD3DXMESH v_d3d_mesh = (LPD3DXMESH)d3d_mesh;
	//	ulong v_face_number = v_d3d_mesh->GetNumFaces();
	//	if(face_index >= v_face_number) return false;
	//	v_surface.m_material_index = 0;
	//	DWORD* v_attribute = NULL;
	//	v_d3d_mesh->LockAttributeBuffer(0, &v_attribute);
	//	if(v_attribute)
	//	{
	//		v_surface.m_material_index = v_attribute[face_index];
	//	}

	//	for( int ni = 0; ni < v_face_number; ni++)
	//	{
	//		if(v_attribute[ni] == v_surface.m_material_index)
	//		{
	//			v_surface.m_face_index_array.push_back(ni);
	//		}
	//	}

	//	LPDIRECT3DINDEXBUFFER9  v_d3d_index_buffer;
	//	LPDIRECT3DVERTEXBUFFER9 v_d3d_vertex_buffer;
	//	v_d3d_mesh->GetIndexBuffer( &v_d3d_index_buffer );
	//	v_d3d_mesh->GetVertexBuffer( &v_d3d_vertex_buffer );

	//	ulong v_total_face_number = v_d3d_mesh->GetNumFaces();
	//	ulong v_verts_number = v_d3d_mesh->GetNumVertices();

	//	D3DINDEXBUFFER_DESC index_desc;
	//	D3DVERTEXBUFFER_DESC vertex_desc;

	//	v_d3d_index_buffer->GetDesc(&index_desc);
	//	v_d3d_vertex_buffer->GetDesc(&vertex_desc);

	//	ulong v_index_size  = index_desc.Size  / v_total_face_number;
	//	ulong v_vertex_size = vertex_desc.Size / v_verts_number;

	//	byte* v_indices = NULL;
	//	byte* v_vertices= NULL;

	//	v_d3d_index_buffer->Lock( 0, 0, (void**)&v_indices, 0 );
	//	v_d3d_vertex_buffer->Lock( 0, 0, (void**)&v_vertices, 0 );
	//	
	//	//std::map<ulong, ulong> v_order_index_map;

	//	v_face_number = v_surface.m_face_index_array.size();

	//	vector3<float> v_center(0,0,0);
	//	if(index_desc.Format == D3DFMT_INDEX16)
	//	{
	//		for(int ni = 0; ni < v_face_number; ni++ )
	//		{
	//			int face_index = v_surface.m_face_index_array[ni];
	//			ushort *p_v1_indx = (ushort*)(v_indices + v_index_size * face_index);
	//			ushort *p_v2_indx = (ushort*)(v_indices + v_index_size * face_index + 2);
	//			ushort *p_v3_indx = (ushort*)(v_indices + v_index_size * face_index + 4);

	//			v_surface.m_compact_index_map[*p_v1_indx] = *p_v1_indx;
	//			v_surface.m_compact_index_map[*p_v2_indx] = *p_v2_indx;
	//			v_surface.m_compact_index_map[*p_v3_indx] = *p_v3_indx;
	//		}
	//	}
	//	else
	//	{
	//		for(int ni = 0; ni < v_face_number; ni++ )
	//		{
	//			int face_index = v_surface.m_face_index_array[ni];
	//			ulong* p_v1_indx = (ulong*)(v_indices + v_index_size * face_index);
	//			ulong* p_v2_indx = (ulong*)(v_indices + v_index_size * face_index + 4);
	//			ulong* p_v3_indx = (ulong*)(v_indices + v_index_size * face_index + 8);

	//			v_surface.m_compact_index_map[*p_v1_indx] = *p_v1_indx;
	//			v_surface.m_compact_index_map[*p_v2_indx] = *p_v2_indx;
	//			v_surface.m_compact_index_map[*p_v3_indx] = *p_v3_indx;
	//		}
	//	}
	//	
	//	std::map<ulong, ulong> ::iterator itr = v_surface.m_compact_index_map.begin();
	//	for( ; itr != v_surface.m_compact_index_map.end(); itr++ )
	//	{
	//		float *p_vert = (float *)(v_vertices + (itr->second)*v_vertex_size); 
	//		vector3<float> v1(*p_vert,*(p_vert+1),*(p_vert+2));
	//		v_center += v1;
	//	}
	//	v_center /= ((float)(v_surface.m_compact_index_map.size()));
	//	v_surface.m_center = v_center;

	//	v_d3d_mesh->UnlockAttributeBuffer();
	//	AUTO_RELEASE(v_d3d_index_buffer);
	//	AUTO_RELEASE(v_d3d_vertex_buffer);
	//	return true;
	//}

	/*
	bool d3d9_intersect_helper::intersect( void* d3d_mesh, ray<double> in_ray, intersect_result& out_result )
	{
		if( d3d_mesh == NULL ) return false;
		bool ret_val = false;

		BOOL is_hit ;
		ulong face_index, hit_count;
		float u, v, v_distance;

		//LPD3DXMESH pMesh;
		//LPD3DXMESH v_d3d_mesh = (LPD3DXMESH)d3d_mesh;
		//v_d3d_mesh->CloneMeshFVF(D3DXMESH_MANAGED, v_d3d_mesh->GetFVF(), v_d3d_mesh->GetDevice(), &pMesh);

		LPD3DXMESH v_d3d_mesh = (LPD3DXMESH)d3d_mesh;

		LPDIRECT3DINDEXBUFFER9  v_d3d_index_buffer;
		LPDIRECT3DVERTEXBUFFER9 v_d3d_vertex_buffer;

		v_d3d_mesh->GetIndexBuffer( &v_d3d_index_buffer );
		v_d3d_mesh->GetVertexBuffer( &v_d3d_vertex_buffer );

		ulong v_face_number = v_d3d_mesh->GetNumFaces();
		ulong v_verts_number = v_d3d_mesh->GetNumVertices();

		D3DINDEXBUFFER_DESC index_desc;
		D3DVERTEXBUFFER_DESC vertex_desc;

		v_d3d_index_buffer->GetDesc(&index_desc);
		v_d3d_vertex_buffer->GetDesc(&vertex_desc);

		ulong v_index_size  = index_desc.Size  / v_face_number;
		ulong v_vertex_size = vertex_desc.Size / v_verts_number;

		byte* v_indices = NULL;
		byte* v_vertices= NULL;

		v_d3d_index_buffer->Lock( 0, 0, (void**)&v_indices, 0 );
		v_d3d_vertex_buffer->Lock( 0, 0, (void**)&v_vertices, 0 );

		// Not using D3DX
		DWORD dwNumFaces = v_d3d_mesh->GetNumFaces();
		FLOAT fDist;

		DWORD v_dwNumIntersections = 0;

		for( DWORD i=0; i<dwNumFaces; i++ )
		{
			vector3<float> v0 = ((vector3<float>*)v_vertices)[v_indices[3*i+0]];
			vector3<float> v1 = ((vector3<float>*)v_vertices)[v_indices[3*i+1]];
			vector3<float> v2 = ((vector3<float>*)v_vertices)[v_indices[3*i+2]];

			 //vector3<float> v_ray_origin((double*)&in_ray.ray_origin.);

			// Check if the pick ray passes through this point
			if( intersect_triangle( vector3<float>((double*)&in_ray.ray_origin), vector3<float>((double*)&in_ray.ray_direction), v0, v1, v2, &fDist, &u, &v ) )
			{
				if( fDist < out_result.m_distance )
				{
					out_result.m_face_index = i;
					out_result.m_distance = fDist;
					out_result.m_tu = u;
					out_result.m_tv = v;

					ushort *p_v1_indx = (ushort*)(v_indices + v_index_size * i);
					ushort *p_v2_indx = (ushort*)(v_indices + v_index_size * i + 2);
					ushort *p_v3_indx = (ushort*)(v_indices + v_index_size * i + 4);

					float *p_vert = (float *)(v_vertices + (*p_v1_indx)*v_vertex_size); 
					vector3<float> v1(*p_vert,*(p_vert+1),*(p_vert+2));

					p_vert = (float *)(v_vertices + (*p_v2_indx)*v_vertex_size); 
					vector3<float> v2(*p_vert,*(p_vert+1),*(p_vert+2));

					p_vert = (float *)(v_vertices + (*p_v3_indx)*v_vertex_size); 
					vector3<float> v3(*p_vert,*(p_vert+1),*(p_vert+2));

					//V1 + U(V2 - V1) + V(V3 - V1).
					out_result.m_position.x = v1.x + u*(v2.x - v1.x) + v*(v3.x - v1.x);
					out_result.m_position.y = v1.y + u*(v2.y - v1.y) + v*(v3.y - v1.y);
					out_result.m_position.z = v1.z + u*(v2.z - v1.z) + v*(v3.z - v1.z);

					v_dwNumIntersections++;
					ret_val = true;
				}
			}
		}

		//if( is_hit )
		//{
		//	if( v_distance < out_result.m_distance )
		//	{
		//		out_result.m_distance = v_distance;
		//		out_result.m_position = calculate_intersect_point( d3d_mesh, face_index, u, v);
		//		ret_val = true;
		//	}
		//}

		return ret_val;
	}
	//*/

	// reference: \\Microsoft DircetX SDK(December 2005)\Sample\Pick\Pick.cpp( line 689, function Pick() )
	vector3<double> d3d9_intersect_helper::calculate_intersect_point(void* d3d_mesh, ulong face_index, float u, float v )
	{
		LPD3DXMESH v_d3d_mesh = (LPD3DXMESH)d3d_mesh;

		LPDIRECT3DINDEXBUFFER9  v_d3d_index_buffer;
		LPDIRECT3DVERTEXBUFFER9 v_d3d_vertex_buffer;

		v_d3d_mesh->GetIndexBuffer( &v_d3d_index_buffer );
		v_d3d_mesh->GetVertexBuffer( &v_d3d_vertex_buffer );

		ulong v_face_number = v_d3d_mesh->GetNumFaces();
		ulong v_verts_number = v_d3d_mesh->GetNumVertices();

		D3DINDEXBUFFER_DESC index_desc;
		D3DVERTEXBUFFER_DESC vertex_desc;

		v_d3d_index_buffer->GetDesc(&index_desc);
		v_d3d_vertex_buffer->GetDesc(&vertex_desc);

		ulong v_index_size  = index_desc.Size  / v_face_number;
		//ulong v_vertex_size = vertex_desc.Size / v_verts_number;
		ulong v_vertex_size = v_d3d_mesh->GetNumBytesPerVertex();

		byte* v_indices = NULL;
		byte* v_vertices= NULL;

		v_d3d_index_buffer->Lock( 0, 0, (void**)&v_indices, 0 );
		v_d3d_vertex_buffer->Lock( 0, 0, (void**)&v_vertices, 0 );

		assert(v_indices);
		assert(v_vertices);

		vector3<double> intersect_vertex;

		if(index_desc.Format == D3DFMT_INDEX16)
		{
			ushort *p_v1_indx = (ushort*)(v_indices + v_index_size * face_index);
			ushort *p_v2_indx = (ushort*)(v_indices + v_index_size * face_index + 2);
			ushort *p_v3_indx = (ushort*)(v_indices + v_index_size * face_index + 4);

			float *p_vert = (float *)(v_vertices + (*p_v1_indx)*v_vertex_size); 
			vector3<float> v1(*p_vert,*(p_vert+1),*(p_vert+2));

			p_vert = (float *)(v_vertices + (*p_v2_indx)*v_vertex_size); 
			vector3<float> v2(*p_vert,*(p_vert+1),*(p_vert+2));

			p_vert = (float *)(v_vertices + (*p_v3_indx)*v_vertex_size); 
			vector3<float> v3(*p_vert,*(p_vert+1),*(p_vert+2));

			//V1 + U(V2 - V1) + V(V3 - V1).
			intersect_vertex.x = v1.x + u*(v2.x - v1.x) + v*(v3.x - v1.x);
			intersect_vertex.y = v1.y + u*(v2.y - v1.y) + v*(v3.y - v1.y);
			intersect_vertex.z = v1.z + u*(v2.z - v1.z) + v*(v3.z - v1.z);
		}
		else if( index_desc.Format == D3DFMT_INDEX32 )
		{
			ulong* p_v1_indx = (ulong*)(v_indices + v_index_size * face_index);
			ulong* p_v2_indx = (ulong*)(v_indices + v_index_size * face_index + 4);
			ulong* p_v3_indx = (ulong*)(v_indices + v_index_size * face_index + 8);

			float *p_vert = (float *)(v_vertices + (*p_v1_indx)*v_vertex_size); 
			vector3<float> v1(*p_vert,*(p_vert+1),*(p_vert+2));

			p_vert = (float *)(v_vertices + (*p_v2_indx)*v_vertex_size); 
			vector3<float> v2(*p_vert,*(p_vert+1),*(p_vert+2));

			p_vert = (float *)(v_vertices + (*p_v3_indx)*v_vertex_size); 
			vector3<float> v3(*p_vert,*(p_vert+1),*(p_vert+2));

			//V1 + U(V2 - V1) + V(V3 - V1).
			intersect_vertex.x = v1.x + u*(v2.x - v1.x) + v*(v3.x - v1.x);
			intersect_vertex.y = v1.y + u*(v2.y - v1.y) + v*(v3.y - v1.y);
			intersect_vertex.z = v1.z + u*(v2.z - v1.z) + v*(v3.z - v1.z);

		}

		v_d3d_index_buffer->Unlock();
		v_d3d_vertex_buffer->Unlock();

		AUTO_RELEASE(v_d3d_index_buffer);
		AUTO_RELEASE(v_d3d_vertex_buffer);

		return intersect_vertex;

	}

	vector3<double> d3d9_intersect_helper::calculate_intersect_point(void* d3d_mesh, ulong face_index, float u, float v, vector3<double>& in_face_normal )
	{
		LPD3DXMESH v_d3d_mesh = (LPD3DXMESH)d3d_mesh;

		LPDIRECT3DINDEXBUFFER9  v_d3d_index_buffer;
		LPDIRECT3DVERTEXBUFFER9 v_d3d_vertex_buffer;

		v_d3d_mesh->GetIndexBuffer( &v_d3d_index_buffer );
		v_d3d_mesh->GetVertexBuffer( &v_d3d_vertex_buffer );
		
		ulong v_face_number = v_d3d_mesh->GetNumFaces();
		ulong v_verts_number = v_d3d_mesh->GetNumVertices();

		D3DINDEXBUFFER_DESC index_desc;
		D3DVERTEXBUFFER_DESC vertex_desc;

		v_d3d_index_buffer->GetDesc(&index_desc);
		v_d3d_vertex_buffer->GetDesc(&vertex_desc);

		ulong v_index_size  = index_desc.Size  / v_face_number;
		//ulong v_vertex_size = vertex_desc.Size / v_verts_number;
		ulong v_vertex_size = v_d3d_mesh->GetNumBytesPerVertex();

		byte* v_indices = NULL;
		byte* v_vertices= NULL;

		v_d3d_index_buffer->Lock( 0, 0, (void**)&v_indices, 0 );
		v_d3d_vertex_buffer->Lock( 0, 0, (void**)&v_vertices, 0 );

		assert(v_indices);
		assert(v_vertices);

		vector3<double> intersect_vertex;

		if(index_desc.Format == D3DFMT_INDEX16)
		{
			ushort *p_v1_indx = (ushort*)(v_indices + v_index_size * face_index);
			ushort *p_v2_indx = (ushort*)(v_indices + v_index_size * face_index + 2);
			ushort *p_v3_indx = (ushort*)(v_indices + v_index_size * face_index + 4);

			float *p_vert = (float *)(v_vertices + (*p_v1_indx)*v_vertex_size); 
			vector3<float> v1(*p_vert,*(p_vert+1),*(p_vert+2));

			p_vert = (float *)(v_vertices + (*p_v2_indx)*v_vertex_size); 
			vector3<float> v2(*p_vert,*(p_vert+1),*(p_vert+2));

			p_vert = (float *)(v_vertices + (*p_v3_indx)*v_vertex_size); 
			vector3<float> v3(*p_vert,*(p_vert+1),*(p_vert+2));

			//V1 + U(V2 - V1) + V(V3 - V1).
			intersect_vertex.x = v1.x + u*(v2.x - v1.x) + v*(v3.x - v1.x);
			intersect_vertex.y = v1.y + u*(v2.y - v1.y) + v*(v3.y - v1.y);
			intersect_vertex.z = v1.z + u*(v2.z - v1.z) + v*(v3.z - v1.z);

			vector3<float> edge1 = v2 - v1;
			vector3<float> edge2 = v3 - v1;

			// Begin calculating determinant - also used to calculate U parameter
			in_face_normal = vector3<double>(((float*)&(edge1.cross(edge2))));
		}
		else if( index_desc.Format == D3DFMT_INDEX32 )
		{
			ulong* p_v1_indx = (ulong*)(v_indices + v_index_size * face_index);
			ulong* p_v2_indx = (ulong*)(v_indices + v_index_size * face_index + 4);
			ulong* p_v3_indx = (ulong*)(v_indices + v_index_size * face_index + 8);

			float *p_vert = (float *)(v_vertices + (*p_v1_indx)*v_vertex_size); 
			vector3<float> v1(*p_vert,*(p_vert+1),*(p_vert+2));

			p_vert = (float *)(v_vertices + (*p_v2_indx)*v_vertex_size); 
			vector3<float> v2(*p_vert,*(p_vert+1),*(p_vert+2));

			p_vert = (float *)(v_vertices + (*p_v3_indx)*v_vertex_size); 
			vector3<float> v3(*p_vert,*(p_vert+1),*(p_vert+2));

			//V1 + U(V2 - V1) + V(V3 - V1).
			intersect_vertex.x = v1.x + u*(v2.x - v1.x) + v*(v3.x - v1.x);
			intersect_vertex.y = v1.y + u*(v2.y - v1.y) + v*(v3.y - v1.y);
			intersect_vertex.z = v1.z + u*(v2.z - v1.z) + v*(v3.z - v1.z);

			vector3<float> edge1 = v2 - v1;
			vector3<float> edge2 = v3 - v1;

			// Begin calculating determinant - also used to calculate U parameter
			in_face_normal = vector3<double>(((float*)&(edge1.cross(edge2))));
		}

		v_d3d_index_buffer->Unlock();
		v_d3d_vertex_buffer->Unlock();

		AUTO_RELEASE(v_d3d_index_buffer);
		AUTO_RELEASE(v_d3d_vertex_buffer);
	
		return intersect_vertex;
	}


	bool d3d9_intersect_helper::intersect_triangle( vector3<float>& orig, vector3<float>& dir, 
													vector3<float>& v0, vector3<float>& v1, vector3<float>& v2, 
													float* t, float* u, float* v )
	{
		// Find vectors for two edges sharing vert0
		vector3<float> edge1 = v1 - v0;
		vector3<float> edge2 = v2 - v0;

		// Begin calculating determinant - also used to calculate U parameter
		vector3<float> pvec;
		D3DXVec3Cross( (D3DXVECTOR3*)&pvec, (D3DXVECTOR3*)&dir, (D3DXVECTOR3*)&edge2 );

		// If determinant is near zero, ray lies in plane of triangle
		float det = D3DXVec3Dot( (D3DXVECTOR3*)&edge1, (D3DXVECTOR3*)&pvec );

		vector3<float> tvec;
		if( det > 0 )
		{
			tvec = orig - v0;
		}
		else
		{
			tvec = v0 - orig;
			det = -det;
		}

		if( det < 0.0001f )
			return FALSE;

		// Calculate U parameter and test bounds
		*u = D3DXVec3Dot( (D3DXVECTOR3*)&tvec, (D3DXVECTOR3*)&pvec );
		if( *u < 0.0f || *u > det )
			return FALSE;

		// Prepare to test V parameter
		vector3<float> qvec;
		D3DXVec3Cross( (D3DXVECTOR3*)&qvec, (D3DXVECTOR3*)&tvec, (D3DXVECTOR3*)&edge1 );

		// Calculate V parameter and test bounds
		*v = D3DXVec3Dot( (D3DXVECTOR3*)&dir, (D3DXVECTOR3*)&qvec );
		if( *v < 0.0f || *u + *v > det )
			return FALSE;

		// Calculate t, scale parameters, ray intersects triangle
		*t = D3DXVec3Dot( (D3DXVECTOR3*)&edge2, (D3DXVECTOR3*)&qvec );
		float fInvDet = 1.0f / det;
		*t *= fInvDet;
		*u *= fInvDet;
		*v *= fInvDet;

		return TRUE;
	}

	double d3d9_intersect_helper::point_to_segment_distance( vector_3f& p, vector_3f& a, vector_3f& b )
	{
		double l = (a-b).length();
		if ( l < math_d::s_zero_tolerance )
		{
			return (p - a).length();
		}


		double r = ( (a.y-p.y) * (a.y-b.y) - (a.x-p.x) * (b.x-a.x) ) / ( l * l );

		if ( r > 1.0 )
		{
			return min( (p-b).length(), (p-a).length() );
		}

		if ( r < -math_d::s_zero_tolerance )
		{
			return min( (p-b).length(), (p-a).length() );
		}

		double s = ( (a.y-p.y) * (b.x-a.x) - (a.x-p.x) * (b.y-a.y) ) / ( l * l );

		return fabs( s * l );
	}

	double d3d9_intersect_helper::point_to_segment_distance_d( vector_3d& p, vector_3d& a, vector_3d& b )
	{
		double l = (a-b).length();
		if ( l < math_d::s_zero_tolerance )
		{
			return (p - a).length();
		}


		double r = ( (a.y-p.y) * (a.y-b.y) - (a.x-p.x) * (b.x-a.x) ) / ( l * l );

		if ( r > 1.0 )
		{
			return min( (p-b).length(), (p-a).length() );
		}

		if ( r < -math_d::s_zero_tolerance )
		{
			return min( (p-b).length(), (p-a).length() );
		}

		double s = ( (a.y-p.y) * (b.x-a.x) - (a.x-p.x) * (b.y-a.y) ) / ( l * l );

		return fabs( s * l );
	}

}