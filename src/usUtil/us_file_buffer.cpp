///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_file_buffer.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : file_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_file_buffer.h"

namespace uniscope_globe
{
	file_buffer::file_buffer(void)
	{
		m_file = NULL;

		m_valid = false;

		m_size = 0;
	}

	file_buffer::~file_buffer(void)
	{
		if (m_file)
		{
			fclose(m_file);
			m_file = NULL;
		}
	}

	bool file_buffer::file_open(cpstr str_path, cpstr open_type)
	{
		/*
		string mb_file_path, mb_open_type;
#ifdef _UNICODE
		mb_file_path = string_ext::from_wstring( str_path );
		mb_open_type = string_ext::from_wstring( open_type );
#else
		mb_file_path = str_path;
		mb_open_type = open_type;
#endif //_UNICODE

		m_file = fopen( mb_file_path.c_str(), mb_open_type.c_str() );

		if ( m_file )
		{
			m_valid = true;
			fseek( m_file, 0, SEEK_END );

			m_size = ftell( m_file );

			fseek( m_file, 0, SEEK_SET );
		}
		//*/

#ifdef _UNICODE
		m_file = _wfopen(str_path, open_type);
#else
		m_file = fopen(str_path, open_type);
#endif

		if (m_file)
		{
			m_valid = true;
			fseek(m_file, 0, SEEK_END);

			m_size = ftell(m_file);

			fseek(m_file, 0, SEEK_SET);
		}

		return m_valid;
	}

	void file_buffer::file_close(void)
	{
		if (m_file)
		{
			fclose(m_file);
			m_file = NULL;
		}
		m_valid = false;
		m_size = 0;
	}

	void file_buffer::file_flush(void)
	{
		fflush(m_file);
	}

	bool file_buffer::is_eof(void)
	{
		if (m_file)
		{
			interior_index current_pos = ftell(m_file);
			return m_size <= current_pos;
		}

		return true;
	}

	void file_buffer::seek_to_begin(void)
	{
		fseek(m_file, 0, SEEK_SET);
	}

	void file_buffer::seek_to_end(void)
	{
		fseek(m_file, 0, SEEK_END);
	}

	void file_buffer::seek_to(ulong v_pos)
	{
		fseek(m_file, v_pos, SEEK_SET);
	}

	interior_index file_buffer::get_size(void)
	{
		return m_size;
	}

	ulong file_buffer::write(void* v_data, ulong data_size)
	{
		return (ulong)fwrite(v_data, 1, data_size, m_file);
	}

	ulong file_buffer::write(string& str)
	{
		return (ulong)fwrite(str.c_str(), 1, str.size() + 1, m_file);
	}

	ulong file_buffer::write(wstring& str)
	{
		return (ulong)fwrite(str.c_str(), 1, (str.size() + 1) * 2, m_file);
	}

	ulong file_buffer::read(void* v_data, ulong data_size)
	{
		return (ulong)fread(v_data, 1, data_size, m_file);
	}

	ulong file_buffer::read_line(string& str)
	{
		str.clear();

		char tmp;

		ulong bytes_read = 0;

		bytes_read += fread(&tmp, 1, 1, m_file);
		while (tmp != '\r')
		{
			str += tmp;

			bytes_read += fread(&tmp, 1, 1, m_file);
		}

		bytes_read += fread(&tmp, 1, 1, m_file);

		return bytes_read;
	}

	ulong file_buffer::read(string& str)
	{
		str.clear();

		char tmp;

		ulong bytes_read = 0;

		do
		{
			bytes_read += fread(&tmp, 1, 1, m_file);
			str += tmp;
		} while (tmp != '\0' && !is_eof());

		return bytes_read;
	}

	ulong file_buffer::read(wstring& str)
	{
		str.clear();

		wchar_t tmp;

		do
		{
			fread(&tmp, 1, 2, m_file);
			str += tmp;
		} while (tmp != L'\0' && !is_eof());

		return ulong((str.size() + 1) * 2);
	}

	int file_buffer::get_tell()
	{
		if (m_file == NULL)
			return 0;

		return ftell(m_file);
	}

	bool file_buffer::get_error()
	{
		if (m_file == NULL)
			return true;

		return ferror(m_file);
	}

}