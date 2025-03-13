#!/usr/bin/env bash

error()   { echo -e "\e[31m${@/@COLOR/\\e[0m\\e[31m}\e[0m" >&2; }
warning() { echo -e "\e[33m${@/@COLOR/\\e[0m\\e[33m}\e[0m" >&2; }
notice()  { echo -e "\e[32m${@/@COLOR/\\e[0m\\e[32m}\e[0m" >&1; }
note()    { echo -e "\e[34m${@/@COLOR/\\e[0m\\e[34m}\e[0m" >&1; }
debug()   { echo -e "\e[35m${@/@COLOR/\\e[0m\\e[35m}\e[0m" >&1; }

##
# usage: file_backup <file> <force>
file_backup() {
	local idx name suffix
	local file=${1} force=${2:-false}
	test -n "$file" || return
	# parse file name
	name=${file%.*}; suffix=${file##*.}
	test "$suffix" == "$file" && unset suffix
	# check file name
	while test -f "$name${suffix:+.$suffix}"; do
		name=${file%.*}-${idx:-0}
	idx=$((idx + 1)); done
	name=$name${suffix:+.$suffix}
	# handle file name
	case ${force:-false} in
	true) test -f $file && mv -f $file $name; name=$file ;;
	esac
	echo $name
}

# usage: run_cmd <prog>...
run_cmd() {
	local ret home=${CMD_HOME:-.command}
	local file="${home}/$(date +%s).log"
	local force=${CMD_FORCE:-false}
	if ! type -t error 2>/dev/null 1>&2; then
		error() { echo -e "\e[31m${@/@COLOR@/\\e[0m\\e[31m}" >&2; }
	fi
	if ! type -t warning 2>/dev/null 1>&2; then
		warning() { echo -e "\e[31m${@/@COLOR@/\\e[0m\\e[31m}" >&2; }
	fi
	if ! type -t notice 2>/dev/null 1>&2; then
		notice() { echo -e "\e[31m${@/@COLOR@/\\e[0m\\e[31m}" >&2; }
	fi
	if ${force:-false}; then test -d "$home" && rm -rf $home; fi
	test -d "${home}" || mkdir -p "${home}"
	file=$(file_backup $file true)
	echo "[$(date +"%Y/%m/%d %H:%M:%S %z")] ${PWD} # $@" > $file
	case ${CMD_VERBOSE:-false} in
	true)
		eval "$@";         ret=$? ;;
	*)
		eval "$@ >>$file"; ret=$?
	esac
	test ${ret:-1} -ne 0 && {
		error "running command error ($ret): \e[34m$@ @COLOR@!"
		case ${CMD_EXIT:-true} in
		false) return $ret ;;
		*)       exit $ret
		esac
	}
}

###############################################################################

##
ARCH=x86_64
TARGET="x86_64-unknown-linux-gnu"
CROSS_PREFIX="x86_64-unknown-linux-gnu-"

##
TOOLCHAIN_HOME="/opt/toolchains"
TOOLCHAIN_NAME="toolchain-${TARGET}"
TOOLCHAIN_PATH="${TOOLCHAIN_HOME}/${TOOLCHAIN_NAME}"

##
PATH="$TOOLCHAIN_PATH/bin${PATH:+:$PATH}"
export PATH

##
WORKPATH=".build"
SRC_PATH=$PWD
DST_PATH=${PWD}/dest
#
test -d "$DST_PATH" && mv -f $DST_PATH ${DST_PATH}.old
test -d "$WORKPATH" || mkdir -p $WORKPATH
cd $WORKPATH && {
    notice "Start build project ..."
###################

# all-target-libstdc++-v3
make -j6 all-target-libgcc

# install-target-libstdc++-v3
make install-target-libgcc  DESTDIR=${DEST_PATH}

###################
	cd - </dev/null
	notice "Project build successful!"
}
