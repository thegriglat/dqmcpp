cmake_minimum_required (VERSION 2.8.11)
project (DQMCpp)

# 
add_definitions("-Wall -Wextra")
set(default_build_type "Release")
 
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if(NOT PLUGINDIR )
  message(STATUS "Will build PFG plugins as -DPLUGINDIR not specified ")
endif()

# CURL

set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

#libdqmcpp
set (libdqmcpplibs
  common
  ECAL
  net
  readers
  writers
  plugins
  colors
  ecalchannels
  cxxopts
)

set (ldqm_sources)
foreach(d ${libdqmcpplibs})
  file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/${d}/*.cc)
  foreach(f ${SRC_FILES})
    list(APPEND ldqm_sources ${SRC_FILES})
  endforeach()
endforeach()

set (lplugin_sources)
if (NOT PLUGINDIR)
  set(PLUGINDIR "../PFGplugins")
endif()
file(GLOB_RECURSE PLUGIN_SRC_FILES ${PLUGINDIR}/*.cc)

add_library(dqmcpp SHARED ${ldqm_sources} ${PLUGIN_SRC_FILES})

target_link_libraries(dqmcpp ${CURL_LIBRARIES})

include_directories(${CMAKE_SOURCE_DIR}/src)

# CLI programs
file(GLOB CLI_FILES ${PROJECT_SOURCE_DIR}/cli/*.cc)
foreach(cli ${CLI_FILES})
  get_filename_component(name ${cli} NAME_WE)
  add_executable(${name} ${cli})
  target_link_libraries(${name} dqmcpp)
endforeach(cli)

# Install headers
