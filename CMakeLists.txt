cmake_minimum_required(VERSION 3.14)
project(tinyWRAP C CXX)

set(CMAKE_C_FLAGS "-Wformat=0 -Wno-int-to-pointer-cast")
set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#find_package(Doxygen
#             REQUIRED dot
#             OPTIONAL_COMPONENTS mscgen dia)

enable_testing()

add_subdirectory(tinySAK)
add_subdirectory(tinyNET)
add_subdirectory(tinyHTTP)
add_subdirectory(tinySDP)
add_subdirectory(tinySIGCOMP)
add_subdirectory(tinyMEDIA)
add_subdirectory(tinyIPSec)
add_subdirectory(tinySIP)
add_subdirectory(tinyRTP)
add_subdirectory(tinyBFCP)
add_subdirectory(tinyDAV)
add_subdirectory(tinySMS)
add_subdirectory(tinyXCAP)
add_subdirectory(tinyMSRP)
add_subdirectory(tinyROHC)

add_subdirectory(tinyDEMO)
add_subdirectory(bindings)
add_subdirectory(plugins)

