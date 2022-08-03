TOPDIR := $(CURDIR)

include $(TOPDIR)/rule.mk
# =========================================================================== #

main-libs := -lcrypto
main-objs := main.o
main-objs += hexdump.o
main-objs += provision.o
$(eval $(call build, program, main))

# =========================================================================== #
include $(TOPDIR)/toplevel.mk
