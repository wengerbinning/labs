
ECHO ?= echo
MAKE ?= make
CC   ?= gcc
AR   ?= ar

CFLAGS     := -g
CPPFLAGS   ?= -Iinclude
MAKEFLAGS  := --no-print-directory
LIBS       :=

DESTDIR ?= $(shell pwd)/destdir
prefix  ?= usr
bin     ?= $(prefix)/bin
sbin    ?= $(prefix)/sbin
include ?= $(prefix)/include
lib     ?= $(prefix)/lib

mods         :=
mods-cflags  :=
mods-ldflags :=

tlibs         :=
tlibs-cflags  :=
tlibs-ldflags :=

slibs         :=
slibs-cflags  :=
slibs-ldflags :=

# prog          :=
# prog-cflags   := -Wl,-rpath=./
# prog-cppflags :=
# prog-ldflags  := -L./
# prog-libs     :=


export TOPDIR MAKEFLAGS
# ---------------------------------------------------------------------------- #

all:

# ---------------------------------------------------------------------------- #

#
define log
@echo -en "\e[32m[$(strip $(2))] \e[0m"; echo $(strip $(3))
endef

# ---------------------------------------------------------------------------- #
## BUild Sub modules
define module_perpare

endef

define module_build
$(strip $(1)):
	$(call log, info, MAKE, $$@)
	@$(MAKE) $(MAKEFLAGS) -C $$@ MOD_NAME=$$@ CPPFLAGS=-I$(TOPDIR)/include LDFLAGS=-L$(TOPDIR)
modules-clean += $(strip $(1))-clean
$(strip $(1))-clean:
	@$(MAKE) clean -C $(strip $(1))
endef

## Build Shared Library
define share_lib_perpare
shlibs     += lib$(strip $(1)).so
prog-libs  += -l$(strip $(1))
objs       := $(if $($(strip $(1))-objs), $($(strip $(1))-objs), $(strip $(1)).o)
cflags     := -fPIC$(if $(CFLAGS), $(CFLAGS))$(if $(shlib-cflags), $(shlib-cflags))$(if $($(strip $(1))-cflags), $($(strip $(1))-cflags))
cppflags   := -Iinclude$(if $(CPPFLAGS), $(CPPFLAGS))$(if $(shlib-cppflags), $(shlib-cppflags))$(if $($(strip $(1))-cppflags), $($(strip $(1))-cppflags))
ldflags    := $(if $(LDFLAGS), $(LDFLAGS))$(if $(shlib-ldflags), $(shlib-ldflags))$(if $($(strip $(1))-ldflags), $($(strip $(1))-ldflags))
libs       := $(if $(LIBS), $(LIBS))$(if $(shlib-libs), $(shlib-libs))$(if $($(strip $(1))-libs), $($(strip $(1))-libs))
endef
#
define share_lib_build
$(objs): %.o: %.c
	$(call log, info, CC, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC) -c$(if $(cflags), $(cflags))$(if $(cppflags), $(cppflags)) $$<
lib$(strip $(1)).so: $(objs)
	$(call log, info, LD, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC)$(if $(cflags), $(cflags))$(if $(ldflags), $(ldflags)) -shared -o $$@ $$^$(if $(libs), $(libs))
endef
#

## Build Static Library
define static_lib_perpare
stlibs    += lib$(strip $(1)).a
prog-libs += -l$(strip $(1))
objs      := $(if $($(strip $(1))-objs), $($(strip $(1))-objs), $(strip $(1)).o)
cflags    := $(if $(CFLAGS), $(CFLAGS))$(if $(stlib-cflags), $(stlib-cflags))$(if $($(strip $(1))-cflags), $($(strip $(1))-cflags))
cppflags  := -Iinclude$(if $(CPPFLAGS), $(CPPFLAGS))$(if $(stlib-cppflags), $(stlib-cppflags))$(if $($(strip $(1))-cppflags), $($(strip $(1))-cppflags))
ldflags   := $(if $(LDFLAGS), $(LDFLAGS))$(if $(stlib-ldflags), $(stlib-ldflags))$(if $($(strip $(1))-ldflags), $($(strip $(1))-ldflags))
libs      := $(if $(LIBS), $(LIBS))$(if $(stlib-libs), $(stlib-libs))$(if $($(strip $(1))-libs), $($(strip $(1))-libs))
endef
#
define static_lib_build
$(objs): %.o: %.c
	$(call log, info, CC, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC) -c$(if $(cflags), $(cflags))$(if $(cppflags), $(cppflags)) $$<
lib$(strip $(1)).a: $(objs)
	$(call log, info, LD, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(AR) crv $$@ $$^ >/dev/null
endef

## Build Library
define library_perpare
$(eval $(call $(strip $(1))_lib_perpare, $(2)))
endef
#
define library_build
$(eval $(call $(strip $(1))_lib_build, $(2)))
endef

## Build program
define program_perpare
progs    += $(strip $(1))
objs     := $(if $($(strip $(1))-objs), $($(strip $(1))-objs), $(strip $(1)).o)
cflags   := $(if $(CFLAGS), $(CFLAGS))$(if $(prog-cflags), $(prog-cflags))$(if $($(strip $(1))-cflags), $($(strip $(1))-cflags))
cppflags := -Iinclude$(if $(CPPFLAGS), $(CPPFLAGS))$(if $(prog-cppflags), $(prog-cppflags))$(if $($(strip $(1))-cppflags), $($(strip $(1))-cppflags))
ldflags  := -L.$(if $(LDFLAGS), $(LDFLAGS))$(if $(prog-ldflags), $(prog-ldflags))$(if $($(strip $(1))-ldflags), $($(strip $(1))-ldflags))
libs     := $(if $(LIBS), $(LIBS))$(if $(prog-libs), $(prog-libs))$(if $($(strip $(1))-libs), $($(strip $(1))-libs))
endef
#
define program_build
$(objs): %.o: %.c
	$(call log, info, CC,  $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC) -c$(if $(cflags), $(cflags))$(if $(cppflags), $(cppflags)) $$<
$(1): $(objs)
	$(call log, info, LD, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC)$(if $(cflags), $(cflags))$(if $(ldflags), $(ldflags)) -o $$@ $$^$(if $(libs), $(libs))
endef

##
define perpare
build_type   := $(strip $(1))
build_target := $(strip $(2))
endef

#
define build
$(if $(findstring $(strip $(1)), module program library static_lib),, $(error Not support this type:$(strip $(1))))
$(eval $(call perpare, $(1), $(2), $(3)))
$(eval $(call $(strip $(1))_perpare, $(2), $(3)))
$(eval $(call $(strip $(1))_build,   $(2), $(3)))
endef


