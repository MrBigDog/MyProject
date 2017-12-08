# Copyright 2016 MongoDB Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(LIBMONGOCXX_VERSION_MAJOR 3)
set(LIBMONGOCXX_VERSION_MINOR 1)
set(LIBMONGOCXX_VERSION_PATCH 1)
set(LIBMONGOCXX_PACKAGE_VERSION 3.1.1-pre)

find_package(libbsoncxx 3.1.1 REQUIRED)


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was libmongocxx-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

set_and_check(LIBMONGOCXX_INCLUDE_DIRS "${PACKAGE_PREFIX_DIR}/include/mongocxx/v_noabi")

set(LIBMONGOCXX_INCLUDE_DIRS ${LIBMONGOCXX_INCLUDE_DIRS} ${LIBBSONCXX_INCLUDE_DIRS})

# We want to provide an absolute path to the library and we know the
# directory and the base name, but not the suffix, so we use CMake's
# find_library() to pick that up.
find_library(LIBMONGOCXX_LIBRARY_PATH mongocxx PATHS "${PACKAGE_PREFIX_DIR}/lib" NO_DEFAULT_PATH)
set(LIBMONGOCXX_LIBRARIES ${LIBMONGOCXX_LIBRARY_PATH} ${LIBBSONCXX_LIBRARIES})

set(LIBMONGOCXX_DEFINITIONS ${LIBBSONCXX_DEFINITIONS})
