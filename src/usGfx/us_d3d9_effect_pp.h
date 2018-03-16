
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_pp.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_pp class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_PP_H_
#define _US_D3D9_EFFECT_PP_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_effect_base.h>
#include <usUtil/us_matrix4.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class USGFX_EXPORT d3d9_effect_pp : public d3d9_effect_base
	{
	protected:
		d3d9_effect_pp(void){}

	public:
		d3d9_effect_pp(render_device* device);		

		virtual ~d3d9_effect_pp( void );

	public:
		virtual bool set_src_texture( ULONG_PTR ht ) { return true; }

		float get_scale_x( void ) { return 1.0f; }

		float get_scale_y( void ) { return 1.0f; }

	public:
		virtual long on_reset_device( void );

	protected:
		virtual bool setup_handle( void ){ return true; }

	protected:
		D3DXHANDLE		m_texture_handle;
		 
	};
}

#endif // _US_D3D9_EFFECT_PP_H_ 