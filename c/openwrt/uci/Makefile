prefix=/usr
DESTDIR=$(shell pwd)/destdir

bin=$(prefix)/bin
sbin=$(prefix)/sbin

ECHO ?= echo


all:

%.o: %.c
	@$(ECHO) [CC] $@
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) $^



LIBS=-luci

uci: uci.o
	@$(ECHO) [LD] $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

config: config.o
	@$(ECHO) [LD] $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

option: option.o
	@$(ECHO) [LD] $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

all: uci config option FORCE

install:

clean:
	-rm -f *.o

.PHONY: all
FORCE: