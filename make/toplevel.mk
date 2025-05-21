
all:

###
_class_type = $(shell v="$(strip $(1))"; echo $${v##*-})
_class_next = $(shell v="$(strip $(1))"; w=$${v%-*}; \
	test $${v} != $${w} && echo $$w)

Q=@
DEPS = FORCE

define log
	@echo -en "\e[31m$(strip $(1)) \e[0m"
	@echo -en "$(strip $(2))"
	@echo -en "\n"
endef

### exectable
define executable_prepare_variable
	cflags := $(CFLAGS)
	cflags += $(if $(target-cflags),$(target-cflags))
	cppflags := $(CPPFLAGS)
	cppflags += $(if $(target-cppflags),$(target-cppflags))
	ldflags := $(LDFLAGS)
	ldflags += $(if $(target-ldflags),$(target-ldflags))
	objs := $(OBJS)
	objs += $(if $(target-objs), $(target-objs))
	libs := $(LIBS)
	libs += $(if $(target-libs), $(target-libs))
	deps := $(DEPS)
	deps += $(if $(target-deps), $(target-deps))
endef
define executable_prepare_option
	compflags := $(if $(cflags), $(cflags))
	compflags += $(if $(cppflags), $(cppflags))
	linkflags := $(if $(cflags), $(cflags))
	linkflags += $(if $(ldflags), $(ldflags))
endef
define executable_prepare_rule
$(target-objs): %.o: %.c
	$$(call log, CC, $$@)
	$(Q)$(CC) -c $(if $(compflags), $(compflags))-o $$@ $$^
$(target): $(objs) $(deps)
	$$(call log, LD, $$@)
	$(Q)$(CC)$(if $(linkflags), $(linkflags)) -o $$@ $(objs) $(libs)
endef
define executable_prepare
	$(eval $(call executable_prepare_variable, $(strip $(1))))
	$(eval $(call executable_prepare_option, $(strip $(1))))
	$(eval $(call executable_prepare_rule, $(strip $(1))))
endef

### object
define object_prepare_common
	target-scope = object$(if $(target-scope),-$(target-scope))
	target-class = $(if $(target-class),$(target-class),executable)
endef
define object_prepare_variable
	target-type = $(call _class_type,$(target-class))
	target-class = $(call _class_next,$(target-class))
endef
define object_prepare
	$(eval $(call object_prepare_common, $(strip $(1))))
	$(eval $(call object_prepare_variable, $(strip $(1))))
	$(if $(value $(target-type)_prepare),
		$(call $(target-type)_prepare, $(strip $(1))),
		$(error Not found build $(target-type)-$(target-scope):$(target)))
endef

### target
_target_types  = $$($(strip $(1))-type)
_target_detect = $(shell test -d $(strip $(1)) && echo module || echo object)
_target_class  = $$(if $(_target_types),$(_target_types),$(_target_detect))
define target_prepare_common
	target=$(strip $(1))
	target-class = $(call _target_class, $(strip $(1)))
endef
define target_prepare_variable
	target-type = $(call _class_type,$(target-class))
	target-class = $(call _class_next,$(target-class))
	target-cflags = $(if $($(target)-cflags), $($(target)-cflags))
	target-cxxflags = $(if $($(target)-cxxflags), $($(target)-cxxflags))
	target-cppflags = $(if $($(target)-cppflags), $($(target)-cppflags))
	target-ldflags = $(if $($(target)-ldflags), $($(target)-ldflags))
	target-objs = $(if $($(target)-objs),$($(target)-objs),$(target).o)
	target-libs = $(if $($(target)-libs),$($(target)-libs))
endef
define target_prepare
	$(eval $(call target_prepare_common, $(strip $(1))))
	$(eval $(call target_prepare_variable, $(strip $(1))))
	$(if $(value $(target-type)_prepare),
		$(call $(target-type)_prepare, $(strip $(1))),
		$(error Not found build $(target-type):$(target)))
endef


$(foreach target, $(targets), $(call target_prepare,$(target)))

all: $(targets)

clean:
	-rm -rf *.o

FORCE:
.PHONY: all install clean distclean