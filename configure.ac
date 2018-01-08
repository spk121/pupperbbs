#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([pupperbbs], [0.0], [pupperbbs@lonelycactus.com])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_CONFIG_SRCDIR([app.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_SUBST([ACLOCAL_AMFLAGS], "-I m4")
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
PKG_PROG_PKG_CONFIG([1.3.0])

# Checks for libraries.
# PKG_CHECK_MODULES([NCURSES_CXX], [ncurses++w ncursesw])

# Checks for header files.
#AC_LANG_PUSH([C++])
#AC_CHECK_HEADER([mysql_devapi.h],,
#                [AC_MSG_ERROR([Couldn't find or include 'mysql_devapi.h'])])
#AC_LANG_POP([C++])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_SUBST([SQL_PASSWORD], [Passw0rd1@])

AC_CONFIG_FILES([Makefile pupper.sql pupper.xml])
AC_OUTPUT
