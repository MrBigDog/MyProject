///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_software_raster.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : software_raster class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_software_raster.h"

// fast fabs
float fast_fabs( float in_val ) 
{
	int ret_val = * ( int* )&in_val;
	ret_val &= 0x7FFFFFFF;
	return *( float* ) &ret_val;
}

namespace uniscope_globe
{
	software_raster::software_raster( void )
	{
		clear();
	}

	software_raster::~software_raster( void )
	{

	}

	void software_raster::set_world_matrix( matrix4<float>& mat )
	{
		m_world_matrix = mat;
		update_matrix();
	}

	void software_raster::set_view_matrix( matrix4<float>& mat )
	{
		m_view_matrix = mat;
		update_matrix();
	}

	void software_raster::set_projection_matrix( matrix4<float>& mat )
	{
		m_projection_matrix = mat;
		update_matrix();
	}

	void software_raster::update_matrix( void )
	{
		m_all_matrix = m_world_matrix * m_view_matrix * m_projection_matrix;
	}

	void software_raster::begin( void )
	{
		// not implement
	}

	void software_raster::end( void )
	{
		// not implement
	}

	void software_raster::clear( void )
	{
		m_query_result.set_default();

		m_is_quary = false;

		m_all_matrix.zero();
		m_world_matrix.zero();
		m_view_matrix.zero();
		m_projection_matrix.zero();
	}

	bool software_raster::get_query( query_result& in_result )
	{
		in_result = m_query_result;
		return m_is_quary;
	}

