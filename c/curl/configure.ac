#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.72])
AC_INIT([wenger-lab-curl], [0.1], [https://github.com/wengerbinning/labs.git])
AM_INIT_AUTOMAKE()
AM_DEFAULT_VERBOSITY=0

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([])

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET

# Checks for libraries.
PKG_CHECK_MODULES(CURL, libcurl)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset strdup])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT()
