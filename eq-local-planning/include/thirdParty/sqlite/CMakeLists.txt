file (GLOB SOURCES *.c *.h)                                                    
 
add_library(sqlite SHARED
                 ${SOURCES}
           )
add_library(sqlite_static STATIC
                 ${SOURCES}
           )
SET_TARGET_PROPERTIES(sqlite_static PROPERTIES OUTPUT_NAME "sqlite" )
SET_TARGET_PROPERTIES(sqlite PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(sqlite_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
 
target_link_libraries(sqlite
                      dl
                      pthread
                     )

