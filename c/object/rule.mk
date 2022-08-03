

#
ECHO ?= echo
MAKE ?= make
CC   ?= gcc
LD   ?= ld
AR   ?= ar
#

CFLAGS     ?= -g
CXXFLAGS   ?=
CPPFLAGS   ?=
LDFLAGS    ?=
LIBS       ?=

DESTDIR ?= $(shell pwd)/destdir

install_prefix  ?= usr/local
install_bin     ?= $(prefix)/bin
install_sbin    ?= $(prefix)/sbin
install_include ?= $(prefix)/include
install_lib     ?= $(prefix)/lib
install_libexec ?= $(prefix)/libexec
install_share	?= $(prefix)/share
install_etc		?= $(prefix)/etc

# module
mods            :=
mod-cflags      :=
mod-cppflags    :=
mod-ldflags     :=
mod-libs        :=

# share library
shlibs          :=
shlib-cflags    :=
shlib-cppflags  :=
shlib-ldflags   :=
shlib-libs      :=
# static library
stlibs          :=
stlib-cflags    :=
stlib-cppflags  :=
stlib-ldflags   :=
stlib-libs      :=

# relocatable object
relos          :=
relo-cflags    :=
relo-cppflags  :=
relo-ldflags   :=
relo-libs      :=
# executable object
execs          :=
exec-cflags    :=
exec-cppflags  :=
exec-ldflags   :=
exec-libs      :=

export TOPDIR
SCRIPTS_HOME :=$(TOPDIR)/scripts
CURDIR=$(PWD)
# =========================================================================== #

all:

# =========================================================================== #

#
define log
@echo -en "\e[32m[$(strip $(2))] \e[0m"; echo $(strip $(3))
endef

# =========================================================================== #

## Build Module
# =============
# perpare module
# usage: module_perpare <mod>
define module_perpare
mods            += $(strip $(1))
mods-install    += $(strip $(1))-install
mods-uninstall  += $(strip $(1))-uninstall
mods-clean      += $(strip $(1))-clean
cppflags  := -I$(TOPDIR)/$(if $(MOD_NAME),$(MOD_NAME)/)include$(if $(CPPFLAGS), $(CPPFLAGS))
ldflags   := -L$(TOPDIR)/$(if $(MOD_NAME),$(MOD_NAME)/)$(if $(LDFLAGS), $(LDFLAGS))
makeflags := --no-print-directory
mod_name  := $(if $(MOD_NAME),$(MOD_NAME)/)$(strip $(1))
modules   := $(mods)
endef

# build module
# usage: module_build <mod>
define module_build
$(strip $(1)): FORCE
	$(call log, info, MAKE, $(if $(MOD_NAME),$(MOD_NAME)/)$(strip $(1)))
	@$(MAKE)$(if $(makeflags), $(makeflags)) -C $$@  MOD_NAME="$(mod_name)" MODS="$(modules)" CPPFLAGS="$(cppflags)" LDFLAGS="$(ldflags)"
$(strip $(1))-install:
	$(call log, info, INSTALL, $(if $(MOD_NAME),$(MOD_NAME)/)$(strip $(1)))
	@$(MAKE)$(if $(makeflags), $(makeflags)) install -C $(strip $(1)) MOD_NAME="$(mod_name)"
$(strip $(1))-uninstall:
	$(call log, info, UNINSTALL, $(if $(MOD_NAME),$(MOD_NAME)/)$(strip $(1)))
	@$(MAKE)$(if $(makeflags), $(makeflags)) uninstall -C $(strip $(1)) MOD_NAME="$(mod_name)"
$(strip $(1))-clean:
	$(call log, info, CLEAN, $(if $(MOD_NAME),$(MOD_NAME)/)$(strip $(1)))
	@$(MAKE)$(if $(makeflags), $(makeflags)) clean -C $(strip $(1))  MOD_NAME="$(mod_name)"
endef

# get module target
# usage: module_target <mod> <target>
define module_target
$$(shell $(SCRIPTS_HOME)/target.sh $1 $2)
endef

## Build Library Target
# =====================

# perpare share library
# usage: library_share_perpare <target>
define library_share_perpare
shlibs     += lib$(strip $(1)).so
objs       := $(if $($(strip $(1))-objs), $($(strip $(1))-objs), $(strip $(1)).o)
cflags     := -fPIC$(if $(CFLAGS), $(CFLAGS))$(if $(shlib-cflags), $(shlib-cflags))$(if $($(strip $(1))-cflags), $($(strip $(1))-cflags))
cppflags   := -Iinclude$(if $(CPPFLAGS), $(CPPFLAGS))$(if $(shlib-cppflags), $(shlib-cppflags))$(if $($(strip $(1))-cppflags), $($(strip $(1))-cppflags))
ldflags    := $(if $(LDFLAGS), $(LDFLAGS))$(if $(shlib-ldflags), $(shlib-ldflags))$(if $($(strip $(1))-ldflags), $($(strip $(1))-ldflags))
libs       := $(if $(LIBS), $(LIBS))$(if $(shlib-libs), $(shlib-libs))$(if $($(strip $(1))-libs), $($(strip $(1))-libs))
mod-libs  += -l$(strip $(1))
endef

# build share library
# usage: library_share_build <target>
define library_share_build
$(objs): %.o: %.c
	$(call log, info, CC, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC) -c$(if $(cflags), $(cflags))$(if $(cppflags), $(cppflags)) $$<
lib$(strip $(1)).so: $(objs)
	$(call log, info, LD, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC)$(if $(cflags), $(cflags))$(if $(ldflags), $(ldflags)) -shared -o $$@ $$^$(if $(libs), $(libs))
endef

