///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_intersect_argument.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : intersect_argument class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_INTERSECT_ARGUMENT_H_
#define _US_INTERSECT_ARGUMENT_H_

#include <usCore\Export.h>

namespace uniscope_globe
{
	class object_base;
	class USCORE_EXPORT intersect_argument
	{
	public:
		intersect_argument(void);

		virtual ~intersect_argument(void);

	public:
		object_base* m_observer;

	};
}

#endif // _US_INTERSECT_ARGUMENT_H_