project(frenet_tentacles)
cmake_minimum_required(VERSION 3.5)
find_package(Qt5 COMPONENTS Widgets REQUIRED)


set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${Qt5Widgets_INCLUDE_DIRS})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(PROJECT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(${PROJECT_INCLUDE_DIR})

add_subdirectory(${PROJECT_INCLUDE_DIR}/thirdParty)
add_subdirectory(src)

file(GLOB HEADERS ${PROJECT_INCLUDE_DIR}/eqDriving/planning/form/*.hpp)
file(GLOB_RECURSE SOURCES ./src/planning/*.cpp)
add_executable(path_painting main.cpp ${SOURCES} ${HEADERS})
target_link_libraries(path_painting 
                        ${Qt5Widgets_LIBRARIES}
                        sqlite
                        eq-vehicle
                        #${eq-vehicle}
                     )
