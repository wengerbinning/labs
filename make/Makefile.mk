
targets := helloworld

helloworld-language=c

# helloworld-type=dede-share-library



# Q=@


all:



## C Programming Languages
# usage: T_C_COMPILING <object...> [flags]
define T_C_COMPILING
$(strip $(1)): %.o: %.c
	$(Q)echo COMP $$@ $$< ...
	$(Q)$(CC)$(if $(2),$(strip $(2))) -o $$@ -c $$^
endef
# usage: T_C_PACK_SHARED <target> <object...> [flags] [link]
define T_C_PACK_SHARED
$(1): $(2)
	$(Q)echo LINK $$@ $$^...
	$(Q)$(CC)$(if $(3), $(3)) -shared -o $$@ $$< $(if $(4), $(4))
endef
# usage: T_C_PACK_STATIC <target> <objet...>
define T_C_PACK_STATIC
$(1): $(2)
	$(Q)echo LINK $$@ $$<...
	$(Q)$(AR) crv $$@ $$< >/dev/null
endef
# usage: T_C_LINK_RELO <target> <object...>
define T_C_LINK_RELO
$(1): $(2)
	$(Q)echo LINK $$@ $$^...
	$(Q)$(LD) --no-gc-sections -o $$@ -r $$^
endef
# usage: T_C_LINK_EXEC <target> <objtect...> [flags] [link]
define T_C_LINK_EXEC
$(1): $(2)
	$(Q)echo LINK $$@ $$^ ...
	$(LD)$(if $(3), $(3)) -o $$@ $$< $(if $(4), $(4))
endef


# Library
define build_library_prepare
	target-type =$(if $(target-class),$(call _class_type, $(target-class)),shared)
	target-class=$(if $(target-class),$(call _class_next, $(target-class)),shared)
endef
define build_static_library
 	$(eval $(call T_C_COMPILING, $(target-objs)), $(cflags) $(cppflags))
	$(eval $(call T_C_LINK_EXEC, $(target), $(target-objs), $(cflags) $(ldflags)))
endef
define build_shared_library
	$(eval $(call T_C_COMPILING, $(target-objs)), $(cflags) $(cppflags))
	$(eval $(call T_C_LINK_EXEC, $(target), $(target-objs), $(cflags) $(ldflags)))
endef
define build_library
	$(eval $(call build_library_prepare,$(target)))
	$(if $(value build_$(target-type)_library),,
		$(error Not found build $(target-type)-library))
	$(call build_$(target-type)_library,$(strip $(1)))
endef

# Object
define c_object_prepare
	target-type=$(if $(target-class),$(call _class_type, $(target-class)),executable)
	target-class=$(if $(target-class),$(call _class_next, $(target-class)),shared)
endef
define c_object_variable
	flags=$(if $(target-cflags), $(target-cflags))
endef
define c_relocatable_object_build
	$(eval $(call T_C_COMPILING,$(target-objs), ))
	$(eval $(call T_C_LINK_EXEC, $(target), $(target-objs), $(cflags) $(ldflags)))
endef
define c_executable_object_build
	$(eval $(call T_C_COMPILING, $(target-objs), $(if $(flags),$(flags))))
	$(eval $(call T_C_LINK_EXEC, $(target), $(target-objs), $(cflags) $(ldflags)))
endef
# usage: c_object_build <target>
define c_object_build
	$(eval $(call c_object_prepare, $(target)))
	$(eval $(call c_object_variable, $(target)))
	$(if $(value c_$(target-type)_object_build),,
		$(error Not found build $(target-type)-object))
	$(call c_$(target-type)_object_build,$(strip $(1)))
endef




###
empty :=
space := $(empty) $(empty)
#
_class_type = $(shell v="$(strip $(1))"; echo $${v##*-})
_class_next = $(shell v="$(strip $(1))"; w=$${v%-*}; test $${v} != $${w} && echo $$w)
#
_target_types = $$($(strip $(1))-type)
_target_detect = $(shell test -d $(strip $(1)) && echo module || echo object)
_target_class = $$(if $(_target_types),$(_target_types),$(_target_detect))

### target
# usage: target_prepare <target>
define target_prepare
	target=$(strip $(1))
	target-class = $(call _target_class, $(strip $(1)))
	target-language = $(if $($(strip $(1))-language), c)
endef
# usage: target_variable <target>
define target_variable
	target-type = $(call _class_type,$(target-class))
	target-class = $(call _class_next,$(target-class))

	target-cflags := $(if $(CFLAGS), $(CFLAGS))
	target-cflags += $(if $($(target)-cflags), $($(target)-cflags))
	target-cxxflags := $(if $(CXXFLAGS), $(CXXFLAGS))
	target-cxxflags += $(if $($(target)-cxxflags), $($(target)-cxxflags))
	target-cppflags := $(if $(CPPFLAGS), $(CPPFLAGS))
	target-cppflags += $(if $($(target)-cppflags), $($(target)-cppflags))
	target-ldflags := $(if $(LDFLAGS), $(LDFLAGS))
	target-ldflags += $(if $($(target)-ldflags), $($(target)-ldflags))
	target-libs := $(if $(LIBS),$(LIBS))
	target-libs += $(if $($(target)-libs),$($(target)-libs))
	target-objs = $(if $($(target)-objs),$($(target)-objs),$(target).o)
endef
# usage: target_build <target>
define target_build
	$(eval $(call target_prepare, $(strip $(1))))
	$(eval $(call target_variable, $(strip $(1))))
	$(if $(value $(target-language)_$(target-type)_build),,
		$(error Not found build $(target):$(target-type)))
	$(call $(target-language)_$(target-type)_build, $(strip $(1)))
endef
# foreach all targets
$(foreach target, $(targets), $(call target_build, $(target)))

all: $(targets)
	@echo NOTHING ...

clean:
	-rm -f *.o $(targets)

.PHONY: all


# target target-language target-class target-type
# target-cflags target-cxxflags target-cppflags target-ldflags