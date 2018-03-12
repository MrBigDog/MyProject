///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_common.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : 
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_RENDER_COMMON_H_
#define _US_RENDER_COMMON_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	enum USCORE_EXPORT culling_mode
	{
		/// Hardware never culls triangles and renders everything it receives.
		CULL_NONE = 1,
		/// Hardware culls triangles whose vertices are listed clockwise in the view (default).
		CULL_CLOCKWISE = 2,
		/// Hardware culls triangles whose vertices are listed anticlockwise in the view.
		CULL_ANTICLOCKWISE = 3
	};
}

#endif // _US_RENDER_COMMON_H_