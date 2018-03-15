///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_vertex_blend.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_vertex_blend class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_VERTEX_BLEND_H_
#define _US_D3D9_EFFECT_VERTEX_BLEND_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_effect_vertex_blend
		: public d3d9_effect_common_mesh
	{
	protected:
		d3d9_effect_vertex_blend(void){}

	public:
		d3d9_effect_vertex_blend(render_device* device, ulong resource_id );		

		virtual ~d3d9_effect_vertex_blend( void );

	public:
		virtual void set_texture( ulong stage_index, texture_handle handle );

		virtual void set_world_matrix( matrix4<float>& in_world_mat );

		virtual void set_view_matrix( matrix4<float>& in_view_mat );

		virtual void set_projection_matrix( matrix4<float>& in_proj_mat );

		virtual void push_transform( void );

		virtual void multiply_transform( matrix4<float>& mat );

		virtual void pop_transform( void );

		bool set_matrix_array(void* matrix_array, ulong count );

		virtual bool commit_changes( void );

	protected:
		virtual bool setup_handle( void );

	protected:
		D3DXHANDLE	m_transformation;

		D3DXHANDLE	m_world_view_projection;

		D3DXHANDLE	m_texture;
	};
}

#endif // _US_D3D9_EFFECT_VERTEX_BLEND_H_