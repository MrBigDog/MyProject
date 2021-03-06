//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_device.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_device class
//	Reference : 
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_DEVICE_H_
#define _US_RENDER_DEVICE_H_

#include <usCore\Export.h>
#include <usUtil\us_common_file.h>
#include <usUtil\us_matrix4.h>
#include <usUtil\us_rect.h>
#include <usCore\us_view_port.h>
#include <usCore\us_render_common.h>
#include <usCore\us_event_base.h>
#include <usCore\us_texture_state.h>

#include <stack>

namespace uniscope_globe
{
	class 	render_transform;
	class	render_state;
	class	texture_stage;
	class	texture_state;
	class render_device_capabilities;
	class raw_buffer;
	class hardware_occlusion_query;
	class vertex_buffer_binding;
	class vertex_declaration;
	class material_set;
	class texture_info;

	class USCORE_EXPORT render_device
	{
	public:
		render_device(void);

		virtual ~render_device(void);

	public:
		virtual bool create(void) = 0;

		virtual void destroy(void) = 0;

		virtual void reset(void) = 0;

		virtual void on_resize_device(uint device_width, uint device_height) = 0;

		virtual void on_lost_device(void) = 0;

		virtual void on_reset_device(void) = 0;

		virtual bool can_create_hardware(void) = 0;

		const render_device_capabilities* get_capabilities(void) const { return m_capabilities; }

		// method
	public:
		virtual void begin_frame(void) = 0;

		virtual void end_frame(void) = 0;

		virtual void clear(ulong color = 0x00000000, float z_buffer = 1.0f, ulong stencil = 3) = 0;

		virtual void clear_target(ulong color = 0xff000000) = 0;

		virtual void clear_zbuffer(float z_buffer = 1.0f) = 0;

		virtual void clear_stencil(ulong stencil = 3) = 0;

		virtual	void save_back_buffer(cpstr str_path, int width, int height) = 0;

		virtual	void save_back_buffer(cpstr str_path, int width, int height, int stretch_type) = 0;

		virtual void grab_back_buffer(raw_buffer* v_buffer, int width, int height, int stretch_type = 0) = 0;

	public:
		virtual vector3<float> projection(vector3<float>& vector_in_world) = 0;

		virtual vector3<float> unprojection(vector3<float>& vector_on_viewport) = 0;

		// property
	public:
		virtual void set_near_plane(float near_plane) { m_near_plane = near_plane; }

		virtual float get_near_plane(void) { return m_near_plane; }

		virtual void set_far_plane(float far_plane) { m_far_plane = far_plane; }

		virtual float get_far_plane(void) { return m_far_plane; }

		virtual void set_view_port(view_port& viewport) = 0;

		virtual view_port get_view_port(void) const = 0;

		virtual render_transform*   get_matrix_stack(void) = 0;

		virtual render_state*		get_render_state(void) = 0;

		virtual texture_stage*		get_texture_stage(ulong texture_stage_index) = 0;

		virtual void set_clip_rect(rect& clip_rect) = 0;

		virtual void set_clip_rect(float x, float y, float w, float h) = 0;




	public:
		virtual ULONG_PTR get_device(void) = 0;

		virtual ULONG_PTR get_sprite(void) = 0;

		virtual ULONG_PTR get_font(void) = 0;

		virtual ULONG_PTR get_effect(long effect_handle) = 0;

		virtual ULONG_PTR get_render_target(long rt_handle) = 0;

		virtual ULONG_PTR get_hwnd(void) = 0;

	public:
		virtual void set_culling_mode(culling_mode mode) = 0;

		virtual culling_mode get_culling_mode(void) const = 0;

		/** Sets the world transform matrix. */
		virtual void set_world_matrix(const matrix4<float>& mat) = 0;

		virtual matrix4<float> get_world_matrix(void) = 0;

		/** Sets multiple world matrices (vertex blending). */
		virtual void set_world_matrices(const matrix4<float>* m, ushort count) = 0;

		/** Sets the view transform matrix */
		virtual void set_view_matrix(const matrix4<float>& mat) = 0;

		virtual matrix4<float> get_view_matrix(void) = 0;

		/** Sets the projection transform matrix */
		virtual void set_projection_matrix(const matrix4<float>& mat) = 0;

		virtual matrix4<float> get_projection_matrix(void) = 0;

		// Sets the clip transform matrix
		virtual void set_clip_matrix(const matrix4<float>& mat) = 0;

		virtual matrix4<float> get_clip_matrix(void) = 0;

		/** Sets the texture coordinate transformation matrix for a texture unit.
		@param unit Texture unit to affect
		@param xform The 4x4 matrix
		*/
		virtual void set_texture_matrix(size_t unit, const matrix4<float>& xform) = 0;

		/** Sets the current vertex declaration, ie the source of vertex data. */
		virtual void set_vertex_declaration(vertex_declaration* decl) = 0;

		// Sets the current vertex declaration, ie the source of vertex data.
		virtual void set_vertex_declaration(uint decl) = 0;

		/** Sets the current vertex buffer binding state. */
		virtual void set_vertex_buffer_binding(vertex_buffer_binding* binding) = 0;

		/** Create an object for performing hardware occlusion queries.
		*/
		virtual hardware_occlusion_query* create_hardware_occlusion_query(void) = 0;

