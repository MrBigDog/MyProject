IF   (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSGFX_LIBRARY)
ELSE (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSGFX_LIBRARY_STATIC)
ENDIF(DYNAMIC_GWEARTH)

IF(WIN32)
    IF(MSVC)
        SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRT")
    ENDIF(MSVC)
ENDIF(WIN32)

SET(LIB_NAME "usGfx")

add_definitions(-DUNICODE -D_UNICODE)

SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME})
#INCLUDE_DIRECTORIES(AFTER ${OSG_INCLUDE_DIR})
#INCLUDE_DIRECTORIES(AFTER ${OSGEARTH_INCLUDE_DIR})

SET(LIB_PUBLIC_HEADERS
				Export.h
				us_building_usd_buffer.h
				us_building_usd_buffer_shadow.h
				us_d3d9_complex_hardware_mesh.h
				us_d3d9_effect_base.h
				us_d3d9_effect_common_mesh.h
				us_d3d9_effect_common_mesh_fp.h
				us_d3d9_effect_common_mesh_gpu.h
				us_d3d9_effect_common_mesh_with_shadow_map.h
				us_d3d9_effect_flood.h
				us_d3d9_effect_flow_direction.h
				us_d3d9_effect_geometry_line_flake.h
				us_d3d9_effect_geometry_line_volume.h
				us_d3d9_effect_manager.h
				us_d3d9_effect_pp.h
				us_d3d9_effect_pp_combine_dof.h
				us_d3d9_effect_pp_combine_hdr.h
				us_d3d9_effect_pp_combine_hsl.h
				us_d3d9_effect_pp_down_filter.h
				us_d3d9_effect_pp_gaussian_blur_h.h
				us_d3d9_effect_pp_gaussian_blur_v.h
				us_d3d9_effect_pp_tone_map.h
				us_d3d9_effect_pp_up_filter.h
				us_d3d9_effect_reflection_surface.h
				us_d3d9_effect_reflection_surface_ex.h
				us_d3d9_effect_render_object_line_flake.h
				us_d3d9_effect_render_object_line_volume.h
				us_d3d9_effect_shadow_volume.h
				us_d3d9_effect_sky_sphere.h
				us_d3d9_effect_terrain_blend.h
				us_d3d9_effect_vertex_blend.h
				us_d3d9_effect_weather.h
				us_d3d9_font_manager.h
				us_d3d9_hardware_index_buffer.h
				us_d3d9_hardware_mesh.h
				us_d3d9_hardware_mesh_buffer.h
				us_d3d9_hardware_occlusion_query.h
				us_d3d9_hardware_texture.h
				us_d3d9_hardware_vertex_buffer.h
				us_d3d9_hierarchy_texture.h
				us_d3d9_intersect_helper.h
				us_d3d9_material_declear.h
				us_d3d9_mesh_subset.h
				us_d3d9_render_device_impl.h
				us_d3d9_render_effect.h
				us_d3d9_render_font.h
				us_d3d9_render_target.h
				us_d3d9_render_target_chain.h
				us_d3d9_render_target_ex.h
				us_d3d9_render_target_manager.h
				us_d3d9_render_target_offscreen.h
				us_d3d9_render_target_shadow_map.h
				us_d3d9_skinning_hardware_mesh.h
				us_d3d9_sky_blend_effect.h
				us_d3d9_sound_device_impl.h
				us_d3d9_sound_impl.h
				us_d3d9_surface_object.h
				us_d3d9_terrain_blend_effect.h
				us_d3d9_vertex_blend_effect.h
				us_d3d9_vertex_declear.h
				us_d3d9_wave_file.h
				us_font_manager.h
				us_generic_render_queue.h
				us_geometry_billboard.h
				us_geometry_line.h
				us_geometry_line_factory.h
				us_geometry_line_flake.h
				us_geometry_line_volume.h
				us_geometry_mark.h
				us_geometry_mesh_subset.h
				#us_geometry_model.h
				us_geometry_points.h
				us_geometry_polygon.h
				us_geometry_render.h
				us_geometry_render_blend.h
				us_geometry_shadow_factory.h
			#	us_geometry_shadow_mesh.h
				us_geometry_textured_face.h
				us_gl_render_device_impl.h
				#us_hardware_geometry_mesh.h
				us_hardware_mesh_factory.h
				us_hardware_mesh_pnt2_buffer.h
				us_input_argument.h
				us_input_device_base.h
				us_input_impl.h
				us_joystick_argument.h
				us_joystick_device_impl.h
				us_keyboard_argument.h
				us_keyboard_device_impl.h
				us_mesh_subset.h
				us_message_argument.h
				us_mouse_3d_device_impl.h
				us_mouse_argument.h
				us_mouse_device_impl.h
				us_network_argument.h
				us_packet_sender.h
				us_packet_taker.h
				us_post_process_render_queue.h
				us_reflection_render.h
				us_reflection_render_ex.h
				us_render_helper.h
				us_render_object_billboard.h
				us_render_object_factory.h
				us_render_object_geometry_mesh.h
				us_render_object_geometry_textured_mesh.h
				us_render_object_hardware_mesh.h
				us_render_object_line.h
				us_render_object_line_factory.h
				us_render_object_line_flake.h
				us_render_object_line_volume.h
				us_render_object_orthographic_volume.h
				us_render_object_points.h
				us_render_object_polygon.h
				us_render_object_polygon_factory.h
				us_render_object_shadow_factory.h
				us_render_object_shadow_mesh.h
				us_render_object_shinning_shadow.h
				us_render_object_shinning_shadow_4s.h
				us_server_linker.h
				us_shadow_clipper_render.h
				us_shadow_map_render.h
				us_shadow_render.h
				us_shadow_render_queue.h
				us_shinning_shadow.h
				us_socket.h
				us_socket_client.h
				us_software_device_impl.h
				us_software_occlusion_query.h
				us_software_raster.h
				us_sorted_render_queue.h
				us_sound_base.h
				us_sound_object.h
)
SET(TARGET_SRC
				us_building_usd_buffer.cpp
				us_building_usd_buffer_shadow.cpp
				us_d3d9_complex_hardware_mesh.cpp
				us_d3d9_effect_base.cpp
				us_d3d9_effect_common_mesh.cpp
				us_d3d9_effect_common_mesh_fp.cpp
				us_d3d9_effect_common_mesh_gpu.cpp
				us_d3d9_effect_common_mesh_with_shadow_map.cpp
				us_d3d9_effect_flood.cpp
				us_d3d9_effect_flow_direction.cpp
				us_d3d9_effect_geometry_line_flake.cpp
				us_d3d9_effect_geometry_line_volume.cpp
				us_d3d9_effect_manager.cpp
				us_d3d9_effect_pp.cpp
				us_d3d9_effect_pp_combine_dof.cpp
				us_d3d9_effect_pp_combine_hdr.cpp
				us_d3d9_effect_pp_combine_hsl.cpp
				us_d3d9_effect_pp_down_filter.cpp
				us_d3d9_effect_pp_gaussian_blur_h.cpp
				us_d3d9_effect_pp_gaussian_blur_v.cpp
				us_d3d9_effect_pp_tone_map.cpp
				us_d3d9_effect_pp_up_filter.cpp
				us_d3d9_effect_reflection_surface.cpp
				us_d3d9_effect_reflection_surface_ex.cpp
				us_d3d9_effect_render_object_line_flake.cpp
				us_d3d9_effect_render_object_line_volume.cpp
				us_d3d9_effect_shadow_volume.cpp
				us_d3d9_effect_sky_sphere.cpp
				us_d3d9_effect_terrain_blend.cpp
				us_d3d9_effect_vertex_blend.cpp
				us_d3d9_effect_weather.cpp
				us_d3d9_font_manager.cpp
				us_d3d9_hardware_index_buffer.cpp
				us_d3d9_hardware_mesh.cpp
				us_d3d9_hardware_mesh_buffer.cpp
				us_d3d9_hardware_occlusion_query.cpp
				us_d3d9_hardware_texture.cpp
				us_d3d9_hardware_vertex_buffer.cpp
				us_d3d9_hierarchy_texture.cpp
				us_d3d9_intersect_helper.cpp
				us_d3d9_mesh_subset.cpp
				us_d3d9_render_device_impl.cpp
				us_d3d9_render_effect.cpp
				us_d3d9_render_font.cpp
				us_d3d9_render_target.cpp
				us_d3d9_render_target_chain.cpp
				us_d3d9_render_target_ex.cpp
				us_d3d9_render_target_manager.cpp
				us_d3d9_render_target_offscreen.cpp
				us_d3d9_render_target_shadow_map.cpp
				us_d3d9_skinning_hardware_mesh.cpp
				us_d3d9_sky_blend_effect.cpp
				us_d3d9_sound_device_impl.cpp
				us_d3d9_sound_impl.cpp
				us_d3d9_surface_object.cpp
				us_d3d9_terrain_blend_effect.cpp
				us_d3d9_vertex_blend_effect.cpp
				us_d3d9_vertex_declear.cpp
				us_d3d9_wave_file.cpp
				us_font_manager.cpp
				us_generic_render_queue.cpp
				us_geometry_billboard.cpp
				us_geometry_line.cpp
				us_geometry_line_factory.cpp
				us_geometry_line_flake.cpp
				us_geometry_line_volume.cpp
				us_geometry_mark.cpp
				us_geometry_mesh_subset.cpp
				#us_geometry_model.cpp
				us_geometry_points.cpp
				us_geometry_polygon.cpp
				us_geometry_render.cpp
				us_geometry_render_blend.cpp
				us_geometry_shadow_factory.cpp
			#	us_geometry_shadow_mesh.cpp
				us_geometry_textured_face.cpp
				us_gl_render_device_impl.cpp
				#us_hardware_geometry_mesh.cpp
				us_hardware_mesh_factory.cpp
				#us_hardware_mesh_skinned_buffer.cpp
				us_input_argument.cpp
				us_input_device_base.cpp
				us_input_impl.cpp
				us_joystick_argument.cpp
				us_joystick_device_impl.cpp
				us_keyboard_argument.cpp
				us_keyboard_device_impl.cpp
				us_mesh_subset.cpp
				us_message_argument.cpp
				us_mouse_3d_device_impl.cpp
				us_mouse_argument.cpp
				us_mouse_device_impl.cpp
				us_network_argument.cpp
				us_packet_sender.cpp
				us_packet_taker.cpp
				us_post_process_render_queue.cpp
				us_reflection_render.cpp
				us_reflection_render_ex.cpp
				us_render_helper.cpp
				us_render_object_billboard.cpp
				us_render_object_factory.cpp
				us_render_object_geometry_mesh.cpp
				us_render_object_geometry_textured_mesh.cpp
				us_render_object_hardware_mesh.cpp
				us_render_object_line.cpp
				us_render_object_line_factory.cpp
				us_render_object_line_flake.cpp
				us_render_object_line_volume.cpp
				us_render_object_orthographic_volume.cpp
				us_render_object_points.cpp
				us_render_object_polygon.cpp
				us_render_object_polygon_factory.cpp
				us_render_object_shadow_factory.cpp
				us_render_object_shadow_mesh.cpp
				us_render_object_shinning_shadow.cpp
				us_render_object_shinning_shadow_4s.cpp
				us_server_linker.cpp
				us_shadow_clipper_render.cpp
				us_shadow_map_render.cpp
				us_shadow_render.cpp
				us_shadow_render_queue.cpp
				us_shinning_shadow.cpp
				us_socket.cpp
				us_socket_client.cpp
				us_software_device_impl.cpp
				us_software_occlusion_query.cpp
				us_software_raster.cpp
				us_sorted_render_queue.cpp
				us_sound_base.cpp
				us_sound_object.cpp
)

ADD_LIBRARY(
    ${LIB_NAME} ${GWEARTH_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${LIB_PUBLIC_HEADERS}
    ${TARGET_SRC} )
	
INCLUDE_DIRECTORIES(${GWEARTH_SOURCE_DIR}/Src ${GWEARTH_SOURCE_DIR}/thirdPart/d3d9)#${OSG_INCLUDE_DIR} ${Boost_INCLUDE_DIR}

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

LINK_INTERNAL(${LIB_NAME} )#gwBase opengl32 glu32usUtil usXml usLua wininet.lib rpcrt4.lib winmm.lib

#LINK_CORELIB_DEFAULT(${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBRARY})

INCLUDE(ModuleInstall OPTIONAL)