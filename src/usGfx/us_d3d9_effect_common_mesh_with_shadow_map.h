
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_common_mesh_with_shadow_map.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_common_mesh_with_shadow_map class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_COMMON_MESH_WITH_SHAODOW_MAP_H_
#define _US_D3D9_EFFECT_COMMON_MESH_WITH_SHAODOW_MAP_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_effect_common_mesh_with_shadow_map 
		: public d3d9_effect_common_mesh
	{
	protected:
		d3d9_effect_common_mesh_with_shadow_map( void ) {}

	public:
		d3d9_effect_common_mesh_with_shadow_map( render_device* device );

		virtual ~d3d9_effect_common_mesh_with_shadow_map( void );

	public:
		virtual void set_vertex_declaration( uint decl );

		virtual void set_texture( ulong stage_index, texture_handle handle );

		virtual void set_blank_texture( ulong stage_index );

		virtual void set_material( ULONG_PTR material_handle );

		virtual void set_transparency_texture( ulong stage_index, ulong texture_idx = 0 );

		virtual void set_world_matrix( matrix4<float>& in_world_mat );

		virtual void set_view_matrix( matrix4<float>& in_view_mat );

		virtual void set_projection_matrix( matrix4<float>& in_proj_mat );

		virtual void push_transform( void );

		virtual void multiply_transform( matrix4<float>& mat );

		virtual void pop_transform( void );

		virtual bool commit_changes(void);

	public:
		virtual void set_shadow_map_technique();

		virtual void set_scene_technique();

	public:
		virtual void set_light_view_projection_matrix( matrix4<float>& in_view_mat );

		virtual void set_light_position( vector_3f& in_light_pos );

		virtual void set_eye_position( vector_3f& in_eye_pos );

		virtual void set_light_color( vector4<float>& in_light_color );
		
		virtual void set_filtered( bool in_filtered );

		virtual void set_shadow_map_texture( ulong stage_index, texture_handle handle );

	protected:
		virtual bool setup_handle( void );

	protected:
		D3DXHANDLE	m_transform_handle;

		D3DXHANDLE	m_texture_handle[2];
	};
}

#endif // _US_D3D9_EFFECT_COMMON_MESH_WITH_SHAODOW_MAP_H_ 