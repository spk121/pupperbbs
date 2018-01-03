#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([pupperbbs], [0.0], [pupperbbs@lonelycactus.com])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_CONFIG_SRCDIR([main.cpp])
AC_CONFIG_MACRO_DIR([m4])
AC_SUBST([ACLOCAL_AMFLAGS], "-I m4")
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
PKG_PROG_PKG_CONFIG([1.3.0])

# Checks for libraries.
PKG_CHECK_MODULES([NCURSES_CXX], [ncurses++w ncursesw])
AC_LANG_PUSH([C++])
#AC_CHECK_LIB([mysqlcppconn8],[mysqlx::Session::Session])
AC_LANG_POP([C++])

# Checks for header files.
AC_LANG_PUSH([C++])
AC_CHECK_HEADER([mysql_devapi.h],,
                [AC_MSG_ERROR([Couldn't find or include 'mysql_devapi.h'])])
AC_LANG_POP([C++])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# AC_SUBST([SQL_USER], [pupper])
AC_ARG_VAR([SQL_USER], [The MySQL database user for this application])
# AC_SUBST([SQL_PASSWORD], [password])
AC_ARG_VAR([SQL_PASSWORD], [The MySQL database password for this application])
m4_ifndef([SQL_USER],AC_MSG_WARN([
 No SQL_USER was specified.
 Make sure to add an appropriate SQL username to pupper.xml and pupper.sql]))
m4_ifndef([SQL_PASSWORD],AC_MSG_WARN([
 No SQL_PASSWORD was specified.
 Make sure to add an appropriate SQL password to pupper.xml and pupper.sql]))


AC_CONFIG_FILES([Makefile pupper.sql pupper.xml])
AC_OUTPUT
