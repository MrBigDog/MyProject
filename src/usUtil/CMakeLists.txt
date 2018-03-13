IF   (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSUTIL_LIBRARY)
ELSE (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSUTIL_LIBRARY_STATIC)
ENDIF(DYNAMIC_GWEARTH)

IF(WIN32)
    IF(MSVC)
        SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRT")
    ENDIF(MSVC)
ENDIF(WIN32)

SET(LIB_NAME "usUtil")

SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME})
#INCLUDE_DIRECTORIES(AFTER ${OSG_INCLUDE_DIR})
#INCLUDE_DIRECTORIES(AFTER ${OSGEARTH_INCLUDE_DIR})

SET(LIB_PUBLIC_HEADERS
				Export.h
				shapefil.h
				TriangleX.h
				us_angle.h
				us_axis_aligned_box.h
				us_axis_aligned_box.inl
				us_base_algorithm.h
				us_base_algorithm_lh.h
				us_bidirectional_chain.h
				us_cartesian_coords.h
				us_cartesian_pose.h
				us_cartesian_transform.h
				us_chunk_data_define.h
				us_circle.h
				us_common_file.h
				us_coordinate_base.h
				us_cubic_spline3.h
				us_curve3.h
				us_custom_message.h
				us_dbf_file.h
				us_file_buffer.h
				us_file_directory.h
				us_frustum.h
				us_frustum.inl
				us_gaussian_coords.h
				us_gauss_projection.h
				us_geocentric_coords.h
				us_heap.h
				us_integrate1.h
				us_interior_index_factory.h
				us_io_stream.h
				us_io_stream.inl
				us_lon_lat_box.h
				us_lon_lat_geometry.h
				us_lon_lat_rect.h
				us_lon_lat_region.h
				us_managed_buffer.h
				us_math.h
				us_math.inl
				us_matrix4.h
				us_matrix4.inl
				us_mercator_projection.h
				us_multiple_curve3.h
				us_mutex.h
				us_object_base.h
				us_plane.h
				us_plane.inl
				us_point.h
				us_polygon_algorithm.h
				us_polygon_triangulation.h
				us_polyline_spline3.h
				us_projection.h
				us_quaternion.h
				us_radian.h
				us_raw_buffer.h
				us_ray.h
				us_rect.h
				us_region.h
				us_safe_ptr.h
				us_shape_creator.h
				us_shp_file.h
				us_singleton.h
				us_spatial_reference.h
				us_sphere.h
				us_sphere.inl
				us_spherical_coords.h
				us_spherical_pose.h
				us_spherical_transform.h
				us_string_ext.h
				us_tangent_coords_base.h
				us_tcb_spline3.h
				us_transform_base.h
				us_triangle.h
				us_triangle.inl
				us_triangulation.h
				us_triangulation3.h
				us_utm_projection.h
				us_vector2.h
				us_vector2.inl
				us_vector3.h
				us_vector3.inl
				us_vector4.h
				us_vector4.inl
				us_xml_parser_base.h
				us_xml_parser_ext.h
				us_xml_parser_manager.h
)
SET(TARGET_SRC
				dbfopen.cpp
				shpopen.cpp
				TriangleX.cpp
				us_angle.cpp
				us_bidirectional_chain.cpp
				us_cartesian_coords.cpp
				us_cartesian_pose.cpp
				us_cartesian_transform.cpp
				us_circle.cpp
				us_coordinate_base.cpp
				us_cubic_spline3.cpp
				us_curve3.cpp
				us_dbf_file.cpp
				us_file_buffer.cpp
				us_file_directory.cpp
				us_gaussian_coords.cpp
				us_gauss_projection.cpp
				us_geocentric_coords.cpp
				us_integrate1.cpp
				us_interior_index_factory.cpp
				us_lon_lat_box.cpp
				us_lon_lat_geometry.cpp
				us_lon_lat_rect.cpp
				us_lon_lat_region.cpp
				us_managed_buffer.cpp
				us_math.cpp
				us_matrix4.cpp
				us_mercator_projection.cpp
				us_multiple_curve3.cpp
				us_mutex.cpp
				us_object_base.cpp
				us_point.cpp
				us_polygon_algorithm.cpp
				us_polygon_triangulation.cpp
				us_polyline_spline3.cpp
				us_projection.cpp
				us_radian.cpp
				us_raw_buffer.cpp
				us_rect.cpp
				us_region.cpp
				us_shape_creator.cpp
				us_shp_file.cpp
				us_spatial_reference.cpp
				us_spherical_coords.cpp
				us_spherical_pose.cpp
				us_spherical_transform.cpp
				us_string_ext.cpp
				us_tangent_coords_base.cpp
				us_tcb_spline3.cpp
				us_transform_base.cpp
				us_triangulation.cpp
				us_triangulation3.cpp
				us_utm_projection.cpp
				us_vector2.cpp
				us_vector3.cpp
				us_vector4.cpp
				us_xml_parser_base.cpp
				us_xml_parser_ext.cpp
				us_xml_parser_manager.cpp
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

LINK_INTERNAL(${LIB_NAME} usXml Shlwapi.lib)#gwBase opengl32 glu32

#LINK_CORELIB_DEFAULT(${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBRARY})

INCLUDE(ModuleInstall OPTIONAL)