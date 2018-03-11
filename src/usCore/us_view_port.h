//////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
//////////////////////////////////////////////////////////////////////////
//
//  Filename: us_view_port.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : view port class
//	Reference : 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _US_VIEW_PORT_H_
#define _US_VIEW_PORT_H_
#include <usCore\Export.h>
namespace uniscope_globe
{
	class USCORE_EXPORT view_port
	{
	public:
		view_port(void);

		~view_port(void);

	public:	
		//top left
		unsigned long m_x;
		unsigned long m_y;

		unsigned long m_width;
		unsigned long m_height;

		float m_z_min;
		float m_z_max;
	};

}

#endif //_US_VIEW_PORT_H_
