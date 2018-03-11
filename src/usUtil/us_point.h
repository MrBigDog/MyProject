///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_point.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : point class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_POINT_H_
#define _US_POINT_H_

#include <usUtil\Export.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT point
	{
	public:
		point(void);
		point(long n_x, long n_y);
		~point(void);

	public:
		long x, y;

	};
}

#endif // _US_POINT_H_