
PROJECT =../../..

CFLAGS = -g

CXX=
CXXFLAGS=

ECHO = echo

CPPFLAGS = -I$(PROJECT)/include -Iinclude
LDFLAGS=


AR = ar

all: executable library

executable: socket
library: libpacket.a libsocket.a

LIBS = libsocket.a libpacket.a





libpacket.a: data.o
	@$(ECHO) [AR] $@
	@$(AR) crs $@ $^

libsocket.a: pf_packet-sock_packet-all.o
	@$(ECHO) [AR] $@
	@$(AR) crs $@ $^

socket: socket.o $(LIBS)
	@$(ECHO) [LD] $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c
	@$(ECHO) [CC] $^
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $^

clean:
	@rm -rf *.o
	@rm -rf *.a



