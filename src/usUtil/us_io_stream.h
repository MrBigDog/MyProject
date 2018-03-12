//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_io_stream.h
//  Author  : Uniscope Team
//  Modifier: Uniscope Team
//  Created :
//  Purpose : io stream
//	Reference :
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_IO_STREAM_H_
#define _US_IO_STREAM_H_

#include <usUtil\Export.h>
#include <usUtil\us_common_file.h>

#include <vector>
#include <assert.h>

using namespace std;

namespace uniscope_globe
{
	class USUTIL_EXPORT io_stream
	{
	public:
		io_stream(void) {}

		io_stream(void* buf, ulong size)
		{
			attach(buf, size);
		}

		void attach(void* buf, ulong size)
		{
			clear();

			m_stream_array.resize(size);

			memcpy((void*)&m_stream_array[0], (void*)buf, size);
		}

		virtual ~io_stream(void) {}

	public:
		// write to stream
		template<typename T>
		size_t write(T const& t)
		{
			size_t size = sizeof(T);
			m_stream_array.insert(m_stream_array.end(), (char const*)&t, ((char const*)&t) + size);
			return size;
		}

		template<typename T>
		size_t write(std::vector<T> const& t)
		{
			if (t.size() > 0)
			{
				size_t size = sizeof(T) * t.size();
				m_stream_array.insert(m_stream_array.end(), (char const*)&t[0], ((char const*)&t[0]) + size);
				return size;
			}
			return 0;
		}

		template<typename T>
		size_t write(T const* t, size_t cnt)
		{
			size_t size = cnt * sizeof(T);
			m_stream_array.insert(m_stream_array.end(), (char const*)t, ((char const*)t) + size);
			return size;
		}

		template<>
		size_t write(ustring const& str)
		{
			size_t size = (str.size() + 1) * 2;
			m_stream_array.insert(m_stream_array.end(), (char const*)str.c_str(), (char const*)str.c_str() + size);
			return size;
		}

		template<>
		size_t write(string const& str)
		{
			size_t size = str.size() + 1;
			m_stream_array.insert(m_stream_array.end(), (char const*)str.c_str(), (char const*)str.c_str() + size);
			return size;
		}

		//template<>
		size_t write(byte* t, size_t size)
		{
			m_stream_array.insert(m_stream_array.end(), (char const*)t, ((char const*)t) + size);
			return size;
		}

		// read from stream
		//template<typename T>
		//size_t read( T& t )
		//    {
		//    size_t size = sizeof( T );
		//    m_stream_array.insert( m_stream_array.end(), ( char const* )&t, ( ( char const* )&t ) + size );
		//    return size;
		//    }
		//
		//template<typename T>
		//size_t read( std::vector<T>& t )
		//    {
		//    if ( t.size() > 0 )
		//        {
		//        size_t size = sizeof( T ) * t.size();
		//        m_stream_array.insert( m_stream_array.end(), ( char const* )&t[0], ( ( char const* )&t[0] ) + size );
		//        return size;
		//        }
		//    return 0;
		//    }
		//
		//template<typename T>
		//size_t read( T* t, size_t cnt )
		//    {
		//    size_t size = cnt * sizeof( T );
		//    m_stream_array.insert( m_stream_array.end(), ( char const* )&t, ( ( char const* )&t ) + size );
		//    return size;
		//    }
		//
		//template<>
		//size_t read( ustring const& str )
		//    {
		//    size_t size = str.size() + 1;
		//
		//    return size;
		//    }
		//
		//template<>
		//size_t read( string const& str )
		//    {
		//    size_t size = str.size() + 1;
		//
		//    return size;
		//    }

		void seek_to_begin(void) { m_cursor = 0; }

		void seek_to_end(void) { m_cursor = m_stream_array.size(); }

		int get_size(void) { return (int)m_stream_array.size(); }

		byte* get_data(void) { return  m_stream_array.empty() ? NULL : &m_stream_array[0]; /*m_stream_array[0];*/ }

		bool save_to_file(ustring& file_name)
		{
			return true;
		}

		bool load_from_file(ustring& file_name)
		{
			return true;
		}

		void clear(void)
		{
			m_stream_array.clear();
			m_cursor = 0;
		}

		///低效，不稳定
		///@TODO ...
		int read(char** str)
		{
			assert(!m_stream_array.empty());
			// m_cursor 是不是要判断 小于 size//???????

			if (*str)
			{
				AUTO_DELETE(*str);
			}

			string tmpstr;

			int k = 0;
			byte* cur_pointer = &m_stream_array[0] + m_cursor;
			while (cur_pointer[k] != '\0')
			{
				tmpstr += cur_pointer[k];
				k++;
			}
			k++;  // skip /0

			*str = new char[k];
			memset(*str, 0, k);

			strcpy(*str, tmpstr.c_str());

			m_cursor += k;

			return k;
		}

	protected:
		std::vector<byte>		m_stream_array;

		size_t					m_cursor;
	};
}

#endif // _US_IO_STREAM_H_