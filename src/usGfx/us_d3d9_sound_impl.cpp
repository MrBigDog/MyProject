///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_sound_impl.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_sound_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_d3d9_sound_impl.h"

namespace uniscope_globe
{
	d3d9_sound_impl::d3d9_sound_impl( LPDIRECTSOUNDBUFFER* in_ds_buffer, ulong ds_buffer_size, ulong number_of_buffers, d3d9_wave_file* wave_file, ulong creation_flags )
	{
		ulong i;

		if( number_of_buffers <= 0 )
			return;


		m_direct_sound_buffer_array = new LPDIRECTSOUNDBUFFER[number_of_buffers];
		if( NULL != m_direct_sound_buffer_array )
		{
			for( i=0; i<number_of_buffers; i++ )
			{
				m_direct_sound_buffer_array[i] = in_ds_buffer[i];
			}

			m_direct_sound_buffer_size = ds_buffer_size;
			m_number_of_buffers   = number_of_buffers;
			m_wave_file      = wave_file;
			m_creation_flags = creation_flags;


			
			fill_buffer_with_sound( m_direct_sound_buffer_array[0], FALSE );

			m_param_buffer.dwSize = sizeof(DS3DBUFFER);
			get_3d_buffer()->GetAllParameters( &m_param_buffer );

			// Set new 3D buffer parameters
			m_param_buffer.dwMode = DS3DMODE_HEADRELATIVE;
			get_3d_buffer()->SetAllParameters( &m_param_buffer, DS3D_IMMEDIATE );

			set_parameter( 0,0, 1, 1000 );
		}
	}

	d3d9_sound_impl::~d3d9_sound_impl( void )
	{
		for( DWORD i=0; i<m_direct_sound_buffer_size; i++ )
		{
			AUTO_RELEASE( m_direct_sound_buffer_array[i] );
		}

		AUTO_DELETE_ARRAY( m_direct_sound_buffer_array );
		AUTO_DELETE( m_wave_file );
	}

	void d3d9_sound_impl::set_parameter( float doppler_factor, float rolloff_factor, float min_distance, float max_distance )
	{
		//DS3DLISTENER v_param_listener;
		//v_param_listener.flMaxDistance = max_distance;
		//v_param_listener.flMinDistance = min_distance;

		//if( g_pDSListener )
		//	g_pDSListener->SetAllParameters( &g_dsListenerParams, dwApplyFlag );

		m_param_buffer.flMinDistance = min_distance;
		m_param_buffer.flMaxDistance = max_distance;

		//if( g_pDS3DBuffer )
		//	g_pDS3DBuffer->SetAllParameters( &g_dsBufferParams, dwApplyFlag );

	}

	//	VOID Set3DParameters( FLOAT fDopplerFactor, FLOAT fRolloffFactor,
	//		FLOAT fMinDistance,   FLOAT fMaxDistance )
	//	{
	//		// Every change to 3-D sound buffer and listener settings causes 
	//		// DirectSound to remix, at the expense of CPU cycles. 
	//		// To minimize the performance impact of changing 3-D settings, 
	//		// use the DS3D_DEFERRED flag in the dwApply parameter of any of 
	//		// the IDirectSound3DListener or IDirectSound3DBuffer methods that 
	//		// change 3-D settings. Then call the IDirectSound3DListener::CommitDeferredSettings 
	//		// method to execute all of the deferred commands at once.
	//		DWORD dwApplyFlag = ( g_bDeferSettings ) ? DS3D_DEFERRED : DS3D_IMMEDIATE;

	//		g_dsListenerParams.flDopplerFactor = fDopplerFactor;
	//		g_dsListenerParams.flRolloffFactor = fRolloffFactor;

	//		if( g_pDSListener )
	//			g_pDSListener->SetAllParameters( &g_dsListenerParams, dwApplyFlag );

	//		g_dsBufferParams.flMinDistance = fMinDistance;
	//		g_dsBufferParams.flMaxDistance = fMaxDistance;

