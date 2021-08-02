file (GLOB SOURCES *.c *.h)
add_library(zlib SHARED 
                 ${SOURCES}
           )
add_library(zlib_static STATIC
                 ${SOURCES}
           )
SET_TARGET_PROPERTIES(zlib_static PROPERTIES OUTPUT_NAME "zlib" )
SET_TARGET_PROPERTIES(zlib PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(zlib_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
