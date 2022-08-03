
TOPDIR ?= $(CURDIR)

include $(TOPDIR)/rule.mk
# =========================================================================== #

# mdirs := std-libc version
# $(eval $(foreach mod,$(mdirs),$(call build, module, $(mod))))


$(eval $(call build, object, executable, main))

# =========================================================================== #
include $(TOPDIR)/toplevel.mk
