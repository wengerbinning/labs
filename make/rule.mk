
## Dependent Command Tools
# This section specifies the command tools required for building the framework,
# which can be overridden using environment variables in across compilation
# environment.

CC   ?= gcc
LD   ?= ld
AR   ?= ar
MAKE ?= make
RM   ?= rm

ifeq ($(shell  echo '\n' | wc -l),2)
  ECHO ?= echo
else
  ECHO ?= echo -e
endif

export ECHO MAKE RM CC LD AR

## Project Build Parameters
# This section specifies the compilation parameters required for the project.

CFLAGS   += -g
CXXFLAGS ?=
CPPFLAGS ?=
LDFLAGS  ?=
LIBS     ?=

TOPDIR  ?= $(CURDIR)
DESTDIR ?= $(shell pwd)/dest

export DESTDIR
## Project Install Parameters
# This section specifies the install parameters required for the project.

prefix  ?= usr/local
bin     ?= $(prefix)/bin
sbin    ?= $(prefix)/sbin
lib     ?= $(prefix)/lib
libexec ?= $(prefix)/libexec
include ?= $(prefix)/include
share	?= $(prefix)/share
etc		?= $(prefix)/etc

## Porject Build Module Parameters
# this section specificies the build parameters required for the module. If you
# need debug, you can analyze it by printing these variables.

mod-cflags   ?=
mod-cxxflags ?=
mod-cppflags ?=
mod-ldflags  ?=
mod-libs     ?=

## Project Build  Share Library Parameters
# This section specifices the build parameters required for the share library.
# If you need debug, you can analyze it byu printing these variables.

shlib-cflags    ?=
shlib-cxxflags  ?=
shlib-cppflags  ?=
shlib-ldflags   ?=
shlib-libs      ?=

## Project Build Static Library Parameters
# This section specifices the build parameters required for the static library.
# If you need debug, you can analyze it byu printing these variables.

stlib-cflags    ?=
stlib-cxxflags  ?=
stlib-cppflags  ?=
stlib-ldflags   ?=
stlib-libs      ?=

## Project Build Relocatable Object Parameters
# This section specifices the build parameters required for the relocatable
# object. If you need debug, you can analyze it byu printing these variables.

relo-cflags    ?=
relo-cxxflags  ?=
relo-cppflags  ?=
relo-ldflags   ?=
relo-libs      ?=

## Project Build Executable Object Parameters
# This section specifices the build parameters required for the executable
# object. If you need debug, you can analyze it byu printing these variables.

exec-cflags    ?=
exec-cxxflags  ?=
exec-cppflags  ?=
exec-ldflags   ?=
exec-libs      ?=




# Convenient variables


# =========================================================================== #
#
# target rules
#

dot-target = $(dir $@).$(notdir $@)
depfile = $(subst $(comma),_,$(dot-target).d)
basetarget = $(basename $(notdir $@))

# =========================================================================== #

export TOPDIR
CURDIR=$(PWD)
SCRIPTS_HOME  ?=$(TOPDIR)/scripts

SCRIPT_TARGET := $(SCRIPTS_HOME)/build-target.sh
SCRIPT_HELP   := $(SCRIPTS_HOME)/build-help.sh

all:

## Build Target Variables
# (No need to specify automatically generated the build framework.)

cflags    :=
cxxflags  :=
cppflags  :=
ldflags   :=
objs      :=
mods-objs :=
libs      :=
mods-libs :=
flags1    :=
flags2    :=

## Build Project Variables
# (No need to specify automatically generated the build framework.)

shlibs         :=
stlibs         :=
relos          :=
execs          :=
mods           :=
mods-install   :=
mods-uninstall :=
mods-clean     :=

# =========================================================================== #

# usage: log <level> <message>
define log
case $1 in \
  error)  echo -en "\e[31m[$(strip $(2))] \e[0m"; echo $(strip $(3)) ;; \
  info)   echo -en "\e[32m[$(strip $(2))] \e[0m"; echo $(strip $(3)) ;; \
  notice) echo -en "\e[33m[$(strip $(2))] \e[0m"; echo $(strip $(3)) ;; \
esac
endef

#usage: target <file>
define target_prepare
@ echo "# Auto Generate by Build Framework" > $1
endef

# usage: target <type> <list> <file>
define target_push
  case $1 in \
    relos)  for each in $2; do echo $${each}    >> $3; done ;; \
    mods)   for each in $2; do echo $${each}'/' >> $3; done ;; \
    stlibs) for each in $2; do echo $${each}    >> $3; done ;; \
    shlibs) for each in $2; do echo $${each}    >> $3; done ;; \
    execs)  for each in $2; do echo $${each}'*' >> $3; done ;; \
  esac
