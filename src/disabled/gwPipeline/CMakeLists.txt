IF   (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DGWEARTH_LIBRARY)
ELSE (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DGWEARTH_LIBRARY_STATIC)
ENDIF(DYNAMIC_GWEARTH)

IF(WIN32)
    IF(MSVC)
        SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRT")
    ENDIF(MSVC)
ENDIF(WIN32)

SET(LIB_NAME "gwPipeline")

SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME})
INCLUDE_DIRECTORIES(AFTER ${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(AFTER ${OSGEARTH_INCLUDE_DIR})

SET(LIB_PUBLIC_HEADERS
			Export.h
			IPipelineSystem.h
			IPipelineLayer.h
           PipelineLayer.h 
           PipelineSystem.h 
		   PipeLineSymbol.h
           PipeJointSymbol.h
           PipeEquipmentSymbol.h
		   BuildPipelineGeometryFilter.h
		   PipelineGeometryCompiler.h
		   PipelineGeomFeatureNodeFactory.h
		   PipelineDeclare.h
		   PipelineEntryJoint.h
		   PipelineGenerateBase.h
		   PipelineGenerateHelper.h
		   PipelineGenerateParameter.h
		   PipelineJointFactory.h
		   PipelineMeshGenerateHelper.h
		   PipelineSourceUtil.h
		   PipelineResetAltFilter.h
		   )

SET(TARGET_SRC
			IPipelineLayer.cpp
           PipelineLayer.cpp 
           PipelineSystem.cpp 
		   PipeLineSymbol.cpp
		   PipeJointSymbol.cpp
		   PipeEquipmentSymbol.cpp
		   BuildPipelineGeometryFilter.cpp
		   PipelineGeometryCompiler.cpp
		   PipelineGeomFeatureNodeFactory.cpp
		   PipelineSourceUtil.cpp
		   PipelineResetAltFilter.cpp
		   joint/PipelineEntryJoint.cpp
		   joint/PipelineGenerateBase.cpp
		   joint/PipelineGenerateHelper.cpp
		   joint/PipelineGenerateParameter.cpp
		   joint/PipelineJointFactory.cpp
		   joint/PipelineMeshGenerateHelper.cpp
)


ADD_LIBRARY(
    ${LIB_NAME} ${GWEARTH_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${LIB_PUBLIC_HEADERS}
    ${TARGET_SRC} )

INCLUDE_DIRECTORIES(${GWEARTH_SOURCE_DIR}/Src ${OSGEARTH_INCLUDE_DIR} ${OSG_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

#IF (WIN32)
#  LINK_EXTERNAL(${LIB_NAME} ${TARGET_EXTERNAL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBRARY})
#ELSE(WIN32)
#  LINK_EXTERNAL(${LIB_NAME} ${TARGET_EXTERNAL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBRARY})
#ENDIF(WIN32)

LINK_WITH_VARIABLES(${LIB_NAME} 
								#OSG_LIBRARY 
								#OSGDB_LIBRARY
								#OSGGA_LIBRARY
								#OSGUTIL_LIBRARY
								#OSGVIEWER_LIBRARY
								OPENTHREADS_LIBRARY
								OSGEARTH_LIBRARY
								OSGEARTHFEATURES_LIBRARY
								OSGEARTHSYMBOLOGY_LIBRARY
								#OSGEARTHUTIL_LIBRARY
								#ZLIB_LIBRARIES
								)
LINK_INTERNAL(${LIB_NAME} gwUtil)

INCLUDE(ModuleInstall OPTIONAL)