
targets := helloworld

helloworld-language=c

# helloworld-type=dede-share-library



Q=@




# build_object = echo $1 $(type)

# define build_library
# 	$(Q)case $(if $($(target)-type),$($(target)-type),shared-library) in \
# 		*shared-library) $(call build_library_target, $(target)) ;; \
# 		*static-library) $(call build_library_target, $(target)) ;; \
# 		esac
# endef

# .SECONDEXPANSION:
# define build_target
# 	$(Q)target=$(strip $(1))
# 	$(Q)case $(if $($(target)-type),$($(target)-type),executable-object) in \
# 		*include)   echo $(type)   ;; \
# 		*object) 	$(call build_object,  $(target))   ;; \
# 		*library)   $(call build_library, $(target))   ;; \
# 		*license)	echo $($(target)-type)   ;; \
# 		*document)  echo $($(target)-type)   ;; \
# 		*config)    echo $($(target)-type)   ;; \
# 		*data)      echo $($(target)-type)   ;; \
# 		*script)	echo $($(target)-type)   ;; \
# 		*)			echo $(target) ; \
# 		esac
# endef




empty :=
space := $(empty) $(empty)

_class_type = $(shell v="$(strip $(1))"; echo $${v##*-})
_class_next = $(shell v="$(strip $(1))"; w=$${v%-*}; test $${v} != $${w} && echo $$w)

C_COMP=$(CC)$(if $(2), $(2)) -o $(1) -c $(patsubst %.o,%.c,$(1))
C_LINK=$(LD)$(if $(3), $(3)) -o $(1) $(2)



define do_build
	@echo BUILD $(1) ...
endef



$(target): $(target-deps) $(target-objs)
	$(call do_build, $(target))




## executable object
define build_executable_object
	$(call file_newer, helloworld.o, helloworld.c)
	$(foreach each, $(target-objs), $(call C_COMP,$(each),$(option)))
	$(call C_LINK,$(target),$(target-objs),$(option))
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

## library
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
_target_class = $$(if $($(strip $(1))-type),$($(strip $(1))-type),object)
define _build_target_prepare1
	target=$(strip $(1))
	target-class=$(call _target_class,$(1))
endef
define _build_target_prepare2
	target-type =$(if $(target-class),$(call _class_type, $(target-class)),object)
	target-class=$(if $(target-class),$(call _class_next, $(target-class)))
endef
define _build_target_variable
	target-cflags=$(if $($(target)-cflags), $($(target)-cflags))
	target-cxxflags=$(if $($(target)-cxxflags), $($(target)-cxxflags))
	target-cppflags=$(if $($(target)-cppflags), $($(target)-cppflags))
	target-ldflags=$(if $($(target)-ldflags), $($(target)-ldflags))
	target-objs=$(if $($(target)-objs),$($(target)-objs),$(target).o)
	target-libs=$(if $($(target)-libs),$($(target)-libs))
endef
define build_target
	$(eval $(call _build_target_prepare1, $(strip $(1))))
	$(eval $(call _build_target_prepare2, $(strip $(1))))
	$(eval $(call _build_target_variable, $(strip $(1))))
	$(if $(value build_$(target-type)),,
		$(error Not found build $(target-type)))
	$(call build_$(target-type),$(strip $(1)))

endef


all:
	$(foreach target, $(targets), $(call build_target, $(target)))



.PHONY: all