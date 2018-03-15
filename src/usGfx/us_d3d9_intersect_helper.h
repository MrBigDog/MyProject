///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_intersect_helper.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_intersect_helper class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_D3D9_INTERSECT_HELPER_H_
#define _US_D3D9_INTERSECT_HELPER_H_

namespace uniscope_globe
{
	class LIB_GFX_API d3d9_intersect_helper
	{
	public:
		d3d9_intersect_helper( void );

		virtual ~d3d9_intersect_helper( void );

	public:
		bool hit_test( void* d3d_mesh, ray<double> in_ray, intersect_result& out_result );

		bool intersect( void* d3d_mesh, ray<double> in_ray, intersect_result& out_result, int cull_mode = 2 );

		vector3<double> calculate_intersect_point(void* d3d_mesh, ulong face_index, float u, float v );

		vector3<double> calculate_intersect_point(void* d3d_mesh, ulong face_index, float u, float v, vector3<double>& in_face_normal );

		bool get_intersect_info(void* d3d_mesh, ulong face_index, float u, float v, intersect_result& out_result );

		bool intersect_triangle( vector3<float>& orig, vector3<float>& dir, 
								vector3<float>& v0, vector3<float>& v1, vector3<float>& v2, 
								float* t, float* u, float* v );

		static double point_to_segment_distance( vector_3f& p, vector_3f& a, vector_3f& b );

		static double point_to_segment_distance_d( vector_3d& p, vector_3d& a, vector_3d& b );

		static bool get_material_index( void* d3d_mesh, ulong face_index, ulong& mat_index );
		 
	};

	class LIB_GFX_API singleton_d3d9_intersect_helper
		: public singleton<d3d9_intersect_helper>
	{
	public:
		singleton_d3d9_intersect_helper( void )
		{
		}

		virtual ~singleton_d3d9_intersect_helper( void )
		{
		}
	};
}

#endif // _US_D3D9_INTERSECT_HELPER_H_