
///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_render_object_geometry_textured_mesh.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : render_object_geometry_textured_mesh class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "us_render_object_geometry_textured_mesh.h"

namespace uniscope_globe
{
	render_object_geometry_textured_mesh::render_object_geometry_textured_mesh( void )
	{
		m_center = vector_3d::s_zero;
		m_rtti = US_RTTI_GEOMETRY_TEXTURED_MESH;
		m_fvf = position_normal_texture::fvf;
		m_vertex_stride = position_normal_texture::fvf;
		m_shadow_type = US_SHADOW_NONE;
		m_trans_matrix = matrix_4d::s_identity;
		m_collapse_matrix = matrix_4d::s_identity;
		m_texture_manager = NULL;

		m_offset_distance = 0.0f;
	}

	render_object_geometry_textured_mesh::render_object_geometry_textured_mesh(  spherical_object* v_parent_object, int material_count )
	{
		m_center = vector_3d::s_zero;
		m_rtti = US_RTTI_GEOMETRY_TEXTURED_MESH;
		m_fvf = position_normal_texture::fvf;
		m_vertex_stride = position_normal_texture::fvf;
		m_shadow_type = US_SHADOW_NONE;
		m_trans_matrix = matrix_4d::s_identity;
		m_collapse_matrix = matrix_4d::s_identity;
		m_texture_manager = NULL;
		m_roof_name = L"";

		m_parent_object = v_parent_object;

		m_material_entry_array.resize( material_count );
		for( int ni = 0; ni < material_count; ni++ )
		{
			geometry_mesh_subset* m_mesh_subset = new geometry_mesh_subset();
			m_mesh_subset->create( this, ni );
			m_material_entry_array[ni] = m_mesh_subset;
		}

		m_subset_info_array.resize( material_count );
		m_texture_array.resize( material_count );
	}


	render_object_geometry_textured_mesh::~render_object_geometry_textured_mesh( void )
	{
		clear_buffer();

		clear_material();
	}

	render_object_geometry_textured_mesh* render_object_geometry_textured_mesh::create_shared_instance( spherical_object* v_parent_object, int material_count )
	{
		render_object_geometry_textured_mesh* v_geometry = new render_object_geometry_textured_mesh(v_parent_object, material_count);
		v_geometry->add_ref();

		return v_geometry;
	}

	//void render_object_geometry_textured_mesh::draw_figure( render_device* device, matrix_4f& in_mat )
	//{
	//	if ( m_frame_index_array.size() > 0 && m_frame_vertex_array.size() > 0 )
	//	{
	//		device->set_texture( 0, NULL );

	//		matrix_4f v_mat = in_mat;
	//		v_mat.m41 = in_mat.m41 - cartesian_coords::s_reference_position_geo.x;
	//		v_mat.m42 = in_mat.m42 - cartesian_coords::s_reference_position_geo.y;
	//		v_mat.m43 = in_mat.m43 - cartesian_coords::s_reference_position_geo.z;

	//		device->set_vertex_declaration( position_color::fvf );
	//		device->push_transform();

	//		device->multiply_transform( v_mat );

	//		render_helper::draw_indexed_primitive_up (  device, D3DPT_LINELIST, 0, 
	//			m_frame_vertex_array[0], m_frame_index_array[0], 
	//			m_frame_vertex_array.size(), m_frame_index_array.size() / 2, position_color::stride );

	//		device->pop_transform();
	//	}
	//}
	/**@add by Felix*/
	resource_container<ustring>* render_object_geometry_textured_mesh::get_texture_containner( const ustring& newVal )
	{
		resource_container<ustring>* v_texture_container = m_texture_manager->get_resource( newVal );

		document_base* v_doc = singleton_common_manager_group::ptr()->get_document();
		if (v_texture_container == NULL)
		{ 
			ustring v_file_ext = file_directory::get_file_ext(newVal.c_str());
			if( v_file_ext == L"swf")
			{
				v_texture_container = new flash_resource_container( v_doc, newVal.c_str(), newVal.c_str(), US_DOWNLOAD_IN_HEAP );
				v_texture_container->add_ref();
				m_texture_manager->add_resource( newVal, v_texture_container );
			}
			else
			{
				v_texture_container = new texture_resource_container( v_doc, newVal.c_str(), newVal.c_str(), US_DOWNLOAD_IN_HEAP );
				v_texture_container->add_ref();
				m_texture_manager->add_resource( newVal, v_texture_container );
			}
		}
		return v_texture_container;
	}

