cmake_minimum_required(VERSION 3.1)
project(eq-v2x)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#include (${CMAKE_SOURCE_DIR}/cmake/*.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/FindBoost.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/FindNetlib.cmake)

set(PROJECT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
#set(PROJECT_LIB_DIR "${CMAKE_SOURCE_DIR}/libs")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 

include_directories(${PROJECT_INCLUDE_DIR})
add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(tools)
add_subdirectory(tests)
