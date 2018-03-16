///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_software_device_impl.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : software_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_software_device_impl.h"

namespace uniscope_globe
{
	software_device_impl::software_device_impl( HWND hwnd )
	{
		m_hwnd = hwnd;

		m_world_matrx.identity();
		m_view_matrix.identity();
		m_project_matrix.identity();
		m_clip_matrix.identity();

		m_batch_count = 0;
		m_render_count = 0;
		m_is_windowed = true;

		create();
	}

	software_device_impl::~software_device_impl( void )
	{

	}

	bool software_device_impl::create( void )
	{
		// not implement
		return true;
	}

	void software_device_impl::destroy( void )
	{
		// not implement
	}

	void software_device_impl::reset( void )
	{
		m_world_matrx.identity();
		m_view_matrix.identity();
		m_project_matrix.identity();
		m_clip_matrix.identity();

		m_batch_count = 0;
		m_render_count = 0;
		m_is_windowed = true;

		m_raster_device.clear();
	}

	void software_device_impl::set_clip_rect( rect& clip_rect )
	{
		float x,y,w,h;
		x = (float)clip_rect.m_left / m_view_port.m_width;
		y = (float)clip_rect.m_top /  m_view_port.m_height;
		w = (float)(clip_rect.get_width()) / m_view_port.m_width;
		h = (float)(clip_rect.get_height()) / m_view_port.m_height;
		h = w;

		m_clip_matrix.m11 =  2.0f / ( 2 * x - 1) ;
		m_clip_matrix.m41 = -1.0f - ( 2 * x - 1) / w;
		m_clip_matrix.m22 =  1.0f / h;
		m_clip_matrix.m42 = -1.0f - (1 - 2 * y - h * 2) / h ;

	}

	bool software_device_impl::get_query_result( query_result& q_result)
	{
		return  m_raster_device.get_query(q_result);
	}

	void software_device_impl::on_resize_device( uint device_width, uint device_height )
	{

	}

	void software_device_impl::on_lost_device( void )
	{ 
		
	}

	void software_device_impl::on_reset_device( void )
	{

	}

	void software_device_impl::begin_frame( void )
	{
//		reset();
	}

	void software_device_impl::end_frame( void )
	{

	}

	void software_device_impl::clear( void )
	{
		m_raster_device.clear();
	}

	void software_device_impl::clear_target( void )
	{
		//m_raster_device.clear_target();
	}

	void software_device_impl::clear_zbuffer( void )
	{
		//m_raster_device.clear_zbuffer();
	}

	void software_device_impl::clear_stencil( void )
	{

	}

	vector3<float> software_device_impl::projection( vector3<float>& vector_in_world )
	{
		vector3<float> ret_vec;
		D3DXVec3Project((D3DXVECTOR3*)&ret_vec, 
			(D3DXVECTOR3*)&vector_in_world, 
			(D3DVIEWPORT9*)&m_view_port,
			(D3DXMATRIX*)&m_project_matrix, 
			(D3DXMATRIX*)&m_view_matrix, 
			(D3DXMATRIX*)&matrix4<float>::s_identity);
		return ret_vec;

	}

	vector3<float> software_device_impl::unprojection( vector3<float>& vector_on_viewport )
	{
		vector3<float> ret_vec;
		D3DXVec3Unproject((D3DXVECTOR3*)&ret_vec, 
			(D3DXVECTOR3*)&vector_on_viewport, 
			(D3DVIEWPORT9*)&m_view_port,
			(D3DXMATRIX*)&m_project_matrix, 
			(D3DXMATRIX*)&m_view_matrix, 
			(D3DXMATRIX*)&matrix4<float>::s_identity);
		return ret_vec;
	}

	void software_device_impl::set_clipping_plane( ulong index, float* in_plane )
	{

	}

	void software_device_impl::set_view_port( view_port& viewport)
	{
		m_view_port = viewport;
	}

	view_port software_device_impl::get_view_port( void ) const
	{
		return m_view_port;
	}

	render_transform* software_device_impl::get_matrix_stack( void )
	{
		// not implement;
		return NULL;
	}

