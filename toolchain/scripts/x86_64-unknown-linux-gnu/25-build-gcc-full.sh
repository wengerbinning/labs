#!/usr/bin/env bash

##
TOOLCHAIN_HOME="/mnt/work/toolchains"
TOOLCHAIN_NAME="toolchain-x86_64-unknown-linux-gnu"
TOOLCHAIN_PATH="${TOOLCHAIN_HOME}/${TOOLCHAIN_NAME}"
#
ARCH=x86_64
TARGET="x86_64-unknown-linux-gnu"
#
CROSS_PREFIX="x86_64-unknown-linux-gnu-"
#
BUILD_RPATH="build"
SOURCE_PATH=$(pwd -P)
DEST_APATH="$SOURCE_PATH/destdir"
#
PATH="$TOOLCHAIN_PATH/bin${PATH:+:$PATH}"
export PATH

##
test -d $DEST_APATH && rm -rf $DEST_APATH

##
cd $BUILD_RPATH && {
###################

make -j6 all-target-libstdc++-v3

make install-target-libstdc++-v3 DESTDIR=${DEST_APATH}
#
make -j6

#
make install DESTDIR=${DEST_APATH}

###################
	cd - >/dev/null
}





## LINKS
# 1. <https://foss-for-synopsys-dwc-arc-processors.github.io/toolchain/maintainer/manual-build.html>
# 2. <https://gcc.gnu.org/pipermail/gcc/2021-January/234737.html>
# 3. <https://gcc.gnu.org/onlinedocs/cpp/Search-Path.html#Search-Path>
# 4. <https://gcc.gnu.org/install/configure.html>
# 5. <https://gcc.gnu.org/onlinedocs/gcc/Directory-Options.html>