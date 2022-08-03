
TOPDIR ?= $(CURDIR)

include $(TOPDIR)/rule.mk
# =========================================================================== #

istream-objs := main.o
istream-objs += hexdump.o

$(eval $(call build, object, executable, istream))

ostream-objs := main.o
ostream-objs += hexdump.o


$(eval $(call build, object, executable, ostream))


# =========================================================================== #
include $(TOPDIR)/toplevel.mk
