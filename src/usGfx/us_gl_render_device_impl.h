///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_gl_render_device_impl.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : gl_render_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GL_RENDER_DEVICE_IMPL_H_
#define _US_GL_RENDER_DEVICE_IMPL_H_
#include <usGfx/Export.h>
#include <usCore/us_render_device.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class USGFX_EXPORT gl_render_device_impl
		: public render_device
	{
	public:
		gl_render_device_impl(void);

		virtual ~gl_render_device_impl(void);

	public:
		virtual bool create(HWND hwnd);

		virtual void destroy(void);

		virtual void reset(void);

		virtual void on_resize_device(uint device_width, uint device_height);

		virtual void on_lost_device(void);

		virtual bool can_create_hardware(void);

		const render_device_capabilities* get_capabilities(void) const { return m_capabilities; }

		// method
	public:
		virtual void begin_frame(void);

		virtual void end_frame(void);

		virtual void clear(void);

		virtual void clear_target(void);

		virtual void clear_zbuffer(void);

		virtual void clear_stencil(void);

	public:
		virtual vector3<float> projection(const vector3<float>& vector_in_world);

		virtual vector3<float> unprojection(const vector3<float>& vector_on_viewport);

		// property
	public:
		virtual void set_view_port(view_port& viewport);

		virtual view_port get_view_port(void) const;

		virtual render_transform* get_matrix_stack(void);

		virtual render_state* get_render_state(void);

		virtual texture_stage* get_texture_stage(ulong texture_stage_index);

	public:
		virtual void set_culling_mode(culling_mode mode);

		virtual culling_mode get_culling_mode(void) const;

		// Sets the world transform matrix. 
		virtual void set_world_matrix(const matrix4<float>& mat);

		// Sets multiple world matrices (vertex blending).
		virtual void set_world_matrices(const matrix4<float>* mats, ushort count);

		// Sets the view transform matrix
		virtual void set_view_matrix(const matrix4<float>& mat);

		/** Sets the projection transform matrix */
		virtual void set_projection_matrix(const matrix4<float>& mat);

		// Sets the texture coordinate transformation matrix for a texture unit.
		// @param unit Texture unit to affect
		// @param xform The 4x4 matrix
		virtual void set_texture_matrix(size_t unit, const matrix4<float>& xform);

		// Sets the current vertex declaration, ie the source of vertex data.
		virtual void set_vertex_declaration(vertex_declaration* decl);

		// Sets the current vertex declaration, ie the source of vertex data.
		virtual void set_vertex_declaration(uint decl);

		// Sets the current vertex buffer binding state.
		virtual void set_vertex_buffer_binding(vertex_buffer_binding* binding);

		// Create an object for performing hardware occlusion queries. 
		virtual hardware_occlusion_query* create_hardware_occlusion_query(void);

		// Destroy a hardware occlusion query object. 
		virtual void destroy_hardware_occlusion_query(hardware_occlusion_query* hq);

	public:
		virtual void set_texture(ulong stage_index, texture_handle handle);

		virtual void push_render_state(render_state& state);

		virtual void pop_render_state(void);

		virtual void push_texture_state(texture_state& state);

		virtual void pop_texture_state(void);

		virtual void push_transform(void);

		virtual void multiply_transform(const matrix4<float>& mat);

		virtual void pop_transform(void);

		virtual void draw_indexed_triangle_list(ulong opt, void* vertices_buffer, ulong vertices_count, void* indices_buffer, ulong indices_count, ulong stride_size);

		virtual void draw_triangle_fan(void* vertices_buffer, ulong vertices_count, ulong stride_size);

		virtual void draw_triangle_strip(void* vertices_buffer, ulong vertices_count, ulong strid_size);

		virtual void draw_line_list(void* vertices_buffer, ulong vertices_count, ulong vertices_color);

		virtual void draw_line_strip(void* vertices_buffer, ulong vertices_count, ulong vertices_color);

		virtual void draw_point_list(void* vertices_buffer, ulong vertices_count, ulong vertices_color, ulong point_size);

		virtual void draw_polygon(void* vertices_buffer, ulong vertices_count, ulong vertices_color);

		virtual void draw_text(const LPCTSTR text_string, vector3<double>& position_in_geo, ulong text_color);

		virtual void draw_mesh(void* mesh_pointer, uint pass_index);

		virtual void draw_transformed_line_strip(void* point_buffer, ulong point_count, ulong point_color);

		virtual void draw_transformed_text(const LPCTSTR text_string, point point_transformed, ulong text_color);

	};
}

#endif // _US_GL_RENDER_DEVICE_IMPL_H_