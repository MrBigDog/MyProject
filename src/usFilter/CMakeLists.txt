IF   (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSFILTER_LIBRARY)
ELSE (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSFILTER_LIBRARY_STATIC)
ENDIF(DYNAMIC_GWEARTH)

IF(WIN32)
    IF(MSVC)
        SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRT")
    ENDIF(MSVC)
ENDIF(WIN32)

SET(LIB_NAME "usFilter")

add_definitions(-DUNICODE -D_UNICODE)

SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME})
#INCLUDE_DIRECTORIES(AFTER ${OSG_INCLUDE_DIR})
#INCLUDE_DIRECTORIES(AFTER ${OSGEARTH_INCLUDE_DIR})

SET(LIB_PUBLIC_HEADERS
				Export.h
				fastdxt/dxt.h
				fastdxt/libdxt.h
				fastdxt/util.h
				us_animation_filter_ex.h
				us_annotation_filter.h
				us_billboard_tree_filter.h
				us_dds_file.h
				us_dds_filter.h
				us_dib_filter.h
				us_dom_filter.h
				us_dynamic_parser.h
				us_edit_parser.h
				us_file_format_declear.h
				us_geo3d_filter.h
				us_grid_filter.h
				us_ipkg_filter.h
				us_jpeg_filter.h
				us_jpeg_loader.h
				us_match_model_filter.h
				us_match_model_filter_v2.h
				us_match_model_filter_v3.h
				us_match_model_f_filter.h
				us_mng_filter.h
				us_png_filter.h
				us_poi_square_filter.h
				us_projection_parser.h
				us_scene_parsers.h
				us_tga_filter.h
				us_tin_filter.h
				us_tpkg_filter.h
				us_track_parser.h
				us_ugp_filter.h
				us_ugp_filter_7z.h
				us_ugp_filter_v1.h
				us_uni_jpeg_filter.h
				us_uni_jpeg_filter_lm.h
				us_uni_mng_filter.h
				us_uni_png_filter.h
				us_usb_filter.h
				us_usd_filter.h
				us_usm_filter.h
				us_usm_filter_7z.h
				us_usm_filter_v1.h
				us_usm_filter_v1_shadow.h
				us_ust_filter.h
				us_ust_filter_v2.h
				us_usx_complex_mesh_exporter.h
				us_usx_exporter.h
				us_usx_exporter_args.h
				us_usx_exporter_assistant.h
				us_usx_exporter_assistant_ex.h
				us_usx_filter.h
				us_usx_hardware_mesh_exporter.h
				us_vector_line_filter.h
				us_vector_line_filter2.h
				us_vector_line_filter3.h
				us_vector_line_filter4.h
				us_vector_line_filter5.h
				us_vector_region_filter.h
				us_vector_region_filter4.h
				us_vector_region_filter5.h
				us_vector_region_filter6.h
				us_xfile_filter.h
				us_xml_document_parser.h
				us_xml_element_parsers.h
				us_xml_filter.h
				us_xml_mission.h
				us_xml_shape_parser.h
)
SET(TARGET_SRC
				fastdxt/dxt.cpp
				fastdxt/intrinsic.cpp
				fastdxt/libdxt.cpp
				fastdxt/util.cpp
				us_animation_filter_ex.cpp
				us_annotation_filter.cpp
				us_billboard_tree_filter.cpp
				us_dds_file.cpp
				us_dds_file_ext.cpp
				us_dds_filter.cpp
				us_dib_filter.cpp
				us_dom_filter.cpp
				us_dynamic_parser.cpp
				us_edit_parser.cpp
				us_geo3d_filter.cpp
				us_grid_filter.cpp
				us_ipkg_filter.cpp
				us_jpeg_filter.cpp
				us_jpeg_loader.cpp
				us_match_model_filter.cpp
				us_match_model_filter_v2.cpp
				us_match_model_filter_v3.cpp
				us_match_model_f_filter.cpp
				us_mng_filter.cpp
				us_png_filter.cpp
				us_poi_square_filter.cpp
				us_projection_parser.cpp
				us_scene_parsers.cpp
				us_tga_filter.cpp
				us_tin_filter.cpp
				us_tpkg_filter.cpp
				us_track_parser.cpp
				us_ugp_filter.cpp
				us_ugp_filter_7z.cpp
				us_ugp_filter_v1.cpp
				us_uni_jpeg_filter.cpp
				us_uni_jpeg_filter_lm.cpp
				us_uni_mng_filter.cpp
				us_uni_png_filter.cpp
				us_usb_filter.cpp
				us_usd_filter.cpp
				us_usm_filter.cpp
				us_usm_filter_7z.cpp
				us_usm_filter_v1.cpp
				us_usm_filter_v1_shadow.cpp
				us_ust_filter.cpp
				us_ust_filter_v2.cpp
				us_usx_complex_mesh_exporter.cpp
				us_usx_exporter.cpp
				us_usx_exporter_args.cpp
				us_usx_exporter_assistant.cpp
				us_usx_exporter_assistant_ex.cpp
				us_usx_filter.cpp
				us_usx_hardware_mesh_exporter.cpp
				us_vector_line_filter.cpp
				us_vector_line_filter2.cpp
				us_vector_line_filter3.cpp
				us_vector_line_filter4.cpp
				us_vector_line_filter5.cpp
				us_vector_region_filter.cpp
				us_vector_region_filter4.cpp
				us_vector_region_filter5.cpp
				us_vector_region_filter6.cpp
				us_xfile_filter.cpp
				us_xml_document_parser.cpp
				us_xml_element_parsers.cpp
				us_xml_filter.cpp
				us_xml_mission.cpp
				us_xml_shape_parser.cpp
)

ADD_LIBRARY(
    ${LIB_NAME} ${GWEARTH_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${LIB_PUBLIC_HEADERS}
    ${TARGET_SRC} )
	
INCLUDE_DIRECTORIES(${GWEARTH_SOURCE_DIR}/Src )#${OSG_INCLUDE_DIR} ${Boost_INCLUDE_DIR}

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