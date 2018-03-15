
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_pp_combine_hdr.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_pp_combine_hdr class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_PP_COMBINE_HSL_H_
#define _US_D3D9_EFFECT_PP_COMBINE_HSL_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_effect_pp_combine_hsl : public d3d9_effect_pp
	{
	protected:
		d3d9_effect_pp_combine_hsl(void){}

	public:
		d3d9_effect_pp_combine_hsl(render_device* device);		

		virtual ~d3d9_effect_pp_combine_hsl( void );

	public:
		bool set_src_texture( ULONG_PTR ht );

		bool set_dest_texture( ULONG_PTR ht );

		bool set_saturation( float val );

		bool set_luminance( float val );

		bool set_contract( float val );

	protected:
		virtual bool setup_handle( void );

	protected:
		D3DXHANDLE		m_src_texture_handle;

		D3DXHANDLE		m_dest_texture_handle;

		D3DXHANDLE		m_saturation_handle;

		D3DXHANDLE		m_luminance_handle;

		D3DXHANDLE		m_contract_handle;

	};
}

#endif // _US_D3D9_EFFECT_PP_COMBINE_HSL_H_ 