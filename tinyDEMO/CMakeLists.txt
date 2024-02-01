cmake_minimum_required(VERSION 3.4.1)
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${name})

if(DOXYGEN_FOUND)
doxygen_add_docs(api-${name} ${CMAKE_CURRENT_SOURCE_DIR} ALL)
endif()

file(GLOB src src/*.c)
add_executable(demo ${src})
target_include_directories(demo PUBLIC src)
target_link_libraries(demo tinySIP tinyMSRP tinyDAV tinySMS)

