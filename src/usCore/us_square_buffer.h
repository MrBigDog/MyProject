///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_square_buffer.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : square_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SQUARE_BUFFER_H_
#define _US_SQUARE_BUFFER_H_

#include <usCore\Export.h>

namespace uniscope_globe
{
	class document_base;

	class USCORE_EXPORT square_buffer : public raw_buffer
	{
	protected:
		square_buffer(void) {}

	public:
		//square_buffer( byte* p_data, int data_size, interior_index index = 0, const LPCTSTR str_path = L"", double v_height = 0.0 );

		square_buffer(document_base* in_doc, byte* p_data, int data_size, interior_index index = 0, const LPCTSTR str_path = L"", double v_height = 0.0);

		virtual ~square_buffer(void);

		interior_index get_index(void) { return m_index; }
		ustring get_path(void) { return m_str_path; }

		void set_height_offset(double v_ho) { m_height_offset = v_ho; }
		double get_height_offset(void) { return m_height_offset; }

		document_base* get_document(void) { return m_document; }

	protected:
		interior_index m_index;
		ustring m_str_path;
		double m_height_offset;
		document_base* m_document;
	};
}

#endif // _US_SQUARE_BUFFER_H_