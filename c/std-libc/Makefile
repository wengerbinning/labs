
TOPDIR ?= $(CURDIR)

include $(TOPDIR)/rule.mk
# =========================================================================== #

mdirs :=  hexdump string stream
$(eval $(foreach mod,$(mdirs),$(call build, module, $(mod))))

# =========================================================================== #
include $(TOPDIR)/toplevel.mk
