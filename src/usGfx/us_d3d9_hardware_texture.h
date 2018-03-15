///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_hardware_texture.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_hardware_texture class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_HARDWARE_TEXTURE_H_
#define _US_D3D9_HARDWARE_TEXTURE_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_hardware_texture
		: public hardware_texture
	{
	public:
		d3d9_hardware_texture( void );

	public:
		d3d9_hardware_texture( raw_buffer* in_buffer, texture_info& tex_info, bool copy_mode = true );

		d3d9_hardware_texture( void* text_handle );

		virtual ~d3d9_hardware_texture( void );

	public:
		//bool CreateFromFileMemory( vgRender* pRender, vgRawBuffer* pBuf );
		bool create_from_file(  render_device* device, const LPCTSTR texture_path );

		bool create_from_resource( render_device* device, ULONG_PTR handle );

		virtual long create( render_device* device );

		virtual bool destroy( void );

		virtual bool refresh( void );

	public:
		virtual void draw( render_device* device, uint stage_index );

		virtual ULONG_PTR get_handle( void );

	private:
		LPDIRECT3DTEXTURE9	m_d3d_texture;

		raw_buffer*			m_raw_buffer;

		bool				m_copy_mode;
		 
	};
}

#endif // _US_D3D9_HARDWARE_TEXTURE_H_