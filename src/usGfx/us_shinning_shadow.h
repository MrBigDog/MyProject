///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_shinning_shadow.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : shinning_shadow class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SHINNING_SHADOW_H_
#define _US_SHINNING_SHADOW_H_
#include <usGfx/Export.h>
#include <usCore/us_render_object.h>
#include <usCore/us_hardware_mesh.h>
#include <usUtil/us_common_file.h>

#include <d3dx9mesh.h>
#include <map>

namespace uniscope_globe
{
	class building_usd_buffer;

	class USGFX_EXPORT shinning_shadow
		: public render_object
		, public hardware_mesh
	{
	public:
		typedef std::vector< vector3<float> > vertex_array;

		typedef std::vector< ushort > index_array;

		typedef std::vector< vector3<int>> index_array_ex;

		typedef std::vector< byte > facing_array;

		typedef std::map< int, int > reorder_map;

		typedef std::vector<edge> edge_array;

		typedef std::vector<face_normal> face_normal_array;

	public:
		shinning_shadow();

		virtual ~shinning_shadow();

	public:
		virtual long create(render_device* device);

		virtual bool destroy(void);

		virtual bool refresh(void);

	public:
		virtual void draw(render_argument* args);

		void create_from_mesh(building_usd_buffer* parent_mesh, vector4<double>& in_light, ulong v_stamp);

		virtual void set_world_matrix(matrix4<double>& mat) { m_world_matrix = mat; }

		virtual matrix4<double> get_world_matrix(void) { return m_world_matrix; }

	protected:
		void generate_vertices(float* in_vertices, ulong num_of_vertices, long stride, vector4<double>& in_light);

		void generate_indices(face_normal_array& in_normal_array, edge_array& in_edge_array, ushort* triangle, ulong num_of_vertices, vector4<double>& in_light);

	protected:
		//virtual void create_facing_array( face_normal_array& in_normal_array, unsigned long in_num_of_triangles, vector4<double> in_light );

		//virtual void create_silhouette_triangles( edge_array& in_array );

		//virtual void create_cap_triangles( ushort* triangle, int num_of_triangles );

		//virtual void generate_volume(  float* in_vertices, long stride, vector4<double> in_light );

	private:
		int  get_new_index(int index);

		void push_in_vertex_array(float* vertices, long stride, vector4<double> in_light, int old_index);

	protected:
		vertex_array	m_vertex_array;

		index_array		m_index_array;

		facing_array	m_facing_array;

		index_array_ex	m_temp_index_array;

		reorder_map		m_reorder_map;

		LPD3DXMESH		m_d3d_mesh;

		matrix4<double>	m_world_matrix;

		bool			m_valid;
	};
}

#endif // _US_SHINNING_SHADOW_H_ 