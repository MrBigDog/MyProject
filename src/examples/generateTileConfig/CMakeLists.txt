
SET(TARGET_H 
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
	us/string_util.h
	us/us_utm_projection.h
	us/us_projection.h
	#StringUtils.h
	#FileUtils.h
	)
SET(TARGET_SRC  
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
	us/us_utm_projection.cpp
	us/us_projection.cpp
	main.cpp
	#StringUtils.cpp
	#FileUtils.cpp
	)

INCLUDE_DIRECTORIES( ${OSGEARTH_INCLUDE_DIR} ${OSG_INCLUDE_DIR})

SET(TARGET_LIBRARIES_VARS 
	OSG_LIBRARY 
	OSGGA_LIBRARY 
	OSGDB_LIBRARY 
	OSGUTIL_LIBRARY 
	OPENTHREADS_LIBRARY
	OSGEARTH_LIBRARY
	#OSGEARTHSYMBOLOGY_LIBRARY
	)
	
SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT_LIBDIR})

SETUP_APPLICATION(generateTileConfig)