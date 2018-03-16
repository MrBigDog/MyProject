///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_d3d9_effect_reflection_surface_ex.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : d3d9_effect_reflection_surface_ex class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "us_d3d9_effect_reflection_surface_ex.h"
#include <usGfx/us_d3d9_render_target_manager.h>
#include <usCore/us_system_environment.h>
#include <usCore/us_extended_material.h>
#include <usCore/us_render_device.h>
#include <usCore/us_render_target.h>
#include <usCore/us_render_argument.h>
#include <usCore/us_observer_base.h>
#include <usCore/us_render_state.h>
#include <usCore/us_frame_counter.h>

#include <usDotx/us_usx_data_struct.h>

namespace uniscope_globe
{
	d3d9_effect_reflection_surface_ex::d3d9_effect_reflection_surface_ex(render_device* device)
		:d3d9_effect_base(device)
	{
		//load_from_module_resource(IDR_SHADER_REFLECTION);	

		ustring v_path = singleton_system_environment::instance().m_root_path + L"shader\\refraction.fx";
		load_from_file(v_path.c_str());


		///////////////////////////////////////////////////////////////
		LPDIRECT3DDEVICE9 v_d3d_device = (LPDIRECT3DDEVICE9(device->get_device()));

		m_water_bump_texture = NULL;
		ustring v_resource_path1 = singleton_system_environment::instance().m_root_path + L"shader\\wavesbump.dds";
		D3DXCreateTextureFromFile(v_d3d_device, v_resource_path1.c_str(), &m_water_bump_texture);

		//m_ocean_bump_texture.resize(32);
		//for(int ni = 0; ni<32; ni++)
		//{
		//	wchar_t str_path[40];
		//	swprintf(str_path, L"res\\bump1\\Seawater%02d.bmp",ni);
		//	ustring v_resource_path = singleton_system_environment::instance().m_root_path + ustring(str_path);
		//	D3DXCreateTextureFromFile( v_d3d_device, v_resource_path.c_str(), &m_ocean_bump_texture[ni] );
		//}

		ustring v_resource_path = singleton_system_environment::instance().m_root_path + L"res\\SkyBox.dds";
		m_sky_texture = NULL;
		D3DXCreateCubeTextureFromFile(v_d3d_device, v_resource_path.c_str(), &m_sky_texture);

		m_texture_offsets = vector4<float>(0.0f, 0.0f, 0.2f, 0.3f);
		mRateNormMap0 = vector3<float>(system_environment::s_water_move_rate_x, system_environment::s_water_move_rate_y, 0.0f);
		mRateNormMap1 = vector3<float>(-0.01f * 0.1f, 0.03f * 0.1f, 0.0f);
	}

	d3d9_effect_reflection_surface_ex::~d3d9_effect_reflection_surface_ex(void)
	{
		//for( int ni = 0; ni < 32; ni++ )
		//{
		//	AUTO_RELEASE(m_ocean_bump_texture[ni])
		//}
		//m_ocean_bump_texture.clear();

		AUTO_RELEASE(m_sky_texture);
		AUTO_RELEASE(m_water_bump_texture);
	}

