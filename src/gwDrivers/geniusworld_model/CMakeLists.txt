IF(WIN32)
    IF(MSVC)
        SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRT")
    ENDIF(MSVC)
ENDIF(WIN32)


SET(TARGET_H
    GeniusWorldModelOptions.h
	us/us_square_description.h
	us/us_lon_lat_rect.h
	us/us_spherical_coords.h
	us/us_math.h
	us/us_cartesian_coords.h
	us/us_vector3.h
	us/us_interior_index_factory.h
	us/us_common_file.h
	us/us_geocentric_coords.h
	us/us_axis_aligned_box.h
	us/us_circle.h
	us/us_base_algorithm.h
	us/us_spherical_transform.h
	us/us_spherical_pose.h
	us/us_plane.h
	us/us_lon_lat_region.h
	us/us_object_base.h
	us/us_lon_lat_geometry.h
	us/us_transform_base.h
	us/us_sphere.h
	us/us_ray.h
	us/us_vector4.h
	)

SET(TARGET_SRC
	GeniusWorldModelSource.cpp
	us/us_square_description.cpp
	us/us_lon_lat_rect.cpp
	us/us_spherical_coords.cpp
	us/us_math.cpp
	us/us_cartesian_coords.cpp
	us/us_vector3.cpp
	us/us_interior_index_factory.cpp
	us/us_geocentric_coords.cpp
	us/us_circle.cpp
	us/us_spherical_transform.cpp
	us/us_spherical_pose.cpp
	us/us_lon_lat_region.cpp
	us/us_object_base.cpp
	us/us_lon_lat_geometry.cpp
	us/us_transform_base.cpp
	us/us_vector4.cpp
)

SET(TARGET_LIBRARIES_VARS 
	OSG_LIBRARY
	OSGDB_LIBRARY 
	#OSGUTIL_LIBRARY 
	#OSGVIEWER_LIBRARY 
	OPENTHREADS_LIBRARY
	OSGEARTH_LIBRARY
	#OSGEARTHUTIL_LIBRARY
	#OSGEARTHFEATURES_LIBRARY
	#OSGEARTHSYMBOLOGY_LIBRARY
	#LIBMONGOCXX_LIBRARIES
	)
	

#SET(TARGET_COMMON_LIBRARIES gwUtil)
#SET(TARGET_COMMON_LIBRARIES gwApp )
SET(TARGET_COMMON_LIBRARIES gwBase )

INCLUDE_DIRECTORIES(${GWEARTH_SOURCE_DIR}/Src ${OSGEARTH_INCLUDE_DIR} ${OSG_INCLUDE_DIR} )
	
setup_plugin(osgearth_model_geniusworld)

SET(LIB_NAME building)
SET(LIB_PUBLIC_HEADERS ${TARGET_H})
INCLUDE(ModuleInstallGwEarthDriverIncludes OPTIONAL)