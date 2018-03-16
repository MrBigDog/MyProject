///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_sound_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : sound_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SOUND_BASE_H_
#define _US_SOUND_BASE_H_

#include <usGfx/Export.h>
#include <usUtil/us_common_file.h>

#include <windows.h>

namespace uniscope_globe
{
	class USGFX_EXPORT sound_base
	{
	public:
		sound_base(void);

		virtual ~sound_base(void);

	public:
		virtual long play(DWORD dwPriority = 0, DWORD dwFlags = 0, LONG lVolume = 0, LONG lFrequency = -1, LONG lPan = 0) { return false; }

		//virtual long play3d( LPDS3DBUFFER p3DBuffer, DWORD dwPriority = 0, DWORD dwFlags = 0, LONG lFrequency = 0 ) { return false; } 

		virtual long stop() { return false; }

		virtual long reset() { return false; }

		virtual bool is_sound_playing() { return false; }
	};


	////-----------------------------------------------------------------------------
	//// Name: class CSound
	//// Desc: Encapsulates functionality of a DirectSound buffer.
	////-----------------------------------------------------------------------------
	//class CSound
	//{
	//protected:
	//	LPDIRECTSOUNDBUFFER* m_apDSBuffer;
	//	DWORD                m_dwDSBufferSize;
	//	CWaveFile*           m_pWaveFile;
	//	DWORD                m_dwNumBuffers;
	//	DWORD                m_dwCreationFlags;

	//	HRESULT RestoreBuffer( LPDIRECTSOUNDBUFFER pDSB, BOOL* pbWasRestored );

	//public:
	//	CSound( LPDIRECTSOUNDBUFFER* apDSBuffer, DWORD dwDSBufferSize, DWORD dwNumBuffers, CWaveFile* pWaveFile, DWORD dwCreationFlags );
	//	virtual ~CSound();

	//	HRESULT Get3DBufferInterface( DWORD dwIndex, LPDIRECTSOUND3DBUFFER* ppDS3DBuffer );
	//	HRESULT FillBufferWithSound( LPDIRECTSOUNDBUFFER pDSB, BOOL bRepeatWavIfBufferLarger );
	//	LPDIRECTSOUNDBUFFER GetFreeBuffer();
	//	LPDIRECTSOUNDBUFFER GetBuffer( DWORD dwIndex );

	//	HRESULT Play( DWORD dwPriority = 0, DWORD dwFlags = 0, LONG lVolume = 0, LONG lFrequency = -1, LONG lPan = 0 );
	//	HRESULT Play3D( LPDS3DBUFFER p3DBuffer, DWORD dwPriority = 0, DWORD dwFlags = 0, LONG lFrequency = 0 );
	//	HRESULT Stop();
	//	HRESULT Reset();
	//	BOOL    IsSoundPlaying();
	//};


	//-----------------------------------------------------------------------------
	// Name: class CStreamingSound
	// Desc: Encapsulates functionality to play a wave file with DirectSound.  
	//       The Create() method loads a chunk of wave file into the buffer, 
	//       and as sound plays more is written to the buffer by calling 
	//       HandleWaveStreamNotification() whenever hNotifyEvent is signaled.
	//-----------------------------------------------------------------------------
	//class CStreamingSound : public CSound
	//{
	//protected:
	//	DWORD m_dwLastPlayPos;
	//	DWORD m_dwPlayProgress;
	//	DWORD m_dwNotifySize;
	//	DWORD m_dwNextWriteOffset;
	//	BOOL  m_bFillNextNotificationWithSilence;

	//public:
	//	CStreamingSound( LPDIRECTSOUNDBUFFER pDSBuffer, DWORD dwDSBufferSize, CWaveFile* pWaveFile, DWORD dwNotifySize );
	//	~CStreamingSound();

	//	HRESULT HandleWaveStreamNotification( BOOL bLoopedPlay );
	//	HRESULT Reset();
	//};

}

#endif // _US_SOUND_BASE_H_