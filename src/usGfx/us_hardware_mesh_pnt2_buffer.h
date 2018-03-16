///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_mesh_pnt2_buffer.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_mesh_pnt2_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_HARDWARE_MESH_PNT2_BUFFER_H_
#define _US_HARDWARE_MESH_PNT2_BUFFER_H_
#include <usGfx/Export.h>
#include <usUtil/us_common_file.h>
namespace uniscope_globe
{		
	class degenerate_quad
	{
	public:
		degenerate_quad()
		{
			m_v1 = m_v2 = m_v3 = m_v4 = 0xffffffff;
		}
	public:
		ulong m_v1;
		ulong m_v2;
		ulong m_v3;
		ulong m_v4;
	};

	//class hardware_mesh_pnt2_buffer
	//	: public hardware_mesh_pt2_buffer
	//{
	//public:
	//	typedef std::pair<ulong, ulong> edge;

	//	typedef std::map< edge, degenerate_quad > edge_map;

	//public:
	//	hardware_mesh_pnt2_buffer( void );

	//	virtual ~hardware_mesh_pnt2_buffer( void );

	//public:
	//	virtual hardware_mesh_buffer* clone( void );

	//	virtual long create( render_device* in_device );	

	//protected:
	//	void generate_mesh( ulong* in_point_rep );

	//	position_normal_texture2 read_vertex( ulong index );

	//	vector3<float> compute_normal( ulong a, ulong b, ulong c );

	//	void insert_triangle( ulong o1, ulong o2, ulong n1, ulong n2, edge_map& in_edge_map, std::vector<usx_tri_face_index>& in_index_array, std::vector<ulong>& in_attribute_array );

	//	void patching( edge_map& in_edge_map, std::vector<position_normal_texture2>& in_new_vertex_array, std::vector<usx_tri_face_index>& in_new_face_index, std::vector<ulong>& in_attribute_array );

	//	//void insert_triangle( edge_map& in_edge_map, edge_triangle_map& in_edge_triangle_map, triangle_normal_map in_triangle_normal_map, std::vector<usx_tri_face_index>& in_index_array, std::vector<ulong>& in_attribute_array );

	//	//void insert_triangle( edge_map& in_edge_map, std::vector<position_normal_texture2>& in_vertex_array,  std::vector<usx_tri_face_index>& in_index_array, std::vector<ulong>& in_attribute_array );

	//	//bool is_same_edge( edge& e1, edge& e2 );
	//};
}

#endif // _US_HARDWARE_MESH_PNT2_BUFFER_H_