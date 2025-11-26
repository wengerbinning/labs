#!/usr/bin/env bash


dst="dst"

#
test -d $dst || mkdir $dst

#
set -x
gcc -Iinclude -o $dst/main main.c
