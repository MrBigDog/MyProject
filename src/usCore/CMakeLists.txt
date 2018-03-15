IF   (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSCORE_LIBRARY)
ELSE (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSCORE_LIBRARY_STATIC)
ENDIF(DYNAMIC_GWEARTH)

IF(WIN32)
    IF(MSVC)
        SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRT")
    ENDIF(MSVC)
ENDIF(WIN32)

SET(LIB_NAME "usCore")

SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME})
#INCLUDE_DIRECTORIES(AFTER ${OSG_INCLUDE_DIR})
#INCLUDE_DIRECTORIES(AFTER ${OSGEARTH_INCLUDE_DIR})

SET(LIB_PUBLIC_HEADERS
				Export.h
				data_bank.h
				data_clerk.h
				DebugTypes.h
				exception_base.h
				gw_exception_public.h
				SimpleSymbolEngine.h
				struct_exception_base.h
				us_application_base.h
				us_brush_style.h
				us_cartesian_object.h
				us_clamp_base.h
				us_collision_node.h
				us_command_base.h
				us_command_instant_executant.h
				us_command_ordered_executant.h
				us_common_loader.h
				us_common_manager_group.h
				us_common_mission_queue.h
				us_common_render.h
				us_common_resource_container.h
				us_data_loader.h
				us_document_base.h
				us_double_buffer_array.h
				us_double_buffer_map.h
				us_downloader.h
				us_download_argument.h
				us_download_mission.h
				us_editable_object.h
				us_event_argument.h
				us_event_base.h
				us_event_handle.h
				us_event_sender.h
				us_extended_material.h
				us_filter_base.h
				us_filter_buffer.h
				us_filter_manager.h
				us_font_style.h
				us_frame_counter.h
				us_function_base.h
				us_guid.h
				us_hardware_base.h
				us_hardware_creator.h
				us_hardware_mesh.h
				us_hardware_occlusion_query.h
				us_hardware_texture.h
				us_http_loader.h
				us_icon_style.h
				us_intersect_argument.h
				us_intersect_base.h
				us_intersect_helper.h
				us_intersect_result.h
				us_intersect_template.h
				us_link_base.h
				us_link_http.h
				us_link_manager.h
				us_logcreator.h
				us_logstream.h
				us_member_function.h
				us_mesh_creator.h
				us_mission_base.h
				us_mission_executant.h
				us_mission_executant_async.h
				us_mission_flow.h
				us_mission_heap.h
				us_mission_queue.h
				us_native_loader.h
				us_network_link.h
				us_node_base.h
				us_node_visitor.h
				us_observer_base.h
				us_observer_helper.h
				us_octree_node.h
				us_operator_base.h
				us_operator_executant.h
				us_operator_instant_executant.h
				us_operator_ordered_executant.h
				us_pen_style.h
				us_physical_system.h
				us_plugin_manager.h
				us_priority_stack.h
				us_proxy_resource_container.h
				us_queue.h
				us_reclaim_base.h
				us_renderable_base.h
				us_render_argument.h
				us_render_common.h
				us_render_device.h
				us_render_device_capabilities.h
				us_render_effect.h
				us_render_manager_base.h
				us_render_node.h
				us_render_object.h
				us_render_sprite.h
				us_render_state.h
				us_render_target.h
				us_render_transform.h
				us_resource_base.h
				us_resource_connection.h
				us_resource_container.h
				us_resource_killer.h
				us_resource_manager.h
				us_resource_reclaimer.h
				us_rigid_body.h
				us_sampler_state.h
				us_script_ext.h
				us_script_module.h
				us_shadow_parameter.h
				us_shadow_style.h
				us_shadow_volume_creator.h
				us_shared_data.h
				us_shared_object.h
				us_shared_ptr.h
				us_sound_base.h
				us_sound_device.h
				us_sound_style.h
				us_spatial_node.h
				us_spatial_object.h
				us_spherical_object.h
				us_square_buffer.h
				us_style_base.h
				us_styling_object.h
				us_swap_chain.h
				us_system_environment.h
				us_texture_creator.h
				us_texture_fill_style.h
				us_texture_map_description.h
				us_texture_state.h
				us_threadsafe_memory_pool.h
				us_thread_base.h
				us_thread_data.h
				us_thread_valve.h
				us_timer_queue_timer.h
				us_time_cost.h
				us_time_style.h
				us_tls_singleton.h
				us_update_argument.h
				us_vertex_buffer_binding.h
				us_vertex_declaration.h
				us_view_base.h
				us_view_port.h
)
SET(TARGET_SRC
				data_bank.cpp
				data_clerk.cpp
				exception_base.cpp
				SimpleSymbolEngine.cpp
				struct_exception_base.cpp
				us_application_base.cpp
				us_brush_style.cpp
				us_cartesian_object.cpp
				us_clamp_base.cpp
				us_collision_node.cpp
				us_command_base.cpp
				us_command_instant_executant.cpp
				us_command_ordered_executant.cpp
				us_common_loader.cpp
				us_common_manager_group.cpp
				us_common_mission_queue.cpp
				us_common_render.cpp
				us_common_resource_container.cpp
				us_data_loader.cpp
				us_document_base.cpp
				us_double_buffer_array.cpp
				us_double_buffer_map.cpp
				us_downloader.cpp
				us_download_argument.cpp
				us_download_mission.cpp
				us_editable_object.cpp
				us_event_argument.cpp
				us_event_base.cpp
				us_event_handle.cpp
				us_event_sender.cpp
				us_extended_material.cpp
				us_filter_base.cpp
				us_filter_buffer.cpp
				us_filter_manager.cpp
				us_font_style.cpp
				us_frame_counter.cpp
				us_guid.cpp
				us_hardware_base.cpp
				us_hardware_creator.cpp
				us_hardware_mesh.cpp
				us_hardware_occlusion_query.cpp
				us_hardware_texture.cpp
				us_http_loader.cpp
				us_icon_style.cpp
				us_intersect_argument.cpp
				us_intersect_base.cpp
				us_intersect_helper.cpp
				us_intersect_result.cpp
				us_intersect_template.cpp
				us_link_base.cpp
				us_link_http.cpp
				us_link_manager.cpp
				us_logcreator.cpp
				us_logstream.cpp
				us_mesh_creator.cpp
				us_mission_base.cpp
				us_mission_executant.cpp
				us_mission_executant_async.cpp
				us_mission_flow.cpp
				us_mission_heap.cpp
				us_mission_queue.cpp
				us_native_loader.cpp
				us_network_link.cpp
				us_node_base.cpp
				us_node_visitor.cpp
				us_observer_base.cpp
				us_observer_helper.cpp
				us_octree_node.cpp
				us_operator_base.cpp
				us_operator_executant.cpp
				us_operator_instant_executant.cpp
				us_operator_ordered_executant.cpp
				us_pen_style.cpp
				us_physical_system.cpp
				us_plugin_manager.cpp
				us_priority_stack.cpp
				us_proxy_resource_container.cpp
				us_reclaim_base.cpp
				us_renderable_base.cpp
				us_render_argument.cpp
				us_render_device.cpp
				us_render_device_capabilities.cpp
				us_render_effect.cpp
				us_render_manager_base.cpp
				us_render_node.cpp
				us_render_object.cpp
				us_render_sprite.cpp
				us_render_state.cpp
				us_render_target.cpp
				us_render_transform.cpp
				us_resource_base.cpp
				us_resource_connection.cpp
				us_resource_container.cpp
				us_resource_killer.cpp
				us_resource_manager.cpp
				us_resource_reclaimer.cpp
				us_rigid_body.cpp
				us_sampler_state.cpp
				us_script_module.cpp
				us_shadow_parameter.cpp
				us_shadow_style.cpp
				us_shadow_volume_creator.cpp
				us_shared_data.cpp
				us_shared_object.cpp
				us_shared_ptr.cpp
				us_sound_base.cpp
				us_sound_device.cpp
				us_sound_style.cpp
				us_spatial_node.cpp
				us_spatial_object.cpp
				us_spherical_object.cpp
				us_square_buffer.cpp
				us_style_base.cpp
				us_styling_object.cpp
				us_swap_chain.cpp
				us_system_environment.cpp
				us_texture_creator.cpp
				us_texture_fill_style.cpp
				us_texture_map_description.cpp
				us_texture_state.cpp
				us_threadsafe_memory_pool.cpp
				us_thread_base.cpp
				us_thread_data.cpp
				us_thread_valve.cpp
				us_timer_queue_timer.cpp
				us_time_cost.cpp
				us_time_style.cpp
				us_update_argument.cpp
				us_vertex_buffer_binding.cpp
				us_vertex_declaration.cpp
				us_view_base.cpp
				us_view_port.cpp
)

