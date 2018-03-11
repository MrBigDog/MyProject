///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_lon_lat_geometry.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : lon_lat_geometry class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_LON_LAT_GEOMETRY_H_
#define _US_LON_LAT_GEOMETRY_H_

#include <usUtil\Export.h>
#include <usUtil\us_object_base.h>

namespace uniscope_globe
{
	class USUTIL_EXPORT lon_lat_geometry : public object_base
	{
	public:
		lon_lat_geometry(void);

		virtual ~lon_lat_geometry(void);

		virtual bool intersect(lon_lat_geometry* in_geometry) { return false; }

	};
}

#endif // _US_LON_LAT_GEOMETRY_H_