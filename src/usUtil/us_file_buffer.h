///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_file_buffer.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : file_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_FILE_BUFFER_H_
#define _US_FILE_BUFFER_H_

#include <usUtil/Export.h>
#include <usUtil/us_common_file.h>

using namespace std;

namespace uniscope_globe
{
	class USUTIL_EXPORT file_buffer
	{
	public:
		file_buffer(void);

		virtual ~file_buffer(void);

	public:
		bool file_open(cpstr str_path, cpstr open_type);

		void file_close(void);

		void file_flush(void);

		void seek_to_begin(void);

		void seek_to_end(void);

		void seek_to(ulong v_pos);

		interior_index get_size(void);

		bool is_eof(void);

	public:
		ulong write(void* v_data, ulong data_size);

		ulong write(string& str);

		ulong write(wstring& str);

		ulong read_line(string& str);

		ulong read(void* v_data, ulong data_size);

		ulong read(string& str);

		ulong read(wstring& str);

	public:
		int get_tell();

		bool get_error();

	protected:
		FILE* m_file;

		bool  m_valid;

		interior_index m_size;
	};
}

#endif // _US_FILE_BUFFER_H_