	bool software_raster::raster( vector3<float>* face_vertex1, vector3<float>* face_vertex2, vector3<float>* face_vertex3 )
	{
		static D3DXVECTOR3  v_vertex3_rhw[3];
		static D3DXVECTOR4	v_vertex4_rhw[3];

		D3DXVec4Transform((D3DXVECTOR4*)(v_vertex4_rhw+0), &D3DXVECTOR4(face_vertex1->x, face_vertex1->y, face_vertex1->z, 1.0f), (D3DXMATRIX*)&m_all_matrix);
		D3DXVec4Transform((D3DXVECTOR4*)(v_vertex4_rhw+1), &D3DXVECTOR4(face_vertex2->x, face_vertex2->y, face_vertex2->z, 1.0f), (D3DXMATRIX*)&m_all_matrix);
		D3DXVec4Transform((D3DXVECTOR4*)(v_vertex4_rhw+2), &D3DXVECTOR4(face_vertex3->x, face_vertex3->y, face_vertex3->z, 1.0f), (D3DXMATRIX*)&m_all_matrix);

		if( !v_vertex4_rhw[0].w || !v_vertex4_rhw[1].w || !v_vertex4_rhw[2].w ) return false;
		v_vertex3_rhw[0] = D3DXVECTOR3(v_vertex4_rhw[0].x/v_vertex4_rhw[0].w,v_vertex4_rhw[0].y/v_vertex4_rhw[0].w,v_vertex4_rhw[0].z/v_vertex4_rhw[0].w);
		v_vertex3_rhw[1] = D3DXVECTOR3(v_vertex4_rhw[1].x/v_vertex4_rhw[1].w,v_vertex4_rhw[1].y/v_vertex4_rhw[1].w,v_vertex4_rhw[1].z/v_vertex4_rhw[1].w);
		v_vertex3_rhw[2] = D3DXVECTOR3(v_vertex4_rhw[2].x/v_vertex4_rhw[2].w,v_vertex4_rhw[2].y/v_vertex4_rhw[2].w,v_vertex4_rhw[2].z/v_vertex4_rhw[2].w);

		v_vertex3_rhw[0].z = (v_vertex3_rhw[0].z + 1) * 0.5;
		v_vertex3_rhw[1].z = (v_vertex3_rhw[1].z + 1) * 0.5;
		v_vertex3_rhw[2].z = (v_vertex3_rhw[2].z + 1) * 0.5;

		ulong result1 =	FAST_SIGN_I(((FAST_SIGN_F(v_vertex4_rhw[0].x)^FAST_SIGN_F(v_vertex4_rhw[1].x)))|FAST_SIGN_I((FAST_SIGN_F(v_vertex4_rhw[0].x)^FAST_SIGN_F(v_vertex4_rhw[2].x))));
		ulong result2 =	FAST_SIGN_I(((FAST_SIGN_F(v_vertex4_rhw[0].y)^FAST_SIGN_F(v_vertex4_rhw[1].y)))|FAST_SIGN_I((FAST_SIGN_F(v_vertex4_rhw[0].y)^FAST_SIGN_F(v_vertex4_rhw[2].y))));
		if(result1 == 0 && (FAST_ULONG_F(v_vertex4_rhw[0].x)&FAST_ULONG_F(v_vertex4_rhw[1].x)&FAST_ULONG_F(v_vertex4_rhw[2].x))) return false;
		if(result2 == 0 && (FAST_ULONG_F(v_vertex4_rhw[0].y)&FAST_ULONG_F(v_vertex4_rhw[1].y)&FAST_ULONG_F(v_vertex4_rhw[2].y))) return false;

		if((((-v_vertex3_rhw[1].y)*(v_vertex3_rhw[2].x-v_vertex3_rhw[1].x)-(v_vertex3_rhw[2].y-v_vertex3_rhw[1].y)*(-v_vertex3_rhw[1].x))*
			((v_vertex3_rhw[0].y-v_vertex3_rhw[1].y)*(v_vertex3_rhw[2].x-v_vertex3_rhw[1].x)-(v_vertex3_rhw[2].y-v_vertex3_rhw[1].y)*(v_vertex3_rhw[0].x-v_vertex3_rhw[1].x)))*v_vertex4_rhw[0].w<0) return false;
		if((((-v_vertex3_rhw[0].y)*(v_vertex3_rhw[2].x-v_vertex3_rhw[0].x)-(v_vertex3_rhw[2].y-v_vertex3_rhw[0].y)*(-v_vertex3_rhw[0].x))*
			((v_vertex3_rhw[1].y-v_vertex3_rhw[0].y)*(v_vertex3_rhw[2].x-v_vertex3_rhw[0].x)-(v_vertex3_rhw[2].y-v_vertex3_rhw[0].y)*(v_vertex3_rhw[1].x-v_vertex3_rhw[0].x)))*v_vertex4_rhw[1].w<0) return false;
		if((((-v_vertex3_rhw[0].y)*(v_vertex3_rhw[1].x-v_vertex3_rhw[0].x)-(v_vertex3_rhw[1].y-v_vertex3_rhw[0].y)*(-v_vertex3_rhw[0].x))*
			((v_vertex3_rhw[2].y-v_vertex3_rhw[0].y)*(v_vertex3_rhw[1].x-v_vertex3_rhw[0].x)-(v_vertex3_rhw[1].y-v_vertex3_rhw[0].y)*(v_vertex3_rhw[2].x-v_vertex3_rhw[0].x)))*v_vertex4_rhw[2].w<0) return false;

		float v_xy =(v_vertex3_rhw[1].x - v_vertex3_rhw[0].x) * (v_vertex3_rhw[2].y - v_vertex3_rhw[0].y) - (v_vertex3_rhw[1].y - v_vertex3_rhw[0].y) * (v_vertex3_rhw[2].x - v_vertex3_rhw[0].x);
		if( v_xy == 0 ) return false;

		float v_xz =(v_vertex3_rhw[1].x - v_vertex3_rhw[0].x) * (v_vertex3_rhw[2].z - v_vertex3_rhw[0].z) - (v_vertex3_rhw[1].z - v_vertex3_rhw[0].z) * (v_vertex3_rhw[2].x - v_vertex3_rhw[0].x);
		float v_yz =(v_vertex3_rhw[1].y - v_vertex3_rhw[0].y) * (v_vertex3_rhw[2].z - v_vertex3_rhw[0].z) - (v_vertex3_rhw[1].z - v_vertex3_rhw[0].z) * (v_vertex3_rhw[2].y - v_vertex3_rhw[0].y);
		
		float v_depth = ( ( ( -v_vertex3_rhw[0].y ) * v_xz - ( -v_vertex3_rhw[0].x ) * v_yz) / v_xy + v_vertex3_rhw[0].z );
		if( v_depth >= m_query_result.m_depth || v_depth < 0.f ) return false;

		m_query_result.m_face_vertex[0] = (*face_vertex1) * m_world_matrix;
		m_query_result.m_face_vertex[1] = (*face_vertex2) * m_world_matrix;
		m_query_result.m_face_vertex[2] = (*face_vertex3) * m_world_matrix;

		//matrix4<double> v_world_matrix( m_world_matrix.m );
		//vector3<double> v_temp_vector1( (float*)face_vertex1 );
		//vector3<double> v_temp_vector2( (float*)face_vertex2 );
		//vector3<double> v_temp_vector3( (float*)face_vertex3 );
		//m_query_result.m_face_vertex[0] = (v_temp_vector1) * v_world_matrix;
		//m_query_result.m_face_vertex[1] = (v_temp_vector2) * v_world_matrix;
		//m_query_result.m_face_vertex[2] = (v_temp_vector3) * v_world_matrix;

		m_query_result.m_depth = v_depth;
		m_is_quary = true;
		return true;
	}
}
