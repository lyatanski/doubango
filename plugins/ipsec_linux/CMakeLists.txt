if(UNIX AND NOT APPLE)
    include(Getlibmnl)

    add_library(ipsec SHARED ipsec_linux.c netlink_xfrm.c)
    set_target_properties(ipsec PROPERTIES PREFIX "")
    target_link_libraries(ipsec libMNL::nl tinyIPSec tinySAK)

    #add_executable(ipsectool tool.c netlink_xfrm.c)
    #target_link_libraries(ipsectool libMNL::nl tinySAK)
endif()

