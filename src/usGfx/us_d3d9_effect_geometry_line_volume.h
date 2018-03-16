
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_geometry_line_volume.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_geometry_line_volume class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_GEOMETRY_LINE_VOLUME_H_
#define _US_D3D9_EFFECT_GEOMETRY_LINE_VOLUME_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_effect_base.h>
#include <usUtil/us_matrix4.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{
	class USGFX_EXPORT d3d9_effect_geometry_line_volume : public d3d9_effect_base
	{
	protected:
		d3d9_effect_geometry_line_volume(void){}

	public:
		d3d9_effect_geometry_line_volume(render_device* device);		

		virtual ~d3d9_effect_geometry_line_volume( void );

	public:
		bool set_transform( const matrix4<float>& wvp );

		bool set_view_proj_matrix( const matrix4<float>& tm );

		bool set_line_width( float line_width );

	protected:
		virtual bool setup_handle( void );

	protected:
		D3DXHANDLE		m_transform_handle;

		D3DXHANDLE		m_view_proj_matrix_handle;

		D3DXHANDLE		m_view_port_scale_handle;

		D3DXHANDLE		m_half_width_handle;

		//D3DXHANDLE		m_view_direction_handle;
	};
}

#endif // _US_D3D9_EFFECT_GEOMETRY_LINE_VOLUME_H_ 