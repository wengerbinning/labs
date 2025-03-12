
# error() {
# 	# test -n "$1" || return
# 	echo $@
# 	return
# }

if ! type -t error 2>/dev/null 1>&2; then
	error() { echo -e $@; }
fi


error demo