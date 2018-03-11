///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_managed_buffer.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : managed_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MANAGED_BUFFER_H_
#define _US_MANAGED_BUFFER_H_

#include <usUtil\Export.h>
#include <usUtil\us_raw_buffer.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT managed_buffer : public raw_buffer
	{
	public:
		managed_buffer(void);
		managed_buffer(managed_buffer& v_buffer);
		managed_buffer(ulong v_size);
		managed_buffer(byte* v_data, ulong v_size);
		virtual ~managed_buffer(void);

		// override from raw_buffer
	public:
		virtual void attach(byte* v_data, ulong v_size);

		virtual void copy(byte* v_data, ulong v_size);

		virtual void clear(void);

		virtual void resize(ulong v_size);

		virtual byte* get_buffer(void);

		virtual ulong write(void* v_data, ulong data_size);

		virtual ulong write(string& str);

		virtual ulong write(wstring& str);

		virtual ulong read(void* v_data, ulong data_size);

		virtual ulong read(string& str);

		virtual ulong read(wstring& str);

	protected:
		std::vector<byte> m_stream_array;

	};
}

#endif // _US_MANAGED_BUFFER_H_