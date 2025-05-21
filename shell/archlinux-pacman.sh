#!/bin/sh

# pkgs=$(pacman -Q | cut -d' ' -f1)
pkgs=(aarch64-linux-gnu-binutils bind dpkg eaglecloud-yunshu filesystem gitlab-shell glibc glusterfs google-chrome krb5 mingw-w64-binutils mingw-w64-crt mingw-w64-gcc mingw-w64-headers mingw-w64-winpthreads nfs-utils xorg-server-common)




table=(
    lib service pkgconfig
    bin include
    share doc man info icons fonts locale license
    etc profile
    others
)



awk_parse_open() {
    local file=${1:-.parse.awk}
    _AWK_OPEN_FILE=$file
cat > $file <<EOF
BEGIN { type="none"; delete table }
# init variable
{ name=\$1; path=gensub(/^\/(.*)/, "\\\\1", "g", \$2) }
# skip folder
/\/$/ { next }
EOF
}
awk_parse_add() {
cat >> ${3:-$_AWK_OPEN_FILE} <<EOF
# $1
\$2 ~ /^${2//\//\\\/}/ { if (type != "$1") {
    type="$1"; table[type] = true; printf "# %s\n", type
} printf "%s\n", path; next }
EOF
}

awk_parse_close() {
cat >> ${1:-$_AWK_OPEN_FILE} << EOF
# others
{ if (type != "others") {
    type="others"; table[type] = true; printf "# %s\n", type
} printf "%s\n", path }
# Finish
END { print "@";
for (type in table) { if (table[type])
    table[type] = false;
    print name >> type;
    printf "%s ", type
} }
EOF
}

awk_parse_open .parse.awk
#
awk_parse_add service   /usr/lib/systemd/system
awk_parse_add pkgconfig /usr/lib/pkgconfig
awk_parse_add lib       /usr/lib
# #
awk_parse_add bin       /usr/bin
#
awk_parse_add include   /usr/include
#
awk_parse_add doc       /usr/share/doc
awk_parse_add man       /usr/share/man
awk_parse_add info      /usr/share/info
awk_parse_add locale    /usr/share/locale
awk_parse_add license   /usr/share/license
awk_parse_add fonts     /usr/share/fonts
awk_parse_add icons     /usr/share/icons
awk_parse_add share     /usr/share
#
awk_parse_add profile   /etc/profile.d
awk_parse_add etc       /etc
#
awk_parse_close



idx=0
for file in ${table[@]}; do test -f ${file} && rm -rf ${file}; done
for pkg in $pkgs; do idx=$((idx + 1))
    pacman -Ql $pkg | awk -f .parse.awk > ${pkg}.txt
    printf "%4d %s\n" $idx $pkg
    # test $idx -ge 3 && break
done
