ECHO ?= echo

CFLAGS   := -g
CPPFLAGS := -Iinclude

LIBS := -lubox -lblobmsg_json

all:

%.o: %.c
	@$(ECHO) [CC] $<
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

parse: main.o
	@$(ECHO) [LD] $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)


all: parse FORCE

clean:
	-rm -f *.o

.PHONY: all clean
FORCE: