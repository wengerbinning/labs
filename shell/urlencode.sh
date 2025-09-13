#!/bin/sh
# This is library

# Printable Characters (95)
s=${s:+$s}" "                           #  1
s=${s:+$s}"!@#$%^&*()"                  # 10
s=${s:+$s}"1234567890"                  # 10
s=${s:+$s}"\`-=[]\;',./"                # 11
s=${s:+$s}"~_+{}|:\"<>?"                # 11
s=${s:+$s}"abcdefghijklmnopqrstuvwxyz"  # 26
s=${s:+$s}"ABCDEFGHIJKLMNOPQRSTUVWSYZ"  # 26

# URL Encode Table:
# -.~_
# -.~_
# 1234567890
# 1234567890
# abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWSYZ
# abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWSYZ
#       !   @   #   $   %   ^   &   *   (   )
# %20 %21 %40 %23 %24 %25 %5E %26 %2A %28 %29
#   `  =   [   ]   \   ;   '   ,   /   +   {   }   |   :   "   <   >   ?
# %60 %3D %5B %5D %5C %3B %27 %2C %2F %2B %7B %7D %7C %3A %22 %3C %3E %3F


# usage: split [n]
split() {
    tr -s ' ' '\30' |
        dd ibs=${1:-1} cbs=${1:-1} obs=2 conv=unblock 2>/dev/null |
            tr -s '\30' ' '
}
# usage: url_decode_awk
url_decode_awk() {
    local awkfile=".$(date +%s).awk~"
cat >$awkfile << 'AWKEOF'
/[0-9a-zA-Z\-\.~_]/ { printf "%s", $1; next }
/[\\`"]/  { t="printf \"%d\" \"'\\"$1"\""; t | getline o; close(t) }
/[^\\`"]/ { t="printf \"%d\" \"'"  $1"\""; t | getline o; close(t) }
/[^0-9a-zA-Z]/ { printf "%%%02X", o }
AWKEOF
    awk -vFS="\n" -f $awkfile
    rm -rf $awkfile
}

# url_decode_v2 <string>
url_decode_v1() {
    local c o s="$1"
    while [ -n "$s" ]; do c=$(echo "$s" | cut -c 1); s=$(echo "$s" | cut -c 2-)
        case "$c" in [^a-zA-Z0-9-.~_]) c="%$(echo -n "$c" | xxd -ps)" ;; esac
    o=${o:+$o}$c; done
    echo $o
}
# url_decode_v2 <string>
url_decode_v2() {
    local c o s="$1"
    echo "$s" | split | url_decode_awk
}
# url_decode <string>
url_decode() { url_decode_v2 "$@"; }

str="$s"
# str="192.168.120.249/gwn7062mffw.bin"
url=$(url_decode "$str")
echo "# $str"
echo "@ $url"

# %20%21%40%23%24%25%5E%26%2A%28%291234567890%60-%3D%5B%5D%3B%27%2C.%2F~_%2B%7B%7D%7C%3A%22%3C%3E%3FabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWSYZ
# %20%21%40%23%24%25%5E%26%2A%28%291234567890%60-%3D%5B%5D%3B%27%2C.%2F~_%2B%7B%7D%7C%3A%22%3C%3E%3FabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWSYZ



*
$PATH
$(touch /root/result)