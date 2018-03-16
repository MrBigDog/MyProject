///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: d3d9_sound_buffer.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_sound_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _D3D9_SOUND_BUFFER_H_
#define _D3D9_SOUND_BUFFER_H_

namespace uniscope_globe
{
	class d3d9_sound_buffer
	{
	public:
		d3d9_sound_buffer( void );

		virtual ~d3d9_sound_buffer( void );

	public:
		void set_position( vector3<double>& v_position );

	protected:
		LPDIRECTSOUND3DBUFFER m_ds3d_buffer;

		DS3DBUFFER	m_ds_buffer_params;
		 
	};
}

#endif // _D3D9_SOUND_BUFFER_H_ 