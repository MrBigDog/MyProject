///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_interior_index_factory.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : interior_index_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_INTERIOR_INDEX_FACTORY_H_
#define _US_INTERIOR_INDEX_FACTORY_H_

#include <usUtil\Export.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_lon_lat_rect.h>
#include <vector>

namespace uniscope_globe
{
	typedef std::vector<int> path_array;
	typedef std::vector<interior_index> interior_index_array;

#define	MASK_24_BITS (0xffffff)
#define MASK_5_BITS  (0x1f)
#define MASK_3_BITS  (0x07)

#define	INTERIOR_INDEX_LAT(id)		((int)((id) & MASK_24_BITS))
#define	INTERIOR_INDEX_LON(id)		((int)(((id)>>24) & MASK_24_BITS))
#define	INTERIOR_INDEX_LEVEL(id)	((int)(((id)>>48) & MASK_5_BITS))

	typedef enum
	{
		QUADRANT_SOUTH_WEST = 0,
		QUADRANT_NORTH_WEST,
		QUADRANT_SOUTH_EAST,
		QUADRANT_NORTH_EAST,
		QUADRANT_COUNT
	}square_quadrant;

	class USUTIL_EXPORT interior_index_factory
	{
	public:
		interior_index_factory(void);

		virtual ~interior_index_factory(void);

	public:
		static interior_index assemble(int level, int longtitude_index, int latitude_index);

		static interior_index assemble(int level, double longtitude, double latitude);

		static interior_index assemble(int level, vector3<double> in_vector);

		static void disassemble(interior_index index, int& level, int& longtitude_index, int& latitude_index);

		static interior_index get_child(interior_index index, int quadrant);

		static void get_children(interior_index index,
			interior_index& index_sw,
			interior_index& index_nw,
			interior_index& index_se,
			interior_index& index_ne);

		static interior_index get_parent(interior_index index, uint upper_level = 1);

		static path_array get_path_array(interior_index upper_index, interior_index down_index);

		static interior_index_array get_index_array(lon_lat_rect& rect, uint level);

		static interior_index_array get_index_array(lon_lat_rect& rect);

		static int get_level(interior_index index);

		static int get_lon_index(interior_index index);

		static int get_lat_index(interior_index index);

		static lon_lat_rect get_bound(interior_index index);

		static double	get_bound_size(int level);

		static void get_int_rect(/*IN*/ lon_lat_rect& rect, /*IN*/ int level, /*OUT*/ int& north_index, /*OUT*/ int& south_index, /*OUT*/ int& east_index, /*OUT*/ int& west_index);

	public:
		static ustring to_encrypt_string(interior_index index);

		static ustring to_string(interior_index index);

		static ustring to_wmts_string(interior_index index, bool xylformat = false);

		static ustring interior_index_factory::to_wmts_string_by_lonlat(interior_index index, bool xylformat = false);

	public:
		static double calculate_elevation(int level, double fov = PI * 0.25, int view_port_height = 1024, int pixel_size = 256);


	private:
		static lon_lat_rect m_world_rect;
	};



	class USUTIL_EXPORT interior_index_helper
	{
	public:
		interior_index_helper(void);

		virtual ~interior_index_helper(void);

	public:
		virtual void set_world_rect(lon_lat_rect& v_rect) { m_world_rect = v_rect; }

	public:
		interior_index assemble(int level, int longtitude_index, int latitude_index);

		interior_index assemble(int level, double longtitude, double latitude);

		interior_index assemble(int level, vector3<double> in_vector);

		void disassemble(interior_index index, int& level, int& longtitude_index, int& latitude_index);

		interior_index get_child(interior_index index, int quadrant);

		void get_children(interior_index index,
			interior_index& index_sw,
			interior_index& index_nw,
			interior_index& index_se,
			interior_index& index_ne);

		interior_index get_parent(interior_index index, uint upper_level = 1);

		path_array get_path_array(interior_index upper_index, interior_index down_index);

		interior_index_array get_index_array(lon_lat_rect& rect, uint level);

		interior_index_array get_index_array(lon_lat_rect& rect);

		int get_level(interior_index index);

		int get_lon_index(interior_index index);

		int get_lat_index(interior_index index);

		lon_lat_rect get_bound(interior_index index);

		double	get_bound_size(int level);

		void get_int_rect(/*IN*/ lon_lat_rect& rect, /*IN*/ int level, /*OUT*/ int& north_index, /*OUT*/ int& south_index, /*OUT*/ int& east_index, /*OUT*/ int& west_index);

	public:
		ustring to_encrypt_string(interior_index index);

		ustring to_string(interior_index index);

	public:
		double calculate_elevation(int level, double fov = PI * 0.25, int view_port_height = 1024, int pixel_size = 256);

	private:
		lon_lat_rect m_world_rect;
	};
}

#endif // _US_INTERIOR_INDEX_FACTORY_H_
