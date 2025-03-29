


# install_file <srcdir> <dstdir> <mode> <file>
install_file() {
	local path file
	local srcdir=$1 dstdir=$2 mode=$3; shift 3
	test -d ${srcdir} || return -1
	for file in "$@"; do
		path=$(dirname $file)
		test -f $srcdir/$file || return -1
		#
		install -m 0755 -d $dstdir/$path
		install -m ${mode} -t $dstdir/$path $srcdir/$file
		#
	done
}

# install_file <srcdir> <dstdir> <mode> <path> <pattern>
install_dirs() {
	local srcdir=$1 dstdir=$2 mode=$3; path=$4; shift 4
	test -d ${srcdir} || return -1
	install -m 0755 -d $dstdir/$path
	for pattern in "$@"; do
		for file in $(ls ${srcdir}/${path}/$pattern 2>/dev/null); do
			install -m ${mode:-0644} -t $dstdir/$path $file
		done
	done
}

# install_file <srcdir> <dstdir> <file>
install_exec() {
	local path file
	local srcdir=$1 dstdir=$2; shift 2
	test -d ${srcdir} || return -1
	for file in "$@"; do
		path=$(dirname $file)
		test -f $srcdir/$file || return -1
		#
		install -m 0755 -d $dstdir/$path
		install -m 0775 -t $dstdir/$path $srcdir/$file
		case $file in
		*.sh|*.js) continue ;;
		*) ${STRIP:-strip} $dstdir/$file
		esac
	done
}