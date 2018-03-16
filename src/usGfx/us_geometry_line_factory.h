///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_line_factory.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_line_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_GEOMETRY_LINE_FACTORY_H_
#define _US_GEOMETRY_LINE_FACTORY_H_
#include <usGfx/Export.h>
#include <usGfx/us_d3d9_vertex_declear.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_vector3.h>
#include <windows.h>

namespace uniscope_globe
{
	class geometry_line_volume;
	class geometry_line_flake;

	class USGFX_EXPORT geometry_line_factory
	{
	public:
		geometry_line_factory(void) {}

		virtual ~geometry_line_factory(void) {}

		//friend class geometry_mesh;
		friend class geometry_line;
		//friend class geometry_polygon;

		// create geometry_line
	public:
		static void create_geometry_line(OUT geometry_line* geo_mesh, IN coordinates_array& in_vec_array, IN vector_3d& center_geo, IN ulong line_color);

		// create geometry_line_flake
	public:
		static void create_geometry_line_flake(OUT geometry_line_flake* geo_mesh, IN coordinates_array& in_vec_array, IN vector_3d& center_geo, IN double line_width, IN ulong line_color);

		static void push_tri_strip_point(std::vector<position_normal_color>& triangle_list, vector3<double>& pt, vector3<double>& normal, ulong color);

		// create geometry_line_volume
	public:
		static void create_geometry_line_volume(geometry_line_volume* geo_mesh, IN coordinates_array& in_vec_array, IN vector_3d& center_geo, IN double line_width, IN ulong line_color, IN double upper_height, IN double lower_height);

		static void insert_index_to_line_mesh(geometry_line_volume* v_mesh, ulong size);
		//static void create_geometry_line_flake( OUT geometry_line_flake* geo_mesh, IN coordinates_array& in_vec_array, IN vector_3d& center_geo, IN double line_width, IN ulong line_color );

		//static void push_tri_strip_point( std::vector<position_normal_color>& triangle_list, vector3<double>& pt, vector3<double>& normal, ulong color );

	};
}

#endif // _US_GEOMETRY_LINE_FACTORY_H_ 