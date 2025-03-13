




## toolchain

### 源码

* binutils:v2.42
* linux:v5.4.260
* gcc:12.3.0(gmp:v6.3.0, mpc:v1.3.1, mpfr:v4.2.1)
* glibc: v2.38

### 软件

* binutils-gdb
* linux-include
* gcc-core: binutils-gdb, linux-include
* glibc-include: gcc-core
* gcc-libs: glibc-include
* glibc: gcc-libs
* gcc: glibc, libcrypt

## base

* busybox
* ncures