endef

# usage: target_pull <target> <mods>
define target_pull
  $(shell $(SCRIPTS_HOME)/build-target.sh $1 $2)
endef

# =========================================================================== #

# =========================================================================== #

## Build Module
# =============

# usage: module_perpare_variables <mod>
define module_perpare_variables
  cppflags := -I$(TOPDIR)
  cppflags += -I$(if $(MOD_NAME),$(MOD_NAME)/)include
  cppflags += $(if $(CPPFLAGS), $(CPPFLAGS))
  ldflags := -L$(TOPDIR)$(if $(MOD_NAME),/$(MOD_NAME))
  ldflags += $(if $(LDFLAGS), $(LDFLAGS))
  dirs := $(if $($(target)-dirs), $($(target)-dirs), $(target))
endef
# usage: module_perpare <mod>
define module_perpare
  mods += $(target)
  mods-install += $(target)-install
  mods-uninstall += $(target)-uninstall
  mods-clean += $(target)-clean

  $(eval $(call module_perpare_variables, $(1)))
  makeflags := --no-print-directory
  makeflags += MOD_NAME="$(if $(MOD_NAME),$(MOD_NAME)/)$(target)"
  buildflags := MODS="$(mods)" CPPFLAGS="$(cppflags)" LDFLAGS="$(ldflags)"
endef
# usage: module_build <mod>
define module_build
$(strip $(1)): FORCE
	@$(call log, notice, MAKE, $(if $(MOD_NAME),$(MOD_NAME)/)$(target))
	@$(MAKE)$(if $(makeflags), $(makeflags)) -C $(target) $(buildflags)
$(strip $(1))-install:
	@$(call log, notice, INSTALL, $(if $(MOD_NAME),$(MOD_NAME)/)$(target))
	@$(MAKE)$(if $(makeflags), $(makeflags)) install -C $(target)
$(strip $(1))-uninstall:
	@$(call log, notice, UNINSTALL, $(if $(MOD_NAME),$(MOD_NAME)/)$(target))
	@$(MAKE)$(if $(makeflags), $(makeflags)) uninstall -C $(target)
$(strip $(1))-clean:
	@$(call log, notice, CLEAN, $(if $(MOD_NAME),$(MOD_NAME)/)$(target))
	@$(MAKE)$(if $(makeflags), $(makeflags)) clean -C $(target)
endef
# usage: module_target <target> <path>...
define module_target
$$(shell $(SCRIPTS_HOME)/build-target.sh $1 $2)
endef

# =========================================================================== #

# usage: library_share_perpare_varibales <target>
define library_share_perpare_varibales
  cflags := -fPIC
  cflags += $(if $(CFLAGS), $(CFLAGS))
  cflags += $(if $(shlib-cflags), $(shlib-cflags))
  cflags += $(if $($(target)-cflags), $($(target)-cflags))
  cppflags := -Iinclude
  cppflags += $(if $(CPPFLAGS), $(CPPFLAGS))
  cppflags += $(if $(shlib-cppflags), $(shlib-cppflags))
  cppflags += $(if $($(target)-cppflags), $($(target)-cppflags))
  ldflags := $(if $(LDFLAGS), $(LDFLAGS))
  ldflags += $(if $(shlib-ldflags), $(shlib-ldflags))
  ldflags += $(if $($(target)-ldflags), $($(target)-ldflags))

  objs := $(if $($(target)-objs), $($(target)-objs), $(target).o)
  libs := $(if $(LIBS), $(LIBS))
  libs += $(if $(shlib-libs), $(shlib-libs))
  libs += $(if $($(target)-libs), $($(target)-libs))
endef
# usage: library_share_perpare <target>
define library_share_perpare
  shlibs += lib$(target).so
  mod-libs += -l$(target)

  $(eval $(call library_share_perpare_varibales, $(1)))
  compflags := -c
  compflags += $(if $(cflags), $(cflags))$(if $(cppflags), $(cppflags))
  linkflags := $(if $(cflags), $(cflags))$(if $(ldflags), $(ldflags))
endef
# usage: library_share_build <target>
define library_share_build
$(objs): %.o: %.c
	@$(call log, info, CC, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC)$(if $(compflags), $(compflags)) $$<
lib$(strip $(1)).so: $(objs)
	@$(call log, info, LD, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC)$(if $(linkflags), $(linkflags)) -shared -o $$@ $$^$(if $(libs), $(libs))
