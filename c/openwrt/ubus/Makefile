

all: progress random

%.o: %.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $^
	@echo [CC] $^


LIBS := -lubox -lubus

progress: main.o
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo [LD] $@

random: random.o
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo [LD] $@

clean:
	-rm  -f *.o *.a
	-rm -rf progress