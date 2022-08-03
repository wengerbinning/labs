

ECHO := echo

all: stdint

%.o: %.c
	@$(ECHO) [CC] $@
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) $^

stdint: stdint.o
	@$(ECHO) [LD] $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
