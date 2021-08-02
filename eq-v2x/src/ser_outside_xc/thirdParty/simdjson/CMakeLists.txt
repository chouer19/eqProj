file (GLOB SOURCES *.cpp *.hpp)

add_library(simdjson SHARED 
                 ${SOURCES}
           )
add_library(simdjson_static STATIC
                 ${SOURCES}
           )
SET_TARGET_PROPERTIES(simdjson_static PROPERTIES OUTPUT_NAME "simdjson" )
SET_TARGET_PROPERTIES(simdjson PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(simdjson_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

