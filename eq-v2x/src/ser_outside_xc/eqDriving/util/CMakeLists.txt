project(eq-util)

FILE(GLOB CPPSOURCE *.cpp)
#foreach(file IN LISTS CPPSOURCE)
#    message(STATUS ${file})
#endforeach(file)
add_library(eq-util 
            ${CPPSOURCE})
#target_link_libraries(eq-sensor
#        ${Boost_LIBRARIES}
#)

