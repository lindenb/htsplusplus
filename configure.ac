AC_INIT([hts++], [0.1])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CC
AC_PROG_CXX([g++ gcc])
AC_LANG([C++])


AC_CONFIG_SRCDIR([src/HtsPlusPlus.cpp])
m4_include([m4/ax_with_htslib.m4])
AC_CONFIG_HEADERS([config.h])


AC_ARG_WITH([htslib],
  [AS_HELP_STRING([--with-htslib=DIR],
    [path to a local htslib source/build directory (default: ../htslib)])],
  [HTSLIB_DIR="$withval"],
  [HTSLIB_DIR="../htslib"])

# make absolute
HTSLIB_DIR=$(cd "$HTSLIB_DIR" 2>/dev/null && pwd)
AS_IF([test -z "$HTSLIB_DIR" || test ! -f "$HTSLIB_DIR/htslib/hts.h"],
  [AC_MSG_ERROR([htslib not found (need DIR/htslib/hts.h); use --with-htslib=DIR])])
AS_IF([test ! -f "$HTSLIB_DIR/libhts.a" && test ! -f "$HTSLIB_DIR/libhts.so"],
  [AC_MSG_ERROR([libhts not built in $HTSLIB_DIR; run 'make' there first])])

HTSLIB_CPPFLAGS="-I$HTSLIB_DIR"
HTSLIB_LDFLAGS="-L$HTSLIB_DIR -Wl,-rpath,$HTSLIB_DIR"
HTSLIB_LIBS="-lhts -lz -lm -lbz2 -llzma -lcurl -lcrypto -lpthread"

AC_SUBST([HTSLIB_CPPFLAGS])
AC_SUBST([HTSLIB_LDFLAGS])
AC_SUBST([HTSLIB_LIBS])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
