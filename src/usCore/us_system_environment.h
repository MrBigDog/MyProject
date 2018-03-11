///////////////////////////////////////////////////////////////////////////
//
//  This source file is part of Uniscope Virtual Globe
//  Copyright (c) 2008-2009 by The Uniscope Team . All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////
//
//  Filename: us_system_environment.h
//  Author  : Uniscope Team 
//  Modifier: Uniscope Team  
//  Created : 
//  Purpose : system_environment class
//	Reference : 
//
///////////////////////////////////////////////////////////////////////////
#ifndef _US_SYSTEM_ENVIRONMENT_H_
#define _US_SYSTEM_ENVIRONMENT_H_

#include <usCore\Export.h>
#include <map>

#ifndef _PUBLISH
#define US_RENDER_TRACE0(format) { wchar_t str[1024]; swprintf( str, format ); singleton_system_environment::instance().m_debug_info += str; }
#define US_RENDER_TRACE1(format, val1) { wchar_t str[1024]; swprintf( str, format, val1 ); singleton_system_environment::instance().m_debug_info += str;}
#define US_RENDER_TRACE2(format, val1, val2) { wchar_t str[1024]; swprintf( str, format, val1, val2 ); singleton_system_environment::instance().m_debug_info += str;}
#define US_RENDER_TRACE3(format, val1, val2, val3) { wchar_t str[1024]; swprintf( str, format, val1, val2, val3 ); singleton_system_environment::instance().m_debug_info += str;}
#else
#define US_RENDER_TRACE0(format)
#define US_RENDER_TRACE1(format, val1) 
#define US_RENDER_TRACE2(format, val1, val2) 
#define US_RENDER_TRACE3(format, val1, val2, val3) 
#endif //_DEBUG

namespace uniscope_globe
{
	class USCORE_EXPORT system_environment
	{
	public:
		typedef std::map<ustring, ustring> string_map;

	public:
		system_environment(void);

		virtual ~system_environment(void);

		void register_key_pair(ustring in_key, ustring in_value);

		ustring get_value_by_key(ustring in_key);

		//static void update_frame_time( void );

		//static void update_logic_time( void );

	public:
		ustring			m_connection_url;

		ustring			m_application_path;

		ustring			m_actor_res_path;

		ustring			m_root_path;

		ustring			m_sky_texture_path;

		ustring			m_halo_texture_path;

		ustring			m_ai_server;

		ulong			m_ai_server_port;

		string_map		m_string_map;

		ustring			m_debug_info;

		ustring			m_computer_performance;

		ustring			m_license_server;

		ustring			m_logo_file_name;

		ustring			m_provider_file_name;

		ustring			m_fecth_height_service;

		ustring			m_analysis_service;

		ustring			m_log_path;///<add by Felix

		ustring			m_cache_path;

		rect			m_navigator_rect;

		rect			m_provider_rect;

		rect			m_logo_rect;

		ustring			m_auxplane_texture_path;

		std::vector<ustring> m_server_link_array;



	public:

		// download config
		static double	s_download_sleep_interval;
		static double	s_download_sleep_time;
		static double	s_download_check_interval;
		static double	s_attenuate_interval;
		static int		s_attenuate_speed;
		static int		s_download_thread_count;
		static double	s_max_process_memory_size;///<modify by felix 将类型由int改为double，目的：支持64位

		static int		s_licence_server_enable;
		static int		s_licence_server_status;

		// create config
		static int		s_create_mesh_num;
		static int		s_create_mesh_size;

		static int		s_create_texture_num;
		static int		s_create_texture_size;

		static bool		s_use_cache;

		// render config
		//static time_value s_last_frame_tick_count;
		//static time_value s_last_logic_tick_count;
		//static time_value s_frame_time;
		//static time_value s_logic_time;

		static	ulong	s_max_frame_time;

		// actor config
		static float	s_actor_walking_volocity;
		static float	s_actor_running_volocity;
		static float	s_actor_rotation_volocity;

		// reclaim
		///modify by felix 
		static double	s_reclaim_memory_size;
		static int		s_resource_life_time;

		// fps
		static int		s_max_fps;
		static int      s_timer_interval;
		static int		s_fps_locking_mode;
		static int		s_hardware_timer;

		// terrain
		static double	s_draw_spread;
		static double	s_draw_distance;
		static int		s_create_terrain_num;
		static float	s_terrain_texture_bias;

		// bldg
		static int		s_max_range;
		static int		s_min_range;
		static int		s_mip_for_distance;;
		static int		s_mip_thr_distance;
		static int		s_mip_two_distance;
		static int		s_mip_one_distance;
		static int		s_mip_zro_distance;

