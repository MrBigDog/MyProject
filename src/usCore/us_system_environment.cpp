///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_system_environment.cpp
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : system_environment class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
//#include "Stdafx.h"
#include "us_system_environment.h"

namespace uniscope_globe
{
	system_environment* singleton_system_environment::s_ins = NULL;

	

	// download config
	double system_environment::s_download_sleep_interval = 20.0;
	double system_environment::s_download_sleep_time = 350.0;
	double system_environment::s_download_check_interval = 3.0;
	double system_environment::s_attenuate_interval = 0.250;
	int system_environment::s_attenuate_speed = 5;
	int	system_environment::s_download_thread_count = 3;
	double	system_environment::s_max_process_memory_size = 1<<20;

	int system_environment::s_licence_server_status = -1;
	int system_environment::s_licence_server_enable = 1;

	// create config
	int	system_environment::s_create_mesh_num = 3;
	int system_environment::s_create_mesh_size = 1<<19;

	int	system_environment::s_create_texture_num = 3;
	int	system_environment::s_create_texture_size = 1<<19;

	int system_environment::s_always_create_hardware = 1;

	// cache config
	bool system_environment::s_use_cache = false;

	// render config
	//time_value system_environment::s_last_frame_tick_count = 0.0;
	//time_value system_environment::s_frame_time = 0.0;
	//time_value system_environment::s_last_logic_tick_count = 0.0;
	//time_value system_environment::s_logic_time = 0.0;

	ulong system_environment::s_max_frame_time = 45;

	// actor config
	float system_environment::s_actor_walking_volocity = 0.1f;
	float system_environment::s_actor_running_volocity = 0.2f;
	float system_environment::s_actor_rotation_volocity = 30.0f;

	// reclaim
	double system_environment::s_reclaim_memory_size = 400;
	int	system_environment::s_resource_life_time = 45;

	// fps
	int system_environment::s_max_fps = 30;
	int system_environment::s_timer_interval = 5;
	int system_environment::s_fps_locking_mode = 1;
	int system_environment::s_hardware_timer = 0;// 0 WM_TIMER, 1 HARDWARE_TIMER

	// terrain
	double system_environment::s_draw_spread = 0.59;
	double system_environment::s_draw_distance = 8.0;
	int system_environment::s_create_terrain_num = 3;
	float system_environment::s_terrain_texture_bias = -1.1f;

	// bldg
	int	system_environment::s_max_range = 8;
	int	system_environment::s_min_range = 6;
	int	system_environment::s_mip_for_distance = 850;
	int	system_environment::s_mip_thr_distance = 1250;
	int	system_environment::s_mip_two_distance = 3200;
	int	system_environment::s_mip_one_distance = 6000;
	int	system_environment::s_mip_zro_distance = 9000;
	int	system_environment::s_material_levels  = 1;
	float system_environment::s_building_texture_bias = 0.2f;

	int system_environment::s_model_render_bios = 50;

	int system_environment::s_lod_grade_distance = 900;

	int system_environment::s_use_log = 0;

	int system_environment::s_model_use_default_texture = 0;

	int	system_environment::s_min_range_in_scene = 2;
	int	system_environment::s_max_range_in_scene = 2;

	int		system_environment::s_network_type = 0;
	double	system_environment::s_core_distance = 500;

	// poi
	int	system_environment::s_poi_refresh_interval = 10;

	// dem
	double	system_environment::s_current_max_elevation = 5.0;
	double	system_environment::s_current_min_elevation = 0.0;

	int system_environment::s_HoverElapseTime = 100;

	int system_environment::s_max_dom_level = 0;

	double system_environment::s_fog_density_factor = 0.05;

	double system_environment::s_fog_distance_factor = 1.0;

	double system_environment::s_fog_density = 0.0;

	bool system_environment::s_debug_mode = false;

	bool system_environment::s_new_building = false;
	
	long system_environment::s_transparent_color = 0x66333333;

	long system_environment::s_transparent_color2 = 0x66333333;

	double system_environment::s_transparent_bengin = 0.3;

	double system_environment::s_transparent_end = 0.8;

	ulong system_environment::s_video_adapter_ram = 268435456;

	long system_environment::s_multi_sample_type = 0;

	bool system_environment::s_show_navigator = true;

	bool system_environment::s_show_logo = true;

	bool system_environment::s_show_information = true;

	bool system_environment::s_full_screen_shadow = false;

	bool system_environment::s_enable_shadow = false;

	bool system_environment::s_preprocess_normal = true;

	double system_environment::s_min_observer_height = 0.0f;

	double system_environment::s_auxplane_elevation = 0.0f;

	bool system_environment::s_under_ground_mode = false;

	long system_environment::s_terrain_render_mode = 0;

