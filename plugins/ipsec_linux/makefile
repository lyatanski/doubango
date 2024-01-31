CFLAGS=-g -O0 -Wall
CFLAGS+=-fPIC -I../../tinySAK/src -I../../tinyIPSec/src $(shell pkg-config --cflags libmnl)
LIBS=$(shell pkg-config --libs libmnl) -lpthread -ldl -ltinySAK -ltinyIPSec

all: ipsec_linux.so linux-ipsec-tool

linux-ipsec-tool: tool.o netlink_xfrm.o
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

ipsec_linux.so: ipsec_linux.o netlink_xfrm.o
	$(CC) -o $@ -shared $^ $(LIBS)

clean:
	@rm -f linux-ipsec-tool ipsec_linux.so *.o
