
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_pp_combine_dof.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_pp_combine_dof class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_PP_COMBINE_DOF_H_
#define _US_D3D9_EFFECT_PP_COMBINE_DOF_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_effect_pp_combine_dof : public d3d9_effect_base
	{
	protected:
		d3d9_effect_pp_combine_dof(void){}

	public:
		d3d9_effect_pp_combine_dof(render_device* device);		

		virtual ~d3d9_effect_pp_combine_dof( void );

	public:
		bool set_src_texture( ULONG_PTR ht );

		bool set_dest_texture( ULONG_PTR ht );

	protected:
		virtual bool setup_handle( void );

	protected:
		D3DXHANDLE		m_src_texture_handle;

		D3DXHANDLE		m_dest_texture_handle;

	};
}

#endif // _US_D3D9_EFFECT_PP_COMBINE_DOF_H_ 