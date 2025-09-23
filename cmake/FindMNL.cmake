find_library(MNL_LIB NAMES mnl)
find_path(MNL_INC NAMES libmnl/libmnl.h)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MNL REQUIRED_VARS MNL_LIB MNL_INC)


if(NOT MNL_LIB)
    include(ExternalProject)

    set(PREFIX ${CMAKE_BINARY_DIR}/_deps/libmnl)
    set(MNL_INSTALL_DIR ${PREFIX}/usr)
    set(MNL_INC ${MNL_INSTALL_DIR}/include)
    set(MNL_LIBRARY_SUFFIX so)
    mark_as_advanced(PREFIX)
    mark_as_advanced(MNL_INSTALL_DIR)
    mark_as_advanced(MNL_LIBRARY_SUFFIX)

    ExternalProject_Add(libmnl
        PREFIX ${PREFIX}
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_EXTRACT_TIMESTAMP ON
        URL https://www.netfilter.org/projects/libmnl/files/libmnl-1.0.5.tar.bz2
        URL_HASH SHA256=274b9b919ef3152bfb3da3a13c950dd60d6e2bcd54230ffeca298d03b40d0525
        BUILD_IN_SOURCE ON
        CONFIGURE_COMMAND ./configure --prefix=${MNL_INSTALL_DIR} --with-doxygen=no
        BUILD_COMMAND make
        TEST_EXCLUDE_FROM_MAIN ON
        INSTALL_COMMAND make install
        INSTALL_DIR ${MNL_INSTALL_DIR}
        BUILD_BYPRODUCTS ${MNL_INSTALL_DIR}/lib/libmnl.${MNL_LIBRARY_SUFFIX}
    )

    file(MAKE_DIRECTORY ${MNL_INC})

    set(MNL_LIB ${MNL_INSTALL_DIR}/lib/libmnl.${MNL_LIBRARY_SUFFIX})
endif()

mark_as_advanced(MNL_LIB)
mark_as_advanced(MNL_INC)

add_library(MNL::lib STATIC IMPORTED GLOBAL)
set_property(TARGET MNL::lib PROPERTY IMPORTED_LOCATION ${MNL_LIB})
set_property(TARGET MNL::lib PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${MNL_INC})

if(NOT MNL_FOUND)
    add_dependencies(MNL::lib libmnl)
endif()