endef

# usage: library_static_perpare_variables <target>
define library_static_perpare_variables
  cflags := $(if $(CFLAGS), $(CFLAGS))
  cflahs += $(if $(stlib-cflags), $(stlib-cflags))
  cflags += $(if $($(target)-cflags), $($(target)-cflags))
  cppflags := -Iinclude
  cppflags += $(if $(CPPFLAGS), $(CPPFLAGS))
  cppflags += $(if $(stlib-cppflags), $(stlib-cppflags))
  cppflags += $(if $($(target)-cppflags), $($(target)-cppflags))
  ldflags := $(if $(LDFLAGS), $(LDFLAGS))
  ldflags += $(if $(stlib-ldflags), $(stlib-ldflags))
  ldflags += $(if $($(target)-ldflags), $($(target)-ldflags))

  objs := $(if $($(target)-objs), $($(target)-objs), $(target).o)
  libs := $(if $(LIBS), $(LIBS))
  libs += $(if $(stlib-libs), $(stlib-libs))
  libs += $(if $($(target)-libs), $($(target)-libs))
endef
# usage: library_static_perpare <target>
define library_static_perpare
  stlibs += lib$(target).a
  mod-libs += -l$(target)

  $(eval $(call library_static_perpare_variables, $(1)))
  compflags := -c
  compflags += $(if $(cflags), $(cflags))$(if $(cppflags), $(cppflags))
endef
# usage: library_static_build <target>
define library_static_build
$(objs): %.o: %.c
	@$(call log, info, CC, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC)$(if $(compflags), $(compflags)) $$<
