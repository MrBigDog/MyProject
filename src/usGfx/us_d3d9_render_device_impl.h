///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_device_impl.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_RENDER_DEVICE_IMPL_H_
#define _US_D3D9_RENDER_DEVICE_IMPL_H_

#include <usGfx/Export.h>
#include <usCore/us_render_device.h>
#include <usUtil/us_common_file.h>

#include <d3d9.h>
#include <d3dx9core.h>

#include <list>

using namespace std;

namespace uniscope_globe
{
	class d3d9_effect_manager;
	class d3d9_hardware_occlusion_query;

	class USGFX_EXPORT d3d9_render_device_impl
		: public render_device
	{
	protected:
		d3d9_render_device_impl(void) {}

	public:
		d3d9_render_device_impl(HWND hwnd);

		virtual ~d3d9_render_device_impl(void);

	public:
		virtual bool create(void);

		virtual void destroy(void);

		virtual void reset(void);

		virtual void on_resize_device(uint device_width, uint device_height);

		virtual void on_lost_device(void);

		virtual void on_reset_device(void);

		virtual bool can_create_hardware(void);

		const render_device_capabilities* get_capabilities(void) const { return m_capabilities; }

		// method
	public:
		virtual void begin_frame(void);

		virtual void end_frame(void);

		virtual void clear(ulong color = 0x00000000, float z_buffer = 1.0f, ulong stencil = 3);

		virtual void clear_target(ulong color = 0xff000000);

		virtual void clear_zbuffer(float z_buffer = 1.0f);

		virtual void clear_stencil(ulong stencil = 3);

		virtual	void save_back_buffer(cpstr str_path, int width, int height);

		virtual	void save_back_buffer(cpstr str_path, int width, int height, int stretch_type);

		virtual void grab_back_buffer(raw_buffer* v_buffer, int width, int height, int stretch_type = 0);

	public:
		virtual vector3<float> projection(vector3<float>& vector_in_world);

		virtual vector3<float> unprojection(vector3<float>& vector_on_viewport);

		// property
	public:
		virtual void set_view_port(view_port& viewport);

		virtual view_port get_view_port(void) const;

		virtual render_transform* get_matrix_stack(void);

		virtual render_state* get_render_state(void);

		virtual texture_stage* get_texture_stage(ulong texture_stage_index);

		virtual void set_clip_rect(rect& clip_rect);

		virtual void set_clip_rect(float x, float y, float w, float h);

		virtual void set_clipping_plane(ulong index, float* in_plane);

	public:
		virtual ULONG_PTR get_device(void);

		virtual ULONG_PTR get_sprite(void);

		virtual ULONG_PTR get_font(void);

		virtual ULONG_PTR get_line(void);

		virtual ULONG_PTR get_effect(long effect_handle);

		virtual ULONG_PTR get_render_target(long rt_handle);

		virtual ULONG_PTR get_hwnd(void) { return (ULONG_PTR)m_hwnd; }

	public:
		virtual uint get_backbuffer_width(void) { return m_d3d_present_param.BackBufferWidth; }

		virtual uint get_backbuffer_height(void) { return m_d3d_present_param.BackBufferHeight; }

		virtual int  get_backbuffer_format(void) { return (int)(m_d3d_present_param.BackBufferFormat); }

		virtual int  get_backbuffer_multisampletype(void) { return (int)(m_d3d_present_param.MultiSampleType); }

		virtual int  get_backbuffer_multisamplequality(void) { return (int)(m_d3d_present_param.MultiSampleQuality); }


		virtual void set_culling_mode(culling_mode mode);

		virtual culling_mode get_culling_mode(void) const;

		// Sets the world transform matrix. 
		virtual void set_world_matrix(const matrix4<float>& mat);

		virtual matrix4<float> get_world_matrix(void);

		// Sets multiple world matrices (vertex blending).
		virtual void set_world_matrices(const matrix4<float>* mats, ushort count);

		// Sets the view transform matrix
		virtual void set_view_matrix(const matrix4<float>& mat);

		virtual matrix4<float> get_view_matrix(void);

		/** Sets the projection transform matrix */
		virtual void set_projection_matrix(const matrix4<float>& mat);

		virtual matrix4<float> get_projection_matrix(void);

		// Sets the clip transform matrix
		virtual void set_clip_matrix(const matrix4<float>& mat);

		virtual matrix4<float> get_clip_matrix(void);

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
		virtual void set_vertex_declear(ulong vertex_declear_type);

		virtual void set_texture(ulong stage_index, texture_handle handle);

		virtual void set_blank_texture(ulong stage_index);

		virtual void set_transparency_texture(ulong stage_index);

		virtual void set_transparency_texture(ulong stage_index, ulong texture_index);

		virtual ULONG_PTR get_blank_texture(void);

		virtual ULONG_PTR get_transparency_texture(ulong texture_index);

		virtual void set_material(ULONG_PTR material_handle);

		virtual void push_render_state(render_state& state);

		virtual void pop_render_state(void);

		virtual void push_texture_state(texture_state& state);

		virtual void pop_texture_state(void);

		virtual void push_transform(void);

		virtual void multiply_transform(const matrix4<float>& mat);

		virtual void pop_transform(void);

		virtual void set_render_state(ulong state_type, ulong state_value);

		virtual void get_render_state(ulong state_type, ulong* state_value);

		virtual void set_texture_state(ulong stage_index, ulong state_type, ulong state_value);

		virtual void get_texture_state(ulong stage_index, ulong state_type, ulong* state_value);

		virtual void set_sampler_state(ulong stage_index, ulong state_type, ulong state_value);

		virtual void get_sampler_state(ulong stage_index, ulong state_type, ulong* state_value);

		virtual void draw_indexed_triangle_list(ulong opt, void* vertices_buffer, ulong vertices_count, void* indices_buffer, ulong indices_count, ulong stride_size);

		virtual void draw_indexed_triangle_fan(void* vertices_buffer, ulong vertices_count, ushort* indices_buffer, ulong indices_count, ulong stride_size);

		virtual void draw_triangle_fan(void* vertices_buffer, ulong vertices_count, ulong stride_size);

		virtual void draw_triangle_strip(void* vertices_buffer, ulong vertices_count, ulong strid_size);

		virtual void draw_line_list(void* vertices_buffer, ulong vertices_count, ulong stride_size);

		virtual void draw_line_strip(void* vertices_buffer, ulong vertices_count, ulong stride_size);

		virtual void draw_point_list(void* vertices_buffer, ulong vertices_count, ulong stride_size);

		virtual void draw_polygon(void* vertices_buffer, ulong vertices_count, ulong stride_size);

		virtual void draw_text(const LPCTSTR text_string, vector3<double>& position_in_geo, ulong text_color);

		virtual void draw_mesh(void* mesh_pointer, uint pass_index);

		virtual void draw_transformed_line_strip(void* point_buffer, ulong point_count, ulong point_color);

		virtual void draw_transformed_text(const LPCTSTR text_string, point point_transformed, ulong text_color);

		virtual void draw_transformed_text_ext(const LPCTSTR text_string, point point_transformed, ulong text_color, double size);


	public:
		virtual long create_mesh_from_buffer(raw_buffer* vertices_buffer,
			raw_buffer* indices_buffer,
			raw_buffer* attribute_buffer,
			ulong vertices_count,
			ulong indices_count,
			ulong vertex_dcls,
			void** mesh_object);

		virtual long create_mesh_from_buffer(raw_buffer* vertices_buffer,
			raw_buffer* indices_buffer,
			raw_buffer* attribute_buffer,
			ulong vertices_count,
			ulong indices_count,
			ulong vertex_dcls,
			ulong options,
			void** mesh_object);

		virtual long create_mesh_from_file_in_memory(raw_buffer* in_buffer, material_set* mtl_buffer, void** mesh_object);

		virtual long create_texture_from_buffer(raw_buffer* in_buffer, texture_info* tex_info, void** tex_object);

		virtual long create_texture_from_file(const LPCTSTR texture_path, void** tex_object);

		virtual long create_texture_from_file_in_memory(raw_buffer* in_buffer, void** tex_object);

		virtual long create_texture_from_resource(ULONG_PTR handle, void** tex_object);

		virtual bool create_effect_from_file(const LPCTSTR effect_file, void** effect);

		virtual bool create_effect_from_resource(ULONG_PTR handle, void** effect);

		virtual bool create_vertex_buffer(ulong buffer_size, ulong usage, ulong fvf, ulong pool, void** vertex_buffer, void** shared_handle);

		virtual bool create_index_buffer(ulong buffer_size, ulong usage, ulong d3d_format, ulong pool, void** index_buffer, void** shared_handle);

		virtual void* create_transparency_texture(ulong v_color);

	public:

		friend class d3d9_render_target_manager;

	protected:
		// create implement
		bool create_device_impl(void);

		bool reset_swapchain(void);

		bool create_font_impl(void);

		bool create_sprite_impl(void);

		bool create_blank_texture(void);

		bool create_transparency_texture(void);

		// test
		bool create_line_impl(void);

		bool create_effect(void);

		bool create_render_target(void);

		void update_clip_matrix(void);

	protected:
		HWND					m_hwnd;

		BOOL					m_is_windowed;
		// d3d Object
		LPDIRECT3D9				m_d3d_object;

		LPDIRECT3DDEVICE9		m_d3d_device;

		LPDIRECT3DSWAPCHAIN9	m_d3d_swapchain;

		LPDIRECT3DSURFACE9		m_d3d_zbuffer;

		LPDIRECT3DTEXTURE9		m_blank_texture;

		//vector<render_target*>	m_render_target_array;

		//LPDIRECT3DTEXTURE9		m_transparency_texture;

		LPDIRECT3DTEXTURE9		m_transparency_texture_array[2];

		LPD3DXSPRITE			m_d3d_sprite;

		LPD3DXLINE				m_d3d_line;

		LPD3DXFONT				m_d3d_font;

		LPD3DXFONT				m_d3d_font_ext;
		double                  m_font_size;

		//d3d9_vertex_blend_effect* m_d3d9_vertex_blend_effect;

		D3DPRESENT_PARAMETERS	m_d3d_present_param;

		D3DDISPLAYMODE			m_d3d_display_mode;

		int						m_batch_count;

		int						m_render_count;

		matrix4<float>			m_project_matrix;

		matrix4<float>			m_view_matrix;

		matrix4<float>			m_world_matrix;

		matrix4<float>			m_clip_matrix;

		list<long>				m_occlusion_queries;

		d3d9_hardware_occlusion_query* m_occlusion_query;

		d3d9_effect_manager*	m_effect_manager;

		d3d9_render_target_manager* m_render_target_manager;

		rect					m_clip_rect;

		//	std::stack<matrix4<float>>	m_matrix_stack;
	};
}

#endif // _US_D3D9_RENDER_DEVICE_IMPL_H_