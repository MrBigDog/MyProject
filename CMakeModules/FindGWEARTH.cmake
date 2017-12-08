# This module defines

# GWBASE_LIBRARY
# GWEARTH_LIBRARY
# GWEARTH_FOUND, if false, do not try to link to osg
# GWEARTH_INCLUDE_DIRS, where to find the headers
# GWEARTH_INCLUDE_DIR, where to find the source headers

# to use this module, set variables to point to the osg build
# directory, and source directory, respectively
# GWEARTHDIR or GWEARTH_SOURCE_DIR: osg source directory, typically OpenSceneGraph
# GWEARTH_DIR or GWEARTH_BUILD_DIR: osg build directory, place in which you've
#    built osg via cmake

# Header files are presumed to be included like
# #include <osg/PositionAttitudeTransform>
# #include <osgUtil/SceneView>

###### headers ######

MACRO( FIND_GWEARTH_INCLUDE THIS_GWEARTH_INCLUDE_DIR THIS_GWEARTH_INCLUDE_FILE )

FIND_PATH( ${THIS_GWEARTH_INCLUDE_DIR} ${THIS_GWEARTH_INCLUDE_FILE}
    PATHS
        ${GWEARTH_DIR}
        $ENV{GWEARTH_SOURCE_DIR}
        $ENV{GWEARTHDIR}
        $ENV{GWEARTH_DIR}
        /usr/local/
        /usr/
        /sw/ # Fink
        /opt/local/ # DarwinPorts
        /opt/csw/ # Blastwave
        /opt/
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;GWEARTH_ROOT]/
        ~/Library/Frameworks
        /Library/Frameworks
    PATH_SUFFIXES
        /include/
)

ENDMACRO( FIND_GWEARTH_INCLUDE THIS_GWEARTH_INCLUDE_DIR THIS_GWEARTH_INCLUDE_FILE )

FIND_GWEARTH_INCLUDE( GWEARTH_INCLUDE_DIR       gwBase/constants.h )

###### libraries ######

MACRO( FIND_GWEARTH_LIBRARY MYLIBRARY MYLIBRARYNAME )

FIND_LIBRARY(${MYLIBRARY}
    NAMES
        ${MYLIBRARYNAME}
    PATHS
        ${GWEARTH_DIR}
        $ENV{GWEARTH_BUILD_DIR}
        $ENV{GWEARTH_DIR}
        $ENV{GWEARTHDIR}
        $ENV{GWEARTH_ROOT}
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;GWEARTH_ROOT]/lib
        /usr/freeware
    PATH_SUFFIXES
        /lib/
        /lib64/
        /build/lib/
        /build/lib64/
        /Build/lib/
        /Build/lib64/
     )

ENDMACRO(FIND_GWEARTH_LIBRARY LIBRARY LIBRARYNAME)

FIND_GWEARTH_LIBRARY( GWBASE_LIBRARY gwBase)
FIND_GWEARTH_LIBRARY( GWBASE_LIBRARY_DEBUG gwBased)

FIND_GWEARTH_LIBRARY( GWEARTH_LIBRARY gwEarth)
FIND_GWEARTH_LIBRARY( GWEARTH_LIBRARY_DEBUG gwEarthd)

FIND_GWEARTH_LIBRARY( GWPIPELINE_LIBRARY gwPipeline )
FIND_GWEARTH_LIBRARY( GWPIPELINE_LIBRARY_DEBUG gwPipelined )

SET( GWEARTH_FOUND "NO" )
IF( GWEARTH_LIBRARY AND GWEARTH_INCLUDE_DIR )
    SET( GWEARTH_FOUND "YES" )
    SET( GWEARTH_INCLUDE_DIRS ${GWEARTH_INCLUDE_DIR})
    GET_FILENAME_COMPONENT( GWEARTH_LIBRARIES_DIR ${GWEARTH_LIBRARY} PATH )
ENDIF( GWEARTH_LIBRARY AND GWEARTH_INCLUDE_DIR )
