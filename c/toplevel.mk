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
	@$(call target_prepare, .target)
	@$(if $(relos),  $(call target_push, relos,  $(relos),  .target))
	@$(if $(mods),   $(call target_push, mods,   $(mods),   .target))
	@$(if $(stlibs), $(call target_push, stlibs, $(stlibs), .target))
	@$(if $(shlibs), $(call target_push, shlibs, $(shlibs), .target))
	@$(if $(execs),  $(call target_push, execs,  $(execs),  .target))

clean: FORCE $(mods-clean)
	-rm -f *.o *.i .target
	-$(if $(stlibs), rm -f $(stlibs))
	-$(if $(shlibs), rm -f $(shlibs))
	-$(if $(relos),  rm -f $(relos))
	-$(if $(execs),  rm -f $(execs))
	-rm -rf dest

install: FORCE $(mods-install) $(stlibs) $(shlibs) $(relos) $(execs)
	install -d $(DESTDIR)/$(bin)
	$(foreach file,$(execs), install -t $(DESTDIR)/$(bin) $(file))

uninstall: $(mods-uninstall) FORCE
	-$(foreach file, $(execs), rm -f $(DESTDIR)/$(bin)/$(file))
	-$(foreach file,$(shlibs), rm -f $(DESTDIR)/$(lib)/$(file))

help: FORCE
	@$(SCRIPTS_HOME)/build-help.sh $(mods) $(stlibs)

FORCE:
PHONY += FORCE all help
PHONY += install unistall clean dist-clean
.PHONY: $(PHONY) $(phony) $(mdirs) $(mods-clean) $(mods-install) $(mods-uninstall)

