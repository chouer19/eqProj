file (GLOB SOURCES *.cpp *.h)
add_library(geographicLib SHARED 
                 ${SOURCES}
           )
add_library(geographicLib_static STATIC
                 ${SOURCES}
           )
SET_TARGET_PROPERTIES(geographicLib_static PROPERTIES OUTPUT_NAME "geographicLib" )
SET_TARGET_PROPERTIES(geographicLib PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(geographicLib_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