# perpare static library
# usage: library_static_perpare <target>
define library_static_perpare
stlibs    += lib$(strip $(1)).a
objs      := $(if $($(strip $(1))-objs), $($(strip $(1))-objs), $(strip $(1)).o)
cflags    := $(if $(CFLAGS), $(CFLAGS))$(if $(stlib-cflags), $(stlib-cflags))$(if $($(strip $(1))-cflags), $($(strip $(1))-cflags))
cppflags  := -Iinclude$(if $(CPPFLAGS), $(CPPFLAGS))$(if $(stlib-cppflags), $(stlib-cppflags))$(if $($(strip $(1))-cppflags), $($(strip $(1))-cppflags))
ldflags   := $(if $(LDFLAGS), $(LDFLAGS))$(if $(stlib-ldflags), $(stlib-ldflags))$(if $($(strip $(1))-ldflags), $($(strip $(1))-ldflags))
libs      := $(if $(LIBS), $(LIBS))$(if $(stlib-libs), $(stlib-libs))$(if $($(strip $(1))-libs), $($(strip $(1))-libs))
mod-libs += -l$(strip $(1))
endef

# build static library
# usage: library_static_build <target>
define library_static_build
$(objs): %.o: %.c
	$(call log, info, CC, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC) -c$(if $(cflags), $(cflags))$(if $(cppflags), $(cppflags)) $$<
lib$(strip $(1)).a: $(objs)
	$(call log, info, LD, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(AR) crv $$@ $$^ >/dev/null
endef

# perpare library
# usage: library_perpare <type> <target>
define library_perpare
$(eval $(call library_$(strip $(1))_perpare, $(2)))
endef

# build library
# usage: library_build <type> <target>
define library_build
$(eval $(call library_$(strip $(1))_build, $(2)))
endef

## Build Object
# =============
# perpare relocatable object
# usage: object_relocatable_perpare <target>
define object_relocatable_perpare
relos   += $(strip $(1)).o
objs     := $(if $($(strip $(1))-objs), $($(strip $(1))-objs), $(strip $(1)).o)
cflags   := $(if $(CFLAGS), $(CFLAGS))$(if $(relo-cflags), $(relo-cflags))$(if $($(strip $(1))-cflags), $($(strip $(1))-cflags))
cppflags := -Iinclude$(if $(CPPFLAGS), $(CPPFLAGS))$(if $(relo-cppflags), $(relo-cppflags))$(if $($(strip $(1))-cppflags), $($(strip $(1))-cppflags))
ldflags  := -L.$(if $(LDFLAGS), $(LDFLAGS))$(if $(relo-ldflags), $(relo-ldflags))$(if $($(strip $(1))-ldflags), $($(strip $(1))-ldflags))
libs     := $(if $(LIBS), $(LIBS))$(if $(relo-libs), $(relo-libs))$(if $($(strip $(1))-libs), $($(strip $(1))-libs))
endef

# build relocatable object
# usage: object_relocatable_build <taregt>
define object_relocatable_build
$(objs): %.o: %.c
	$(call log, info, CC, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC) -c$(if $(cflags), $(cflags))$(if $(cppflags), $(cppflags)) $$<
$(strip $(1)).o: $(objs)
	$(call log, info, LD, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(LD) --no-gc-sections -r $$^ -o $$@
endef

# parpare executable object
# usage: object_executable_perpare
define object_executable_perpare
execs    += $(strip $(1))
objs     := $(if $($(strip $(1))-objs), $($(strip $(1))-objs), $(strip $(1)).o)
cflags   := $(if $(CFLAGS), $(CFLAGS))$(if $(exec-cflags), $(exec-cflags))$(if $($(strip $(1))-cflags), $($(strip $(1))-cflags))
cppflags := -Iinclude$(if $(CPPFLAGS), $(CPPFLAGS))$(if $(exec-cppflags), $(exec-cppflags))$(if $($(strip $(1))-cppflags), $($(strip $(1))-cppflags))
ldflags  := $(if $(LDFLAGS), $(LDFLAGS))$(if $(mod-ldflags), $(mod-ldflags))$(if $(exec-ldflags), $(exec-ldflags))$(if $($(strip $(1))-ldflags), $($(strip $(1))-ldflags))
libs     := $(if $(LIBS), $(LIBS))$(if $(mod-libs), $(mod-libs))$(if $(exec-libs), $(exec-libs))$(if $($(strip $(1))-libs), $($(strip $(1))-libs))
mod-objs := $(foreach mod, $(mods),$$(call module_target,$(mod), relos))
endef

# build executable object
# usage: object_executable_build <target>
define object_executable_build
$(objs): %.o: %.c
	$(call log, info, CC, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC) -c$(if $(cflags), $(cflags))$(if $(cppflags), $(cppflags)) $$<
$(1): $(objs) $(mod-objs)
	$(call log, info, LD, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC)$(if $(cflags), $(cflags))$(if $(ldflags), $(ldflags)) -o $$@ $(objs)$(if $(mod-objs), $(mod-objs))$(if $(libs), $(libs))
endef

# perpare object
# usage: object_perpare <type> <target>
define object_perpare
$(eval $(call object_$(strip $(1))_perpare, $(2)))
endef

# build object
# usage: object_build <type> <target>
define object_build
$(eval $(call object_$(strip $(1))_build, $(2)))
endef

## Perpare
# ========
define perpare
type   := $(strip $(1))/$(strip $(2))
target := $(strip $(3))
endef

## build
# =======
define build
$(if $(findstring $(strip $(1)), module library object), , $(error Not support this type:$(strip $(1))))
$(eval $(call perpare, $(1), $(2), $(3)))
$(eval $(call $(strip $(1))_perpare, $(2), $(3)))
$(eval $(call $(strip $(1))_build,   $(2), $(3)))
endef


