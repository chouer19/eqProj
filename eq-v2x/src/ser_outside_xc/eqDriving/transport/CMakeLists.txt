file (GLOB SOURCES *.cpp)
add_library(eq-transport ${SOURCES})
target_link_libraries(eq-transport
                      ${Boost_LIBRARIES}
                     )

