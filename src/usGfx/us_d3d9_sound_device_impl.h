///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_sound_device_impl.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_sound_device_impl class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_SOUND_DEVICE_IMPL_H_
#define _US_D3D9_SOUND_DEVICE_IMPL_H_
#include <usGfx/Export.h>
#include <usCore/us_sound_device.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_vector3.h>

#include <dsound.h>
#include <map>

namespace uniscope_globe
{
	class sound_base;

	class USGFX_EXPORT d3d9_sound_device_impl : public sound_device
	{
	public:
		typedef std::map<ustring, sound_base*> sound_array;

	protected:
		d3d9_sound_device_impl(void) {}

	public:
		d3d9_sound_device_impl(HWND hwnd);

		virtual ~d3d9_sound_device_impl(void);

	public:
		virtual bool create(HWND hwnd, dword coop_level);

		virtual void destory(void);

		inline ULONG_PTR get_direct_sound(void) { return (ULONG_PTR)m_direct_sound; }

		//int  set_primary_buffer_format( ulong primary_channels, ulong primary_freq, ulong primary_bit_rate );

		//int  get_3dlistener_interface( LPDIRECTSOUND3DLISTENER* ppDSListener );
	public:
		virtual void render(vector3<double>& vec) { }

		sound_base* get_sound(ustring str_name);

	public:
		long create_sound_from_file(sound_base** out_sound, LPWSTR wave_file_name, ulong creation_flags, GUID guid3DAlgorithm, ulong number_of_buffer);

	protected:
		IDirectSound8* m_direct_sound;

		sound_array m_sound_array;

		//public:
		//	CSoundManager();
		//	~CSoundManager();

		//	HRESULT Initialize( HWND hWnd, DWORD dwCoopLevel );
		//	inline  LPDIRECTSOUND8 GetDirectSound() { return m_pDS; }
		//	HRESULT SetPrimaryBufferFormat( DWORD dwPrimaryChannels, DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate );
		//	HRESULT Get3DListenerInterface( LPDIRECTSOUND3DLISTENER* ppDSListener );

		//	HRESULT Create( CSound** ppSound, LPWSTR strWaveFileName, DWORD dwCreationFlags = 0, GUID guid3DAlgorithm = GUID_NULL, DWORD dwNumBuffers = 1 );
		//	HRESULT CreateFromMemory( CSound** ppSound, BYTE* pbData, ULONG ulDataSize, LPWAVEFORMATEX pwfx, DWORD dwCreationFlags = 0, GUID guid3DAlgorithm = GUID_NULL, DWORD dwNumBuffers = 1 );
		//	HRESULT CreateStreaming( CStreamingSound** ppStreamingSound, LPWSTR strWaveFileName, DWORD dwCreationFlags, GUID guid3DAlgorithm, DWORD dwNotifyCount, DWORD dwNotifySize, HANDLE hNotifyEvent );


	};
}

#endif // _US_D3D9_SOUND_DEVICE_IMPL_H_