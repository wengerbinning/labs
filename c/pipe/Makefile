include $(TOPDIR)/rule.mk

##
$(eval $(call build, library, share, center))

##
worker-objs := main.o
$(eval $(call build, program, worker))

include $(TOPDIR)/toplevel.mk