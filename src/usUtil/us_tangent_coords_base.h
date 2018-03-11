///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_tangent_coords_base.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : tangent_coords_base class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_TANGENT_COORDS_BASE_H_
#define _US_TANGENT_COORDS_BASE_H_

#include <usUtil\Export.h>
#include <usUtil\us_coordinate_base.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT tangent_coords_base : public coordinate_base
	{
	protected:
		tangent_coords_base(void);

	public:
		tangent_coords_base(double in_longitude, double in_latitude);

		tangent_coords_base(vector3<double> in_origin_sphr);

		virtual ~tangent_coords_base(void);
	};
}

#endif // _US_TANGENT_COORDS_BASE_H_