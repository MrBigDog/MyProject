///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shadow_style.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shadow_style class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SHADOW_STYLE_H_
#define _US_SHADOW_STYLE_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
#define US_SHADOW_NONE				0x0000
#define US_SHADOW_TERRAIN			0x0001
#define US_SHADOW_MODEL				0x0002
#define US_SHADOW_TERRAIN_MODEL		0x0003

	class USCORE_EXPORT shadow_style : public style_base
	{
	public:
		shadow_style( void );
		shadow_style( const shadow_style& v_style );
		virtual ~shadow_style( void );

	public:
		ulong m_shadow_color;
		int m_shadow_type;
	};
}

#endif // _US_SHADOW_STYLE_H_ 