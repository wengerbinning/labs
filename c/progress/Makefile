
ECHO ?= echo
CFLAGS ?= -g



all:

%.o: %.c
	@$(ECHO) [CC] $^
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) $^


worker: worker.o
	@$(ECHO) [LD] $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

manager: fork.o
	@$(ECHO) [LD] $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

all: worker manager


clean:
	-rm -f *.o
	-rm -f worker
	-rm -f manager

.PHONY: all clean

