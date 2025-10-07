cmake_minimum_required(VERSION 3.16)
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${name})

find_file(IFADDR ifaddrs.h)
find_package(OpenSSL REQUIRED)
find_package(Resolv)

file(GLOB src src/*.c src/*/*.c)
ragel(${name} src/dns/tnet_dns_regexp.c ragel/tnet_dns_regexp.rl)
ragel(${name} src/dns/tnet_dns_resolvconf.c ragel/tnet_dns_resolvconf.rl)

add_library(${name} ${src} ${RAGEL_${name}_FILES})
target_include_directories(${name} PUBLIC src)
if(IFADDR)
    target_compile_definitions(${name} PRIVATE HAVE_IFADDRS_H HAVE_GETIFADDRS)
endif()
target_compile_definitions(${name} PUBLIC HAVE_OPENSSL)
target_link_libraries(${name} tinySAK OpenSSL::SSL resolv)
if(WIN32)
  target_link_libraries(${name} wsock32 ws2_32 Iphlpapi)
endif()

test()

