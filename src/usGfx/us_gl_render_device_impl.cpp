///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_gl_render_device_impl.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : gl_render_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_gl_render_device_impl.h"

#include <usUtil/us_point.h>

namespace uniscope_globe
{
	gl_render_device_impl::gl_render_device_impl(void)
	{

	}

	gl_render_device_impl::~gl_render_device_impl(void)
	{

	}

	bool gl_render_device_impl::create(HWND hwnd)
	{
		return true;
	}

	void gl_render_device_impl::destroy(void)
	{

	}

	void gl_render_device_impl::reset(void)
	{
		// not implement
	}

	void gl_render_device_impl::on_resize_device(uint device_width, uint device_height)
	{

	}

	void gl_render_device_impl::on_lost_device(void)
	{

	}

	bool gl_render_device_impl::can_create_hardware(void)
	{
		////modify by felixÄÚ´æ¿ØÖÆ
		/*if ( system_environment::s_always_create_hardware == 1 )
			return true;

		if ( system_environment::s_reclaim_memory_size > singleton_resource_reclaimer::ptr()->get_memory_size() )
			return false;*/

		return true;
	}

	void gl_render_device_impl::begin_frame(void)
	{

	}

	void gl_render_device_impl::end_frame(void)
	{

	}

	void gl_render_device_impl::clear(void)
	{

	}

	void gl_render_device_impl::clear_target(void)
	{

	}

	void gl_render_device_impl::clear_zbuffer(void)
	{

	}

	void gl_render_device_impl::clear_stencil(void)
	{

	}

	vector3<float> gl_render_device_impl::projection(const vector3<float>& vector_in_world)
	{
		vector3<float> ret_vec;
		return ret_vec;
	}

	vector3<float> gl_render_device_impl::unprojection(const vector3<float>& vector_on_viewport)
	{
		vector3<float> ret_vec;
		return ret_vec;
	}

	void gl_render_device_impl::set_view_port(view_port& viewport)
	{

	}

	view_port gl_render_device_impl::get_view_port(void) const
	{
		return m_view_port;
	}

	render_transform* gl_render_device_impl::get_matrix_stack(void)
	{
		// not implement;
		return NULL;
	}

	render_state* gl_render_device_impl::get_render_state(void)
	{
		// not implement;
		return NULL;
	}

	texture_stage* gl_render_device_impl::get_texture_stage(ulong texture_stage_index)
	{
		// not implement;
		return NULL;
	}

	void gl_render_device_impl::set_culling_mode(culling_mode mode)
	{

	}

	culling_mode gl_render_device_impl::get_culling_mode(void) const
	{
		return (culling_mode)0;
	}

	// Sets the world transform matrix. 
	void gl_render_device_impl::set_world_matrix(const matrix4<float>& mat)
	{

	}

	// Sets multiple world matrices (vertex blending).
	void gl_render_device_impl::set_world_matrices(const matrix4<float>* mats, ushort count)
	{

	}

	// Sets the view transform matrix.
	void gl_render_device_impl::set_view_matrix(const matrix4<float>& mat)
	{

	}

	// Sets the projection transform matrix.
	void gl_render_device_impl::set_projection_matrix(const matrix4<float>& mat)
	{

	}

	// Sets the texture coordinate transformation matrix for a texture unit.
	// @param unit Texture unit to affect
	// @param xform The 4x4 matrix
	void gl_render_device_impl::set_texture_matrix(size_t unit, const matrix4<float>& mat)
	{

	}

	// Sets the current vertex declaration, ie the source of vertex data.
	void gl_render_device_impl::set_vertex_declaration(vertex_declaration* decl)
	{
		// not implement;		
	}

	// Sets the current vertex declaration, ie the source of vertex data.
	void gl_render_device_impl::set_vertex_declaration(uint decl)
	{

	}

	// Sets the current vertex buffer binding state.
	void gl_render_device_impl::set_vertex_buffer_binding(vertex_buffer_binding* binding)
	{
		// not implement;
	}

	// Create an object for performing hardware occlusion queries. 
	hardware_occlusion_query* gl_render_device_impl::create_hardware_occlusion_query(void)
	{
		// not implement;
		return NULL;
	}

	// Destroy a hardware occlusion query object. 
	void gl_render_device_impl::destroy_hardware_occlusion_query(hardware_occlusion_query* hq)
	{
		// not implement;
	}

	void gl_render_device_impl::set_texture(ulong stage_index, texture_handle handle)
	{

	}

	void gl_render_device_impl::push_render_state(render_state& state)
	{
		// not implement;
	}

	void gl_render_device_impl::pop_render_state(void)
	{
		// not implement;
	}

	void gl_render_device_impl::push_texture_state(texture_state& state)
	{
		// not implement;
	}

	void gl_render_device_impl::pop_texture_state(void)
	{
		// not implement;
	}

	void gl_render_device_impl::push_transform(void)
	{
		// not implement;
	}

	void gl_render_device_impl::multiply_transform(const matrix4<float>& mat)
	{
		// not implement;
	}

	void gl_render_device_impl::pop_transform(void)
	{
		// not implement;
	}

	void gl_render_device_impl::draw_indexed_triangle_list(ulong opt, void* vertices_buffer,
		ulong vertices_count,
		void* indices_buffer,
		ulong indices_count,
		ulong stride_size)
	{

	}

	void gl_render_device_impl::draw_triangle_fan(void* vertices_buffer, ulong vertices_count, ulong stride_size)
	{

	}

	void gl_render_device_impl::draw_triangle_strip(void* vertices_buffer, ulong vertices_count, ulong strid_size)
	{

	}

	void gl_render_device_impl::draw_line_list(void* vertices_buffer, ulong vertices_count, ulong vertices_color)
	{

	}

	void gl_render_device_impl::draw_line_strip(void* vertices_buffer, ulong vertices_count, ulong vertices_color)
	{

	}

	void gl_render_device_impl::draw_point_list(void* vertices_buffer, ulong vertices_count, ulong vertices_color, ulong point_size)
	{

	}

	void gl_render_device_impl::draw_polygon(void* vertices_buffer, ulong vertices_count, ulong vertices_color)
	{

	}

	void gl_render_device_impl::draw_text(const LPCTSTR text_string, vector3<double>& position_in_geo, ulong text_color)
	{

	}

	void gl_render_device_impl::draw_mesh(void* mesh_pointer, uint pass_index)
	{

	}

	void gl_render_device_impl::draw_transformed_line_strip(void* point_buffer, ulong point_count, ulong point_color)
	{

	}

	void gl_render_device_impl::draw_transformed_text(const LPCTSTR text_string, point point_transformed, ulong text_color)
	{

	}
}
