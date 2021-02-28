cmake_minimum_required (VERSION 2.8.11)
project (DQMCpp)


# CURL

set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED) 
include_directories(${CURL_INCLUDE_DIR})
message(STATUS ${CURL_INCLUDE_DIR})

#libsqmcpp
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cc EXCLUDE cli)
include_directories(${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "${SRC_FILES}")
add_library(libdqmcpp SHARED ${SRC_FILES})

file(GLOB cli_src_FILES ${PROJECT_SOURCE_DIR}/cli/*.cc)
add_executable(dqm ${cli_src_FILES})
target_link_libraries(dqm libdqmcpp ${CURL_LIBRARIES})