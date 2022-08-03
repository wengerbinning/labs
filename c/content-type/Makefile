
TOPDIR ?= $(CURDIR)

include $(TOPDIR)/rule.mk
# =========================================================================== #

CFLAGS := -std=c99



istream-objs := istream.o hexdump.o

$(eval $(call build, object, executable, istream))

ostream-objs := ostream.o hexdump.o

$(eval $(call build, object, executable, ostream))
# =========================================================================== #
include $(TOPDIR)/toplevel.mk
