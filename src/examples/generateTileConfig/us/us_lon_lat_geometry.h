#ifndef _US_LON_LAT_GEOMETRY_H_
#define _US_LON_LAT_GEOMETRY_H_

#include "us_object_base.h"

namespace uniscope_globe
{
	class lon_lat_geometry : public object_base
	{
	public:
		lon_lat_geometry(void);
		virtual ~lon_lat_geometry(void);
		virtual bool intersect(lon_lat_geometry* in_geometry) { return false; }
	};
}

#endif // _US_LON_LAT_GEOMETRY_H_