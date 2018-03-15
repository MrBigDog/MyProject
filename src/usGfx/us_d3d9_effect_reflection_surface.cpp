///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_reflection_surface.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_reflection_surface class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
//#include "resource.h"
#include "us_d3d9_effect_reflection_surface.h"


namespace uniscope_globe
{
	d3d9_effect_reflection_surface::d3d9_effect_reflection_surface( render_device* device )
		:d3d9_effect_base(device)
	{
		load_from_module_resource(IDR_SHADER_REFLECTION);	
		
		//ustring v_path = singleton_system_environment::instance().m_root_path + L"res\\reflection.fx";
		//load_from_file(v_path.c_str());


		///////////////////////////////////////////////////////////////
		LPDIRECT3DDEVICE9 v_d3d_device = (LPDIRECT3DDEVICE9(device->get_device()));

		//m_water_bump_texture = NULL;
		//ustring v_resource_path = singleton_system_environment::instance().m_root_path + L"res\\WaterBump.jpg";
		//D3DXCreateTextureFromFile( v_d3d_device, v_resource_path.c_str(), &m_water_bump_texture );

		m_ocean_bump_texture.resize(32);
		for(int ni = 0; ni<32; ni++)
		{
			wchar_t str_path[40];
			swprintf(str_path, L"res\\bump1\\Seawater%02d.bmp",ni);
			ustring v_resource_path = singleton_system_environment::instance().m_root_path + ustring(str_path);
			D3DXCreateTextureFromFile( v_d3d_device, v_resource_path.c_str(), &m_ocean_bump_texture[ni] );
		}

		ustring v_resource_path = singleton_system_environment::instance().m_root_path + L"res\\SkyBox.dds";
		m_sky_texture = NULL;
		D3DXCreateCubeTextureFromFile(v_d3d_device, v_resource_path.c_str(), &m_sky_texture);
	}

	d3d9_effect_reflection_surface::~d3d9_effect_reflection_surface( void )
	{
		for( int ni = 0; ni < 32; ni++ )
		{
			AUTO_RELEASE(m_ocean_bump_texture[ni])
		}
		m_ocean_bump_texture.clear();

		AUTO_RELEASE(m_sky_texture);
		//AUTO_RELEASE(m_water_bump_texture);
	}

	bool d3d9_effect_reflection_surface::setup_handle( void )
	{
		if (m_d3dx_effect)
		{	
			m_technique					= m_d3dx_effect->GetTechniqueByName("ReflectionSurfaceTech");
			m_view_proj_matrix_handle	= m_d3dx_effect->GetParameterByName(0, "gViewProj");
			m_world_matrix_handle		= m_d3dx_effect->GetParameterByName(0, "gWorld");
			m_sky_texture_handle		= m_d3dx_effect->GetParameterByName(0, "gEnvMap");
			m_bump_texture_handle		= m_d3dx_effect->GetParameterByName(0, "gBumpMap");
			m_ref_texture_handle		= m_d3dx_effect->GetParameterByName(0, "gRefMap");
			m_eye_positon_handle		= m_d3dx_effect->GetParameterByName(0, "vEyePos");
			m_sun_position_handle		= m_d3dx_effect->GetParameterByName(0, "vSunPos");
			m_water_bump_repeat_handle	= m_d3dx_effect->GetParameterByName(0, "gWaterBumpRepeat");
			m_water_color_handle		= m_d3dx_effect->GetParameterByName(0, "gWaterColor");

			////m_sun_alpha_handle		= m_d3dx_effect->GetParameterByName(0, "fSunAlpha");
			////m_sun_theta_handle		= m_d3dx_effect->GetParameterByName(0, "fSunTheta");
			////m_sun_distance_handle		= m_d3dx_effect->GetParameterByName(0, "fSunDistance");
			//m_sun_shininess_handle		= m_d3dx_effect->GetParameterByName(0, "g_fSunShininess");
			//m_sun_strength_handle		= m_d3dx_effect->GetParameterByName(0, "g_fSunStrength");
			//
			//m_cube_texture_handle		= m_d3dx_effect->GetParameterByName(0, "g_txEnvMap");
			//m_bump_texture_handle		= m_d3dx_effect->GetParameterByName(0, "g_txBump");
			
		}
		return true;
	}

	bool d3d9_effect_reflection_surface::set_view_proj_matrix( const matrix4<float>& mat )
	{
		m_d3dx_effect->SetMatrix( m_view_proj_matrix_handle, (D3DXMATRIX *)&mat );

		return true;
	}

	bool d3d9_effect_reflection_surface::set_world_matrix( const matrix4<float>& mat )
	{
		m_d3dx_effect->SetMatrix( m_world_matrix_handle, (D3DXMATRIX *)&mat );

		return true;
	}

	bool d3d9_effect_reflection_surface::set_eye_positon( const vector3<float>& vec )
	{
		m_d3dx_effect->SetVector( m_eye_positon_handle, &D3DXVECTOR4(vec.x, vec.y, vec.z, 1) );

		return true;
	}

	bool d3d9_effect_reflection_surface::set_sun_position( const vector3<float>& vec )
	{
		m_d3dx_effect->SetVector( m_sun_position_handle, &D3DXVECTOR4(vec.x, vec.y, vec.z, 0) );

		return true;
	}

	bool d3d9_effect_reflection_surface::set_sky_texture( ULONG_PTR ht )
	{
		m_d3dx_effect->SetTexture( m_sky_texture_handle, (LPDIRECT3DBASETEXTURE9)ht );
		return true;
	}

	bool d3d9_effect_reflection_surface::set_bump_texture( ULONG_PTR ht )
	{
		m_d3dx_effect->SetTexture( m_bump_texture_handle, (LPDIRECT3DBASETEXTURE9)ht );
		return true;
	}

