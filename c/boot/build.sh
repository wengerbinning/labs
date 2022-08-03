#!/bin/bash

#
CROSS_PREFIX=aarch64-linux-gnu-
QEMU_BARE="qemu-aarch64"
QEMU_SYSTEM="qemu-system-aarch64"
AS=${CROSS_PREFIX}as
LD=${CROSS_PREFIX}ld

$AS -o start.o start_aarch64.S
$LD -o start start.o
$QEMU_BARE ./start

$AS -o kernel.o kernel.S
$LD -o kernel.elf -T kernel.ld kernel.o
$QEMU_SYSTEM -machine virt -cpu cortex-a53 -nographic -kernel kernel.elf



exit

#
unset CROSS_PREFIX
# CROSS_PREFIX="x86_64-linux-gnu-"
QEMU_BARE="qemu-x86_64"
AS=${CROSS_PREFIX}as
LD=${CROSS_PREFIX}ld

$AS --64 -o start.o start_x86_64.S
$LD -o start start.o
$QEMU_BARE ./start
