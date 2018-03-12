///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_raw_buffer.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : raw_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RAW_BUFFER_H_
#define _US_RAW_BUFFER_H_

#include <usUtil\Export.h>
#include <usUtil\us_common_file.h>

#include <vector>
#include <string>

using namespace std;

namespace uniscope_globe
{
	class USUTIL_EXPORT raw_buffer
	{
	public:
		raw_buffer(void);

		raw_buffer(raw_buffer& buffer);

		raw_buffer(byte* p_data, ulong data_size);

		virtual ~raw_buffer(void);

	public:
		void seek_to_begin(void);

		void seek_to_end(void);

		int seek_to(ulong v_pos);

		ulong get_size(void);

		ulong get_cursor(void);

	public:
		virtual void attach(byte* v_data, ulong v_size);

		virtual void  clear(void);

		virtual byte* get_buffer(void);

		virtual void resize(ulong v_size);

		virtual ulong write(void* v_data, ulong data_size);

		virtual ulong write(string& str);

		virtual ulong write(wstring& str);

		virtual ulong read(void* v_data, ulong data_size);

		virtual ulong read(string& str);

		virtual ulong read(wstring& str);

	protected:
		ulong m_size;
		ulong m_cursor;
		byte* m_buffer;
	};

	typedef std::vector<raw_buffer> simple_buffer_array;

}

#endif // _US_RAW_BUFFER_H_