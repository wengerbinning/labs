
ECHO := echo
CFLAGS := -g

all: stdlib

%.o: %.c
	@$(ECHO) [CC] $@
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) $^

stdlib: stdlib.o
	@$(ECHO) [LD] $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	-rm -rf *.o