		static int		s_material_levels;

		static int		s_model_render_bios;

		static int		s_lod_grade_distance;///add by Felix 用于第二级lod的距离设置

		static int		s_use_log;///是否使用日志开关，日志分为本地日志和网络日志

		static int		s_model_use_default_texture;///add by Felix 是否使用默认纹理

		static int		s_min_range_in_scene;
		static int		s_max_range_in_scene;

		static float	s_building_texture_bias;

		static int		s_network_type;
		static double	s_core_distance;

		// poi
		static int		s_poi_refresh_interval;

		// dem
		static double	s_current_max_elevation;
		static double	s_current_min_elevation;

		static int	   s_max_dom_level;

		static int		s_HoverElapseTime;

		static double	s_fog_density_factor;

		static double   s_fog_distance_factor;

		static double	s_fog_density;

		static bool		s_debug_mode;

		static bool		s_new_building;

		static long	    s_transparent_color;

		static long	    s_transparent_color2;

		static double	s_transparent_bengin;

		static double	s_transparent_end;

		static ulong	s_video_adapter_ram;

		static int		s_always_create_hardware;

		static long		s_multi_sample_type;

		static bool		s_show_navigator;

		static bool		s_show_logo;

		static bool		s_show_information;

		static bool		s_full_screen_shadow;

		static bool     s_enable_shadow;

		static bool		s_preprocess_normal;

		static double	s_min_observer_height;

		static double   s_auxplane_elevation;

		static bool		s_under_ground_mode;

		static long		s_terrain_render_mode;

		static long		s_browse_mode;

		static double	s_camera_height;

		static long		s_terrain_transparency;

		static bool		s_reclaim_mesh;

		static bool     s_draw_center_point;

		static ulong	s_base_data_type;

		static int		m_event_dispatch_mode;

		static bool		m_enable_hover_message;

		static float	full_screen_saturation;

		static float	full_screen_luminance;

		static float	full_screen_contract;

		static bool		hsl_adjustment_enable;

		static bool		s_light_enable;

		static bool     s_is_light_inverse; //byl  add 20170515 光照方向，向下是false,向上是true 


		static bool		s_custom_precision_mode;

		static double	s_custom_precision;

		static bool		s_all_model_cull_none;

		static bool		s_generate_topologic;

		static bool		s_remain_raw_buffer;

		static ulong	s_water_color;

		static float	s_water_bump_repeat;

		static double	s_water_height;

		static bool		s_use_high_quality_model;

		static bool		s_document_enable_update;

		static double	s_terrain_vertical_scale;

		static bool		s_zoom_center_mode;

		static bool     s_right_button_zoom;

		static double	s_water_move_rate_x;

		static double   s_water_move_rate_y;

		static bool		s_custom_water_suface;

		static vector3<double> s_water_normal;

		static vector3<double> s_water_position;

		static double   s_water_depth;

		static double	s_reclaim_memory_per_frame;///<modify by felix 控制每一帧内存卸载的极大值 


		// 语言，0：英文，1：中文
		static int		s_information_language;


		// 是否支持鼠标惯性
		static bool     s_enable_fly;

		static bool		s_use_compressed_texture;


		static int	s_update_interval;

		static int		s_ajust_tile_count;

		static double	s_average_terrain_height;

		static bool s_enable_light_map;



		// license check
	public:
		static bool		s_enable_ude_server;

		static bool		s_enable_analysis_server;

		static bool		s_enable_search_server;

		static bool		s_enable_export_vedio;

		static bool		s_enable_export_map;

		static bool		s_enable_export_model;




#ifdef _PERFORMANCE_TEST_
		static ulong	s_request_download_mission_count;

		static ulong	s_update_download_mission_count;

		static ulong	s_update_download_mission_heap_size;

		static bool s_enable_mesh_texture;
		static bool s_enable_displayDem;//-默认是显示DEM的,但有时候需要看模型的三角面，这时去除DEM效果比较好
		static ustring s_sTouchInfo;

		// 测试用 每一帧绘制的 [11/6/2017 mahc]

		static ulong  s_nNumberOfBeingRender;
		static interior_index s_nDrawingVertexNum;
		static interior_index s_nDrawingTriangleNum;
#endif
	};

	class USCORE_EXPORT singleton_system_environment
		: public singleton<system_environment>
	{

	public:
		singleton_system_environment(void)
		{
		}

		virtual ~singleton_system_environment(void)
		{
		}
	};
}

#endif // _US_SYSTEM_ENVIRONMENT_H_