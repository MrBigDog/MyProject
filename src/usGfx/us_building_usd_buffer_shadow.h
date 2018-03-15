///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_building_usd_buffer_shadow.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : building_usd_buffer_shadow class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_BUILDING_USD_BUFFER_SHADOW_H_
#define _US_BUILDING_USD_BUFFER_SHADOW_H_

namespace uniscope_globe
{
	struct  usx_tri_face_index_ushort
	{
		ushort a, b, c ;
		usx_tri_face_index_ushort()
		{
			a = b = c = 0;
		}
	};


	class LIB_GFX_API building_usd_buffer_shadow
		: public building_usd_buffer
	{
	public:
		typedef std::pair< ulong, ulong > edge;

		typedef std::map< edge, edge > edge_map;

		typedef std::map< edge, ulong > edge_triangle_map;

		typedef std::map< ulong, vector3<float> > triangle_normal_map;

	protected:
		building_usd_buffer_shadow( void ){}

	public:
		building_usd_buffer_shadow( raw_buffer* v_stream );

		virtual ~building_usd_buffer_shadow( void );

	public:
		virtual bool generate_mesh( void );

	protected:
		bool generate_mesh_pos_tex( void );
		bool generate_mesh_pos_tex2( void );
		bool generate_mesh_pos_dif_tex( void );

		position_normal_texture2 read_vertex( ulong index, vector3<float>& in_normal );
		void insert_triangle( edge old_e, edge new_e, edge_map& in_edge_map, std::vector<usx_tri_face_index>& in_index_array, std::vector<ulong>& in_attribute_array );
		vector3<float> compute_normal( usx_tri_face_index& in_triangle );

	};
}

#endif // _US_BUILDING_USD_BUFFER_SHADOW_H_ 