ADD_LIBRARY(
    ${LIB_NAME} ${GWEARTH_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${LIB_PUBLIC_HEADERS}
    ${TARGET_SRC} )
	
INCLUDE_DIRECTORIES(${GWEARTH_SOURCE_DIR}/Src ${GWEARTH_SOURCE_DIR}/thirdPart)#${OSG_INCLUDE_DIR} ${Boost_INCLUDE_DIR}

#IF (WIN32)
#  LINK_EXTERNAL(${LIB_NAME} ${TARGET_EXTERNAL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBRARY})
#ELSE(WIN32)
#  LINK_EXTERNAL(${LIB_NAME} ${TARGET_EXTERNAL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBRARY})
#ENDIF(WIN32)

#OPTION(NRL_STATIC_LIBRARIES "Link osgEarth against static GDAL and cURL, including static OpenSSL, Proj4, JPEG, PNG, and TIFF." OFF)
#if(NOT NRL_STATIC_LIBRARIES)
#  LINK_WITH_VARIABLES(${LIB_NAME} OSG_LIBRARY ZLIB_LIBRARIES)
#else(NOT NRL_STATIC_LIBRARIES)
#  LINK_WITH_VARIABLES(${LIB_NAME} OSG_LIBRARY OSGUTIL_LIBRARY OSGSIM_LIBRARY OSGTERRAIN_LIBRARY OSGDB_LIBRARY OSGFX_LIBRARY OSGVIEWER_LIBRARY OSGTEXT_LIBRARY OSGGA_LIBRARY OSGSHADOW_LIBRARY OPENTHREADS_LIBRARY CURL_LIBRARY GDAL_LIBRARY ZLIB_LIBRARIES OSGMANIPULATOR_LIBRARY SSL_EAY_RELEASE LIB_EAY_RELEASE TIFF_LIBRARY PROJ4_LIBRARY PNG_LIBRARY JPEG_LIBRARY)
#endif(NOT NRL_STATIC_LIBRARIES)

LINK_WITH_VARIABLES(${LIB_NAME} 
	#OSG_LIBRARY
	#OSGEARTH_LIBRARY 
	#OSGEARTHFEATURES_LIBRARY 
	#OSGEARTHSYMBOLOGY_LIBRARY 
	)

LINK_INTERNAL(${LIB_NAME} )#gwBase opengl32 glu32

#LINK_CORELIB_DEFAULT(${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBRARY})

INCLUDE(ModuleInstall OPTIONAL)