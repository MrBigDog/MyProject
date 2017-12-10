
SET(TARGET_H
	SliceOperator.h
	Cutter.h
	WellGenerator.h
	GeneratorHelp.h
	MeshConsolidator.h
	TunnelGenerator.h
	FoundationDitchGenerator.h
	ProfileGenerator.h
	AssistPlaneGenerator.h
	clipper.hpp
	ThreadingUtils.h
	NodeUtils.h
	TileRegister.h
	MapNode.h
	Map.h
	GeologicalLayer.h
	GeologicalProfileLayer.h
	TypeDefine.h
	Register.h
	SlicePlane.h
	FenceGenerator.h
	StringUtils.h
	PlaneIntersector.h
	LineSegmentIntersector.h

	)
SET(TARGET_SRC  main.cpp SliceOperator.cpp
   WellGenerator.cpp
   MeshConsolidator.cpp
   TunnelGenerator.cpp
   GeneratorHelp.cpp
   FoundationDitchGenerator.cpp
   ProfileGenerator.cpp
   AssistPlaneGenerator.cpp
   clipper.cpp
   #ProfileRegister.cpp
   ThreadingUtils.cpp
   NodeUtils.cpp
   TileRegister.cpp
   MapNode.cpp
   Map.cpp
   GeologicalLayer.cpp
   GeologicalProfileLayer.cpp
   Register.cpp
   SlicePlane.cpp
   FenceGenerator.cpp
   StringUtils.cpp
   PlaneIntersector.cpp
   LineSegmentIntersector.cpp)

INCLUDE_DIRECTORIES(${GWEARTH_SOURCE_DIR}/Src ${OSGEARTH_INCLUDE_DIR} ${OSG_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

SET(TARGET_LIBRARIES_VARS 
	OSG_LIBRARY 
	OSGGA_LIBRARY 
	OSGDB_LIBRARY 
	OSGUTIL_LIBRARY 
	OSGVIEWER_LIBRARY 
	OPENTHREADS_LIBRARY
	OSGEARTH_LIBRARY
	OSGEARTHSYMBOLOGY_LIBRARY
	)
	
#LINK_INTERNAL(${LIB_NAME} gwUtil)

#SET(TARGET_COMMON_LIBRARIES gwApp )
	
SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT_LIBDIR})

SETUP_APPLICATION(lsTest)