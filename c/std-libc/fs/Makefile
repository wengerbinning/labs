


all: file

%.o: %.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $<
	@echo "[CC] $@"


file: file_fread.o hexdump.o
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo "[LD] $@"

clean:
	-rm -rf *.o