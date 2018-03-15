
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_surface.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_surface class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_SURFACE_H_
#define _US_USX_SURFACE_H_

#include <usDotx/Export.h>
#include <usUtil/us_common_file.h>
#include <vector>

namespace uniscope_globe
{
	class USDOTX_EXPORT usx_surface
	{
	public:
		usx_surface(void);

		virtual ~usx_surface(void);

	public:
		std::vector<ulong> m_face_index_array;

		uint m_material_index;
	};
}

#endif // _US_USX_SURFACE_H_ 