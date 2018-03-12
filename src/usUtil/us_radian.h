//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_radian.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : radian template
//	Reference : http://www.geometrictools.com ( Wild Magic Library )
//				http://www.ogre3d.org/		  ( Ogre Object-Oriented Graphics Rendering Engine )
//				http://mathworld.wolfram.com/Matrix.html
//
//////////////////////////////////////////////////////////////////////////
#ifndef _US_RADIAN_H_
#define _US_RADIAN_H_

#include <usUtil\Export.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT radian
	{
	public:
		radian(void);
		~radian(void);

	public:
		static double to_degree(double rad);

		static double from_degree(double deg);
	};
}

#endif // _US_RADIAN_H_