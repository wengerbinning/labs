
targets := helloworld

helloworld-language=c

# helloworld-type=dede-share-library



Q=@


all:



##

define T_C_COMPILING
$(strip $(1)): %.o: %.c
	$(Q)echo COMP $$@ $$<...
	$(CC)$(if $(2),$(strip $(2))) -o $$@ -c $$^
endef
define T_C_PACK_SHARED
$(1): $(2)
	$(Q)echo LINK $$@ $$^...
	$(Q)$(CC)$(if $(3), $(3)) -shared -o $$@ $$< $(if $(4), $(4))
endef
define T_C_PACK_STATIC
$(1): $(2)
	$(Q)echo LINK $$@ $$<...
	$(Q)$(AR) crv $$@ $$< >/dev/null
endef
define T_C_LINK_RELO
$(1): $(2)
	$(Q)echo LINK $$@ $$^...
	$(Q)$(LD) --no-gc-sections -o $$@ -r $$^
endef
define T_C_LINK_EXEC
$(1): $(2)
	$(Q)echo LINK $$@ $$^...
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
define build_object_prepare
	target-type =$(if $(target-class),$(call _class_type, $(target-class)),executable)
	target-class=$(if $(target-class),$(call _class_next, $(target-class)),shared)
endef
define build_object_variable
	target-type =$(if $(target-class),$(call _class_type, $(target-class)),executable)
	target-class=$(if $(target-class),$(call _class_next, $(target-class)),shared)
endef
define build_relocatable_object
	$(eval $(call T_C_COMPILING,$(target-objs)))
	$(eval $(call T_C_LINK_EXEC, $(target), $(target-objs), $(cflags) $(ldflags)))
endef
define build_executable_object
	$(eval $(call T_C_COMPILING, $(target-objs)))
	$(eval $(call T_C_LINK_EXEC, $(target), $(target-objs), $(cflags) $(ldflags)))
endef
define build_object
	$(eval $(call build_object_prepare, $(target)))
	$(warning $(target-type))
	$(if $(value build_$(target-type)_object),,
		$(error Not found build $(target-type)-object))
	$(call build_$(target-type)_object,$(strip $(1)))
endef


empty :=
space := $(empty) $(empty)

_class_type = $(shell v="$(strip $(1))"; echo $${v##*-})
_class_next = $(shell v="$(strip $(1))"; w=$${v%-*}; test $${v} != $${w} && echo $$w)
##
_target_types = $$($(strip $(1))-type)
_target_detect = $(shell test -d $(strip $(1)) && echo module || echo object)
_target_class = $$(if $(_target_types),$(_target_types),$(_target_detect))

define build_target_prepare
	target=$(strip $(1))
	target-class = $(call _target_class, $(strip $(1)))
endef
define build_target_variable
	target-type = $(call _class_type,$(target-class))
	target-class = $(call _class_next,$(target-class))
	target-cflags = $(if $($(target)-cflags), $($(target)-cflags))
	target-cxxflags = $(if $($(target)-cxxflags), $($(target)-cxxflags))
	target-cppflags = $(if $($(target)-cppflags), $($(target)-cppflags))
	target-ldflags = $(if $($(target)-ldflags), $($(target)-ldflags))
	target-objs = $(if $($(target)-objs),$($(target)-objs),$(target).o)
	target-libs = $(if $($(target)-libs),$($(target)-libs))
endef
define build_target
	$(eval $(call build_target_prepare, $(strip $(1))))
	$(eval $(call build_target_variable, $(strip $(1))))
	$(warning $(target-class))
	$(if $(value build_$(target-type)),,
		$(error Not found build $(target):$(target-type)))
	$(call build_$(target-type),$(strip $(1)))
endef

$(foreach target, $(targets), $(call build_target, $(target)))

all: $(targets)
	echo NOTHING ...

clean:
	-rm -f *.o $(targets)



.PHONY: all