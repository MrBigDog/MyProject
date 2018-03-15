
// us_d3d9_effect_weather.cpp

#include "stdafx.h"
#include "us_d3d9_effect_weather.h"

namespace uniscope_globe
{

	d3d9_effect_weather::d3d9_effect_weather( render_device* device, ulong resource_id )
		: d3d9_effect_base(device)
	{		
		load_from_module_resource(resource_id);		
	}


	d3d9_effect_weather::~d3d9_effect_weather( void )
	{

	}

	
} 