cmake_minimum_required(VERSION 3.4.1)
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${name})

find_package(OpenSSL REQUIRED)

if(DOXYGEN_FOUND)
doxygen_add_docs(api-${name} ${CMAKE_CURRENT_SOURCE_DIR} ALL)
endif()

file(GLOB src src/*.c src/*/*.c)
add_library(${name} ${src})
target_include_directories(${name} PUBLIC src)
target_compile_definitions(${name} PUBLIC HAVE_OPENSSL)
target_link_libraries(${name} tinySAK OpenSSL::SSL) # for include inheritance

file(GLOB tests test/*.c)
foreach(test_src ${tests})
    get_filename_component(test ${test_src} NAME_WE)
    add_executable(${test}-${name} ${test_src})
    target_link_libraries(${test}-${name} ${name})
    add_test(${name} ./${test}-${name})
endforeach()

