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

# CURL

set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

# libecalchannels
file(GLOB_RECURSE ecalchannels_src ${PROJECT_SOURCE_DIR}/ecalchannels/*.cc )
add_library(ecalchannels SHARED ${ecalchannels_src} )

#libdqmcpp
set (libdqmcpplibs
  common
  ECAL
  net
  readers
  writers
  plugins
  colors
)
set (ldqm_sources)
foreach(d ${libdqmcpplibs})
  file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/${d}/*.cc)
  foreach(f ${SRC_FILES})
    list(APPEND ldqm_sources ${SRC_FILES})
  endforeach()
endforeach()

add_library(dqmcpp SHARED ${ldqm_sources})
target_link_libraries(dqmcpp ${CURL_LIBRARIES} ecalchannels)

# Plugins
set (plugins_sources)
file(GLOB_RECURSE plugins_FILES ${PROJECT_SOURCE_DIR}/PFGplugins/*.cc)
foreach(f ${plugins_FILES})
  list(APPEND plugins_sources ${plugins_FILES})
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/src)

add_library(dqmplugins SHARED ${plugins_sources})
target_link_libraries(dqmplugins dqmcpp)

# CLI programs
file(GLOB CLI_FILES ${PROJECT_SOURCE_DIR}/cli/*.cc)
foreach(cli ${CLI_FILES})
  get_filename_component(name ${cli} NAME_WE)
  add_executable(${name} ${cli})
  target_link_libraries(${name} dqmcpp dqmplugins)
endforeach(cli)

# Install headers
