///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_usx_mesh.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : usx_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_USX_MESH_H_
#define _US_USX_MESH_H_

#include <usDotx/Export.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_axis_aligned_box.h>
#include <vector>

namespace uniscope_globe
{
	class usx_material;
	class usx_skin_info;
	class usx_frame;
	class usx_color_value;
	class usx_tri_face_index;

	typedef std::vector<float*> float_data_array;

	class USDOTX_EXPORT  usx_mesh
	{
	public:
		usx_mesh(void);

		virtual ~usx_mesh(void);

		typedef std::vector<usx_material*> usx_material_array;

	public:
		void combine(void);

		void remove_material(void);

		void sort(long* in_new_order);

	public:
		ustring	m_name;

		bool	m_valid;

		usx_frame* m_parent_frame;

		// vertex
		ulong				m_number_of_vertices;
		vector3<float>*		m_vertex_data;

		usx_color_value*	m_vertex_color;

		vector3<float>*		m_normals;

		vector3<float>*		m_binormal;

		vector3<float>*		m_tangent;

		float_data_array	m_vertex_texture_coords;

		float_data_array	m_decl_data;

		// face
		ulong				m_number_of_faces;
		usx_tri_face_index*	m_face_index;

		std::vector<int>	m_old_vertex_num_per_face;
		axis_aligned_box_f  m_bbox;

		ulong*				m_attribute_data;

		// material
		bool				m_material_safe;
		usx_material_array	m_materials;

		// √…∆§–≈œ¢
		usx_skin_info*      m_skin_info;

		ulong				m_fvf;
		ulong				m_vertex_stride; // vertex stride in bytes
	};
}

#endif // _US_USX_MESH_H_