///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_transform_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : transform_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TRANSFORM_BASE_H_
#define _US_TRANSFORM_BASE_H_

#include <usUtil\Export.h>
#include <usUtil\us_matrix4.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT transform_base
	{
	public:
		transform_base(void);

		virtual ~transform_base(void);

	public:
		virtual matrix_4f get_matrix_f(void) { return matrix_4f::s_identity; }

		virtual matrix_4d get_matrix_d(void) { return matrix_4d::s_identity; }


	};
}

#endif // _US_TRANSFORM_BASE_H_ 