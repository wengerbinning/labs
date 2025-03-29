DESTDIR ?= dest


all: init

%.o: %.c
	@$(CC) -c $(CFLAGS) $(CPPLAGS) $^
	@echo [CC] $@

init: init.o
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo [LD] $@

clean:
	-rm -f *.o *.a *.so
	-rm -f init

install:
	@install -d $(DESTDIR)
	install -t $(DESTDIR) -m 775 init
	@install -d $(DESTDIR)/etc
	install -t $(DESTDIR)/etc -m 664 banner