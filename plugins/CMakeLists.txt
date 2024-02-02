add_library(ipsec SHARED ipsec_linux/ipsec_linux.c ipsec_linux/netlink_xfrm.c)
set_target_properties(ipsec PROPERTIES PREFIX "")
target_include_directories(ipsec PRIVATE ../tinyIPSec/src ../tinySAK/src)
target_link_libraries(ipsec mnl)

add_executable(ipsectool ipsec_linux/tool.c ipsec_linux/netlink_xfrm.c)
target_link_libraries(ipsectool mnl)

