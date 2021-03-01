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
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cc EXCLUDE cli)
include_directories(${PROJECT_SOURCE_DIR})
add_library(dqmcpp SHARED ${SRC_FILES} ecalchannels.o)

file(GLOB cli_src_FILES ${PROJECT_SOURCE_DIR}/cli/*.cc)
add_executable(dqm ${cli_src_FILES})
target_link_libraries(dqm dqmcpp ${CURL_LIBRARIES})