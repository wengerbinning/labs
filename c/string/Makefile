

ECHO ?= echo

CC ?= gcc

CFLAGS := -g




all: string


%.o: %.c
	@$(ECHO) [CC] $^
	@$(CC) $(CFLAGS) $(CPPLAGS) -c $<

string: main.o
	@$(ECHO) [LD] $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

string-clean:
	-rm -f string

clean: string-clean
	-rm -f *.o *.a *.so