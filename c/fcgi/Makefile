
TOPDIR ?= $(CURDIR)

include $(TOPDIR)/rule.mk
# =========================================================================== #

CFLAGS := -std=c99

iostream-objs := main.o hexdump.o
iostream-libs := -lfcgi -lssl -lcrypto
$(eval $(call build, object, executable, iostream))

# request-objs := socket.o hexdump.o
# $(eval $(call build, object, executable, request))


# sslreq-objs := socket_ssl.o hexdump.o
# sslreq-libs := -lssl -lcrypto
# $(eval $(call build, object, executable, sslreq))
# =========================================================================== #
include $(TOPDIR)/toplevel.mk