cmake_minimum_required (VERSION 2.8.11)
project (DQMCpp)

# CURL

set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})

# ecalchannels
add_custom_command(
  OUTPUT ecalchannels.o
  COMMAND cp ${PROJECT_SOURCE_DIR}/data/ecalchannels.dat ecalchannels.dat
  COMMAND objcopy --input binary --output elf64-x86-64 ecalchannels.dat ecalchannels.o
  DEPENDS ${PROJECT_SOURCE_DIR}/data/ecalchannels.dat
  VERBATIM)

#libdqmcpp
set (libdqmcpplibs
  common
  dataclasses
  filters
  plugins
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

include_directories(${PROJECT_SOURCE_DIR})
add_library(dqmcpp SHARED ${ldqm_sources} ecalchannels.o)
install(TARGETS dqmcpp LIBRARY)

# CLI programs
file(GLOB CLI_FILES ${PROJECT_SOURCE_DIR}/cli/*.cc)
foreach(cli ${CLI_FILES})
  get_filename_component(name ${cli} NAME_WE)
  add_executable(${name} ${cli})
  target_link_libraries(${name} dqmcpp ${CURL_LIBRARIES})
  install(TARGETS ${name} RUNTIME)
endforeach(cli)

# Install headers
install(DIRECTORY "." TYPE INCLUDE FILES_MATCHING PATTERN "*.hh")
