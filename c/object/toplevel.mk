# ifeq ($(MAKELEVEL), 0)
# include/autoconf.h: .config
# 	@echo "Rebuilding autoconf.h..."
# 	@(test -d include || mkdir include)
# 	@cat .config | ${SCRIPTS_HOME}/autconf.sh > include/autoconf.h

# autoconf: include/autoconf.h

# prepare += autoconf
# phony += autoconf
# endif

all: $(prepare) $(relos) $(mods) $(stlibs) $(shlibs) $(execs)
	@$(shell echo "# Auto Generate by make" > .target)
	@$(if $(mods),   $(shell echo "modules:$(mods)"              >> .target))
	@$(if $(relos),  $(shell echo "relocatable objects:$(relos)" >> .target))
	@$(if $(stlibs), $(shell echo "static libraries:$(stlibs)"   >> .target))
	@$(if $(shlibs), $(shell echo "shared libraries:$(shlib)"    >> .target))
	@$(if $(execs),  $(shell echo "executable objects:$(execs)"  >> .target))

clean: FORCE $(mods-clean)
	-rm -f *.o .target$(if $(stlibs), $(stlibs))$(if $(shlibs), $(shlibs))$(if $(relos), $(relos))$(if $(execs), $(execs))

install: FORCE $(mods-install) $(stlibs) $(shlibs) $(relos) $(execs)
	install -d $(DESTDIR)/$(bin)
	install -t $(DESTDIR)/$(bin) $(execs)

unistall: $(mods-uninstall) FORCE
	-rm -rf $(DESTDIR)/$(bin)

help: FORCE

.PHONY: FORCE all install unistall clean help $(phony) $(mdirs) $(mods-clean) $(mods-install) $(mods-uninstall)
FORCE: