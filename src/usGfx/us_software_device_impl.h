///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_software_device_impl.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : software_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SOFTWARE_DEVICE_IMPL_H_
#define _US_SOFTWARE_DEVICE_IMPL_H_

namespace uniscope_globe
{
	class LIB_GFX_API software_device_impl
		: public render_device
	{
	protected:
		software_device_impl( void ) {}

	public:
		software_device_impl( HWND hwnd );

		virtual ~software_device_impl( void );

	public:
		virtual bool create( void );

		virtual void destroy( void );

		virtual void reset( void );

		virtual void on_resize_device( uint device_width, uint device_height );

		virtual void on_lost_device( void );

		virtual void on_reset_device( void );

		const render_device_capabilities* get_capabilities(void) const { return m_capabilities; }

		// method
	public:
		virtual void begin_frame( void );

		virtual void end_frame( void );

		virtual void clear( void );

		virtual void clear_target( void );

		virtual void clear_zbuffer( void );

		virtual void clear_stencil( void );

	public:
		virtual vector3<float> projection( vector3<float>& vector_in_world );

		virtual vector3<float> unprojection( vector3<float>& vector_on_viewport );

		// property
	public:
		virtual void set_view_port( view_port& viewport );

		virtual view_port get_view_port( void ) const;

		virtual render_transform* get_matrix_stack( void );

		virtual render_state* get_render_state( void );

		virtual texture_stage* get_texture_stage( ulong texture_stage_index );

		virtual void set_clip_rect(  rect& clip_rect  );

		virtual bool get_query_result( query_result& q_result);

	public:
		virtual ULONG_PTR get_device(void);

		virtual ULONG_PTR get_sprite( void );

		virtual ULONG_PTR get_font( void );

		virtual ULONG_PTR get_vertex_blend_effect( void );

		virtual ULONG_PTR get_line(void);

	public:
		virtual void set_culling_mode( culling_mode mode );

		virtual culling_mode get_culling_mode(void) const;

		// Sets the world transform matrix. 
		virtual void set_world_matrix( const matrix4<float>& mat );

		virtual matrix4<float> get_world_matrix(void);

		// Sets multiple world matrices (vertex blending).
		virtual void set_world_matrices( const matrix4<float>* mats, ushort count );

		// Sets the view transform matrix
		virtual void set_view_matrix( const matrix4<float>& mat );

		virtual matrix4<float> get_view_matrix(void);

		/** Sets the projection transform matrix */
		virtual void set_projection_matrix( const matrix4<float>& mat );

		virtual matrix4<float> get_projection_matrix(void);

		// Sets the texture coordinate transformation matrix for a texture unit.
		// @param unit Texture unit to affect
		// @param xform The 4x4 matrix
		virtual void set_texture_matrix( size_t unit, const matrix4<float>& xform );

		// Sets the current vertex declaration, ie the source of vertex data.
		virtual void set_vertex_declaration( vertex_declaration* decl );

		// Sets the current vertex declaration, ie the source of vertex data.
		virtual void set_vertex_declaration( uint decl );

		// Sets the current vertex buffer binding state.
		virtual void set_vertex_buffer_binding( vertex_buffer_binding* binding );

		// Create an object for performing hardware occlusion queries. 
		virtual hardware_occlusion_query* create_hardware_occlusion_query(void);

		// Destroy a hardware occlusion query object. 
		virtual void destroy_hardware_occlusion_query( hardware_occlusion_query* hq );

		// 
		virtual bool get_query_result( void );

		virtual void set_clipping_plane( ulong index, float* in_plane );

	public:
		virtual void set_vertex_declear( ulong vertex_declear_type );

		virtual void set_texture( ulong stage_index, texture_handle handle );

		virtual void set_blank_texture( ulong stage_index );

		virtual void push_render_state( render_state& state );

		virtual void pop_render_state( void );

		virtual void push_texture_state( texture_state& state );

		virtual void pop_texture_state( void );

		virtual void push_transform( void );

		virtual void multiply_transform( matrix4<float>& mat );

		virtual void pop_transform( void );

		virtual void set_render_state( ulong state_type, ulong state_value );

		virtual void get_render_state( ulong state_type, ulong* state_value);

		virtual void set_texture_state( ulong stage_index, ulong state_type, ulong state_value );

		virtual void get_texture_state( ulong stage_index, ulong state_type, ulong* state_value);

		virtual void set_sampler_state( ulong stage_index, ulong state_type, ulong state_value );

		virtual void get_sampler_state( ulong stage_index, ulong state_type, ulong* state_value);

		virtual void draw_indexed_triangle_list( ulong v_opt, void* vertices_buffer, ulong vertices_count, void* indices_buffer, ulong indices_count, ulong stride_size );

		virtual void draw_indexed_triangle_fan( void* vertices_buffer, ulong vertices_count, ushort* indices_buffer, ulong indices_count, ulong stride_size );

		virtual void draw_triangle_fan( void* vertices_buffer, ulong vertices_count, ulong stride_size );

		virtual void draw_triangle_strip( void* vertices_buffer, ulong vertices_count, ulong strid_size );

		virtual void draw_line_list( void* vertices_buffer, ulong vertices_count, ulong strid_size );

		virtual void draw_line_strip( void* vertices_buffer, ulong vertices_count, ulong strid_size );

		virtual void draw_point_list( void* vertices_buffer, ulong vertices_count, ulong strid_size );

		virtual void draw_polygon( void* vertices_buffer, ulong vertices_count, ulong strid_size );

		virtual void draw_text( const LPCTSTR text_string, vector3<double>& position_in_geo, ulong text_color );

		virtual void draw_mesh( void* mesh_pointer, uint pass_index );

		virtual void draw_transformed_line_strip( void* point_buffer, ulong point_count, ulong point_color );

		virtual void draw_transformed_text( const LPCTSTR text_string, point point_transformed, ulong text_color );

	public:
		virtual long create_mesh_from_buffer(	raw_buffer* vertices_buffer, 
												raw_buffer* indices_buffer,
												raw_buffer* attribute_buffer,
												ulong vertices_count,
												ulong indices_count,
												ulong vertex_dcls, 
												void** mesh_object );

		virtual long create_mesh_from_file_in_memory( raw_buffer* in_buffer, material_set* mtl_buffer, void** mesh_object );

		virtual long create_texture_from_buffer( raw_buffer* in_buffer, texture_info* tex_info, void** tex_object );

		virtual long create_texture_from_file( const LPCTSTR texture_path, void** tex_object );

		virtual long create_texture_from_file_in_memory( raw_buffer* in_buffer, void** tex_object );

		virtual long create_texture_from_resource( ULONG_PTR handle, void** tex_object );

		virtual bool create_effect_from_file( const LPCTSTR effect_file, void** effect);

		virtual bool create_effect_from_resource( ULONG_PTR handle, void** effect );

		virtual bool create_vertex_buffer( ulong buffer_size,ulong usage, ulong fvf, ulong pool, void** vertex_buffer, void** shared_handle );

		virtual bool create_index_buffer( ulong buffer_size, ulong usage, ulong d3d_format, ulong pool, void** index_buffer, void** shared_handle );

		virtual void* create_transparency_texture( ulong v_color );

	protected:
		HWND					m_hwnd;

		BOOL					m_is_windowed;

		software_raster			m_raster_device;	

		int						m_batch_count;

		int						m_render_count;

		matrix4<float>			m_project_matrix;

		matrix4<float>			m_view_matrix;

		matrix4<float>			m_world_matrx;

		matrix4<float>			m_clip_matrix;

		//	std::stack<matrix4<float>>	m_matrix_stack;
	};
}

#endif // _US_SOFTWARE_DEVICE_IMPL_H_