	bool d3d9_effect_reflection_surface_ex::setup_handle(void)
	{
		if (m_d3dx_effect)
		{
			m_technique = m_d3dx_effect->GetTechniqueByName("ReflectionSurfaceTech");
			m_view_proj_matrix_handle = m_d3dx_effect->GetParameterByName(0, "gViewProj");
			m_world_matrix_handle = m_d3dx_effect->GetParameterByName(0, "gWorld");
			m_world_matrix_inv_handle = m_d3dx_effect->GetParameterByName(0, "gWorldInv");
			m_sky_texture_handle = m_d3dx_effect->GetParameterByName(0, "gEnvMap");
			m_bump0_texture_handle = m_d3dx_effect->GetParameterByName(0, "gNormMap0");
			m_bump1_texture_handle = m_d3dx_effect->GetParameterByName(0, "gNormMap1");
			m_refract_texture_handle = m_d3dx_effect->GetParameterByName(0, "gRefractMap");
			m_reflect_texture_handle = m_d3dx_effect->GetParameterByName(0, "gReflectMap");
			m_alpha_color_texture_handle = m_d3dx_effect->GetParameterByName(0, "gAlphaColorMap");
			m_eye_positon_handle = m_d3dx_effect->GetParameterByName(0, "gEyePosW");
			m_sun_position_handle = m_d3dx_effect->GetParameterByName(0, "vSunPos");
			m_water_bump_repeat_handle = m_d3dx_effect->GetParameterByName(0, "fWaveLength");
			m_water_color_handle = m_d3dx_effect->GetParameterByName(0, "gDiffuseMtrl");
			m_texture_offset_handle = m_d3dx_effect->GetParameterByName(0, "gTexOffsets");
			m_water_depth_handle = m_d3dx_effect->GetParameterByName(0, "fWaterDeepVisable");


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

	bool d3d9_effect_reflection_surface_ex::set_view_proj_matrix(const matrix4<float>& mat)
	{
		m_d3dx_effect->SetMatrix(m_view_proj_matrix_handle, (D3DXMATRIX *)&mat);

		return true;
	}

	bool d3d9_effect_reflection_surface_ex::set_world_matrix(const matrix4<float>& mat)
	{
		matrix4<float> matInv = matrix4<float>::inverse(mat);
		m_d3dx_effect->SetMatrix(m_world_matrix_handle, (D3DXMATRIX *)&mat);
		m_d3dx_effect->SetMatrix(m_world_matrix_inv_handle, (D3DXMATRIX *)&matInv);

		return true;
	}

	bool d3d9_effect_reflection_surface_ex::set_eye_positon(const vector3<float>& vec)
	{
		m_d3dx_effect->SetVector(m_eye_positon_handle, &D3DXVECTOR4(vec.x, vec.y, vec.z, 1));

		return true;
	}

	bool d3d9_effect_reflection_surface_ex::set_sun_position(const vector3<float>& vec)
	{
		m_d3dx_effect->SetVector(m_sun_position_handle, &D3DXVECTOR4(vec.x, vec.y, vec.z, 0));

		return true;
	}

	bool d3d9_effect_reflection_surface_ex::set_sky_texture(ULONG_PTR ht)
	{
		m_d3dx_effect->SetTexture(m_sky_texture_handle, (LPDIRECT3DBASETEXTURE9)ht);
		return true;
	}

	bool d3d9_effect_reflection_surface_ex::set_bump0_texture(ULONG_PTR ht)
	{
		m_d3dx_effect->SetTexture(m_bump0_texture_handle, (LPDIRECT3DBASETEXTURE9)ht);
		return true;
	}

	bool d3d9_effect_reflection_surface_ex::set_bump1_texture(ULONG_PTR ht)
	{
		m_d3dx_effect->SetTexture(m_bump1_texture_handle, (LPDIRECT3DBASETEXTURE9)ht);
		return true;
	}

	bool d3d9_effect_reflection_surface_ex::set_reflect_texture(ULONG_PTR ht)
	{
		m_d3dx_effect->SetTexture(m_reflect_texture_handle, (LPDIRECT3DBASETEXTURE9)ht);
		return true;
	}

	bool d3d9_effect_reflection_surface_ex::set_refract_texture(ULONG_PTR ht)
	{
		m_d3dx_effect->SetTexture(m_refract_texture_handle, (LPDIRECT3DBASETEXTURE9)ht);
		return true;
	}

	bool d3d9_effect_reflection_surface_ex::set_alpha_color_texture(ULONG_PTR ht)
	{
		m_d3dx_effect->SetTexture(m_alpha_color_texture_handle, (LPDIRECT3DBASETEXTURE9)ht);
		return true;
	}

	bool d3d9_effect_reflection_surface_ex::set_water_color(const color_value& vec)
	{
		m_d3dx_effect->SetVector(m_water_color_handle, &D3DXVECTOR4(vec.r, vec.g, vec.b, vec.a));
		return true;
	}

	bool d3d9_effect_reflection_surface_ex::set_water_depth(float repeat)
	{
		m_d3dx_effect->SetFloat(m_water_depth_handle, repeat);
		return true;
	}

	//bool d3d9_effect_reflection_surface_ex::set_sun_alpha( float sun_alpha )
	//{
	//	m_d3dx_effect->SetFloat( m_sun_alpha_handle, sun_alpha );
	//	return true;
	//}

	//bool d3d9_effect_reflection_surface_ex::set_sun_theta( float sun_theta )
	//{
	//	m_d3dx_effect->SetFloat( m_sun_theta_handle, sun_theta );
	//	return true;
	//}

	bool d3d9_effect_reflection_surface_ex::set_texture_offset(const vector4<float>& vec)
	{
		m_d3dx_effect->SetVector(m_texture_offset_handle, (D3DXVECTOR4 *)&vec);
		return true;
	}


	bool d3d9_effect_reflection_surface_ex::set_sun_shininess(float sun_shininess)
	{
		m_d3dx_effect->SetFloat(m_sun_shininess_handle, sun_shininess);
		return true;
	}

	bool d3d9_effect_reflection_surface_ex::set_sun_strength(float sun_strength)
	{
		m_d3dx_effect->SetFloat(m_sun_strength_handle, sun_strength);
		return true;
	}

	bool d3d9_effect_reflection_surface_ex::set_water_bump_repeat(float repeat)
	{
		m_d3dx_effect->SetFloat(m_water_bump_repeat_handle, repeat);
		return true;
	}

	//bool d3d9_effect_reflection_surface_ex::set_sun_distance( float sun_distance )
	//{
	//	m_d3dx_effect->SetFloat( m_sun_distance_handle, sun_distance );
	//	return true;
	//}

	bool d3d9_effect_reflection_surface_ex::begin_render(render_argument* args)
	{
		render_device* device = args->m_device;
		observer_base* v_observer = args->m_observer;
		double v_distance = v_observer->get_position_sphr().z - v_observer->get_dem_height();

		color_value v_water_color;
		USCOLOR_ULONG_TO_COLORVALUE(v_water_color, system_environment::s_water_color);
		int v_rate = USCOLOR_ALPHA(system_environment::s_water_color);
		render_state v_render_state(device);
		v_render_state.set_state(D3DRS_BLENDFACTOR, 0xFF000000 | (v_rate << 16) | (v_rate << 8) | v_rate);
		//v_render_state.set_state( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		//v_render_state.set_state(D3DRS_ALPHABLENDENABLE, TRUE);
		//v_render_state.set_state(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
		//v_render_state.set_state(D3DRS_DESTBLEND,D3DBLEND_INVBLENDFACTOR);
		//v_render_state.set_state( D3DRS_ALPHATESTENABLE, TRUE );
		//v_render_state.set_state( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
		//v_render_state.set_state( D3DRS_ALPHAREF, 1);
		v_render_state.set_state(D3DRS_CULLMODE, D3DCULL_NONE);
		//v_render_state.set_state( D3DRS_ZWRITEENABLE, FALSE);

		texture_state v_texture_state(device);
		v_texture_state.set_state(0, D3DTSS_TEXTURETRANSFORMFLAGS, 2);



		//matrix4<double> v_mat_tnb = geocentric_coords::transform_from_cartesian_d( vec_position.x * RADIAN, vec_position.y * RADIAN,  v_dem_height);
		//vector3<double> v_eye_pos = v_observer->get_position_geo() * v_mat_tnb.inverse();
		//v_mat_tnb.m41 -= cartesian_coords::s_reference_position_geo.x;
		//v_mat_tnb.m42 -= cartesian_coords::s_reference_position_geo.y;
		//v_mat_tnb.m43 -= cartesian_coords::s_reference_position_geo.z;


		//device->push_transform();
		//device->multiply_transform(matrix4<float>((double*)&v_mat_tnb));

		vector3<double> v_sun_pos;
		double v_sky_radius = 1;
		v_sun_pos.x = math<double>::cos_(args->m_shadow_param.m_elevation_angle) * math<double>::sin_(args->m_shadow_param.m_azimuth_angle) * v_sky_radius;
		v_sun_pos.y = math<double>::sin_(args->m_shadow_param.m_elevation_angle) * v_sky_radius;
		v_sun_pos.z = math<double>::cos_(args->m_shadow_param.m_elevation_angle) * math<double>::cos_(args->m_shadow_param.m_azimuth_angle) * v_sky_radius;

		int v_frame_counter = args->m_frame_counter->m_frame_count;



		this->set_technique();
		this->begin(0, 0);
		this->begin_pass(0);

		this->set_reflect_texture(((render_target*)device->get_render_target(US_RENDER_TARGET_REFLECT))->get_texture());
		this->set_refract_texture(((render_target*)device->get_render_target(US_RENDER_TARGET_REFRACT))->get_texture());
		//this->set_bump0_texture( (ulong)(m_ocean_bump_texture[v_frame_counter%m_ocean_bump_texture.size()]) );
		this->set_bump0_texture((ULONG_PTR)(m_water_bump_texture));
		this->set_water_color(v_water_color);
		this->set_sky_texture((ULONG_PTR)m_sky_texture);
		this->set_water_bump_repeat(system_environment::s_water_bump_repeat);
		//
		//this->update_offset(0.05);
		this->set_texture_offset(m_texture_offsets);
		/////////////////////////////////////////////////////////////////////////////////
		this->set_view_proj_matrix(device->get_view_matrix() * device->get_projection_matrix());
		this->set_world_matrix(device->get_world_matrix());
		//this->set_world_matrix(matrix4<float>::s_identity);
		this->set_sun_position(vector_3f((double*)(&v_sun_pos)));
		//this->set_eye_positon( vector_3f((double*)(&v_eye_pos)));


		return true;
	}

	bool d3d9_effect_reflection_surface_ex::update_offset(float dt)
	{
		m_texture_offsets += (vector4<float>(mRateNormMap0.x, mRateNormMap0.y, mRateNormMap1.x, mRateNormMap1.y) * dt);

		if (m_texture_offsets.x >= 1.0f) m_texture_offsets.x -= 1.0f;
		if (m_texture_offsets.y >= 1.0f) m_texture_offsets.y -= 1.0f;
		if (m_texture_offsets.z >= 1.0f) m_texture_offsets.z -= 1.0f;
		if (m_texture_offsets.w >= 1.0f) m_texture_offsets.w -= 1.0f;

		if (m_texture_offsets.x <= -1.0f) m_texture_offsets.x += 1.0f;
		if (m_texture_offsets.y <= -1.0f) m_texture_offsets.y += 1.0f;
		if (m_texture_offsets.z <= -1.0f) m_texture_offsets.z += 1.0f;
		if (m_texture_offsets.w <= -1.0f) m_texture_offsets.w += 1.0f;
		return true;
	}

	bool d3d9_effect_reflection_surface_ex::end_render(render_argument* args)
	{
		render_device* device = args->m_device;

		this->end_pass();
		this->end();
		//device->pop_transform();
		return true;
	}



}