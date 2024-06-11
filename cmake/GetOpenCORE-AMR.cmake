include(ExternalProject)

set(PREFIX ${CMAKE_BINARY_DIR}/_deps/amr)
set(INSTALL_DIR ${PREFIX}/usr)
set(INCLUDE_DIR ${INSTALL_DIR}/include)
set(LIBRARY_SUFFIX so)

ExternalProject_Add(opencore-amr
    PREFIX ${PREFIX}
    DOWNLOAD_NO_PROGRESS ON
    URL https://downloads.sourceforge.net/project/opencore-amr/opencore-amr/opencore-amr-0.1.6.tar.gz
    URL_HASH SHA1=3c2c618a5a38757a530034f80aa5356789589bcc
    BUILD_IN_SOURCE ON
    CONFIGURE_COMMAND ./configure --prefix=${INSTALL_DIR}
    BUILD_COMMAND make
    TEST_EXCLUDE_FROM_MAIN ON
    INSTALL_COMMAND make install
    INSTALL_DIR ${INSTALL_DIR}
    BUILD_BYPRODUCTS ${INSTALL_DIR}/lib/libopencore-amrnb.${LIBRARY_SUFFIX}
)

file(MAKE_DIRECTORY ${INCLUDE_DIR})

add_library(AMR::lib STATIC IMPORTED GLOBAL)
set_property(TARGET AMR::lib PROPERTY IMPORTED_LOCATION ${INSTALL_DIR}/lib/libopencore-amrnb.${LIBRARY_SUFFIX})
#set_property(TARGET AMR::lib PROPERTY IMPORTED_LOCATION ${INSTALL_DIR}/lib/libopencore-amrwb.${LIBRARY_SUFFIX})
set_property(TARGET AMR::lib PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${INCLUDE_DIR})
set_property(TARGET AMR::lib PROPERTY INTERFACE_COMPILE_DEFINITIONS HAVE_OPENCORE_AMR)
add_dependencies(AMR::lib opencore-amr)

