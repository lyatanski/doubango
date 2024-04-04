include(ExternalProject)

set(PREFIX ${CMAKE_BINARY_DIR}/_deps/libmnl)
set(MNL_INSTALL_DIR ${PREFIX}/usr)
set(MNL_INCLUDE_DIR ${MNL_INSTALL_DIR}/include)
set(MNL_LIBRARY_SUFFIX so)

ExternalProject_Add(libmnl
    PREFIX ${PREFIX}
    DOWNLOAD_NO_PROGRESS ON
    URL https://www.netfilter.org/projects/libmnl/files/libmnl-1.0.5.tar.bz2
    URL_HASH SHA256=274b9b919ef3152bfb3da3a13c950dd60d6e2bcd54230ffeca298d03b40d0525
    BUILD_IN_SOURCE ON
    CONFIGURE_COMMAND ./configure --prefix=${MNL_INSTALL_DIR}
    BUILD_COMMAND make
    TEST_EXCLUDE_FROM_MAIN ON
    INSTALL_COMMAND make install
    INSTALL_DIR ${MNL_INSTALL_DIR}
)

file(MAKE_DIRECTORY ${MNL_INCLUDE_DIR})

add_library(libMNL::nl STATIC IMPORTED GLOBAL)
set_property(TARGET libMNL::nl PROPERTY IMPORTED_LOCATION ${MNL_INSTALL_DIR}/lib/libmnl.${MNL_LIBRARY_SUFFIX})
set_property(TARGET libMNL::nl PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${MNL_INCLUDE_DIR})
add_dependencies(libMNL::nl libmnl)

