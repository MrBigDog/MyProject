
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_hardware_mesh_factory.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : hardware_mesh_factory class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_HARDWARE_MESH_FACTORY_H_
#define _US_HARDWARE_MESH_FACTORY_H_

#include <usGfx/Export.h>
#include <usUtil/us_common_file.h>
#include <usUtil/us_vector3.h>
#include <map>

namespace uniscope_globe
{
	class raw_buffer;
	class render_device;
	class usx_mesh;
	class document_base;
	class hardware_mesh;
	class d3d9_hardware_mesh;
	class render_object_geometry_mesh;
	class render_object_geometry_textured_mesh;
	class usx_parser_object_container;
	class d3d9_skinning_hardware_mesh;

	class USGFX_EXPORT hardware_mesh_factory
	{
	public:
		hardware_mesh_factory(void) {}

		virtual ~hardware_mesh_factory(void) {}

	public:
		//static hardware_mesh* create( usx_mesh* in_mesh, usx_frame* in_root_frame, cpstr in_path );

		static hardware_mesh* create_complex_mesh(document_base* in_doc, usx_parser_object_container* v_containe);

		static d3d9_hardware_mesh* create_single_mesh(document_base* in_doc, usx_mesh* in_mesh, usx_parser_object_container* v_container);

		static d3d9_hardware_mesh* create_single_mesh_from_polygon(document_base* in_doc, vector_3d* in_pos_array, ulong v_size, ulong line_color, ulong fill_color);

		static d3d9_hardware_mesh* create_hardware_mesh_from_buffer(render_device* v_device, raw_buffer* verts_buf, raw_buffer* indices_buf, int verts_count, int indices_count, int fvf);

		//static hardware_mesh* create_complex_mesh( hardware_mesh_buffer_map& in_buffer_map, usx_frame* in_root_frame, usx_animation_set_array& in_animation_set_array );

		//static d3d9_hardware_mesh* create( hardware_mesh_buffer* v);

	protected:
		static void copy_single_mesh(document_base* in_doc, d3d9_hardware_mesh* out_mesh, usx_mesh* in_usx_mesh);

		static void copy_skinning_mesh(document_base* in_doc, d3d9_skinning_hardware_mesh* out_mesh, usx_mesh* in_usx_mesh);

		static vector3<double> get_blended_vertex(d3d9_skinning_hardware_mesh* in_buffer, ulong vert_index);


	public:
		static d3d9_hardware_mesh* create_single_mesh_from_element_textured(document_base* in_doc, render_object_geometry_textured_mesh* in_mesh);

		static d3d9_hardware_mesh* create_single_mesh_from_element(document_base* in_doc, render_object_geometry_mesh* in_mesh);


	public:
		static void compact_index(void* index_buffer, int index_cnt, bool m_is_32_bit, std::map<ulong, ulong>& compacted_map);

	protected:
		static void get_uv_info_from_fvf(ulong fvf, int& uv_offset, int& uv_cnt);

	};
}

#endif // _US_HARDWARE_MESH_FACTORY_H_ 