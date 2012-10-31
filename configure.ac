m4_define(kixmail_released, 0)

m4_define([kixmail_major_version], [0])
m4_define([kixmail_minor_version], [0])
m4_define([kixmail_micro_version], [1])
m4_define([kixmail_nano_version], [0])

dnl Display the nano_version only if it's not '0'
m4_define([kixmail_base_version],
          [kixmail_major_version.kixmail_minor_version.kixmail_micro_version])
m4_define([kixmail_full_version],
          [m4_if(kixmail_nano_version, 0, [kixmail_base_version],
              [kixmail_base_version].[kixmail_nano_version])])

m4_define(kixmail_maybe_datestamp,
          m4_esyscmd([if test x]kixmail_released[ != x1; then date +.%Y%m%d | tr -d '\n\r'; fi]))

m4_define(kixmail_version, kixmail_full_version[]kixmail_maybe_datestamp)

dnl initialisation
AC_INIT([kixMail],
        [kixmail_version],
        [http://spartha.org],
        [kixmail],
        [http://spartha.org])

AC_PREREQ([2.64])

dnl Mandatory dependencies
FOLKS_REQUIRED=0.7.3
GNUTLS_REQUIRED=2.8.5

GLIB_REQUIRED=2.33.3
AC_DEFINE(GLIB_VERSION_MIN_REQUIRED, GLIB_VERSION_2_30, [Ignore post 2.30 deprecations])
AC_DEFINE(GLIB_VERSION_MAX_ALLOWED, GLIB_VERSION_2_34, [Prevent post 2.34 APIs])

GTK_REQUIRED=3.5.1
AC_DEFINE(GDK_VERSION_MIN_REQUIRED, GDK_VERSION_3_4, [Ignore post 3.4 deprecations])
AC_DEFINE(GDK_VERSION_MAX_ALLOWED, GDK_VERSION_3_4, [Prevent post 3.4 APIs])

CLUTTER_REQUIRED=1.10.0
AC_DEFINE(CLUTTER_VERSION_MIN_REQUIRED, CLUTTER_VERSION_1_8, [Ignore post 1.8 deprecations])
AC_DEFINE(CLUTTER_VERSION_MAX_ALLOWED, CLUTTER_VERSION_1_10, [Prevent post 1.10 APIs])

CLUTTER_GTK_REQUIRED=1.1.2
LIBSECRET_REQUIRED=0.5
GCR_REQUIRED=2.91.4
LIBNOTIFY_REQUIRED=0.7.0
WEBKIT_REQUIRED=1.3.13
GOA_REQUIRED=3.5.1


dnl Optional dependencies
ENCHANT_REQUIRED=1.2.0
ISO_CODES_REQUIRED=0.35
NETWORK_MANAGER_REQUIRED=0.7.0
LIBACCOUNTS_REQUIRED=1.1

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_AUX_DIR(.)
AM_MAINTAINER_MODE([enable])

AM_INIT_AUTOMAKE([1.11 no-define -Wno-portability tar-ustar no-dist-gzip dist-xz])


dnl Support silent build rules. Disable
dnl by either passing --disable-silent-rules to configure or passing V=1
dnl to make
AM_SILENT_RULES([yes])

dnl Check for programs
AC_PROG_CC
AC_HEADER_STDC
AC_PROG_MKDIR_P
AM_PATH_GLIB_2_0
AC_PATH_XTRA

dnl Initialize libtool
LT_PREREQ([2.2])
LT_INIT

AC_PATH_PROG(DBUS_BINDING_TOOL, dbus-binding-tool)
GLIB_GSETTINGS
GLIB_GENMARSHAL=`$PKG_CONFIG glib-2.0 --variable=glib_genmarshal`
AC_SUBST(GLIB_GENMARSHAL)

AC_CHECK_PROGS([XSLTPROC], [xsltproc])
if test -z "$XSLTPROC"; then
  AC_MSG_ERROR([xsltproc (from libxslt) is required])
fi
AM_PATH_PYTHON([2.3])

KIXMAIL_ARG_VALGRIND

dnl Error flags
AS_COMPILER_FLAG(-Wall, ERROR_CFLAGS="-Wall", ERROR_CFLAGS="")
AS_COMPILER_FLAG(-Werror, werror=yes, werror=no)

AC_ARG_ENABLE(Werror,
  AS_HELP_STRING([--disable-Werror],
                 [compile without -Werror (normally enabled in development builds)]),
    werror=$enableval, :)

AS_COMPILER_FLAG(-Wextra, wextra=yes, wextra=no)
AS_COMPILER_FLAG(-Wno-missing-field-initializers,
        wno_missing_field_initializers=yes,
        wno_missing_field_initializers=no)
AS_COMPILER_FLAG(-Wno-unused-parameter,
        wno_unused_parameter=yes,
        wno_unused_parameter=no)

ifelse(kixmail_released, 1, [],
    [
        if test x$werror = xyes; then
            ERROR_CFLAGS="$ERROR_CFLAGS -Werror"
        fi
        if test x$wextra = xyes -a \
            x$wno_missing_field_initializers = xyes -a \
            x$wno_unused_parameter = xyes; then
            ERROR_CFLAGS="$ERROR_CFLAGS -Wextra -Wno-missing-field-initializers -Wno-unused-parameter"
        fi
    ])

AS_COMPILER_FLAG(-Wdeclaration-after-statement, ERROR_CFLAGS="$ERROR_CFLAGS -Wdeclaration-after-statement")
AS_COMPILER_FLAG(-Wshadow, ERROR_CFLAGS="$ERROR_CFLAGS -Wshadow")
AS_COMPILER_FLAG(-Wmissing-prototypes, ERROR_CFLAGS="$ERROR_CFLAGS -Wmissing-prototypes")
AS_COMPILER_FLAG(-Wmissing-declarations, ERROR_CFLAGS="$ERROR_CFLAGS -Wmissing-declarations")
AS_COMPILER_FLAG(-Wstrict-prototypes, ERROR_CFLAGS="$ERROR_CFLAGS -Wstrict-prototypes")

AC_SUBST(ERROR_CFLAGS)

AC_ARG_ENABLE(gprof,
            AS_HELP_STRING([--enable-gprof=@<:@no/yes@:>@],
            [build with support for gprof]),,
            [enable_gprof=no])
if test "x$enable_gprof" != "xno" ; then
    CFLAGS="$CFLAGS -pg"
    LDFLAGS="$LDFLAGS -pg"
fi

AC_SUBST(LDFLAGS)

dnl pkg-config dependency checks
AC_CHECK_LIBM
AC_SUBST(LIBM)

PKG_CHECK_MODULES(KIXMAIL,
[
   dbus-glib-1
   folks >= $FOLKS_REQUIRED
   glib-2.0 >= $GLIB_REQUIRED
   gio-2.0 >= $GLIB_REQUIRED
   gio-unix-2.0 >= $GLIB_REQUIRED
   libsecret-1 >= $LIBSECRET_REQUIRED
   gnutls >= $GNUTLS_REQUIRED
   gmodule-export-2.0
   gobject-2.0
   gsettings-desktop-schemas
   libxml-2.0
   x11
   gtk+-3.0 >= $GTK_REQUIRED
   libnotify >= $LIBNOTIFY_REQUIRED
   gcr-3 >= $GCR_REQUIRED
   webkitgtk-3.0 >= $WEBKIT_REQUIRED
   libsoup-2.4
   gee-1.0
])


dnl Enable debug
AC_ARG_ENABLE(debug,
  AS_HELP_STRING([--disable-debug],
                 [compile without debug code]),
    enable_debug=$enableval, enable_debug=yes )

if test x$enable_debug = xyes; then
  AC_DEFINE(ENABLE_DEBUG, [], [Enable debug code])
fi

dnl Language Support
IT_PROG_INTLTOOL([0.50.0])

GETTEXT_PACKAGE=kixmail
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE",[Gettext package name])

dnl spellchecking checks: enchant and iso-codes
AC_ARG_ENABLE(spell,
              AS_HELP_STRING([--enable-spell=@<:@no/yes/auto@:>@],
                             [Enable spell checking]), ,
                             enable_spell=auto)

if test "x$enable_spell" != "xno"; then
   PKG_CHECK_MODULES(ENCHANT,
   [
      enchant >= $ENCHANT_REQUIRED,
      iso-codes >= $ISO_CODES_REQUIRED
   ], have_enchant="yes", have_enchant="no")

   if test "x$have_enchant" = "xyes"; then
      AC_MSG_CHECKING([whether iso-codes has iso-639 and iso-3166 domains])
      if $PKG_CONFIG --variable=domains iso-codes | grep 639 > /dev/null && \
         $PKG_CONFIG --variable=domains iso-codes | grep 3166 > /dev/null ; then
            AC_DEFINE_UNQUOTED(ISO_CODES_PREFIX, ["`$PKG_CONFIG --variable=prefix iso-codes`"], [ISO codes prefix])
            AC_DEFINE(HAVE_ENCHANT, [], [Define if you have libenchant])
            result=yes
      else
            result=no
            have_enchant="no"
      fi
      AC_MSG_RESULT([$result])
   fi
else
   have_enchant=no
fi

if test "x$enable_spell" = "xyes" -a "x$have_enchant" != "xyes"; then
   AC_MSG_ERROR([Could not find spell-checking dependencies:

$ENCHANT_PKG_ERRORS])
fi

AM_CONDITIONAL(HAVE_ENCHANT, test "x$have_enchant" = "xyes")

dnl goa
AC_ARG_ENABLE(goa,
              AS_HELP_STRING([--enable-goa=@<:@no/yes/auto@:>@],
                             [Enable GOA support]), ,
                             enable_goa=auto)

if test "x$enable_goa" != "xno"; then
   PKG_CHECK_MODULES(GOA,
    [
       goa-1.0 >= $GOA_REQUIRED
    ], have_goa="yes", have_goa="no")
else
   have_goa=no
fi

if test "x$enable_goa" = "xyes" -a "x$have_goa" != "xyes"; then
   AC_MSG_ERROR([Could not find GOA dependencies:

$GOA_PKG_ERRORS])
fi

if test "x$have_goa" = "xyes"; then
   AC_DEFINE(HAVE_GOA, 1, [Define to 1 to enable GNOME Online Accounts])
fi

AM_CONDITIONAL(HAVE_GOA, test "x$have_goa" = "xyes")

dnl Help documentation
YELP_HELP_INIT


dnl Coding style checks
AC_ARG_ENABLE(coding-style-checks,
  AS_HELP_STRING([--disable-coding-style-checks],
                 [do not check coding style using grep]),
    [ENABLE_CODING_STYLE_CHECKS=$enableval],
    [ENABLE_CODING_STYLE_CHECKS=yes])

AC_SUBST([ENABLE_CODING_STYLE_CHECKS])


AC_CONFIG_FILES([
        Makefile
        src/Makefile
        po/Makefile.in
        libkix/Makefile
        libkix-gtk/Makefile
        tools/Makefile
        tests/Makefile
])
AC_OUTPUT


dnl Status summary
echo ""
echo "*************************************"
echo "** kixMail is ready to be compiled **"
echo "*************************************"
echo ""
echo "Compiler                 : ${CC}"
echo "Compiler Flags           : ${CFLAGS} ${ERROR_FLAGS}"
echo "Linker Flags             : ${LDFLAGS}"
echo "Prefix                   : ${prefix}"
echo "Coding style checks      : ${ENABLE_CODING_STYLE_CHECKS}"
echo "Enable debug             : $enable_debug"
echo ""
echo ""
echo "Features:"
echo "Spell checking (enchant) : ${have_enchant}"
echo "GOA support              : ${have_goa}"
echo ""
echo ""
echo "Configure successful. Type 'make' to compile"
echo ""
