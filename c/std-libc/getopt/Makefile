PROJECT =../../

CFLAGS = -g

CXX=
CXXFLAGS=

ECHO = echo

CPPFLAGS = -I$(PROJECT)/include
LDFLAGS=

LIBS=

all: getopt_long

getopt_long: getopt_long.o
	@$(ECHO) [LD] $@
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)


%.o: %.c
	@$(ECHO) [CC] $^
	@$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $^

clean:
	@rm -rf *.o



