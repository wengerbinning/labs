
TOPDIR ?= $(CURDIR)




# CROSS_PREFIX="i686-w64-mingw32-"

# CC=${CROSS_PREFIX}gcc
# CFLAGS="-O3 -g -Wno-addttributes"

# export CC

include $(TOPDIR)/rule.mk
# =========================================================================== #

CFLAGS := -std=c99

mdirs :=  fcgi
# $(eval $(foreach mod,$(mdirs),$(call build, module, $(mod))))


$(eval $(call build, object, executable, helloworld))

# =========================================================================== #
include $(TOPDIR)/toplevel.mk
