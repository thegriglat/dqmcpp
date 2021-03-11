cmake_minimum_required (VERSION 2.8.11)
project (DQMCpp)

# CURL

set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

# ecalchannels
add_custom_command(
  OUTPUT ecalchannels.o
  COMMAND cp ${PROJECT_SOURCE_DIR}/ECAL/ecalchannels.dat ecalchannels.dat
  COMMAND objcopy --input binary --output elf64-x86-64 ecalchannels.dat ecalchannels.o
  DEPENDS ${PROJECT_SOURCE_DIR}/ECAL/ecalchannels.dat
  VERBATIM)

#libdqmcpp
set (libdqmcpplibs
  common
  ECAL
  net
  filters
  readers
  writers
)
set (ldqm_sources)
foreach(d ${libdqmcpplibs})
  file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/${d}/*.cc)
  foreach(f ${SRC_FILES})
    list(APPEND ldqm_sources ${SRC_FILES})
  endforeach()
endforeach()

add_library(dqmcpp SHARED ${ldqm_sources} ecalchannels.o)
target_link_libraries(dqmcpp ${CURL_LIBRARIES})
install(TARGETS dqmcpp LIBRARY)

# Plugins
set (plugins_sources)
file(GLOB_RECURSE plugins_FILES ${PROJECT_SOURCE_DIR}/plugins/*.cc)
foreach(f ${plugins_FILES})
  list(APPEND plugins_sources ${plugins_FILES})
endforeach()

add_library(dqmplugins SHARED ${plugins_sources})
target_link_libraries(dqmplugins dqmcpp)
install(TARGETS dqmplugins LIBRARY)

# CLI programs
file(GLOB CLI_FILES ${PROJECT_SOURCE_DIR}/cli/*.cc)
foreach(cli ${CLI_FILES})
  get_filename_component(name ${cli} NAME_WE)
  add_executable(${name} ${cli})
  target_link_libraries(${name} dqmcpp dqmplugins)
  install(TARGETS ${name} RUNTIME)
endforeach(cli)

# Install headers
install(DIRECTORY "." TYPE INCLUDE FILES_MATCHING PATTERN "*.hh")
