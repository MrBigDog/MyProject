///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_shadow_volume.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_shadow_volume class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_SHADOW_VOLUME_H_
#define _US_D3D9_EFFECT_SHADOW_VOLUME_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_effect_shadow_volume 
		: public d3d9_effect_common_mesh
	{
	protected:
		d3d9_effect_shadow_volume(void){}

	public:
		d3d9_effect_shadow_volume(render_device* device);		

		virtual ~d3d9_effect_shadow_volume( void );

	public:
		//bool set_world_view_project_matrix( const matrix4<float>& wvp );

		//bool set_world_view_matrix( const matrix4<float>& wv );

		//bool set_proj_matrix( const matrix4<float>& proj );

		//bool set_inv_world_view_matrix( const matrix4<float>& iwv );

		//bool set_light_range( float v_range );

		//bool set_light_pos( const vector3<float>& v_pos );

		bool set_light_dir( const vector3<float>& v_dir );

		bool set_far_plane( const float& in_far_plane );

		bool set_line_width( const float& in_line_width_plane );

		//bool set_light_color( const vector4<float>& v_color );

		// override
	public:
		virtual void set_vertex_declaration( uint decl );

		virtual void set_texture( ulong stage_index, texture_handle handle );

		virtual void set_world_matrix( matrix4<float>& in_world_mat );

		virtual void set_view_matrix( matrix4<float>& in_view_mat );

		virtual void set_projection_matrix( matrix4<float>& in_proj_mat );

		virtual void push_transform( void );

		virtual void multiply_transform( matrix4<float>& mat );

		virtual void pop_transform( void );

		virtual void draw( hardware_mesh* in_mesh );

		virtual bool commit_changes( void );

	protected:
		virtual bool setup_handle( void );

	protected:
		D3DXHANDLE		m_world_view_proj_matrix_handle;

		D3DXHANDLE		m_world_view_matrix_handle;

		D3DXHANDLE		m_proj_matrix_handle;

		D3DXHANDLE		m_inv_world_view_matrix_handle;

		D3DXHANDLE		m_light_range_handle;

		D3DXHANDLE		m_light_pos_handle;

		D3DXHANDLE		m_light_dir_handle;

		D3DXHANDLE		m_light_color_handle;

		D3DXHANDLE		m_far_plane_handle;

		D3DXHANDLE		m_line_width_handle;

	};
}

#endif // _US_D3D9_EFFECT_SHADOW_VOLUME_H_ 