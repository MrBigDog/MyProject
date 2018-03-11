///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_lon_lat_region.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : lon_lat_region class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_lon_lat_region.h"

namespace uniscope_globe
{
	lon_lat_region::lon_lat_region( void )
	{

	}

	lon_lat_region::~lon_lat_region( void )
	{

	}

	void lon_lat_region::push_back( vector3<double>& vert )
	{
		m_vertex_array.push_back( vert );

		m_bound.combine( vert );
	}

	void lon_lat_region::clear( void )
	{
		m_vertex_array.clear();
	}


	vector3<double> lon_lat_region::operator[] ( ulong index ) const
	{
		if ( index >= m_vertex_array.size() )
			return vector3<double>::s_zero;

		return m_vertex_array[index];
	}

	ulong lon_lat_region::get_count( void ) const
	{
		return m_vertex_array.size();
	}
	
	//bool lon_lat_region::contain( lon_lat_rect& in_rect )
	//{
	//	//
	//	// test rect vertices in geocentric coordinates
	//	//
	//	vertex_array rect_vertex_array_geo;
	//	rect_vertex_array_geo.push_back ( geocentric_coords::from_spherical_d( vector3<double> ( in_rect.m_west, in_rect.m_south, spherical_coords::s_semi_major ) ) );
	//	rect_vertex_array_geo.push_back ( geocentric_coords::from_spherical_d( vector3<double> ( in_rect.m_west, in_rect.m_north, spherical_coords::s_semi_major ) ) );
	//	rect_vertex_array_geo.push_back ( geocentric_coords::from_spherical_d( vector3<double> ( in_rect.m_east, in_rect.m_north, spherical_coords::s_semi_major ) ) );
	//	rect_vertex_array_geo.push_back ( geocentric_coords::from_spherical_d( vector3<double> ( in_rect.m_east, in_rect.m_south, spherical_coords::s_semi_major ) ) );

	//	int v_region_size = m_vertex_array.size();
	//	for ( int j = 0; j < rect_vertex_array_geo.size(); j++ )
	//	{
	//		vector3<double> test = rect_vertex_array_geo[j];
	//		for ( int i = 0; i < v_region_size; i++ )
	//		{
	//			vector3<double> p1 = geocentric_coords::from_spherical_d( m_vertex_array[i] );
	//			vector3<double> p2 = geocentric_coords::from_spherical_d( m_vertex_array[(i+1) % v_region_size] );

	//			vector3<double> n1 = vector3<double>::normalize( vector3<double>::cross( p2 - p1, test - p1 ) );
	//			if( vector3<double>::dot( n1, vector3<double>::normalize( p1 ) ) > 0.0 )
	//			{
	//				return false;
	//			}
	//		}
	//	}

	//	return true;
	//}

	bool lon_lat_region::contain( vector3<double> pos_sphr ) const
	{
		bool inside = false;

		int size = m_vertex_array.size();
		for ( int i = 0; i < size; i++ )
		{
			vector3<double> p1 = m_vertex_array[ ( i + size) % size ];
			vector3<double> p2 = m_vertex_array[ ( i+1 + size ) % size ];

			if ( pos_sphr.y < p2.y )
			{
				//p1 above ray
				if ( pos_sphr.y >= p1.y )
				{
					//p1 on or blew ray
					if (  ( pos_sphr.y - p1.y ) * ( p2.x - p1.x ) > ( pos_sphr.x - p1.x ) * ( p2.y - p1.y ) )
					{
						inside = !inside;
					}
				}
			}
			else if ( pos_sphr.y < p1.y )
			{
				if( ( pos_sphr.y - p1.y ) * ( p2.x - p1.x )  < ( pos_sphr.x - p1.x ) * ( p2.y - p1.y ) )
				{
					inside = !inside;
				}
			}
		}
		return inside;
	}

	lon_lat_rect lon_lat_region::get_bound( void )
	{
		return m_bound;
	}

	bool lon_lat_region::intersect( const lon_lat_rect& in_rect )
	{
		for( int ni = 0; ni < 4; ni++)
		{
			if(this->contain(in_rect.get_conner(ni)))
				return true;
		}

		for ( int ni = 0; ni < (int)(m_vertex_array.size()); ni++ )
		{
			if ( in_rect.contain( m_vertex_array[ni] ) )
				return true;
		}

		return false;
	}


	//int lon_lat_region::intersect_rect( const lon_lat_rect& in_rect )
	//{
	//	for( int ni = 0; ni < 4; ni++)
	//	{
	//		if(this->contain(in_rect.get_conner(ni)))
	//			return true;
	//	}

	//	for ( int ni = 0; ni < (int)(m_vertex_array.size()); ni++ )
	//	{
	//		if ( in_rect.contain( m_vertex_array[ni] ) )
	//			return true;
	//	}

	//	return false;
	//}
}