		/** Destroy a hardware occlusion query object.
		*/
		virtual void destroy_hardware_occlusion_query(hardware_occlusion_query* hq) = 0;

		virtual void set_clipping_plane(ulong index, float* in_plane) = 0;

	public:
		/** Sets the vertex declear */
		virtual void set_vertex_declear(ulong vertex_declear_type) = 0;

		virtual void set_texture(ulong stage_index, texture_handle handle) = 0;

		virtual void set_blank_texture(ulong stage_index) = 0;

		virtual void set_transparency_texture(ulong stage_index) = 0;

		virtual void set_transparency_texture(ulong stage_index, ulong texture_index) = 0;

		virtual ULONG_PTR get_blank_texture(void) = 0;

		virtual ULONG_PTR get_transparency_texture(ulong texture_index) = 0;

		virtual void set_material(ULONG_PTR material_handle) = 0;

		virtual void push_render_state(render_state& state) = 0;

		virtual void pop_render_state(void) = 0;

		virtual void push_texture_state(texture_state& state) = 0;

		virtual void pop_texture_state(void) = 0;

		virtual void push_transform(void) = 0;

		virtual void multiply_transform(const matrix4<float>& mat) = 0;

		virtual void pop_transform(void) = 0;

		virtual void set_render_state(ulong state_type, ulong state_value) = 0;

		virtual void get_render_state(ulong state_type, ulong* state_value) = 0;

		virtual void set_texture_state(ulong stage_index, ulong state_type, ulong state_value) = 0;

		virtual void get_texture_state(ulong stage_index, ulong state_type, ulong* state_value) = 0;

		virtual void set_sampler_state(ulong stage_index, ulong state_type, ulong state_value) = 0;

		virtual void get_sampler_state(ulong stage_index, ulong state_type, ulong* state_value) = 0;

		virtual void draw_indexed_triangle_list(ulong opt, void* vertices_buffer, ulong vertices_count, void* indices_buffer, ulong indices_count, ulong stride_size) = 0;

		virtual void draw_indexed_triangle_fan(void* vertices_buffer, ulong vertices_count, ushort* indices_buffer, ulong indices_count, ulong stride_size) = 0;

		virtual void draw_triangle_fan(void* vertices_buffer, ulong vertices_count, ulong stride_size) = 0;

		virtual void draw_triangle_strip(void* vertices_buffer, ulong vectices_count, ulong strid_size) = 0;

		virtual void draw_line_list(void* vertices_buffer, ulong vertices_count, ulong strid_size) = 0;

		virtual void draw_line_strip(void* vertices_buffer, ulong vertices_count, ulong strid_size) = 0;

		virtual void draw_point_list(void* vertices_buffer, ulong vertices_count, ulong strid_size) = 0;

		virtual void draw_mesh(void* mesh_pointer, uint pass_index) = 0;

		virtual void draw_transformed_line_strip(void* point_buffer, ulong point_count, ulong point_color) = 0;

		virtual void draw_transformed_text(const LPCTSTR text_string, point point_transformed, ulong text_color) = 0;

		virtual void draw_transformed_text_ext(const LPCTSTR text_string, point point_transformed, ulong text_color, double scale) = 0;

		virtual void draw_text(const LPCTSTR text_string, vector3<double>& position_in_geo, ulong text_color) = 0;


		// create function
	public:
		virtual long create_mesh_from_buffer(raw_buffer* vertices_buffer,
			raw_buffer* indices_buffer,
			raw_buffer* attribute_buffer,
			ulong vertices_count,
			ulong indices_count,
			ulong vertex_dcls,
			void** mesh_object) = 0;

		virtual long create_mesh_from_buffer(raw_buffer* vertices_buffer,
			raw_buffer* indices_buffer,
			raw_buffer* attribute_buffer,
			ulong vertices_count,
			ulong indices_count,
			ulong vertex_dcls,
			ulong options,
			void** mesh_object) = 0;

		virtual long create_mesh_from_file_in_memory(raw_buffer* in_buffer, material_set* mtl_buffer, void** mesh_object) = 0;

		virtual long create_texture_from_buffer(raw_buffer* in_buffer, texture_info* tex_info, void** tex_object) = 0;

		virtual long create_texture_from_file(const LPCTSTR text_string, void** tex_object) = 0;

		virtual long create_texture_from_file_in_memory(raw_buffer* in_buffer, void** tex_object) = 0;

		virtual long create_texture_from_resource(ULONG_PTR handle, void** tex_object) = 0;

		virtual bool create_effect_from_file(const LPCTSTR effect_file, void** effect) = 0;

		virtual bool create_effect_from_resource(ULONG_PTR handle, void** effect) = 0;

		virtual bool create_vertex_buffer(ulong buffer_size, ulong usage, ulong fvf, ulong pool, void** vertex_buffer, void** shared_handle) = 0;

		virtual bool create_index_buffer(ulong buffer_size, ulong usage, ulong d3d_format, ulong pool, void** index_buffer, void** shared_handle) = 0;

		virtual void* create_transparency_texture(ulong v_color) = 0;

	protected:
		render_device_capabilities* m_capabilities;

		view_port					m_view_port;

		std::stack<matrix4<float>>	m_matrix_stack;

		float						m_near_plane;

		float						m_far_plane;

	public:
		event_base					m_event_lost_device;

		event_base					m_event_reset_device;

	};

}

#endif // _US_RENDER_DEVICE_H_
