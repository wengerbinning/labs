# TARGET_TYPE=executable-object
# FORCE


# =========================================================================== #
# ------------------------------------- #
## target variables & rule for library ##
# ------------------------------------- #

# target variables
library-class := share static
# target rules
define build_static_library_compile
	$(CC)$(if $(compflags), $(compflags)) $$<
endef
define build_static_library_link
	$(AR) crv $$@ $$^ >/dev/null
endef
define build_share_library_compile
	$(CC) -c$(if $(compflags), $(compflags)) $$<
endef
define build_share_library_link
	$(LD)$(if $(linkflags), $(linkflags)) -shared -o $$@ $$^$(if $(libs), $(libs))
endef

# ------------------------------------- #
## target variables & rules for object ##
# ------------------------------------- #

define run_cmd
	@echo "$1" > $(if $2,$2,.$(target).cmd)
	$(Q)$(1)
endef
# target variables
object-class := executable relocatable
# target rules
define build_relocatable_object_compile
	$(if $(Q),$(call rule_log, CC, $$@))
	$(Q)$(CC)$(if $(compflags), $(compflags)) $$<
endef
define build_relocatable_object_link
	$(if $(Q),$(call rule_log, LD, $$@))
	$(Q)$(LD) --no-gc-sections -o $$@ -r $$^
endef
define build_executable_object_compile
	$(if $(Q),$(call rule_log, CC, $$@))
	$(Q)$(CC)$(if $(compflags), $(compflags)) -o $$@ $$<
	$(call run_cmd, $(CC)$(if $(compflags), $(compflags)) -o $$@ $$<, .$$@.cmd)
endef
define build_executable_object_link
	$(if $(Q),$(call rule_log, LD,$$(if $$(findstring $$@,$(target)),$(target),$(target) ($$@))))
	$(call run_cmd, $(LD)$(if $(linkflags), $(linkflags)) -o $$@ $(if $(link),$(link)), .$(target).cmd)
endef

# =========================================================================== #

target-class := object library script file module project
## build prepare
# usage: build_target_common_variables <target>
define build_target_common0_variables
	target := $(strip $(1))
	target_name := $(strip $(1))
	target_type := $(if $($(target)-type), $($(target)-type), $(TARGET_TYPE))
	target_objs := $(if $($(target)-objs), $($(target)-objs), $(target).o)
	target_class := $(target-class)
	target_class += $(if $(TARGET_CLASS), $(TARGET_CLASS))
	target_force := $(if $(FORCE), FORCE)
endef
define build_target_common1_variables
	type1 := $(lastword $(subst -, ,$(target_type)))
	types := $(subst -, ,$(target_type))
endef
define build_target_common2_variables
	type1 := $(if $(type1), $(type1), $(target-default-type))
	type2 := $(if $(types), $(lastword $(filter-out $(type1),$(types))))
	types := $(if $(types),$(filter-out $(type1),$(types)))
endef
define build_target_common3_variables
	type1 := $(if $(type1), $(type1), $(firstword $(target_class)))
	type2 :=$(if $(type2), $(type2),$($(type1)-default-class))
	types := (if $(types),$(filter-out $(type2),$(types)))
endef
define build_target_common4_variables
	type2 :=$(if $(type2), $(type2), $(firstword $($(type1)-class)))
endef
define build_target_common5_variables
	target_type := $(if $(target_type), $(target_type), $(type2)-$(type1))
	target_prefix := $($(type2)-$(type1)-prefix)
	target_suffix := $($(type2)-$(type1)-suffix)
endef

# usage: build_target_variables <target type>
define build_target1_variables
	cflags := $(if $(CFLAGS), $(CFLAGS))
	cflags := $(if $($(target)-cflags), $($(target)-cflags))
	ldflags := -L.
	ldflags += $(if $($(target)-ldflags), $($(target)-ldflags))
	cppflags := -Iinclude
	cppflags += $(if $(CPPFLAGS), $(CPPFLAGS))
	cppflags += $(if $($(target)-cppflags), $($(target)-cppflags))
	objs := $(if $(target_objs), $(target_objs))
	libs := $(if $(LIBS), $(LIBS))
	libs += $(if $($(strip $(1))-libs), $($(strip $(1))-libs))
	libs += $(if $($(target)-libs), $($(target)-libs))
endef
define build_target2_variables
	compflags := -c
	compflags += $(if $(cflags), $(cflags))$(if $(cppflags), $(cppflags))
	linkflags := $(if $(cflags), $(cflags))$(if $(ldflags), $(ldflags))
	link := $(objs)
	link += $(if $(libs), $(libs))
endef
# usage: build_target_prepare <target>
define build_target_prepare
	$(eval $(call build_target_common0_variables, $(1)))
	$(eval $(call build_target_common1_variables, $(1)))
	$(eval $(call build_target_common2_variables, $(1)))
	$(eval $(call build_target_common3_variables, $(1)))
	$(eval $(call build_target_common4_variables, $(1)))
	$(eval $(call build_target_common5_variables, $(1)))
	$(eval $(call build_target1_variables, $(type1)))
	$(eval $(call build_target2_variables, $(type1)))
	IGNORES += $(if $(target_suffix), *$(target_suffix))
	$(shell $(ECHO) "$(target) # $(target_type)" >> .targets)
endef

## build doing
# usage: build_target_doing <target>
define build_target_doing

$(target_objs): %.o: %.c $(target_force)
	$(if $(findstring $(type1), $(target_class)),
	$(if $(findstring $(type2), $($(type1)-class)),
		$(call build_$(type2)_$(type1)_compile, $(target)),
	$(call make_log, error, "Not support target ($(target)) type $(type2)-$(type1) !")),
	$(call make_log, error, "Not support target ($(target)) type $(type1) !"))
$(target_prefix)$(target)$(target_suffix): $(target_objs) $(target_deps)
	$(if $(findstring $(type1), $(target_class)),
	$(if $(findstring $(type2), $($(type1)-class)),
		$(call build_$(type2)_$(type1)_link, $(target)),
	$(call make_log, error, "Not support target ($(target)) type $(type2)-$(type1) !")),
	$(call make_log, error, "Not support target ($(target)) type $(type1) !"))
	@echo "$$@ # $(target_type)" > .$(target).target
ifneq ($(target),$(target_prefix)$(target)$(target_suffix))
PHONY += $(target)
$(target):$(target_prefix)$(target)$(target_suffix)
endif
endef

## build finish
# usage: build_ctarget_finish <target>
target_clean_files=$$(shell cut -d '\#' -f1 .$(target).target 2>/dev/null | xargs)
define build_target_finish
PHONY += $(target)-clean
$(target)-clean:
	$(if $(shell test -f .$(target).target && echo .$(target).target), \
		rm -rf $(shell awk '{printf "%s ", $$1}' .$(target).target))
endef

## build entry
# usage: build <target>
define build_target
	$(eval $(call build_target_prepare,   $(1)))
	$(eval $(call build_target_doing,     $(1)))
	$(eval $(call build_target_finish,    $(1)))
endef