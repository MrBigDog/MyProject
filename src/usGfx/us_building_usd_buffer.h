///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_building_usd_buffer.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : building_usd_buffer class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_BUILDING_USD_BUFFER_H_
#define _US_BUILDING_USD_BUFFER_H_

namespace uniscope_globe
{
	struct indice_data
	{
		ushort a, b, c;
		indice_data( ushort v_a = 0, short v_b = 0, short v_c = 0 )
		{
			a = v_a;
			b = v_b;
			c = v_c;
		}
	};

	struct texture_index
	{
		interior_index m_diffuse;
		interior_index m_self_illuminant;
		interior_index m_specular;
		interior_index m_bump;
		texture_index( void )
		{
			m_diffuse = 0;
			m_self_illuminant = 0;
			m_specular = 0;
			m_bump = 0;
		}
	};
	typedef std::vector<texture_index> texture_index_array;

#define US_MESH_ALPHA_NONE		0x0000
#define US_MESH_ALPHA_TEST		0x0001
#define US_MESH_ALPHA_BLEND		0x0002


	class LIB_GFX_API building_create_buffer : public raw_buffer
	{
	protected:
		building_create_buffer(){}

	public:
		building_create_buffer( byte* p_data, int data_size )
			: raw_buffer(p_data,data_size)
		{

		}

		~building_create_buffer(){}

	public:
		double m_height;
	};


	class LIB_GFX_API building_usd_buffer
		: public object_base
	{
	protected:
		building_usd_buffer( void ){}

	public:
		building_usd_buffer( raw_buffer* v_stream );

		virtual ~building_usd_buffer( void );

		typedef std::vector<usx_material*> bldg_material_array;

	protected:
		bool parse_raw_buffer( raw_buffer* v_stream );
		bool read_vertex( raw_buffer* v_stream );
		bool read_indice( raw_buffer* v_stream );
		bool read_aabb( raw_buffer* v_stream );
		bool read_material( raw_buffer* v_stream );
		bool read_figure( raw_buffer* v_stream );
		bool read_topologic( raw_buffer* v_stream );
		void* get_vertex_buffer( usc_vertex_head& vert_head );
		face_normal	create_normal_f( vector_3f& vec0, vector_3f& vec1, vector_3f& vec2 );

	public:
		virtual bool generate_topological( void );
		virtual bool generate_normals( void );
		virtual bool generate_mesh( void ) { return true; }
		
	public:
		wstring				m_file_name;
		wstring				m_name;

		int					m_texture_level;
		int					m_lod_level;
		ulong				m_alpha_state;

		bool				m_valid;
		bldg_builder_head	m_usc_head;

		// vertex
		usc_vertex_head		m_vertex_head;
		ulong				m_vertex_buffer_size;
		void*				m_vertex_buffer;

		// index
		usc_index_head		m_index_head;
		ulong				m_index_buffer_size;
		void*				m_index_buffer;

		// attribute
		ulong				m_attribute_buffer_size;
		ulong*				m_attribute_buffer;

		std::vector<int>	m_indice_segment;

		// bound box
		axis_aligned_box_d	m_box;

		// figure
		usc_figure_head		m_figure_buf_head;
		position_color*		m_figure_buf;

		usc_figure_head		m_figure_index_head;
		ushort*				m_figure_index;

		// material
		usd_material_head	m_material_head;
		material_set*		m_materials;
		std::vector<int>	m_materials_segment;
		std::vector<int>	m_materials_type;
		bool				m_material_blend;
		bool				m_mateiral_animation;

		texture_index_array m_text_index;

		// for shadow_cast
		edge_array			m_lines;
		face_normal_array	m_normals;
	};
}

#endif // _US_BUILDING_USD_BUFFER_H_