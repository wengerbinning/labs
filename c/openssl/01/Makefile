TOPDIR := $(CURDIR)

include $(TOPDIR)/rule.mk
# =========================================================================== #

signsha1-objs := sign_sha1.o
signsha1-objs += hexdump.o
signsha1-libs := -lcrypto

$(eval $(call build, program, signsha1))

signsha256-objs := sign_sha256.o
signsha256-objs += hexdump.o
signsha256-libs := -lcrypto

$(eval $(call build, program, signsha256))

verifysha1-objs := verify_sha1.o
verifysha1-objs += hexdump.o
verifysha1-libs := -lcrypto

$(eval $(call build, program, verifysha1))

verifysha256-objs := verify_sha256.o
verifysha256-objs += hexdump.o
verifysha256-libs := -lcrypto

$(eval $(call build, program, verifysha256))

# =========================================================================== #
include $(TOPDIR)/toplevel.mk
