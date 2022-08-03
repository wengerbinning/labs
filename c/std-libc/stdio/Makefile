

ECHO := echo

all: stdio printf

%.o: %.c
	@$(ECHO) [CC] $@
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) $^

stdio: stdio.o
	@$(ECHO) [LD] $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)


printf: printf.o
	@$(ECHO) [LD] $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	-rm -rf *.o
