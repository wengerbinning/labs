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

# all-target-libstdc++-v3
make -j6 all-target-libgcc

# install-target-libstdc++-v3
make install-target-libgcc  DESTDIR=${DEST_APATH}

###################
	cd - >/dev/null
}