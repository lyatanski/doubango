cmake_minimum_required(VERSION 3.16)
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${name})

file(GLOB src src/*.c src/*/*.c)

# SDP Message parser
ragel(${name} src/content/tmedia_content_cpim.c ragel/tmedia_content_cpim.rl)
# 'image-attr' (RFC 6236)
ragel(${name} src/tmedia_imageattr.c ragel/tmedia_imageattr.rl)

add_library(${name} ${src} ${RAGEL_${name}_FILES})
target_include_directories(${name} PUBLIC include)
target_link_libraries(${name} tinySDP tinyNET)

test()

