///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_common_mesh_gpu.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_common_mesh_gpu class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_COMMON_MESH_GPU_H_
#define _US_D3D9_EFFECT_COMMON_MESH_GPU_H_

#include <usGfx/Export.h>
#include <usGfx/us_d3d9_effect_common_mesh.h>
#include <usUtil/us_common_file.h>

namespace uniscope_globe
{
	class USGFX_EXPORT d3d9_effect_common_mesh_gpu
		: public d3d9_effect_common_mesh
	{
	protected:
		d3d9_effect_common_mesh_gpu( void ) {}

	public:
		d3d9_effect_common_mesh_gpu( render_device* device );

		virtual ~d3d9_effect_common_mesh_gpu( void );

	public:
		virtual void set_vertex_declaration( uint decl );

		virtual void set_texture( ulong stage_index, texture_handle handle );

		virtual void set_blank_texture( ulong stage_index );

		virtual void set_transparency_texture( ulong stage_index, ulong texture_idx = 0 );

		virtual void set_material( ULONG_PTR material_handle );

		virtual void set_world_matrix( matrix4<float>& in_world_mat );

		virtual void set_view_matrix( matrix4<float>& in_view_mat );

		virtual void set_projection_matrix( matrix4<float>& in_proj_mat );

		virtual void push_transform( void );

		virtual void multiply_transform( matrix4<float>& mat );

		virtual void pop_transform( void ); 

		virtual bool commit_changes(void);

	protected:
		virtual bool setup_handle( void );

	protected:
		D3DXHANDLE	m_transform_handle;

		D3DXHANDLE	m_texture_handle[2];

	};
}

#endif // _US_D3D9_EFFECT_COMMON_MESH_GPU_H_ 

