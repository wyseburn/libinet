dnl
dnl Libos - Common base library. 
dnl
 
dnl make sure we're interpreted by some minimal autoconf
AC_PREREQ(2.52)

dnl Process this file with autoconf to produce configure.
AC_INIT(libos, 1.0.3, hlxxxx@gmail.com)

dnl ******************************************************************
dnl
dnl The release number
dnl
if test -e .svn; then
  RELEASE=`svn info | grep 'Last Changed Rev:' | awk '{print $4}'`
else
  RELEASE='nosvn'
fi

AC_SUBST(RELEASE)
AC_DEFINE_UNQUOTED(RELEASE,["$RELEASE"],[The release number of package])

AC_CONFIG_AUX_DIR(./)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(../include/config.h)
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_YACC
AM_PROG_LEX

dnl
dnl Checks for header files.
dnl
AC_HEADER_STDC
AC_CHECK_HEADERS(fcntl.h syslog.h unistd.h sys/poll.h)

AC_CHECK_FUNCS(poll select syslog)

AC_MSG_CHECKING("build almost static")
AC_ARG_ENABLE(almost-static,
      build_almost_static=$enableval,
      build_almost_static="no")
AC_MSG_RESULT($build_almost_static)

# Write generated configuration file
AC_OUTPUT(Makefile  src/Makefile)

