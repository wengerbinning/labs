#!/bin/bash

# MAKEFLAGS="V=0"
MAKEFLAGS+="${MAKEFLAGS:+MAKEFLAGS }--no-print-directory"

test -f configure || ./bootstrap
test -f Makefile  && make distclean
test -f Makefile  || ./configure ${CONFIGURE_FLAGS} --prefix=/usr --disable-silent-rules --with-sysroot=${TOOLCHAIN_BASE_DIR%/}
make ${MAKEFLAGS:+$MAKEFLAGS} -j$(nproc) || { echo "Build Error!"; exit 1; }
make ${MAKEFLAGS:+$MAKEFLAGS} install DESTDIR=${PWD}/dest
