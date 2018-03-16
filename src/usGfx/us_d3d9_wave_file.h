///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_wave_file.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_wave_file class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_WAVE_FILE_H_
#define _US_D3D9_WAVE_FILE_H_

#include <usGfx/Export.h>
#include <usUtil/us_common_file.h>

#include <windows.h>

#define WAVEFILE_READ   1
#define WAVEFILE_WRITE  2
#define DXTRACE_ERR(str, ret_val) ret_val

namespace uniscope_globe
{
	class USGFX_EXPORT d3d9_wave_file
	{
	public:
		d3d9_wave_file(void);

		virtual ~d3d9_wave_file(void);

	public:
		HRESULT read_mmio();

		HRESULT write_mmio(WAVEFORMATEX *pwfxDest);

		HRESULT open(LPWSTR strFileName, WAVEFORMATEX* pwfx, DWORD dwFlags);

		HRESULT open_from_memory(BYTE* pbData, ULONG ulDataSize, WAVEFORMATEX* pwfx, DWORD dwFlags);

		HRESULT close();

		HRESULT read(BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead);

		HRESULT write(UINT nSizeToWrite, BYTE* pbData, UINT* pnSizeWrote);

		DWORD   get_size();

		HRESULT reset_file();

		WAVEFORMATEX* get_format() { return m_pwfx; };

	public:
		WAVEFORMATEX* m_pwfx;        // Pointer to WAVEFORMATEX structure

		HMMIO         m_hmmio;       // MM I/O handle for the WAVE

		MMCKINFO      m_ck;          // Multimedia RIFF chunk

		MMCKINFO      m_ckRiff;      // Use in opening a WAVE file

		DWORD         m_dwSize;      // The size of the wave file

		MMIOINFO      m_mmioinfoOut;

		DWORD         m_dwFlags;

		BOOL          m_bIsReadingFromMemory;

		BYTE*         m_pbData;

		BYTE*         m_pbDataCur;

		ULONG         m_ulDataSize;

		CHAR*         m_pResourceBuffer;



	};
}

#endif // _US_D3D9_WAVE_FILE_H_