	bool d3d9_effect_reflection_surface::set_ref_texture( ULONG_PTR ht )
	{
		m_d3dx_effect->SetTexture( m_ref_texture_handle, (LPDIRECT3DBASETEXTURE9)ht );
		return true;
	}

	bool d3d9_effect_reflection_surface::set_water_color( const color_value& vec )
	{
		m_d3dx_effect->SetVector( m_water_color_handle, &D3DXVECTOR4(vec.r, vec.g, vec.b, vec.a));
		return true;
	}

	//bool d3d9_effect_reflection_surface::set_sun_alpha( float sun_alpha )
	//{
	//	m_d3dx_effect->SetFloat( m_sun_alpha_handle, sun_alpha );
	//	return true;
	//}

	//bool d3d9_effect_reflection_surface::set_sun_theta( float sun_theta )
	//{
	//	m_d3dx_effect->SetFloat( m_sun_theta_handle, sun_theta );
	//	return true;
	//}

	bool d3d9_effect_reflection_surface::set_sun_shininess( float sun_shininess )
	{
		m_d3dx_effect->SetFloat( m_sun_shininess_handle, sun_shininess );
		return true;
	}

	bool d3d9_effect_reflection_surface::set_sun_strength( float sun_strength )
	{
		m_d3dx_effect->SetFloat( m_sun_strength_handle, sun_strength );
		return true;
	}

	bool d3d9_effect_reflection_surface::set_water_bump_repeat( float repeat )
	{
		m_d3dx_effect->SetFloat( m_water_bump_repeat_handle, repeat );
		return true;
	}

	//bool d3d9_effect_reflection_surface::set_sun_distance( float sun_distance )
	//{
	//	m_d3dx_effect->SetFloat( m_sun_distance_handle, sun_distance );
	//	return true;
	//}

	bool d3d9_effect_reflection_surface::begin_render( render_argument* args)
	{
		render_device* device = args->m_device;
		observer_base* v_observer = args->m_observer;
		double v_distance = v_observer->get_position_sphr().z - v_observer->get_dem_height();

		color_value v_water_color;
		USCOLOR_ULONG_TO_COLORVALUE(v_water_color, system_environment::s_water_color);
		int v_rate = USCOLOR_ALPHA(system_environment::s_water_color);
		render_state v_render_state( device );
		v_render_state.set_state(D3DRS_BLENDFACTOR, 0xFF000000 | (v_rate << 16) | (v_rate << 8) | v_rate );
		//v_render_state.set_state( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
		v_render_state.set_state(D3DRS_DESTBLEND,D3DBLEND_INVBLENDFACTOR);
		v_render_state.set_state( D3DRS_ALPHATESTENABLE, TRUE );
		v_render_state.set_state( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
		v_render_state.set_state( D3DRS_ALPHAREF, 1);
		v_render_state.set_state( D3DRS_CULLMODE, D3DCULL_NONE );
		v_render_state.set_state( D3DRS_ZWRITEENABLE, FALSE);

		texture_state v_texture_state( device );
		v_texture_state.set_state( 0, D3DTSS_TEXTURETRANSFORMFLAGS, 2 );


		//matrix4<double> v_mat_tnb = geocentric_coords::transform_from_cartesian_d( vec_position.x * RADIAN, vec_position.y * RADIAN,  v_dem_height);
		//vector3<double> v_eye_pos = v_observer->get_position_geo() * v_mat_tnb.inverse();
		//v_mat_tnb.m41 -= cartesian_coords::s_reference_position_geo.x;
		//v_mat_tnb.m42 -= cartesian_coords::s_reference_position_geo.y;
		//v_mat_tnb.m43 -= cartesian_coords::s_reference_position_geo.z;

		
		//device->push_transform();
		//device->multiply_transform(matrix4<float>((double*)&v_mat_tnb));

		vector3<double> v_sun_pos;
		double v_sky_radius = 1;
		v_sun_pos.x = math<double>::cos_( args->m_shadow_param.m_elevation_angle ) * math<double>::sin_( args->m_shadow_param.m_azimuth_angle ) * v_sky_radius;
		v_sun_pos.y = math<double>::sin_( args->m_shadow_param.m_elevation_angle ) * v_sky_radius;
		v_sun_pos.z = math<double>::cos_( args->m_shadow_param.m_elevation_angle ) * math<double>::cos_( args->m_shadow_param.m_azimuth_angle ) * v_sky_radius;

		int v_frame_counter = args->m_frame_counter->m_frame_count;



		this->set_technique();
		this->begin(0,0);
		this->begin_pass(0);

		this->set_ref_texture( ((render_target*)device->get_render_target(US_RENDER_TARGET_REFLECT))->get_texture() );
		this->set_bump_texture( (ULONG_PTR)(m_ocean_bump_texture[v_frame_counter%m_ocean_bump_texture.size()]) );
		this->set_water_color(v_water_color);
		this->set_sky_texture( (ULONG_PTR)m_sky_texture );
		this->set_water_bump_repeat(system_environment::s_water_bump_repeat );

		/////////////////////////////////////////////////////////////////////////////////
		this->set_view_proj_matrix(device->get_world_matrix() * device->get_view_matrix() * device->get_projection_matrix());
		this->set_world_matrix(matrix4<float>::s_identity);
		this->set_sun_position(vector_3f((double*)(&v_sun_pos)));
		//this->set_eye_positon( vector_3f((double*)(&v_eye_pos)));

		return true;
	}

	bool d3d9_effect_reflection_surface::end_render( render_argument* args)
	{
		render_device* device = args->m_device;

		this->end_pass();
		this->end();
		//device->pop_transform();
		return true;
	}


	
}