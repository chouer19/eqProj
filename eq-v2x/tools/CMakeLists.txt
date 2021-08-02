file(GLOB SOURCES *.cpp)
foreach(tool ${SOURCES})
    get_filename_component(toolName "${tool}" NAME_WE)
    add_executable(${toolName} ${tool})
    target_link_libraries(${toolName} 
                          ${Boost_LIBRARIES}
                          ${CPPNETLIB_LIBRARIES}
                          eq-v2x
                          eq-sensor
                          eq-util
                          simdjson
                         )
endforeach()

add_executable(sqlite_shell sqlite_shell.c)
target_link_libraries(sqlite_shell sqlite)

