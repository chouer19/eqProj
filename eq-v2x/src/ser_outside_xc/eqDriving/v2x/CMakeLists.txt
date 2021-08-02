file (GLOB SOURCES *.cpp)

add_library(eq-v2x ${SOURCES})
target_link_libraries(eq-v2x
                      geographicLib
                      sqlite 
                      zlib 
                      ${CPPNETLIB_LIBRARIES}
                      ${Boost_LIBRARIES}
                     )

