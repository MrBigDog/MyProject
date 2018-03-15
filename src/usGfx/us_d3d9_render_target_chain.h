
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_render_target_chain.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_render_target_chain class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_RENDER_TARGET_CHAIN_H_
#define _US_D3D9_RENDER_TARGET_CHAIN_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_render_target_chain
		: public render_target
	{
	protected:
		d3d9_render_target_chain( void ) {}

	public:
		d3d9_render_target_chain( render_device* rd );

		virtual ~d3d9_render_target_chain( void );

	public:
		bool create( uint width, uint height, int format, int multi_sample_type, ulong multi_sample_quality, bool lockable = false );

		//bool create_offline( uint width, uint height, int format, int multi_sample_type, ulong multi_sample_quality, bool lockable );

		void destroy( void );

	public:
		virtual void on_lost_device( event_argument* args );

		virtual void on_reset_device( event_argument* args );

		virtual void begin( void );

		virtual void end( void );

		virtual ULONG_PTR get_texture( void );

		virtual void draw_texture( int stage );

		//virtual void save_file( cpstr str_path, int width, int height );

		virtual void flip( void );

		virtual	void set_render_target( int n_stage );

	public:
		std::vector<LPDIRECT3DTEXTURE9>		m_texture_array;

		std::vector<LPDIRECT3DSURFACE9>		m_surface_array;

		LPDIRECT3DDEVICE9		m_d3d_device;

		LPDIRECT3DSURFACE9		m_old_target_surface;

		render_device*			m_device;

		ulong					m_cur_curser;

		ulong					m_next_curser;

		ulong					m_chain_count;

	};
}

#endif // _US_D3D9_RENDER_TARGET_CHAIN_H_ 