
ECHO    ?= echo
CC      ?= gcc
#
CFLAGS   ?= -g
CPPFLAGS ?= -Iinclude
LIBS     ?= -lubox -lblobmsg_json
#
DESTDIR ?=$(shell pwd)/destdir
prefix  ?=usr
bin     ?= $(prefix)/bin
sbin    ?= $(prefix)/sbin
include ?= $(prefix)/include
lib     ?= $(prefix)/lib
#
tlibs :=
slibs :=
slibs-cflags := -fPIC
slibs-ldflags :=
progs :=
progs-libs :=
progs-cflags := -Wl,-rpath=./
progs-ldflags := -L./


all:

# =========================================================================== #

## macro function

#
define log
echo -en "\e[32m [$(strip $(2))] \e[0m"; echo $(strip $(3))
endef

# ---------------------------------------------------------------------------- #

#
define slibs_perpare
slibs += lib$(strip $(1)).so
progs-libs += -l$(strip $(1))
objs := $(if $($(strip $(1))-objs), $($(strip $(1))-objs), $(strip $(1)).o)
cflags   := $(CFLAGS) $(slibs-cflags) $($(strip $(1))-cflags)
cppflags = $(CPPFLAGS) $(slibs-cppflags) $($(strip $(1))-cppflags)
ldflags  := $(LDFLAGS) $(slibs-ldflags) $($(strip $(1))-ldflags)
libs  := $(LIBS) $(slibs-libs) $($(strip $(1))-libs)
endef

#
define progs_perpare
progs += $(strip $(1))
objs  := $(if $($(strip $(1))-objs), $($(strip $(1))-objs), $(strip $(1)).o)
cflags   := $(CFLAGS) $(progs-cflags) $($(strip $(1))-cflags)
cppflags = $(CPPFLAGS) $(progs-cppflags) $($(strip $(1))-cppflags)
ldflags  := $(LDFLAGS) $(progs-ldflags) $($(strip $(1))-ldflags)
libs  := $(LIBS) $(progs-libs) $($(strip $(1))-libs)
endef

#
define slibs_build
$(objs): %.o: %.c
	@$(call log, info, CC, $$@)
	@$(CC) -c $(cflags) $(cppflags) $$<
lib$(strip $(1)).so: $(objs)
	@$(call log, info, LD, $$@)
	@$(CC) $(cflags) $(ldflags) -shared -o $$@ $$^ $(libs)
endef

#
define progs_build
$(objs): %.o: %.c
	@$(call log, info, CC, $$@)
	@$(CC) -c $(cflags) $(cppflags) $$<
$(1): $(objs)
	@$(call log, info, LD, $$@)
	@$(CC) $(cflags) $(ldflags) -o $$@ $$^ $(libs)
endef

#
define perpare
build_type   := $(strip $(1))
build_target := $(strip $(2))
endef

#
define build
$(if $(findstring $(strip $(1)), progs slibs tlibs),, $(error Not support this type:$(strip $(1))))
$(eval $(call perpare, $(1), $(2)))
$(eval $(call $(strip $(1))_perpare, $(2)))
$(eval $(call $(strip $(1))_build, $(2)))
endef

# =========================================================================== #

##
$(eval $(call build,progs,ulog))
#
$(eval $(call build,progs,klist))
#
$(eval $(call build,progs,blobmsg))

# =========================================================================== #

##
all: $(tlibs) $(slibs) $(progs)

##
clean:
	-rm -f *.o
	-rm -f $(tlibs) $(slibs)
	-rm -f $(progs)

##
install: $(progs) FORCE
	install -d $(DESTDIR)/$(bin)
	install -t $(DESTDIR)/$(bin) $(progs)

##
unistall:
	-rm -rf $(DESTDIR)/$(bin)

##
.PHONY: all clean install
FORCE:
