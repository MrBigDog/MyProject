//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_transform.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_transform class
//	Reference : 
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_TRANSFORM_H_
#define _US_RENDER_TRANSFORM_H_
#include <usCore\Export.h>
#include <usUtil\us_matrix4.h>
#include <stack>
namespace uniscope_globe
{
	class render_device;
	class USCORE_EXPORT render_transform
	{
	public:
		render_transform(render_device* rd);

		virtual ~render_transform(void);

	public:
		void push(matrix4<float>& mat);

		void pop(void);

		void flush(void);

	protected:
		std::stack<matrix_4f> m_matrix_stack;

		render_device*		  m_render_device;
	};
}

#endif // _US_RENDER_TRANSFORM_H_