	render_state* software_device_impl::get_render_state( void )
	{
		// not implement;
		return NULL;
	}

	texture_stage* software_device_impl::get_texture_stage( ulong texture_stage_index )
	{
		// not implement;
		return NULL;
	}

	ULONG_PTR software_device_impl::get_device(void)
	{
		return 0;
	}

	ULONG_PTR software_device_impl::get_sprite( void )
	{
		return 0;
	}

	ULONG_PTR software_device_impl::get_font( void )
	{
		return 0;
	}

	ULONG_PTR software_device_impl::get_vertex_blend_effect( void )
	{
		return 0;
	}


	ULONG_PTR software_device_impl::get_line(void)
	{
		return 0;
	}

	void software_device_impl::set_culling_mode( culling_mode mode )
	{
		
	}

	culling_mode software_device_impl::get_culling_mode(void) const
	{
		return (culling_mode)0;
	}

	// Sets the world transform matrix. 
	void software_device_impl::set_world_matrix( const matrix4<float>& mat )
	{
		m_world_matrx = mat;
		m_raster_device.set_world_matrix( m_world_matrx );
	}

	// Get the world transform matrix.
	matrix4<float> software_device_impl::get_world_matrix(void)
	{
		return m_world_matrx;
	}

	// Sets multiple world matrices (vertex blending).
	void software_device_impl::set_world_matrices( const matrix4<float>* mats, ushort count )
	{
		// not implement
	}

	// Sets the view transform matrix.
	void software_device_impl::set_view_matrix( const matrix4<float>& mat )
	{
		m_view_matrix = mat;
		m_raster_device.set_view_matrix( m_view_matrix );
	}

	// Get the view transform matrix.
	matrix4<float> software_device_impl::get_view_matrix(void)
	{
		return m_project_matrix;
	}

	// Sets the projection transform matrix.
	void software_device_impl::set_projection_matrix( const matrix4<float>& mat )
	{
		m_project_matrix = mat;
		m_raster_device.set_projection_matrix( m_project_matrix  * m_clip_matrix  );
	}

	// Get the projection transform matrix.
	matrix4<float> software_device_impl::get_projection_matrix(void)
	{
		return m_project_matrix;
	}

	// Sets the texture coordinate transformation matrix for a texture unit.
	// @param unit Texture unit to affect
	// @param xform The 4x4 matrix
	void software_device_impl::set_texture_matrix( size_t unit, const matrix4<float>& mat )
	{
		// not implement;
	}

	// Sets the current vertex declaration, ie the source of vertex data.
	void software_device_impl::set_vertex_declaration( vertex_declaration* decl )
	{
		// not implement;		
	}

	// Sets the current vertex declaration, ie the source of vertex data.
	void software_device_impl::set_vertex_declaration( uint decl )
	{
		// not implement;
	}

	// Sets the current vertex buffer binding state.
	void software_device_impl::set_vertex_buffer_binding( vertex_buffer_binding* binding )
	{
		// not implement;
	}

	// Create an object for performing hardware occlusion queries. 
	hardware_occlusion_query* software_device_impl::create_hardware_occlusion_query(void)
	{
		// not implement;
		return NULL;
	}

	// Destroy a hardware occlusion query object. 
	void software_device_impl::destroy_hardware_occlusion_query( hardware_occlusion_query* hq )
	{
		// not implement;
	}

	bool software_device_impl::get_query_result( void )
	{
		return false;
	}

	void software_device_impl::set_vertex_declear( ulong vertex_declear_type )
	{
		// not implement;
	}

	void software_device_impl::set_texture( ulong stage_index, texture_handle handle )
	{
		// not implement;
	}

	void software_device_impl::set_blank_texture( ulong stage_index )
	{
		// not implement;
	}

	void software_device_impl::push_render_state( render_state& state )
	{
		// not implement;
	}

	void software_device_impl::pop_render_state( void )
	{
		// not implement;
	}

	void software_device_impl::push_texture_state( texture_state& state )
	{
		// not implement;
	}

	void software_device_impl::pop_texture_state( void )
	{
		// not implement;
	}

