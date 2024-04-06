cmake_minimum_required(VERSION 3.16)
project(Doubango C CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

#include(GetOpenSSL)
#include(GetSRTP)
#include(GetiLBC)

find_package(Doxygen COMPONENTS dot OPTIONAL_COMPONENTS mscgen dia)

if(DOXYGEN_FOUND)
    #include(GetDoxygenAwesome)
    #set(DOXYGEN_HTML_EXTRA_STYLESHEET ${CMAKE_BINARY_DIR}/doxygen-awesome-css-2.3.2/doxygen-awesome.css)
    set(DOXYGEN_EXCLUDE_PATTERNS ${CMAKE_BINARY_DIR} README.md Licensing.md Wall_of_Shame.md */test/*)
    doxygen_add_docs(api-doc ALL)
endif()

#set(CMAKE_C_FLAGS "-Wformat=0 -Wno-int-to-pointer-cast")
#set(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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

set(CMAKE_INSTALL_PREFIX /usr)
set(CPACK_GENERATOR "TGZ")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME})
include(CPack)
