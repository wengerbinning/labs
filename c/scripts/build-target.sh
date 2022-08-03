#!/usr/bin/env bash

MOD_PATH=0
MOD_MAX_PATH=4

SCRIPT_HOME=$(dirname ${BASH_SOURCE})
TARGET_AWK=$SCRIPT_HOME/target.awk

target_awk() {
	local target=$1; shift
	for path in $@; do path=${path%/}
		test -e $path/.target || continue
		awk -v home=$path -v target=$target -f $TARGET_AWK $path/.target
	done
}

# usage: target_get <target> <dirs>...
target_get() {
	local target=$1; shift
	local targets
	MOD_PATH=$((MOD_PATH + 1))
	if [ $MOD_PATH -gt $MOD_MAX_PATH ]; then return; fi
	mods=$(target_awk mods $@)
	# echo "$MOD_PATH {$@} @" $mods >> result
	if [ -n "$mods" ]; then
		targets=${targets:+$targets }$(target_get $target $mods)
	fi
	targets=${targets:+$targets }$(target_awk $target $@)
	MOD_PATH=$((MOD_PATH - 1))
	echo $targets
}

target_get "$@"