	void software_device_impl::push_transform( void )
	{
		m_matrix_stack.push(m_world_matrx);
	}

	void software_device_impl::multiply_transform( matrix4<float>& mat )
	{
		m_world_matrx *= mat;
		m_raster_device.set_world_matrix( m_world_matrx );
	}

	void software_device_impl::pop_transform( void )
	{
		if( m_matrix_stack.size() == 0) return;
		m_world_matrx = m_matrix_stack.top();
		m_matrix_stack.pop();
	}

	void software_device_impl::set_render_state( ulong state_type, ulong state_value )
	{
		// not implement;
	}

	void software_device_impl::get_render_state( ulong state_type, ulong* state_value)
	{
		// not implement;
	}

	void software_device_impl::set_texture_state( ulong stage_index, ulong state_type, ulong state_value )
	{
		// not implement;
	}

	void software_device_impl::get_texture_state( ulong stage_index, ulong state_type, ulong* state_value)
	{
		// not implement;
	}

	void software_device_impl::set_sampler_state( ulong stage_index, ulong state_type, ulong state_value )
	{
		// not implement;
	}

	void software_device_impl::get_sampler_state( ulong stage_index, ulong state_type, ulong* state_value)
	{
		// not implement;
	}

	void software_device_impl::draw_indexed_triangle_list(	ulong v_opt,
															void* vertices_buffer,
															ulong vertices_count,
															void* indices_buffer,
															ulong indices_count, 
															ulong stride_size )
	{
		if(indices_count < 3) return;

		ushort* indices_buf = (ushort*)indices_buffer;
		for( int ni = 0; ni < (int)(indices_count / 3); ni++ )
		{
			m_raster_device.raster( (vector3<float>*)((ushort*)vertices_buffer + indices_buf[0] * stride_size),
									(vector3<float>*)((ushort*)vertices_buffer + indices_buf[1] * stride_size),
									(vector3<float>*)((ushort*)vertices_buffer + indices_buf[2] * stride_size) );

			indices_buf += 3;
		}
	}

	void software_device_impl::draw_indexed_triangle_fan( void* vertices_buffer,
														  ulong vertices_count,
														  ushort* indices_buffer,
														  ulong indices_count,
														  ulong stride_size )
	{

	}

	void software_device_impl::draw_triangle_fan( void* vertices_buffer, ulong vertices_count, ulong stride_size )
	{

		//m_raster_device.raster( vertices_buffer );
	}

	void software_device_impl::draw_triangle_strip( void* vertices_buffer, ulong vertices_count, ulong strid_size )
	{
		//m_raster_device.raster( vertices_buffer );
	}

	void software_device_impl::draw_line_list( void* vertices_buffer, ulong vertices_count, ulong stride_size )
	{
		// not implement;
	}

	void software_device_impl::draw_line_strip( void* vertices_buffer, ulong vertices_count, ulong stride_size )
	{
		// not implement;
	}

	void software_device_impl::draw_point_list( void* vertices_buffer, ulong vertices_count, ulong vertices_color )
	{
		// not implement;
	}

	void software_device_impl::draw_polygon( void* vertices_buffer, ulong vertices_count, ulong vertices_color )
	{
		// not implement;
	}

	void software_device_impl::draw_text( const LPCTSTR text_string, vector3<double>& position_in_geo, ulong text_color )
	{
		// not implement;
	}

