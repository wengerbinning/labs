#!/bin/bash


CROSS_PREFIX=aarch64-linux-gnu-
QEMU_BARE="qemu-aarch64"
AS=${CROSS_PREFIX}as
LD=${CROSS_PREFIX}ld

$AS -o start.o start_aarch64.S
$LD -o start start.o
$QEMU_BARE ./start


#
unset CROSS_PREFIX
# CROSS_PREFIX="x86_64-linux-gnu-"
QEMU_BARE="qemu-x86_64"
AS=${CROSS_PREFIX}as
LD=${CROSS_PREFIX}ld

$AS --64 -o start.o start_x86_64.S
$LD -o start start.o
$QEMU_BARE ./start