	void render_object_geometry_textured_mesh::draw( render_argument* args )
	{
		d3d9_effect_common_mesh* v_mesh_render = (d3d9_effect_common_mesh*)args->m_render;
		v_mesh_render->set_texture( 0, NULL );
		v_mesh_render->set_texture( 1, NULL );

		matrix_4d v_mat = m_collapse_matrix * m_trans_matrix;
		v_mat.m41 = v_mat.m41 - cartesian_coords::s_reference_position_geo.x;
		v_mat.m42 = v_mat.m42 - cartesian_coords::s_reference_position_geo.y;
		v_mat.m43 = v_mat.m43 - cartesian_coords::s_reference_position_geo.z;

		v_mesh_render->set_vertex_declaration( m_fvf );
		v_mesh_render->push_transform();
		v_mesh_render->multiply_transform( matrix_4f( v_mat.m ) );
		v_mesh_render->commit_changes();

		render_state v_render_state( args->m_device );
		v_render_state.set_state( D3DRS_LIGHTING, TRUE );
		v_render_state.set_state( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );
		if(m_depth_bias != 0.0f )
		{
			v_render_state.set_state( D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&m_depth_bias));
		}
		texture_state v_texture_state( args->m_device );
		v_texture_state.set_state( 0, D3DTSS_TEXTURETRANSFORMFLAGS, 2 );
		v_texture_state.set_state( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

		if ( m_blend_color != 0x00000000 )
		{
			v_render_state.set_state( D3DRS_TEXTUREFACTOR, m_blend_color );	

			v_texture_state.set_state( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
			v_texture_state.set_state( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			v_texture_state.set_state( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR );

			v_texture_state.set_state( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
			v_texture_state.set_state( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			v_texture_state.set_state( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );
		}

		//m_render_state.do(args->m_device);
		//m_texture_state.do(args->m_device);

		d3d9_effect_flood* v_flood_effect = (d3d9_effect_flood*)args->m_device->get_effect(US_EFFECT_FLOOD_ANALYSIS);
		render_device* device = args->m_device;
		//observer_base* in_observer = args->m_observer;
		static hardware_texture* tex_source = render_helper::create_texture_from_file_ex( device, m_roof_name.c_str());
		/*texture_resource_container* res = (texture_resource_container*)get_texture_containner( m_roof_name );
		if ( NULL != res )
		{
		tex_source = (hardware_texture*)res->get_resource();

		}*/

		for(int ni = 0; ni < m_subset_info_array.size(); ni++)
		{
			//render_state v_child_render_state( args->m_device );
			//texture_state v_child_texture_state( args->m_device );
			//if ( m_blend_color_array[ni] != 0x00000000 )
			//{
			//	
			//	v_child_render_state.set_state( D3DRS_TEXTUREFACTOR, m_blend_color_array[ni] );	

			//	
			//	v_child_texture_state.set_state( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
			//	v_child_texture_state.set_state( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			//	v_child_texture_state.set_state( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR );

			//	v_child_texture_state.set_state( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
			//	v_child_texture_state.set_state( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			//	v_child_texture_state.set_state( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );
			//}

			if( m_texture_array[ni].m_diffuse != NULL )
			{
				v_mesh_render->set_texture( 0, m_texture_array[ni].m_diffuse->get_handle());
			}
			else
			{
				v_mesh_render->set_blank_texture(0);
			}

			v_mesh_render->set_material( (ULONG_PTR)m_material_entry_array[ni]->get_material_entry() );

			//if(m_material_entry_array[ni]->get_material_entry())
			if ( ni == 1 && m_roof_name != L"" && tex_source != NULL )
			{
				double v_cur_tick_time = tls_singleton_frame_counter::instance().m_frame_time;
				/*m_offset_distance += v_cur_tick_time*0.002;
				if(m_offset_distance>1.0)
				{
					m_offset_distance=0.0;
				}*/
				UINT numPasses = 0;
				v_flood_effect->begin(&numPasses,0);
				v_flood_effect->begin_pass(0);
				v_flood_effect->commit_changes();
				v_flood_effect->set_texture( tex_source->get_handle());
				v_flood_effect->set_flood_offset(v_cur_tick_time*0.02);
				v_flood_effect->set_flood_uvdistance(v_cur_tick_time);
				v_flood_effect->set_upDataTime(0.001f);
				v_flood_effect->set_view_matrix(matrix4<double>((float*)&device->get_view_matrix()));
				v_flood_effect->set_proj_matrix(matrix4<double>((float*)&device->get_projection_matrix()));
				v_flood_effect->set_world_matrix(matrix4<double>((float*)&device->get_world_matrix()));
					
				draw_subset( args, ni );
				v_flood_effect->end_pass();
				v_flood_effect->end();
			}
			else
				draw_subset( args, ni );
		}
		
		v_mesh_render->pop_transform();
	}

	void render_object_geometry_textured_mesh::draw_subset( render_argument* args, int v_material_index)
	{
		int v_index_stride = m_32bit_index?4:2;

		args->m_device->set_texture_matrix( 0, m_material_entry_array[v_material_index]->get_texture_matrix() );
		args->m_device->draw_indexed_triangle_list( m_32bit_index?D3DFMT_INDEX32:D3DFMT_INDEX16, m_vertex_buffer, m_num_of_vertices,
			(byte*)(m_index_buffer) + m_subset_info_array[v_material_index].first * v_index_stride , m_subset_info_array[v_material_index].second, m_vertex_stride );
	}

	bool render_object_geometry_textured_mesh::refresh( void )
	{
		bool m_is_texture_ready = true;
		for ( int i = 0; i < (int)m_material_entry_array.size(); i++ )
		{
			//**@add By Felix**/
			//if ( i == 0 && m_roof_name != L"" )
			//{
				//texture_resource_container* res = (texture_resource_container*)get_texture_containner( m_roof_name );
				//if( NULL != res )
				//{
				//	m_material_entry_array[i]->get_material_entry()->m_diffuse = res;//(hardware_texture*)res->get_resource();
				//}
			//	 //render_helper::create_texture_from_file_ex( m_device, m_roof_name.c_str());

			//}
			if ( m_material_entry_array[i]->get_material_entry()->m_diffuse )
			{
				m_texture_array[i].m_diffuse = (hardware_texture*)m_material_entry_array[i]->get_material_entry()->m_diffuse->get_resource();
				if ( m_texture_array[i].m_diffuse )
				{
					m_is_texture_ready = m_is_texture_ready && m_texture_array[i].m_diffuse->refresh();
				}
			}
			else
			{
				m_texture_array[i].m_diffuse = NULL;
			}
		}

		return m_is_texture_ready;
	}

	void render_object_geometry_textured_mesh::clear_buffer( void )
	{
		d3d9_hardware_mesh_buffer::clear_buffer();

		//m_frame_vertex_array.clear();
		//m_frame_index_array.clear();

		m_collapse_matrix = matrix_4d::s_identity;
		m_trans_matrix = matrix_4d::s_identity;

		m_render_aabb.make_invalid();
	}

	void render_object_geometry_textured_mesh::clear_material()
	{
		material_entry_array::iterator itr = m_material_entry_array.begin();
		for ( ; itr != m_material_entry_array.end(); itr++ )
		{
			AUTO_DELETE(*itr);
		}
		m_material_entry_array.clear();

		m_texture_array.clear();
		m_subset_info_array.clear();
	}

	bool render_object_geometry_textured_mesh::intersect( const ray<double>& a_ray, intersect_result& result )
	{
		bool b_ret = false;
		ray<double> in_ray = a_ray;

		vector_3d pick_point;
		double pick_length = 0.0;
		if ( m_32bit_index )
		{
			ulong* v_index_buf = (ulong*)m_index_buffer;
			for ( int i = 0; i < m_num_of_triangles * 3; i+=3 )
			{
				vector_3d vec1( (float*)(this->get_fat_vertex( v_index_buf[i] ) ) );
				vector_3d vec2( (float*)(this->get_fat_vertex( v_index_buf[i+1] ) ) );
				vector_3d vec3( (float*)(this->get_fat_vertex( v_index_buf[i+2] ) ) );

				triangle_d tri( vec1, vec2, vec3 );
				bool b_pick = triangle_d::intersect( in_ray, tri );
				if ( triangle_d::pick( in_ray, tri, pick_point, 10000000.0 ) )
				{
					pick_length = (pick_point - in_ray.ray_origin).length();
					if ( result.m_distance > pick_length )
					{
						result.m_distance = pick_length;
						result.m_position = pick_point + m_center;
						result.m_face_index = i / 3;
					}

					b_ret = true;
				}
			}
		}
		else
		{
			ushort* v_index_buf = (ushort*)m_index_buffer;
			for ( int i = 0; i < m_num_of_triangles * 3; i+=3 )
			{
				vector_3d vec1( (float*)(this->get_fat_vertex( v_index_buf[i] ) ) );
				vector_3d vec2( (float*)(this->get_fat_vertex( v_index_buf[i+1] ) ) );
				vector_3d vec3( (float*)(this->get_fat_vertex( v_index_buf[i+2] ) ) );

				triangle_d tri( vec1, vec2, vec3 );
				bool b_pick = triangle_d::intersect( in_ray, tri );
				if ( triangle_d::pick( in_ray, tri, pick_point, 10000000.0 ) )
				{
					pick_length = (pick_point - in_ray.ray_origin).length();
					if ( result.m_distance > pick_length )
					{
						result.m_distance = pick_length;
						result.m_position = pick_point + m_center;
						result.m_face_index = i / 3;
					}

					b_ret = true;
				}
			}
		}

		if ( b_ret )
		{
			result.m_inner_object = (object_base*)this;
		}

		return b_ret;
	}

	bool render_object_geometry_textured_mesh::intersect_material(const ray<double>& a_ray, intersect_result& result )
	{
		bool b_ret = false;
		ray<double> in_ray = a_ray;

		vector_3d pick_point;
		double pick_length = 0.0;

		if ( m_32bit_index )
		{
			ulong* v_index_buf = (ulong*)m_index_buffer;
			for ( int i = 0; i < m_num_of_triangles * 3; i+=3 )
			{
				vector_3d vec1( (float*)(this->get_fat_vertex( v_index_buf[i] ) ) );
				vector_3d vec2( (float*)(this->get_fat_vertex( v_index_buf[i+1] ) ) );
				vector_3d vec3( (float*)(this->get_fat_vertex( v_index_buf[i+2] ) ) );

				triangle_d tri( vec1, vec2, vec3 );
				bool b_pick = triangle_d::intersect( in_ray, tri );
				if ( triangle_d::pick( in_ray, tri, pick_point, 10000000.0 ) )
				{
					pick_length = (pick_point - in_ray.ray_origin).length();
					if ( result.m_distance > pick_length )
					{
						result.m_distance = pick_length;
						result.m_position = pick_point + m_center;
						result.m_face_index = i / 3;
					}

					b_ret = true;
				}
			}
		}
		else
		{
			ushort* v_index_buf = (ushort*)m_index_buffer;
			for ( int i = 0; i < m_num_of_triangles * 3; i+=3 )
			{
				vector_3d vec1( (float*)(this->get_fat_vertex( v_index_buf[i] ) ) );
				vector_3d vec2( (float*)(this->get_fat_vertex( v_index_buf[i+1] ) ) );
				vector_3d vec3( (float*)(this->get_fat_vertex( v_index_buf[i+2] ) ) );

				triangle_d tri( vec1, vec2, vec3 );
				bool b_pick = triangle_d::intersect( in_ray, tri );
				if ( triangle_d::pick( in_ray, tri, pick_point, 10000000.0 ) )
				{
					pick_length = (pick_point - in_ray.ray_origin).length();
					if ( result.m_distance > pick_length )
					{
						result.m_distance = pick_length;
						result.m_position = pick_point + m_center;
						result.m_face_index = i / 3;
					}

					b_ret = true;
				}
			}
		}

		if ( b_ret )
		{
			int attri_sum = 0;
			for ( int i = 0; i < m_subset_info_array.size(); i ++ )
			{
				if ( ((m_subset_info_array[i].first + m_subset_info_array[i].second ) / 3) > result.m_face_index)
				{
					result.m_inner_object = m_material_entry_array[i];
					break;
				}
			}
		}

		return b_ret;
	}


	void render_object_geometry_textured_mesh::copy_from( render_object_geometry_textured_mesh* v_mesh )
	{
		render_object::copy_from( v_mesh );

		this->lock();
		this->copy_buffer_from( v_mesh );
		this->unlock();

		this->m_texture_manager	= v_mesh->m_texture_manager;
		this->m_subset_info_array = v_mesh->m_subset_info_array;

		this->m_trans_matrix		= v_mesh->m_trans_matrix;
		this->m_collapse_matrix	= v_mesh->m_collapse_matrix;
		this->m_center			= v_mesh->m_center;

		this->m_texture_array.resize( m_texture_array.size() );

		this->m_material_entry_array.resize( m_material_entry_array.size() );
		
		for (int i = 0; i < m_material_entry_array.size(); i++ )
		{
			this->m_material_entry_array[i]->copy_from(v_mesh->m_material_entry_array[i]);
		}
	}


	void render_object_geometry_textured_mesh::set_collapse( matrix_4d& mat )
	{
		m_collapse_matrix = m_collapse_matrix * mat;
	}

	render_object* render_object_geometry_textured_mesh::get_shadow_object( void )
	{
		return this;
	}


}