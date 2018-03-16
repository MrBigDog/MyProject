///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_geometry_model.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : geometry_model class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_geometry_model.h"
#include <usGfx/us_d3d9_hardware_mesh.h>
#include <usCore/us_square_buffer.h>
#include <usCore/us_priority_stack.h>
#include <usCore/us_download_argument.h>
#include <usCore/us_render_state.h>
#include <usCore/us_texture_state.h>
#include <usCore/us_render_argument.h>
#include <usCore/us_render_device.h>
#include <usUtil/us_raw_buffer.h>
#include <usUtil/us_cartesian_coords.h>

namespace uniscope_globe
{
	//////////////////////////////////////////////////////////////////////////
	//  geometry_model
	//////////////////////////////////////////////////////////////////////////
	geometry_model::geometry_model(LPCTSTR str_link)
		: common_resource_container(str_link, str_link, US_DOWNLOAD_IN_HEAP)
	{
		m_link = str_link;
		m_render_model = NULL;
		//common_resource_container::common_resource_container( m_link, str_link, US_DOWNLOAD_IN_HEAP );
	}

	geometry_model::~geometry_model(void)
	{
		AUTO_DELETE(m_render_model);
	}

	geometry_render_model* geometry_model::get_render_model(void)
	{
		singleton_priority_stack::instance().push(0xffffffff);
		resource_base* v_rs = get_resource();
		if (v_rs == NULL)
			return NULL;

		if (m_render_model)
		{
			m_render_model->set_render_mesh((hardware_mesh*)v_rs);
		}
		else
		{
			m_render_model = new geometry_render_model((d3d9_hardware_mesh*)v_rs);
		}

		return m_render_model;
	}

	bool geometry_model::intersect(const ray<double>& a_ray, intersect_result& result)
	{
		bool ret_val = false;
		if (m_render_model)
		{
			ret_val = m_render_model->m_mesh->get_bound_box().intersect(a_ray);
			if (ret_val)
			{
				m_render_model->m_mesh->intersect(a_ray, result);
			}
		}

		return ret_val;
	}

	void geometry_model::on_download_finish(event_argument* args)
	{
		download_argument* v_args = (download_argument*)args;
		if (v_args->m_download_status == US_DOWNLOAD_SUCCEED)
		{
			download_argument* v_download_args = (download_argument*)args;

			square_buffer v_square_buffer(v_download_args->m_stream->get_buffer(),
				v_download_args->m_stream->get_size(),
				0, m_link.c_str());

			m_resource = dynamic_cast<resource_base*>(singleton_filter_manager::instance().parse(&v_square_buffer));
		}

		enable_reclaim(true);

		shared_data::Release();
	}


	//////////////////////////////////////////////////////////////////////////
	//  geometry_render_model
	//////////////////////////////////////////////////////////////////////////
	geometry_render_model::geometry_render_model(d3d9_hardware_mesh* v_mesh)
	{
		m_mesh = v_mesh;
		m_mat = matrix_4d::s_identity;
	}

	geometry_render_model::~geometry_render_model(void)
	{
		//m_texture_array.clear();
	}


	void geometry_render_model::set_matrix(matrix_4d& v_mat)
	{
		m_mat = v_mat;
	}

	void geometry_render_model::set_render_mesh(hardware_mesh* v_mesh)
	{
		m_mesh = (d3d9_hardware_mesh*)v_mesh;
	}

	void geometry_render_model::update(void)
	{
		m_mesh->refresh();

		//multi_texture_container_array& v_text_array = m_mesh->get_texture_container_array();
		//if ( m_texture_array.size() == 0 )
		//{
		//	m_texture_array.resize( v_text_array.size() );
		//}

		//for ( int i = 0; i < (int)v_text_array.size(); i++ )
		//{
		//	if ( v_text_array[i].m_diffuse )
		//	{
		//		m_texture_array[i].m_diffuse = (hardware_texture*)v_text_array[i].m_diffuse->get_resource();
		//		if ( m_texture_array[i].m_diffuse )
		//		{
		//			m_texture_array[i].m_diffuse->refresh();
		//		}
		//	}
		//}

		//for ( int i = 0; i < (int)v_text_array.size(); i++ )
		//{
		//	if ( v_text_array[i].m_self_illuminant )
		//	{
		//		m_texture_array[i].m_self_illuminant = (hardware_texture*)v_text_array[i].m_self_illuminant->get_resource();
		//		if ( m_texture_array[i].m_self_illuminant )
		//		{
		//			m_texture_array[i].m_self_illuminant->refresh();
		//		}
		//	}
		//}

	}

	void geometry_render_model::draw(render_argument* args)
	{
		// set cur render state
		render_state v_render_state(args->m_device);
		v_render_state.set_state(D3DRS_CULLMODE, D3DCULL_CCW);

		//v_render_state.set_state( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		v_render_state.set_state(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		float slopeScaleDepthBias = 0.0f;
		v_render_state.set_state(D3DRS_SLOPESCALEDEPTHBIAS, *((DWORD*)&slopeScaleDepthBias));

		// set cur texture stage
		texture_state v_ts(args->m_device);
		v_ts.set_state(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		v_ts.set_state(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

		for (int i = 1; i < 2; i++)
		{
			v_ts.set_state(i, D3DTSS_COLOROP, D3DTOP_MODULATE);
			v_ts.set_state(i, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			v_ts.set_state(i, D3DTSS_COLORARG2, D3DTA_CURRENT);
			v_ts.set_state(i, D3DTSS_TEXCOORDINDEX, i);
		}

		matrix_4d v_mat = m_mat;

		v_mat.m41 -= cartesian_coords::s_reference_position_geo.x;
		v_mat.m42 -= cartesian_coords::s_reference_position_geo.y;
		v_mat.m43 -= cartesian_coords::s_reference_position_geo.z;

		args->m_device->push_transform();
		args->m_device->multiply_transform(matrix_4f((double*)v_mat.m));

		//int tex_cnt = (int)m_texture_array.size();

		//for ( int i = 0; i < tex_cnt ; i++ )
		//{
		//	if ( m_texture_array[i].m_diffuse )
		//	{
		//		if ( m_texture_array[i].m_diffuse->is_valid() )
		//			m_texture_array[i].m_diffuse->draw( args->m_device, 0 );
		//		else 
		//			args->m_device->set_texture( 0, NULL );
		//	}
		//	else
		//	{
		//		args->m_device->set_texture( 0, NULL );
		//	}

		//	if ( m_texture_array[i].m_self_illuminant )
		//	{
		//		m_texture_array[i].m_self_illuminant->draw( args->m_device, 1 );
		//	}
		//	else
		//	{
		//		args->m_device->set_texture( 1, NULL );
		//	}

		//	m_mesh->draw( i, args->m_device );

		//}

		m_mesh->draw(args);

		args->m_device->pop_transform();

	}







}