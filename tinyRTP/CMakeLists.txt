cmake_minimum_required(VERSION 3.16)
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${name})

file(GLOB src src/*.c src/*/*.c)
add_library(${name} ${src})
target_include_directories(${name} PUBLIC include)
target_link_libraries(${name} tinyNET tinyMEDIA)
if(WITH_SRTP)
    target_link_libraries(${name} SRTP::lib)
endif()

add_subdirectory(test)
