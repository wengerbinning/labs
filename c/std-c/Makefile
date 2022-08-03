
TOPDIR ?= $(CURDIR)

include $(TOPDIR)/rule.mk
# =========================================================================== #

mdirs := C99
$(eval $(foreach mod,$(mdirs),$(call build, module, $(mod))))

# =========================================================================== #
include $(TOPDIR)/toplevel.mk
