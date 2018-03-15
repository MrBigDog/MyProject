///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_EFFECT_BASE_H_
#define _US_D3D9_EFFECT_BASE_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_effect_base : public render_effect
	{
	protected:
		d3d9_effect_base(void){}

	public:
		d3d9_effect_base( render_device* rd );

		virtual ~d3d9_effect_base( void );

	public:
		virtual bool load_from_file(const LPCTSTR technique_file);

		virtual bool load_from_module_resource( ULONG_PTR resource_handle );

		virtual long on_lost_device( void );

		virtual long on_reset_device( void );

		virtual bool begin( uint* pass, ulong flags );

		virtual bool end( void );

		virtual bool begin_pass( uint pass );

		virtual bool end_pass(void);

		virtual bool set_technique(void);

		virtual bool commit_changes(void);

	protected:
		virtual bool setup_handle( void );

	protected:
		ID3DXEffect*	m_d3dx_effect;

		D3DXHANDLE		m_technique;
		 
	};
}

#endif // _US_D3D9_EFFECT_BASE_H_