lib$(strip $(1)).a: $(objs)
	@$(call log, info, LD, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(AR) crv $$@ $$^ >/dev/null
endef

## Build Library
# ==============

# usage: library_perpare <type> <target>
define library_perpare
  $(eval $(call library_$(strip $(1))_perpare, $(2)))
endef
# usage: library_build <type> <target>
define library_build
  $(eval $(call library_$(strip $(1))_build, $(2)))
endef

# =========================================================================== #

# usage: object_relocatable_perpare_variables <target>
define object_relocatable_perpare_variables
  cflags := $(if $(CFLAGS), $(CFLAGS))
  cflags += $(if $(relo-cflags), $(relo-cflags))
  cflags += $(if $($(target)-cflags), $($(target)-cflags))
  cppflags := -Iinclude
  cppflags += $(if $(CPPFLAGS), $(CPPFLAGS))
  cppflags += $(if $(relo-cppflags), $(relo-cppflags))
  cppflags += $(if $($(target)-cppflags), $($(target)-cppflags))
  ldflags := -L.
  ldflags += $(if $(LDFLAGS), $(LDFLAGS))
  ldflags += $(if $(relo-ldflags), $(relo-ldflags))
  ldflags += $(if $($(target)-ldflags), $($(target)-ldflags))

  objs := $(if $($(target)-objs), $($(target)-objs), $(target).o)
  libs := $(if $(LIBS), $(LIBS))
  libs += $(if $(relo-libs), $(relo-libs))
  libs += $(if $($(target)-libs), $($(target)-libs))
endef
# usage: object_relocatable_perpare <target>
define object_relocatable_perpare
  relos += $(target).o

  $(eval $(call object_relocatable_perpare_variables, $(1)))
  compflags := -c
  compflags += $(if $(cflags), $(cflags))$(if $(cppflags), $(cppflags))
endef

# usage: object_relocatable_build <taregt>
define object_relocatable_build
$(objs): %.o: %.c
	@$(call log, info, CC, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC) -c$(if $(compflags), $(compflags)) $$<
$(strip $(1)).o: $(objs)
	@$(call log, info, LD, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(LD) --no-gc-sections -o $$@ -r $$^
endef

# usage: object_executable_perpare_variables <target>
define object_executable_perpare_variables
  cflags := $(if $(CFLAGS), $(CFLAGS))$(if $(exec-cflags), $(exec-cflags))
  cflags += $(if $($(target)-cflags), $($(target)-cflags))
  cppflags := -Iinclude
  cppflags += $(if $(CPPFLAGS), $(CPPFLAGS))
  cppflags += $(if $(exec-cppflags), $(exec-cppflags))
  cppflags += $(if $($(target)-cppflags), $($(target)-cppflags))
  ldflags := -L.
  ldflags += $(if $(LDFLAGS), $(LDFLAGS))
  ldflags += $(if $(mod-ldflags), $(mod-ldflags))
  ldflags += $(if $(exec-ldflags), $(exec-ldflags))
  ldflags += $(if $($(target)-ldflags), $($(target)-ldflags))

  objs := $(if $($(target)-objs), $($(target)-objs), $(target).o)
  libs := $(if $(LIBS), $(LIBS))$(if $(mod-libs), $(mod-libs))
  libs += $(if $(exec-libs), $(exec-libs))
  libs += $(if $($(target)-libs), $($(target)-libs))

  mods-objs := $(if $(mods),$$(call module_target,relos, $(mods)))
  mods-libs := $(if $(mods), $$(call module_target, shlibs, $(mods)))
  mods-libs += $(if $(mods), $$(call module_target, stlibs, $(mods)))
endef
# usage: object_executable_perpare <target>
define object_executable_perpare
  execs += $(target)

  $(eval $(call object_executable_perpare_variables, $(1)))
  compflags := -c
  compflags += $(if $(cflags), $(cflags))$(if $(cppflags), $(cppflags))
  linkflags := $(if $(cflags), $(cflags))$(if $(ldflags), $(ldflags))
  link := $(objs)$(if $(relos), $(relos))
  link += $(if $$(mods-objs), $$(mods-objs))
  link += $(if $(libs), $(libs))
  link += $(if $$(mods-libs), $$(mods-libs))
endef
# usage: object_executable_build <target>
define object_executable_build
$(objs): %.o: %.c FORCE
	@$(call log, info, CC, $(if $(MOD_NAME),$(MOD_NAME)/)$$@)
	@$(CC) $(if $(compflags), $(compflags)) $$<
$(1): $(objs) $(relo-objs)
	@$(call log, info, LD, $(if $(MOD_NAME),$(MOD_NAME)/)$$@ $(mods-libs))
	@$(CC)$(if $(linkflags), $(linkflags)) -o $$@$(if $(link), $(link))
endef

## Build Object
# =============

# usage: object_perpare <type> <target>
define object_perpare
  $(eval $(call object_$(strip $(1))_perpare, $(2)))
endef
# usage: object_build <type> <target>
define object_build
  $(eval $(call object_$(strip $(1))_build, $(2)))
endef

# =========================================================================== #

## Build
# ======

# usage: perpare_common_variables <type> <target>
define perpare_common_variables
  type := $(if $(strip $(3)),$(strip $(2))-$(strip $(1)),$(strip $(1)))
  target :=  $(if $(strip $(3)),$(strip $(3)),$(strip $(2)))
endef
# usage: perpare <type> <target> <name>
define perpare
  $(eval $(call perpare_common_variables, $(1), $(2), $(3)))
endef
# usage: perpare <type> <target> <name>
define build
  $(if $(findstring $(strip $(1)), object library module), ,
    $(error Not support this type:$(strip $(1))))
  $(eval $(call perpare, $(1), $(2), $(3)))
  $(eval $(call $(strip $(1))_perpare, $(2), $(3)))
  $(eval $(call $(strip $(1))_build,   $(2), $(3)))
endef


# =========================================================================== #


empty :=
space := $(empty) $(empty)

_class_type = $(shell v="$(strip $(1))"; echo $${v##*-})
_class_next = $(shell v="$(strip $(1))"; w=$${v%-*}; test $${v} != $${w} && echo $$w)

define do_build
	@echo BUILD $(1) ...
endef



## executable object
define build_executable_object
	$(warning $(call C_COMP,$(target-objs)))
	$(call C_COMP,$(target-objs))
	$(call C_LINK, $(taregt), $(target-objs))
endef

define _build_object_prepare
	target-type =$(if $(target-class),$(call _class_type, $(target-class)),executable)
	target-class=$(if $(target-class),$(call _class_next, $(target-class)),shared)
endef

##
define build_object
	$(eval $(call _build_object_prepare,$(target)))
	$(if $(value build_$(target-type)_object),,
		$(error Not found build $(target-type)-object))
	$(call build_$(target-type)_object,$(strip $(1)))
endef

## share library
define build_share_library
	$(foreach obj, $(target-objs), $(call C_COMP,$(obj),$(cflags),$(cppflags)))
	echo $(target-objs)
endef
define _build_library_prepare
	target-type =$(if $(target-class),$(call _class_type, $(target-class)),shared)
	target-class=$(if $(target-class),$(call _class_next, $(target-class)),shared)
endef
define build_library
	$(eval $(call _build_library_prepare,$(target)))
	$(if $(value build_$(target-type)_library),,
		$(error Not found build $(target-type)-library))
	$(call build_$(target-type)_library,$(strip $(1)))
endef


## target


