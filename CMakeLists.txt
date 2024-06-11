cmake_minimum_required(VERSION 3.16)
project(Doubango C CXX)

option(GENDOC "Generate Doxygen Documentation" ON)
option(WITH_SRTP "Secure Real-time Transport Protocol " ON)
option(WITH_OPUS "Opus audio codec" ON)
option(WITH_AMR "AMR audio codec" ON)
option(WITH_FFMPEG "FFmpeg is the leading multimedia framework, able to decode, encode" OFF)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type specified, setting 'Debug'")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include(AStyle)
if(WITH_SRTP)
    include(GetSRTP)
endif()
if(WITH_OPUS)
    include(GetOpus)
endif()
if(WITH_AMR)
    include(GetOpenCORE-AMR)
endif()
if(WITH_FFMPEG)
    include(GetFFmpeg)
endif()

find_package(Doxygen COMPONENTS dot OPTIONAL_COMPONENTS mscgen dia)

if(GENDOC AND DOXYGEN_FOUND)
    #include(FetchContent)
    #FetchContent_Declare(doxygen_css
    #    URL      https://github.com/jothepro/doxygen-awesome-css/archive/refs/tags/v2.3.2.tar.gz
    #    URL_HASH MD5=9571966298ee8bcb33aa7b296d439561
    #)
    #FetchContent_MakeAvailable(doxygen_css)
    #set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxygen_css_SOURCE_DIR}/doxygen-awesome.css)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_SOURCE_DIR}/README.md)
    set(DOXYGEN_EXCLUDE_PATTERNS ${CMAKE_BINARY_DIR} Licensing.md Wall_of_Shame.md */test/* website)
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
