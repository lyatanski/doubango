cmake_minimum_required(VERSION 3.16)
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${name})

file(GLOB src src/*.c src/*/*.c)
add_library(${name} ${src})
target_include_directories(${name} PUBLIC include)
target_link_libraries(${name} tinyNET tinyMEDIA)

#file(GLOB tests test/*.c)
#foreach(test_src ${tests})
#    get_filename_component(test ${test_src} NAME_WE)
#    add_executable(${test}-${name} ${test_src})
#    target_link_libraries(${test}-${name} ${name})
#    add_test(${name} ./${test}-${name})
#endforeach()