	//		if( g_pDS3DBuffer )
	//			g_pDS3DBuffer->SetAllParameters( &g_dsBufferParams, dwApplyFlag );
	//	}
	//}

	long d3d9_sound_impl::fill_buffer_with_sound( LPDIRECTSOUNDBUFFER ds_buffer, bool b_repeat_wav_if_buffer_larger )
	{
		HRESULT hr;
		void*   ds_locked_buffer      = NULL; // Pointer to locked buffer memory
		ulong   ds_locked_buffer_size = 0;    // Size of the locked DirectSound buffer
		ulong   v_wav_data_read        = 0;    // Amount of data read from the wav file

		if( ds_buffer == NULL ) return CO_E_NOTINITIALIZED;

		// Make sure we have focus, and we didn't just switch in from
		// an app which had a DirectSound device
		if( FAILED( hr = restore_buffer( ds_buffer, NULL ) ) )
			return E_FAIL;

		// Lock the buffer down
		if( FAILED( hr = ds_buffer->Lock( 0, m_direct_sound_buffer_size, &ds_locked_buffer, &ds_locked_buffer_size, NULL, NULL, 0L ) ) )
			return E_FAIL;

		// Reset the wave file to the beginning
		m_wave_file->reset_file();

		if( FAILED( hr = m_wave_file->read( (byte*) ds_locked_buffer, ds_locked_buffer_size, &v_wav_data_read ) ) )
			return E_FAIL;

		if( v_wav_data_read == 0 )
		{
			// Wav is blank, so just fill with silence
			FillMemory( (byte*) ds_locked_buffer, ds_locked_buffer_size, (byte)(m_wave_file->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
		}
		else if( v_wav_data_read < ds_locked_buffer_size )
		{
			// If the wav file was smaller than the DirectSound buffer,
			// we need to fill the remainder of the buffer with data
			if( b_repeat_wav_if_buffer_larger )
			{
				// Reset the file and fill the buffer with wav data
				ulong v_read_sofar = v_wav_data_read;    // From previous call above.
				while( v_read_sofar < ds_locked_buffer_size )
				{
					// This will keep reading in until the buffer is full
					// for very short files
					if( FAILED( hr = m_wave_file->reset_file() ) )
						return E_FAIL;

					hr = m_wave_file->read( (byte*)ds_locked_buffer + v_read_sofar, ds_locked_buffer_size - v_read_sofar, &v_wav_data_read );
					if( FAILED(hr) ) return E_FAIL;

					v_read_sofar += v_wav_data_read;
				}
			}
			else
			{
				// Don't repeat the wav file, just fill in silence
				FillMemory( (byte*) ds_locked_buffer + v_wav_data_read, ds_locked_buffer_size - v_wav_data_read,
							(byte)(m_wave_file->m_pwfx->wBitsPerSample == 8 ? 128 : 0 ) );
			}
		}

		// Unlock the buffer, we don't need it anymore.
		ds_buffer->Unlock( ds_locked_buffer, ds_locked_buffer_size, NULL, 0 );

		return S_OK;
	}

	long d3d9_sound_impl::restore_buffer( LPDIRECTSOUNDBUFFER ds_buffer, bool* b_was_restored )
	{
		HRESULT hr;

		if( ds_buffer == NULL ) return CO_E_NOTINITIALIZED;
		if( b_was_restored )
			*b_was_restored = false;

		ulong v_status;
		if( FAILED( hr = ds_buffer->GetStatus( &v_status ) ) ) return E_FAIL;

		if( v_status & DSBSTATUS_BUFFERLOST )
		{
			// Since the app could have just been activated, then
			// DirectSound may not be giving us control yet, so
			// the restoring the buffer may fail.
			// If it does, sleep until DirectSound gives us control.
			do
			{
				hr = ds_buffer->Restore();
				if( hr == DSERR_BUFFERLOST )
					Sleep( 10 );
			}
			while( ( hr = ds_buffer->Restore() ) == DSERR_BUFFERLOST );

			if( b_was_restored != NULL )
				*b_was_restored = true;

			return S_OK;
		}
		else
		{
			return S_FALSE;
		}
	}


	//-----------------------------------------------------------------------------
	// Name: CSound::GetFreeBuffer()
	// Desc: Finding the first buffer that is not playing and return a pointer to
	//       it, or if all are playing return a pointer to a randomly selected buffer.
	//-----------------------------------------------------------------------------
	LPDIRECTSOUNDBUFFER d3d9_sound_impl::get_free_buffer()
	{
		if( m_direct_sound_buffer_array == NULL ) return NULL;

		ulong i;
		for( i = 0; i < m_number_of_buffers; i++ )
		{
			if( m_direct_sound_buffer_array[i] )
			{
				ulong v_status = 0;
				m_direct_sound_buffer_array[i]->GetStatus( &v_status );
				if ( ( v_status & DSBSTATUS_PLAYING ) == 0 )
					break;
			}
		}

		if( i != m_number_of_buffers )
		{
			return m_direct_sound_buffer_array[ i ];
		}
		else
		{
			return m_direct_sound_buffer_array[ rand() % m_number_of_buffers ];
		}
	}

	LPDIRECTSOUNDBUFFER d3d9_sound_impl::get_buffer( ulong index )
	{
		if( m_direct_sound_buffer_array == NULL ) return NULL;
		if( index >= m_number_of_buffers ) return NULL;
		return m_direct_sound_buffer_array[index];
	}

	long d3d9_sound_impl::get_3dbuffer_interface( ulong index, LPDIRECTSOUND3DBUFFER* out_ds3d_buffer )
	{
		if( m_direct_sound_buffer_array == NULL ) return CO_E_NOTINITIALIZED;
		if( index >= m_number_of_buffers ) return E_INVALIDARG;

		*out_ds3d_buffer = NULL;
		return m_direct_sound_buffer_array[index]->QueryInterface( IID_IDirectSound3DBuffer, (void**)out_ds3d_buffer );
	}

	long d3d9_sound_impl::play( ulong priority, ulong flags, long volume, long frequency, long pan )
	{
		HRESULT hr;
		bool    b_restored;

		if( m_direct_sound_buffer_array == NULL )
			return false;

		LPDIRECTSOUNDBUFFER ds_buffer = get_free_buffer();

		if( ds_buffer == NULL )
			return E_FAIL;

		// Restore the buffer if it was lost
		if( FAILED( hr = restore_buffer( ds_buffer, &b_restored ) ) )
			return hr;

		if( b_restored )
		{
			// The buffer was restored, so we need to fill it with new data
			if( FAILED( hr = fill_buffer_with_sound( ds_buffer, FALSE ) ) )
				return hr;
		}

		if( m_creation_flags & DSBCAPS_CTRLVOLUME )
		{
			ds_buffer->SetVolume( volume );
		}

		if( frequency != -1 &&
			(m_creation_flags & DSBCAPS_CTRLFREQUENCY) )
		{
			ds_buffer->SetFrequency( frequency );
		}

		if( m_creation_flags & DSBCAPS_CTRLPAN )
		{
			ds_buffer->SetPan( pan );
		}

		return ds_buffer->Play( 0, priority, flags );
	}

	long d3d9_sound_impl::play3d(ulong priority, ulong flags, ulong frequency )
	{
		HRESULT hr;
		bool    b_restored;
		//ulong   v_base_frequency;

		if( m_direct_sound_buffer_array == NULL )
			return CO_E_NOTINITIALIZED;

		LPDIRECTSOUNDBUFFER v_current_buffer = get_free_buffer();
		if( v_current_buffer == NULL ) return E_FAIL;

		// Restore the buffer if it was lost
		if( FAILED( hr = restore_buffer( v_current_buffer, &b_restored ) ) )
			return hr;

		if( b_restored )
		{
			// The buffer was restored, so we need to fill it with new data
			if( FAILED( hr = fill_buffer_with_sound( v_current_buffer, false ) ) )
				return hr;
		}

		//if( m_creation_flags & DSBCAPS_CTRLFREQUENCY )
		//{
		//	v_free_buffer->GetFrequency( &v_base_frequency );
		//	v_free_buffer->SetFrequency( v_base_frequency + frequency );
		//}

		// QI for the 3D buffer
		LPDIRECTSOUND3DBUFFER v_current_3d_buffer;
		hr = v_current_buffer->QueryInterface( IID_IDirectSound3DBuffer, (VOID**) &v_current_3d_buffer );
		if( SUCCEEDED( hr ) )
		{
			hr = v_current_3d_buffer->SetAllParameters( &m_param_buffer, DS3D_IMMEDIATE );
			if( SUCCEEDED( hr ) )
			{
				hr = v_current_buffer->Play( 0, priority, flags );
			}
			v_current_3d_buffer->Release();
		}
		return S_OK;
	}

	LPDIRECTSOUND3DBUFFER d3d9_sound_impl::get_3d_buffer()
	{
		HRESULT hr;
		bool    b_restored;
		//ulong   v_base_frequency;

		if( m_direct_sound_buffer_array == NULL )
			return NULL;

		LPDIRECTSOUNDBUFFER v_current_buffer = get_free_buffer();
		if( v_current_buffer == NULL ) return NULL;

		// Restore the buffer if it was lost
		if( FAILED( hr = restore_buffer( v_current_buffer, &b_restored ) ) )
			return NULL;

		if( b_restored )
		{
			// The buffer was restored, so we need to fill it with new data
			if( FAILED( hr = fill_buffer_with_sound( v_current_buffer, false ) ) )
				return NULL;
		}

		// QI for the 3D buffer
		LPDIRECTSOUND3DBUFFER v_current_3d_buffer;
		hr = v_current_buffer->QueryInterface( IID_IDirectSound3DBuffer, (VOID**) &v_current_3d_buffer );

		return v_current_3d_buffer;
	}

	long d3d9_sound_impl::set_position( vector3<float> v_pos )
	{
		LPDIRECTSOUND3DBUFFER v_current_3d_buffer = get_3d_buffer();
		memcpy( (void*)&m_param_buffer.vPosition, (void*)&v_pos, sizeof(D3DVECTOR) );
		v_current_3d_buffer->SetAllParameters( &m_param_buffer, DS3D_IMMEDIATE );
		AUTO_RELEASE( v_current_3d_buffer );
		return 0;
	}

	long d3d9_sound_impl::stop()
	{
		if( m_direct_sound_buffer_array == NULL ) return false;

		HRESULT hr = 0;
		for( DWORD i = 0; i < m_number_of_buffers; i++ )
		{
			hr |= m_direct_sound_buffer_array[i]->Stop();
		}

		return hr;
	}

	long d3d9_sound_impl::reset()
	{
		if( m_direct_sound_buffer_array == NULL ) return false;

		HRESULT hr = 0;
		for( DWORD i = 0; i < m_number_of_buffers; i++ )
		{
			hr |= m_direct_sound_buffer_array[i]->SetCurrentPosition( 0 );
		}

		return hr;
	}

	bool d3d9_sound_impl::is_sound_playing()
	{
		if( m_direct_sound_buffer_array == NULL ) return false;

		bool b_is_playing = false;

		for( ulong i=0; i<m_number_of_buffers; i++ )
		{
			if( m_direct_sound_buffer_array[i] )
			{
				DWORD v_status = 0;
				m_direct_sound_buffer_array[i]->GetStatus( &v_status );
				b_is_playing |= ( ( v_status & DSBSTATUS_PLAYING ) != 0 );
			}
		}

		return b_is_playing;
	}	
}