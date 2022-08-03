
define env_save
	echo "$(strip $(1))=\"$($(strip $(1)))\"" \
		>> $(if $(strip $(2)), $(strip $(2)),.environment)
endef
define env_export
	echo "export $(strip $(1))" \
		>> $(if $(2), $(2),.environment)
endef

define common_environment
	echo -e "###\n### Generate by build framework, don't change! \n###" \
		> $(if $(strip $(1)), $(strip $(1)),.environment)
	echo >> $(if $(strip $(1)), $(strip $(1)),.environment)
endef
define project_environment
	$(if  $(VAR0)$(ENV0), \
		echo "# Project Variables & Environments" \
		>> $(if $(1),$(1),.environment))
	$(if $(PROJECT_NAME), $(call env_save, PROJECT_NAME, $(1)))
	$(if $(PROJECT_HOME), $(call env_save, PROJECT_HOME, $(1)))
	$(if $(PROJECT_LANGUAGES), $(call env_save, PROJECT_LANGUAGES, $(1)))
	$(if $(ENV0),  $(call env_export, $(ENV0), $(1)))
endef
define toolchain_environment
	$(if $(VAR1)$(ENV1), \
		echo "# Toolchain Variables & Environments" \
		>> $(if $(1), $(1),.environment))
	$(if $(TOOLCHAIN), $(call env_save, TOOLCHAIN, $(1)))
	$(if $(TOOLCHAIN_NAME), $(call env_save, TOOLCHAIN_NAME, $(1)))
	$(if $(TOOLCHAIN_HOME), $(call env_save, TOOLCHAIN_HOME, $(1)))
	$(if $(TOOLCHAIN_PATH), $(call env_save, TOOLCHAIN_PATH, $(1)))
	$(if $(ARCH), $(call env_save, ARCH, $(1)))
	$(if $(TARGET),  $(call env_save, TARGET, $(1)))
	$(if $(CROSS_PREFIX), $(call env_save, CROSS_PREFIX, $(1)))
	$(if $(ENV1),  $(call env_export, $(ENV1), $(1)))
endef
define build_environment
	$(if $(VAR2)$(ENV2), \
		echo "# Build Common Environment" \
		>> $(if $(1), $(1),.environment))
	$(if $(CC), $(call env_save, CC, $(1)))
	$(if $(LD), $(call env_save, LD, $(1)))
	$(if $(AR), $(call env_save, AR, $(1)))
	$(if $(ENV2),  $(call env_export, $(ENV2), $(1)))
	$(if $(executable-object-suffix), \
		$(call env_save, executable-object-suffix, $(1)))
endef

define environment
	$(call common_environment,    $(1))
	$(call project_environment,   $(1))
	$(call toolchain_environment, $(1))
	$(call build_environment,     $(1))
endef