///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_sound_impl.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_sound_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_SOUND_IMPL_H_
#define _US_D3D9_SOUND_IMPL_H_
#include <usGfx/Export.h>
#include <usGfx/us_sound_base.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_vector3.h>
#include <dsound.h>

namespace uniscope_globe
{
	class d3d9_wave_file;

	class USGFX_EXPORT d3d9_sound_impl : public sound_base
	{
	protected:
		d3d9_sound_impl(void) {}

	public:
		d3d9_sound_impl(LPDIRECTSOUNDBUFFER* out_ds_buffer, ulong ds_buffer_size, ulong number_of_buffers, d3d9_wave_file* wave_file, ulong creation_flags);

		virtual ~d3d9_sound_impl(void);

	public:
		virtual long play(ulong priority = 0, ulong flags = 0, long volume = 0, long frequency = -1, long lPan = 0);

		virtual long play3d(ulong priority = 0, ulong flags = 0, ulong frequency = 0);

		virtual long set_position(vector3<float> v_pos);

		virtual long stop();

		virtual long reset();

		virtual bool is_sound_playing();

		void set_parameter(float doppler_factor, float rolloff_factor, float min_distance, float max_distance);

	protected:
		long fill_buffer_with_sound(LPDIRECTSOUNDBUFFER ds_buffer, bool b_repeat_wav_if_buffer_larger);

		long restore_buffer(LPDIRECTSOUNDBUFFER ds_buffer, bool* b_was_restored);

		long get_3dbuffer_interface(ulong index, LPDIRECTSOUND3DBUFFER* out_ds3d_buffer);

		LPDIRECTSOUND3DBUFFER get_3d_buffer();

		LPDIRECTSOUNDBUFFER get_free_buffer();

		LPDIRECTSOUNDBUFFER get_buffer(ulong index);

	protected:
		LPDIRECTSOUNDBUFFER* m_direct_sound_buffer_array;

		ulong                m_direct_sound_buffer_size;

		LPDIRECTSOUNDBUFFER  m_current_buffer;

		DS3DBUFFER			 m_param_buffer;

		d3d9_wave_file*      m_wave_file;

		ulong                m_number_of_buffers;

		ulong                m_creation_flags;
	};

}

#endif // _US_D3D9_SOUND_IMPL_H_