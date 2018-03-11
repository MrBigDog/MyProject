///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_raw_buffer.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : raw_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////

#include "us_raw_buffer.h"

namespace uniscope_globe
{
	raw_buffer::raw_buffer( void )
	{
		m_buffer = NULL;
		m_size = 0;
		m_cursor = 0;
	}

	raw_buffer::raw_buffer( raw_buffer& buffer )
	{
		m_buffer = buffer.get_buffer();
		m_size = buffer.get_size();
		m_cursor = 0;
	}

	raw_buffer::raw_buffer( byte* p_data, ulong data_size )
	{
		m_buffer = p_data;
		m_size	 = data_size;
		m_cursor = 0;
	}

	raw_buffer::~raw_buffer( void )
	{
		m_buffer = NULL;
		m_size   = 0;
		m_cursor = 0;
	}

	void raw_buffer::seek_to_begin( void ) 
	{
		m_cursor = 0; 
	}

	void raw_buffer::seek_to_end( void )
	{ 
		m_cursor = m_size; 
	}

	int raw_buffer::seek_to( ulong v_pos ) 
	{
		if ( v_pos < m_size )
			return m_cursor = v_pos;

		return -1;
	}

	ulong raw_buffer::get_size( void ) 
	{
		return m_size; 
	}

	ulong raw_buffer::get_cursor( void )
	{
		return m_cursor;
	}

	void raw_buffer::attach( byte* v_data, ulong v_size )
	{
		clear();
		m_buffer = v_data;
		m_size	 = v_size;
		m_cursor = 0;
	}

	void raw_buffer::resize( ulong v_size )
	{
		return;
	}

	void raw_buffer::clear( void )
	{
		m_buffer = NULL;
		m_size = 0;
		m_cursor = 0;
	}

	byte* raw_buffer::get_buffer( void )
	{
		return m_buffer;
	}

	ulong raw_buffer::write( void* v_data, ulong data_size )
	{
		if ( ( m_cursor + data_size ) <= m_size )
		{
			memcpy( m_buffer + m_cursor, (byte*)v_data, data_size );
			m_cursor += data_size;
		}

		return m_cursor;
	}

	ulong raw_buffer::write( string& str )
	{
		int data_size = (int)str.size() + 1;
		if ( ( m_cursor + data_size ) <= m_size )
		{
			memcpy( m_buffer + m_cursor, (byte*)str.c_str(), data_size );
			m_cursor += data_size;
		}
		return m_cursor;
	}

	ulong raw_buffer::write( wstring& str )
	{
		int data_size = ( (int)str.size() + 1 ) * 2;
		if ( ( m_cursor + data_size ) <= m_size )
		{
			memcpy( m_buffer + m_cursor, (byte*)str.c_str(), data_size );
			m_cursor += data_size;
		}
		return m_cursor;
	}

	ulong raw_buffer::read( void* v_data, ulong data_size )
	{
		if ( ( m_cursor + data_size ) <= m_size )
		{
			memcpy( (byte*)v_data, m_buffer + m_cursor, data_size );
			m_cursor += data_size;

			return data_size;
		}

		return 0;
	}

	ulong raw_buffer::read( string& str )
	{
		str.clear();

		ulong k = 0;
		char* v_data = (char*)(m_buffer + m_cursor);
		while( v_data[k] != '\0')
		{
			str += v_data[k];
			k++;
		}
		k++;
		m_cursor += k;

		return k;
	}

	ulong raw_buffer::read( wstring& str )
	{
		str.clear();

		ulong k = 0;
		wchar_t* v_data = (wchar_t*)(m_buffer + m_cursor);
		while( v_data[k] != L'\0')
		{
			str += v_data[k];
			k++;
		}
		k++;
		m_cursor += k*2;

		return k*2;
	}

}
