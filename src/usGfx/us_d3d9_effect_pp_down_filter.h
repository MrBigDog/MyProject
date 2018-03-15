
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_pp_down_filter.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_pp_down_filter class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_PP_DOWN_FILTER_H_
#define _US_D3D9_EFFECT_PP_DOWN_FILTER_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_effect_pp_down_filter : public d3d9_effect_pp
	{
	protected:
		d3d9_effect_pp_down_filter(void){}

	public:
		d3d9_effect_pp_down_filter(render_device* device);		

		virtual ~d3d9_effect_pp_down_filter( void );

	public:
		bool set_src_texture( ULONG_PTR ht );

		bool set_dest_texture( ULONG_PTR ht );

		float get_scale_x( void ) { return 0.25f; }

		float get_scale_y( void ) { return 0.25f; }

	protected:
		virtual bool setup_handle( void );

	protected:
		D3DXHANDLE		m_src_texture_handle;

		D3DXHANDLE		m_dest_texture_handle;
	};
}

#endif // _US_D3D9_EFFECT_PP_DOWN_FILTER_H_ 