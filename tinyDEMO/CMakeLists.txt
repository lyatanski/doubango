cmake_minimum_required(VERSION 3.16)
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${name})

file(GLOB src src/*.c)
add_executable(demo ${src})
target_include_directories(demo PUBLIC src)
target_link_libraries(demo tinySIP tinyMSRP tinyDAV tinySMS)

