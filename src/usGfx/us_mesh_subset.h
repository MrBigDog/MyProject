
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2010 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_mesh_subset.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : mesh_subset class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_MESH_SUBSET_H_
#define _US_MESH_SUBSET_H_

namespace uniscope_globe
{
	class material_changed_argument : public event_argument
	{
	public:
		material_changed_argument( void )
		{
			m_material_entry = NULL;
		}

		material_changed_argument( material_entry* v_entry )
		{
			m_material_entry = v_entry;
		}

		~material_changed_argument()
		{

		}

	public:
		material_entry* m_material_entry;
	};

	class mesh_subset
		: public d3d9_hardware_mesh_buffer
		, public spatial_object
		, public editable_object
		, public shared_data
	{
	public:
		mesh_subset( void );

		virtual ~mesh_subset( void );

	public:
		virtual void initialize( document_base* v_doc ) {}

		virtual void draw( render_argument* args );

		virtual vector_3f get_center( void ){ return m_center; }

		virtual d3d9_hardware_mesh_buffer* get_parent_mesh_buffer( void ){ return m_parent_mesh_buffer; }

		virtual void set_parent_mesh_buffer( d3d9_hardware_mesh_buffer* v_parent_mesh_buffer ){ m_parent_mesh_buffer = v_parent_mesh_buffer; }

		virtual int get_material_index( void ) { return m_material_index; }	

		virtual material_entry* get_material_entry( void );

	public:
		virtual void translate_object( vector_3d& vec_offset_cart );

		virtual void transform_uv( matrix_4f& mat_uv );

	protected:
		virtual void update_center( void );

	public:
		std::vector<ulong>				m_face_index_array;

		std::map<ulong, ulong>			m_compact_index_map;

	protected:
		d3d9_hardware_mesh_buffer*		m_parent_mesh_buffer;

		uint							m_material_index;
	
		vector_3f						m_center; 
	};
}

#endif // _US_MESH_SUBSET_H_ 