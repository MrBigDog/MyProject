IF   (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSLUA_LIBRARY)
ELSE (DYNAMIC_GWEARTH)
    ADD_DEFINITIONS(-DUSLUA_LIBRARY_STATIC)
ENDIF(DYNAMIC_GWEARTH)

IF(WIN32)
    IF(MSVC)
        SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRT")
    ENDIF(MSVC)
ENDIF(WIN32)

SET(LIB_NAME "usLua")

#add_definitions(-DUNICODE -D_UNICODE)

SET(HEADER_PATH ${PROJECT_SOURCE_DIR}/include/${LIB_NAME})


SET(LIB_PUBLIC_HEADERS
				#Export.h
				auxiliar.h
				LuaAutoBlock.h
				LuaCall.h
				LuaFunction.h
				LuaHelper.h
				LuaHelper_Object.h
				LuaHelper_StackObject.h
				LuaLink.h
				LuaObject.h
				LuaObject.inl
				LuaPlus.h
				LuaPlusCD.h
				LuaPlusFunctions.h
				LuaPlusInternal.h
				LuaStackObject.h
				LuaStackObject.inl
				LuaStackTableIterator.h
				LuaState.h
				LuaState.inl
				LuaStateOutFile.h
				LuaTableIterator.h
				luathread.h
				pt.h
				srm.h
				
				src/lapi.h
				src/lauxlib.h
				src/lcode.h
				src/ldebug.h
				src/lzio.h
				src/lvm.h
				src/lundump.h
				src/ltm.h
				src/lua.h
				src/luaconf.h
				src/lualib.h
				src/ltable.h
				src/lstring.h
				src/lstate.h
				src/lparser.h
				src/lopcodes.h
				src/lobject.h
				src/lmem.h
				src/llex.h
				src/llimits.h
				src/lgc.h
				src/lfunc.h
				src/ldo.h
)
SET(TARGET_SRC
				LuaState_DumpObject.cpp
				LuaTableIterator.cpp
				lwstrlib.c
				LuaStateOutFile.cpp
				LuaState.cpp
				LuaStackTableIterator.cpp
				LuaPlus_Libs.cpp
				LuaStackObject.cpp
				LuaPlusFunctions.cpp
				LuaPlusAddons.c
				LuaPlus.cpp
				LuaObject.cpp
				LuaCall.cpp
				
				src/lapi.c
				src/lauxlib.c
				src/lbaselib.c
				src/lcode.c
				src/ldblib.c
				src/ldebug.c
				src/ldo.c
				src/ldump.c
				src/lfunc.c
				src/lgc.c
				src/linit.c
				src/liolib.c
				src/llex.c
				src/lmathlib.c
				src/lmem.c
				src/loadlib.c
				src/lobject.c
				src/lopcodes.c
				src/loslib.c
				src/lparser.c
				src/lstate.c
				src/lstring.c
				src/lstrlib.c
				src/ltable.c
				src/ltablib.c
				src/ltm.c
				src/lundump.c
				src/lvm.c
				src/lzio.c
				src/print.c
)

ADD_LIBRARY(
    ${LIB_NAME} STATIC
    ${LIB_PUBLIC_HEADERS}
    ${TARGET_SRC} 
	)#${GWEARTH_USER_DEFINED_DYNAMIC_OR_STATIC
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

LINK_INTERNAL(${LIB_NAME} )#usXml Shlwapi.lib

#LINK_CORELIB_DEFAULT(${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBRARY})

INCLUDE(ModuleInstall OPTIONAL)