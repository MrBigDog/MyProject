IF(WIN32)
    IF(MSVC)
        SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRT")
    ENDIF(MSVC)
ENDIF(WIN32)


SET(TARGET_H
	WjhConfigOptions.h
	)

SET(TARGET_SRC
	#WjhConfigSource.cpp
	WjhConfigSource.cpp
)

SET(TARGET_LIBRARIES_VARS 
	OSG_LIBRARY
	OSGDB_LIBRARY 
	#OSGUTIL_LIBRARY 
	#OSGVIEWER_LIBRARY 
	OPENTHREADS_LIBRARY
	OSGEARTH_LIBRARY
	OSGEARTHUTIL_LIBRARY
	OSGEARTHFEATURES_LIBRARY
	OSGEARTHSYMBOLOGY_LIBRARY
	)
	
INCLUDE_DIRECTORIES(${GWEARTH_SOURCE_DIR}/Src ${OSGEARTH_INCLUDE_DIR} ${OSG_INCLUDE_DIR})
	
setup_plugin(osgearth_feature_wjh)

SET(LIB_NAME building)
SET(LIB_PUBLIC_HEADERS ${TARGET_H})
INCLUDE(ModuleInstallGwEarthDriverIncludes OPTIONAL)