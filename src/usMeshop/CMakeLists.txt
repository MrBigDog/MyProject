IF   (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSMESHOP_LIBRARY)
ELSE (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSMESHOP_LIBRARY_STATIC)
ENDIF(DYNAMIC_GWEARTH)

IF(WIN32)
    IF(MSVC)
        SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRT")
    ENDIF(MSVC)
ENDIF(WIN32)

SET(LIB_NAME "usMeshop")

#add_definitions(-DUNICODE -D_UNICODE)

SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME})


SET(LIB_PUBLIC_HEADERS
				BOP_BBox.h
				BOP_BSPNode.h
				BOP_BSPTree.h
				BOP_Chrono.h
				BOP_Edge.h
				BOP_Face.h
				BOP_Face2Face.h
				BOP_Indexs.h
				BOP_Interface.h
				BOP_MathUtils.h
				BOP_Merge.h
				BOP_Merge2.h
				BOP_Mesh.h
				BOP_Misc.h
				BOP_Segment.h
				BOP_Splitter.h
				BOP_Tag.h
				BOP_Triangulator.h
				BOP_Vertex.h
				DNA_listBase.h
				GEN_List.h
				GEN_Map.h
				MEM_SmartPtr.h
				MT_assert.h
				MT_CmMatrix4x4.h
				MT_Matrix3x3.h
				MT_Matrix3x3.inl
				MT_Matrix4x4.h
				MT_Matrix4x4.inl
				MT_MinMax.h
				MT_Optimize.h
				MT_Plane3.h
				MT_Plane3.inl
				MT_Point2.h
				MT_Point2.inl
				MT_Point3.h
				MT_Point3.inl
				MT_Quaternion.h
				MT_Quaternion.inl
				MT_random.h
				MT_Scalar.h
				MT_Stream.h
				MT_Transform.h
				MT_Tuple2.h
				MT_Tuple3.h
				MT_Tuple4.h
				MT_Vector2.h
				MT_Vector2.inl
				MT_Vector3.h
				MT_Vector3.inl
				MT_Vector4.h
				MT_Vector4.inl
				NM_Scalar.h
				
				common.h
)
SET(TARGET_SRC
				BOP_BBox.cpp
				BOP_BSPNode.cpp
				BOP_BSPTree.cpp
				BOP_Edge.cpp
				BOP_Face.cpp
				BOP_Face2Face.cpp
				BOP_Interface.cpp
				BOP_MathUtils.cpp
				BOP_Merge.cpp
				BOP_Merge2.cpp
				BOP_Mesh.cpp
				BOP_Segment.cpp
				BOP_Splitter.cpp
				BOP_Tag.cpp
				BOP_Triangulator.cpp
				BOP_Vertex.cpp
				MT_Assert.cpp
				MT_CmMatrix4x4.cpp
				MT_Matrix3x3.cpp
				MT_Matrix4x4.cpp
				MT_Plane3.cpp
				MT_Point3.cpp
				MT_Quaternion.cpp
				MT_random.cpp
				MT_Transform.cpp
				MT_Vector2.cpp
				MT_Vector3.cpp
				MT_Vector4.cpp
)

ADD_LIBRARY(
    ${LIB_NAME} STATIC
    ${LIB_PUBLIC_HEADERS}
    ${TARGET_SRC} 
	)#${GWEARTH_USER_DEFINED_DYNAMIC_OR_STATIC
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

LINK_INTERNAL(${LIB_NAME} )#usXml Shlwapi.lib

#LINK_CORELIB_DEFAULT(${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBRARY})

INCLUDE(ModuleInstall OPTIONAL)