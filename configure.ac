#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([sip2smpp], [0.2.0], [mickael.rousse@bjtmai.com])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Wno-portability foreign subdir-objects])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([pthread], [-lpthread])
AC_CHECK_LIB([dl], [-ldl])
AC_CHECK_LIB([iconv], [-liconv])
AC_CHECK_LIB([rt], [-lrt])
AC_CHECK_LIB([smpp34], [-lsmpp34])
AC_CHECK_LIB([zdb], [-lzdb])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h limits.h netdb.h netinet/in.h stdint.h stdlib.h string.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT8_T
AC_TYPE_PID_T
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([inet_ntoa memset select socket strchr strdup strerror strstr])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
