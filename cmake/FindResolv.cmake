find_library(RESOLV_LIBRARIES resolv)

add_library(resolv INTERFACE)

if(NOT RESOLV_LIBRARIES)
 return()
endif()

target_link_libraries(resolv INTERFACE ${RESOLV_LIBRARIES})