	void software_device_impl::draw_mesh( void* mesh_pointer, uint pass_index )
	{
		LPD3DXMESH v_d3d_mesh = (LPD3DXMESH)mesh_pointer;

		LPDIRECT3DINDEXBUFFER9  v_d3d_index_buffer = NULL;
		LPDIRECT3DVERTEXBUFFER9 v_d3d_vertex_buffer = NULL;
		LPD3DXATTRIBUTERANGE	v_d3d_attri_table = NULL;
		ulong					v_attrid_size = 0;

		v_d3d_mesh->GetAttributeTable( v_d3d_attri_table, &v_attrid_size );
		if( pass_index != 0 && pass_index >= v_attrid_size ) return;
		v_d3d_mesh->GetIndexBuffer( &v_d3d_index_buffer );
		v_d3d_mesh->GetVertexBuffer( &v_d3d_vertex_buffer );

		ulong v_face_start = 0;
		ulong v_face_number = v_d3d_mesh->GetNumFaces();
		ulong v_verts_start = 0;
		ulong v_verts_number = v_d3d_mesh->GetNumVertices();
		ulong v_verts_stride = v_d3d_mesh->GetNumBytesPerVertex();

		if( v_d3d_attri_table != NULL )
		{
			v_face_start   = v_d3d_attri_table[pass_index].FaceStart;
			v_face_number   = v_d3d_attri_table[pass_index].FaceCount;
			v_verts_start = v_d3d_attri_table[pass_index].VertexStart;
			v_verts_number = v_d3d_attri_table[pass_index].VertexCount;
		}

		// lock vb & ib
		ushort* v_indices  = NULL;
		byte* v_vertices = NULL;

		v_d3d_index_buffer->Lock( v_face_start * 3, v_face_number * 3, (void**)&v_indices, D3DLOCK_DISCARD  );
		v_d3d_vertex_buffer->Lock( v_verts_start, v_verts_number, (void**)&v_vertices, D3DLOCK_DISCARD );

		assert(v_indices);
		assert(v_vertices);

		for( int ni = 0; ni < (int)v_face_number; ni++, v_indices += 3)
		{
			m_raster_device.raster( (vector3<float>*)((byte*)v_vertices + v_indices[0] * v_verts_stride),
									(vector3<float>*)((byte*)v_vertices + v_indices[1] * v_verts_stride),
									(vector3<float>*)((byte*)v_vertices + v_indices[2] * v_verts_stride));
		}

		v_d3d_index_buffer->Unlock();
		v_d3d_vertex_buffer->Unlock();

		AUTO_RELEASE(v_d3d_index_buffer);
		AUTO_RELEASE(v_d3d_vertex_buffer);
	}

	void software_device_impl::draw_transformed_line_strip( void* point_buffer, ulong point_count, ulong point_color )
	{
		// not implement;
	}

	void software_device_impl::draw_transformed_text( const LPCTSTR text_string, point point_transformed, ulong text_color )
	{
		// not implement;
	}

	long software_device_impl::create_mesh_from_buffer(	raw_buffer* vertices_buffer, 
														raw_buffer* indices_buffer,
														raw_buffer* attribute_buffer,
														ulong vertices_count,
														ulong indices_count,
														ulong vertex_dcls, 
														void** mesh_object )
	{
		return 0;
	}

	long software_device_impl::create_mesh_from_file_in_memory( raw_buffer* in_buffer, material_set* mtl_buffer, void** mesh_object )
	{
		return 0;
	}

	long software_device_impl::create_texture_from_buffer( raw_buffer* in_buffer, texture_info* tex_info, void** tex_object )
	{
		return 0;
	}

	long software_device_impl::create_texture_from_file( const LPCTSTR texture_path, void** tex_object )
	{
		return 0;
	}

	long software_device_impl::create_texture_from_file_in_memory( raw_buffer* in_buffer, void** tex_object )
	{
		return 0;
	}

	long software_device_impl::create_texture_from_resource( ULONG_PTR handle, void** tex_object )
	{
		return 0;
	}

	bool software_device_impl::create_effect_from_file( const LPCTSTR effect_file, void** effect )
	{
		return true;
	}

	bool software_device_impl::create_effect_from_resource( ULONG_PTR handle, void** effect )
	{
		return true;
	}

	bool software_device_impl::create_vertex_buffer( ulong buffer_size,ulong usage, ulong fvf, ulong pool, void** vertex_buffer, void** shared_handle )
	{
		return true;
	}

	bool software_device_impl::create_index_buffer( ulong buffer_size, ulong usage, ulong d3d_format, ulong pool, void** index_buffer, void** shared_handle )
	{
		return true;
	}

	void* software_device_impl::create_transparency_texture( ulong v_color )
	{
		return NULL;
	}
}
