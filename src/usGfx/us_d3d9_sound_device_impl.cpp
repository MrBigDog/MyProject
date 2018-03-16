///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_sound_device_impl.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_sound_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_sound_device_impl.h"
#include <usGfx/us_d3d9_wave_file.h>
#include <usGfx/us_d3d9_sound_impl.h>

namespace uniscope_globe
{
	d3d9_sound_device_impl::d3d9_sound_device_impl(HWND hwnd)
	{
		m_direct_sound = NULL;
		create(hwnd, DSSCL_PRIORITY);
	}

	d3d9_sound_device_impl::~d3d9_sound_device_impl(void)
	{
		destory();
	}

	bool d3d9_sound_device_impl::create(HWND hwnd, dword coop_level)
	{
		AUTO_RELEASE(m_direct_sound);

		HRESULT hr;
		// Create IDirectSound using the primary sound device
		if (FAILED(hr = DirectSoundCreate8(NULL, &m_direct_sound, NULL)))
			return false;

		// Set DirectSound coop level
		if (FAILED(hr = m_direct_sound->SetCooperativeLevel(hwnd, coop_level)))
			return false;

		return S_OK;
	}

	void d3d9_sound_device_impl::destory(void)
	{
		AUTO_RELEASE(m_direct_sound);
	}

	sound_base* d3d9_sound_device_impl::get_sound(ustring str_name)
	{
		sound_base* v_sound_base = NULL;
		sound_array::iterator itr = m_sound_array.find(str_name);
		if (itr != m_sound_array.end())
		{
			return itr->second;
		}

		GUID guid;
		this->create_sound_from_file(&v_sound_base, LPWSTR(str_name.c_str()), 0, guid, 1);
		return v_sound_base;
	}

	long d3d9_sound_device_impl::create_sound_from_file(sound_base** out_sound, LPWSTR wave_file_name, ulong creation_flags, GUID guid_3d_algorithm, ulong number_of_buffer)
	{
		HRESULT hr;
		HRESULT hrRet = S_OK;
		ulong   i;
		LPDIRECTSOUNDBUFFER* ds_buffer_array = NULL;
		ulong                ds_buffer_size = NULL;
		d3d9_wave_file*      new_wave_file = NULL;

		if (m_direct_sound == NULL) return CO_E_NOTINITIALIZED;
		if (wave_file_name == NULL || number_of_buffer < 1)
			return E_INVALIDARG;

		ds_buffer_array = new LPDIRECTSOUNDBUFFER[number_of_buffer];
		if (ds_buffer_array == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto LFail;
		}

		new_wave_file = new d3d9_wave_file();
		if (new_wave_file == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto LFail;
		}

		new_wave_file->open(wave_file_name, NULL, WAVEFILE_READ);

		if (new_wave_file->get_size() == 0)
		{
			// Wave is blank, so don't create it.
			hr = E_FAIL;
			goto LFail;
		}

		// Make the DirectSound buffer the same size as the wav file
		ds_buffer_size = new_wave_file->get_size();

		// Create the direct sound buffer, and only request the flags needed
		// since each requires some overhead and limits if the buffer can
		// be hardware accelerated
		DSBUFFERDESC dsbd;
		ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = creation_flags;
		dsbd.dwBufferBytes = ds_buffer_size;
		dsbd.guid3DAlgorithm = guid_3d_algorithm;
		dsbd.lpwfxFormat = new_wave_file->m_pwfx;

		// DirectSound is only guarenteed to play PCM data.  Other
		// formats may or may not work depending the sound card driver.
		hr = m_direct_sound->CreateSoundBuffer(&dsbd, &ds_buffer_array[0], NULL);

		// Be sure to return this error code if it occurs so the
		// callers knows this happened.
		if (hr == DS_NO_VIRTUALIZATION)
			hrRet = DS_NO_VIRTUALIZATION;

		if (FAILED(hr))
		{
			// DSERR_BUFFERTOOSMALL will be returned if the buffer is
			// less than DSBSIZE_FX_MIN and the buffer is created
			// with DSBCAPS_CTRLFX.

			// It might also fail if hardware buffer mixing was requested
			// on a device that doesn't support it.
			//DXUT_ERR( L"CreateSoundBuffer", hr );


			goto LFail;
		}

		// Default to use DuplicateSoundBuffer() when created extra buffers since always
		// create a buffer that uses the same memory however DuplicateSoundBuffer() will fail if
		// DSBCAPS_CTRLFX is used, so use CreateSoundBuffer() instead in this case.
		if ((creation_flags & DSBCAPS_CTRLFX) == 0)
		{
			for (i = 1; i < number_of_buffer; i++)
			{
				if (FAILED(hr = m_direct_sound->DuplicateSoundBuffer(ds_buffer_array[0], &ds_buffer_array[i])))
				{
					//DXUT_ERR( L"DuplicateSoundBuffer", hr );
					goto LFail;
				}
			}
		}
		else
		{
			for (i = 1; i < number_of_buffer; i++)
			{
				hr = m_direct_sound->CreateSoundBuffer(&dsbd, &ds_buffer_array[i], NULL);
				if (FAILED(hr))
				{
					//DXUT_ERR( L"CreateSoundBuffer", hr );
					goto LFail;
				}
			}
		}

		// Create the sound
		*out_sound = new d3d9_sound_impl(ds_buffer_array, ds_buffer_size, number_of_buffer, new_wave_file, creation_flags);

		AUTO_DELETE_ARRAY(ds_buffer_array);
		return hrRet;

	LFail:
		// Cleanup
		AUTO_DELETE(new_wave_file);
		AUTO_DELETE_ARRAY(ds_buffer_array);
		return hr;
	}





}