	long system_environment::s_browse_mode = 0;

	double system_environment::s_camera_height = 0;

	long system_environment::s_terrain_transparency = 0xff;

	bool system_environment::s_reclaim_mesh = true;

	bool system_environment::s_draw_center_point = true;

	ulong system_environment::s_base_data_type = 0x3ff;

	int	system_environment::m_event_dispatch_mode = 3; //US_EVENT_DISPATCH_ALL

	bool system_environment::m_enable_hover_message = false;

	float system_environment::full_screen_saturation = 1.35f;

	float system_environment::full_screen_luminance = 0.42f;

	float system_environment::full_screen_contract = 1.0;

	bool  system_environment::hsl_adjustment_enable = false;

	// DX9 如果光照关闭的话，没有材质的情况下，管线渲染那即使设置了颜色，也会不显示，所以默认打开 [9/7/2017 mahc]
	bool system_environment::s_light_enable = true;

	bool system_environment::s_is_light_inverse = false;

	bool	system_environment::s_custom_precision_mode = false;

	double	system_environment::s_custom_precision = 1.0;


	bool system_environment::s_all_model_cull_none = false;

	bool system_environment::s_document_enable_update = true;

	bool system_environment::s_use_high_quality_model = false;

	bool system_environment::s_generate_topologic = true;

	bool system_environment::s_remain_raw_buffer = true;

	ulong system_environment::s_water_color = 0xff0033366;

	float system_environment::s_water_bump_repeat = 1;

	double system_environment::s_water_height = 0.0;

	bool system_environment::s_enable_ude_server = false;

	bool system_environment::s_enable_analysis_server = false;

	bool system_environment::s_enable_search_server = false;

	bool system_environment::s_enable_export_vedio = false;

	bool system_environment::s_enable_export_map = false;

	bool system_environment::s_enable_export_model = false;


	double system_environment::s_terrain_vertical_scale = 1.0;

	bool system_environment::s_zoom_center_mode = false;

	bool system_environment::s_right_button_zoom = true;

	double	system_environment::s_water_move_rate_x = 0.002f;

	double	system_environment::s_water_move_rate_y = 0.006f;

	bool system_environment::s_custom_water_suface = false;

	vector3<double> system_environment::s_water_normal = vector3<double>(0,1,0);

	vector3<double> system_environment::s_water_position = vector3<double>(0,0,0);

	double system_environment::s_water_depth = 0.05f;

	int system_environment::s_information_language = 0;

	bool system_environment::s_enable_fly = true;

	bool system_environment::s_use_compressed_texture = true;

	int system_environment::s_update_interval = 50;

	int system_environment::s_ajust_tile_count = 1;

	double system_environment::s_average_terrain_height = 0;

	double system_environment::s_reclaim_memory_per_frame = 104857600;///<modify by felix 控制每一帧内存卸载的极大值100M 

	bool system_environment::s_enable_light_map = 1;

#ifdef _PERFORMANCE_TEST_
	ulong system_environment::s_request_download_mission_count = 0;

	ulong system_environment::s_update_download_mission_count = 0;
	ulong system_environment::s_update_download_mission_heap_size = 0;

	bool system_environment::s_enable_mesh_texture = true;
	bool system_environment::s_enable_displayDem = true;
	ustring system_environment::s_sTouchInfo;

	interior_index system_environment::s_nDrawingVertexNum = 0;
	interior_index system_environment::s_nDrawingTriangleNum = 0;
	ulong  system_environment::s_nNumberOfBeingRender = 0;

#endif

	system_environment::system_environment( void )
	{
		m_ai_server_port = 80;

		m_server_link_array.resize(12);
	}

	system_environment::~system_environment( void )
	{
		m_string_map.clear();
	}

	void system_environment::register_key_pair( ustring in_key, ustring in_value )
	{
		m_string_map.insert( make_pair( in_key, in_value) );
	}

	ustring system_environment::get_value_by_key( ustring in_key )
	{
		string_map::iterator itr = m_string_map.find(in_key);
		if ( itr != m_string_map.end() )
		{
			return itr->second;
		}
		
		return L"";
	}

	//void system_environment::update_frame_time( void )
	//{
	//	static time_value v_last_frame_time;
	//	s_frame_time = (double)timeGetTime() * 0.001;
	//	s_last_frame_tick_count = s_frame_time - v_last_frame_time;
	//	v_last_frame_time = s_frame_time;
	//}

	//void system_environment::update_logic_time( void )
	//{
	//	static time_value v_last_logic_time;
	//	s_logic_time = (double)timeGetTime() * 0.001;
	//	s_last_logic_tick_count = s_logic_time - v_last_logic_time;
	//	v_last_logic_time = s_logic_time;
	//}	
}