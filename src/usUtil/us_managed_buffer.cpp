///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_managed_buffer.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : managed_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_managed_buffer.h"

namespace uniscope_globe
{
	managed_buffer::managed_buffer(void)
	{
		m_size = 0;
		m_cursor = 0;
		m_buffer = NULL;
	}

	managed_buffer::managed_buffer(managed_buffer& v_buffer)
	{
		m_buffer = NULL;
		m_size = v_buffer.get_size();
		m_cursor = 0;
		m_stream_array.resize(m_size);
		memcpy((void*)&m_stream_array[0], (void*)v_buffer.get_buffer(), m_size);
	}

	managed_buffer::managed_buffer(ulong v_size)
	{
		m_buffer = NULL;
		m_size = v_size;
		m_cursor = 0;
		m_stream_array.resize(m_size);
	}

	managed_buffer::managed_buffer(byte* v_data, ulong v_size)
	{
		m_buffer = NULL;
		m_size = v_size;
		m_cursor = 0;
		m_stream_array.resize(m_size);
		memcpy((void*)&m_stream_array[0], (void*)v_data, m_size);
	}

	managed_buffer::~managed_buffer(void)
	{
		m_stream_array.clear();
	}

	void managed_buffer::attach(byte* v_data, ulong v_size)
	{
		m_stream_array.clear();
		m_cursor = 0;
		m_size = v_size;
		m_stream_array.resize(v_size);
		memcpy((void*)&m_stream_array[0], (void*)v_data, m_size);
	}

	void managed_buffer::copy(byte* v_data, ulong v_size)
	{
		memcpy((void*)&m_stream_array[0], (void*)v_data, m_size);
	}

	void managed_buffer::resize(ulong v_size)
	{
		if (m_size >= v_size)
			return;

		m_size = v_size;
		m_stream_array.resize(m_size);
	}

	void managed_buffer::clear(void)
	{
		m_stream_array.clear();
		m_cursor = 0;
		m_size = 0;
	}

	byte* managed_buffer::get_buffer(void)
	{
		return (byte*)&m_stream_array[0];
	}

	ulong managed_buffer::write(void* v_data, ulong data_size)
	{
		m_size += data_size;
		m_stream_array.insert(m_stream_array.end(), (char const*)v_data, ((char const *)v_data) + data_size);
		m_cursor += data_size;
		return m_size;
	}

	ulong managed_buffer::write(string& str)
	{
		ulong data_size = (ulong)str.size() + 1;
		m_size += data_size;

		m_stream_array.insert(m_stream_array.end(), (char const*)str.c_str(), ((char const *)str.c_str()) + data_size);
		m_cursor += data_size;
		return m_size;
	}

	ulong managed_buffer::write(wstring& str)
	{
		ulong data_size = ((ulong)str.size() + 1) * 2;
		m_size += data_size;
		m_stream_array.insert(m_stream_array.end(), (char const*)str.c_str(), ((char const *)str.c_str()) + data_size);
		m_cursor += data_size;
		return m_size;
	}

	ulong managed_buffer::read(void* v_data, ulong data_size)
	{
		if ((m_cursor + data_size) <= m_size)
		{
			memcpy((byte*)v_data, (byte*)&m_stream_array[0] + m_cursor, data_size);
			m_cursor += data_size;

			return data_size;
		}

		return 0;
	}

	ulong managed_buffer::read(string& str)
	{
		str.clear();

		ulong k = 0;
		while (m_stream_array[k + m_cursor] != '\0')
		{
			str += m_stream_array[k + m_cursor];
			k++;
		}
		k++;

		m_cursor += k;

		return k;
	}

	ulong managed_buffer::read(wstring& str)
	{
		str.clear();

		ulong k = 0;
		wchar_t* v_data = (wchar_t*)(&m_stream_array[0] + m_cursor); //(wchar_t*)m_stream_array[m_cursor];
		while (v_data[k] != L'\0')
		{
			str += v_data[k];
			k++;
		}
		k++;

		m_cursor += k * 2;

		return m_cursor;
	}


}