
# CC AR LD OBJDUMP READELF
# RM MKDIR INSTALL
# MAKE
# LDCOFNIG

# TOOLCHAIN
# CROSS_PREFIX


ifneq ($(TOOLCHAIN),)
	TOOLCHAIN_NAME := $(TOOLCHAIN_NAME)
	CROSS_PREFIX :=
	ENV1 := TOOLCHAIN_NAME
endif

ifneq ($(CROSS_PREFIX),)
	ENV1 += CROSS_PREFIX
	executable-object-suffix := $(if $(findstring mingw, $(CROSS_PREFIX)),.exe)
endif

CC := $(CROSS_PREFIX)gcc
LD := $(CROSS_PREFIX)gcc
AR := $(CROSS_PREFIX)ar
ENV2 := CC LD AR
# export CC LD AR



ifeq ($(shell  echo '\n' | wc -l),2)
  ECHO0 := echo -E
  ECHO1 := echo
  ECHO := echo
else
  ECHO0 := echo
  ECHO1 := echo -e
  ECHO := echo -e
endif

ECHO3 := $(ECHO) -n




