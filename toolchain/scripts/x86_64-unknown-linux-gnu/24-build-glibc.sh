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
CC=${CROSS_PREFIX}gcc
CFLAGS="-O3 -g -Wno-attributes"
export PATH CC

##
test -d $DEST_APATH && rm -rf $DEST_APATH

##
cd $BUILD_RPATH && {
###################

#
make -j6

#
make install DESTDIR=$DEST_APATH


###################
	cd - >/dev/null
}