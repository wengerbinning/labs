
# Q1:
:<<EOF

    kconfig: fix menuconfig on archlinux.

    error message:
    error: return type defaults to ‘int’ [-Wimplicit-int]

diff --git a/scripts/kconfig/lxdialog/check-lxdialog.sh b/scripts/kconfig/lxdialog/check-lxdialog.sh
index 5075ebf2d..910ca1f7c 100755
--- a/scripts/kconfig/lxdialog/check-lxdialog.sh
+++ b/scripts/kconfig/lxdialog/check-lxdialog.sh
@@ -47,7 +47,7 @@ trap "rm -f $tmp" 0 1 2 3 15
 check() {
         $cc -x c - -o $tmp 2>/dev/null <<'EOF'
 #include CURSES_LOC
-main() {}
+int main() { return 0; }
 EOF
        if [ $? != 0 ]; then
            echo " *** Unable to find the ncurses libraries or the"       1>&2

EOF

# Q2: network->tc


# source repo: <git://busybox.net/busybox.git>

make CROSS_COMPILE=${CROSS_PREFIX} menuconfig

make -j6 CROSS_COMPILE=${CROSS_PREFIX}

#
make -j6  CROSS_COMPILE=${CROSS_PREFIX} CONFIG_PREFIX=${PWD}/dest install





