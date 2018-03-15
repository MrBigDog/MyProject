IF   (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSDOTX_LIBRARY)
ELSE (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSDOTX_LIBRARY_STATIC)
ENDIF(DYNAMIC_GWEARTH)

IF(WIN32)
    IF(MSVC)
        SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRT")
    ENDIF(MSVC)
ENDIF(WIN32)

SET(LIB_NAME "usDotx")

add_definitions(-DUNICODE -D_UNICODE)

SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME})
#INCLUDE_DIRECTORIES(AFTER ${OSG_INCLUDE_DIR})
#INCLUDE_DIRECTORIES(AFTER ${OSGEARTH_INCLUDE_DIR})

SET(LIB_PUBLIC_HEADERS
				Export.h
				us_usc_declaration.h
				us_usx_animation_key_parser.h
				us_usx_animation_parser.h
				us_usx_animation_set.h
				us_usx_animation_set_parser.h
				us_usx_data_struct.h
				us_usx_decl_data_parser.h
				us_usx_frame.h
				us_usx_frame_parser.h
				us_usx_material.h
				us_usx_mesh.h
				us_usx_mesh_parser.h
				us_usx_object.h
				us_usx_parser.h
				us_usx_parser_assistant.h
				us_usx_parser_object_container.h
				us_usx_skin_info.h
				us_usx_skin_mesh_header_parser.h
				us_usx_skin_weights_parser.h
				us_usx_surface.h
				us_usx_template.h
)
SET(TARGET_SRC
				us_usx_animation_key_parser.cpp
				us_usx_animation_parser.cpp
				us_usx_animation_set.cpp
				us_usx_animation_set_parser.cpp
				us_usx_data_struct.cpp
				us_usx_decl_data_parser.cpp
				us_usx_frame.cpp
				us_usx_frame_parser.cpp
				us_usx_material.cpp
				us_usx_mesh.cpp
				us_usx_mesh_parser.cpp
				us_usx_object.cpp
				us_usx_parser.cpp
				us_usx_parser_assistant.cpp
				us_usx_parser_object_container.cpp
				us_usx_skin_info.cpp
				us_usx_skin_mesh_header_parser.cpp
				us_usx_skin_weights_parser.cpp
				us_usx_surface.cpp
				us_usx_template.cpp
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