


ECHO ?= echo

all:

phony += clean distclean
clean:
	$(ECHO) This $@ target rules

distclean:
	$(ECHO) This $@ target rules

phony += install uninstall
install:
	$(ECHO) This $@ target rules

uninstall:
	$(ECHO) This $@ target rules

phony += help test
help:
	$(ECHO) This $@ target rules

test:
	$(ECHO) This $@ target rules

phony += check build
all: check
check:
	$(ECHO) This $@ target rules

all: build
build:
	$(ECHO) This $@ target rules



all:
	$(ECHO) This $@ target rules

.PHONY: all $(phony)
