# usage: split [n]
split() {
    tr -s ' ' '\30' |
        dd ibs=${1:-1} cbs=${1:-1} obs=2 conv=unblock 2>/dev/null |
            tr -s '\30' ' '
}



test() {
    echo -n "$s" | split | xxd
}
