#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([libutil], [0.2.6], [meteor1113@gmail.com])
AC_CONFIG_SRCDIR([include/str.hpp])
AM_INIT_AUTOMAKE
#AC_CONFIG_HEADER([config.h])

# sets build, host, target variables and the same with _alias
#AC_CANONICAL_BUILD
AC_CANONICAL_HOST
#AC_CANONICAL_TARGET

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
#AC_CHECK_HEADERS([inttypes.h memory.h stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
#AC_HEADER_STDBOOL
#AC_C_CONST
#AC_C_INLINE
#AC_TYPE_SIZE_T
#AC_STRUCT_TM
#AC_C_VOLATILE

# Checks for library functions.
#AC_FUNC_MALLOC
#AC_FUNC_STRFTIME
#AC_CHECK_FUNCS([floor memset pow sqrt])

# debug options support
AC_ARG_ENABLE([debug],
	      AC_HELP_STRING([--enable-debug], [enable debug (default: no)]),
	      [enable_debug=$enableval],
	      [enable_debug=no])
if test x$enable_debug = xyes ; then
   AC_DEFINE(_DEBUG, 1, [_DEBUG])
#   CFLAGS="-g"	     # "${CFLAGS} -g"
#   CXXFLAGS="-g"     # "${CXXFLAGS} -g"
else
   AC_DEFINE(NDEBUG)
fi

AC_CONFIG_FILES([Makefile
		include/Makefile
		test/Makefile])
AC_OUTPUT
#AC_OUTPUT([Makefile src/Makefile])

AC_MSG_RESULT([
Build options:
  Version                  $VERSION
  Install prefix           $prefix
  Debug	  		   $enable_debug
])
