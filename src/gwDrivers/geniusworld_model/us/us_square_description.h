///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_square_description.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : square_description class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SQUARE_DESCRIPTION_H_
#define _US_SQUARE_DESCRIPTION_H_

#include "us_lon_lat_rect.h"
#include "us_common_file.h"

namespace uniscope_globe
{
	class square_description
	{
	public:
		square_description(void);
		square_description(interior_index index);
		square_description(lon_lat_rect& rect, int level);
		square_description(int level, double north, double south, double east, double west, int quadrant = 0);
		virtual ~square_description(void);

	public:
		void create(interior_index index);
		void create(lon_lat_rect& rect, int level);
		void create(int level, double dNorth, double dSouth, double dEast, double dWest);

		static square_description from_index(interior_index index);
		static square_description from_rect(lon_lat_rect& rect, int level);
		static square_description from_rect(int level, double dNorth, double dSouth, double dEast, double dWest);

	public:
		interior_index	m_index;
		lon_lat_rect	m_bound;
		int				m_level;
		int				m_quadrant;
	};
}

#endif // _US_SQUARE_DESCRIPTION_H_