#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
AC_INIT(libsmpp34, 1.10, ultraismo@yahoo.com)
AC_CONFIG_AUX_DIR(aux_config)
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([aux_config/config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([malloc.h netinet/in.h stdint.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])
AC_OUTPUT([Makefile
           def_frame/Makefile
           def_list/Makefile
           binary/Makefile])
