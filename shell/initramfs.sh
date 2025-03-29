#!/usr/bin/env bash



# ============================================================================ #

# usage: check_default_dirs
check_default_dirs() {
    local dirs_1="lib bin sbin etc"
    local dirs_2="sys dev proc"
	# local dirs_3="var run tmp"
	# local dirs_4="usr opt root home"
    # local dirs_3=" etc/config etc/hotplug.d"
    # local dirs_4="srv db release work"

    local dirs="$dirs_1${dirs_2:+ $dirs_2}${dirs_3:+ $dirs_3}"

    for dir in $dirs; do test -d $dir || mkdir -p $dir;  done
}

# ============================================================================ #

# usage: initramfs_init <rootfs>
initramfs_init () {
	local rootfs=${1:-initramfs}
	test -d $rootfs || mkdir -p $rootfs
	cd $rootfs && {
		check_default_dirs

	cd - >/dev/null; }
}

# usage: initramfs_build <rootfs> <image>
initramfs_build () {
	local rootfs=${1:-initramfs}
	local image=${2:-initramfs.img}
	test -d $rootfs && cd $rootfs && {
		find ./ -path ./.git -prune -o -print0 | cpio --create --null --verbose --format=newc > ../$image
	cd - >/dev/null; }
}


cmd=$1; shift
rootfs=${1:-initramfs}; shift

case $cmd in
init)
	initramfs_init $rootfs
	;;
build)
	initramfs_build $rootfs $@
	;;
esac



