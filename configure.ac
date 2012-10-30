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
AC_INIT([kixMail], [kixmail_version], [http://spartha.org])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CC
AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_SRCDIR([configure.ac])
GNOME_DOC_INIT([0.10.13])


dnl libkix libtool version
LT_RELEASE=
LT_CURRENT=2
LT_REVISION=0
LT_AGE=1
AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)


dnl Internationalization
GETTEXT_PACKAGE=kixmail
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.35.0])

dnl check for headers
AC_CHECK_HEADERS(malloc.h fcntl.h sys/select.h sys/param.h \
                 sys/utsname.h netdb.h regex.h sys/mman.h \
                 unistd.h)
dnl Support silent build rules, requires at least automake-1.11. Disable
dnl by either passing --disable-silent-rules to configure or passing V=1
dnl to make
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AM_MAINTAINER_MODE([enable])

dnl libtool intialization
LT_PREREQ(2.2)
LT_INIT([dlopen disable-static])

AC_C_BIGENDIAN
AC_C_CONST
AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AM_DISABLE_STATIC
AC_HEADER_STDC

AC_PATH_PROG([GLIB_GENMARSHAL], [glib-genmarshal])
AC_PATH_PROG([GLIB_MKENUMS],[glib-mkenums])

AC_CHECK_LIB(m, floor)

GLIB_GSETTINGS

PKG_PROG_PKG_CONFIG

MORE_WARN_FLAGS="-Wdeclaration-after-statement"
DEPRECATION_FLAGS=

if test "$enable_maintainer_mode" = "yes"; then
	AC_DEFINE([MAINTAINER_MODE],[1],[Define to enable 'maintainer-only' behaviour])
	enable_debug=yes
	DEPRECATION_FLAGS="-DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGDK_PIXBUF_DISABLE_DEPRECATED -DPANGO_DISABLE_DEPRECATED -DGNOME_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED -DGSEAL_ENABLE"
fi


dnl Extra verbose warning switches
CFLAGS="$CFLAGS -DG_DISABLE_SINGLE_INCLUDES -DGTK_DISABLE_SINGLE_INCLUDES -DGSEAL_ENABLE"
CFLAGS="$CFLAGS -DG_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGDK_PIXBUF_DISABLE_DEPRECATED"

dnl (Platform independent) dependencies for kixmail
platform=GNOME

dnl **** REMOVE ****
dnl PKG_CHECK_MODULES(KIXMAIL, libxml-2.0
dnl [gtk+-3.0 >= 2.90.3 sqlite3 gnutls check enchant libssl gmime-2.6 atk shared-mime-info dbus-glib-1 gtk+-3.0 webkitgtk-3.0 libnotify])
dnl **** REMOVE ****


AC_CHECK_LIB(gthread-2.0, g_thread_init)

GLIB_MIN_VERSION=2.16
PKG_CHECK_MODULES(GLIB, glib-2.0 >= $GLIB_MIN_VERSION)
KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $GLIB_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $GLIB_LIBS"

PKG_CHECK_MODULES(GIO, gio-2.0 >= $GLIB_MIN_VERSION)
KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $GIO_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $GIO_LIBS"

PKG_CHECK_MODULES(GIO_UNIX, gio-unix-2.0 >= $GLIB_MIN_VERSION)
KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $GIO_UNIX_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $GIO_UNIX_LIBS"

LIBXML2_MIN_VERSION=2.6.8
PKG_CHECK_MODULES(LIBXML2, libxml-2.0 >= $LIBXML2_MIN_VERSION)
KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $LIBXML2_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $LIBXML2_LIBS"

DBUS_GLIB_REQUIRED=0.35
PKG_CHECK_MODULES(DBUS_GLIB, dbus-glib-1 >= $DBUS_GLIB_REQUIRED)
KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $DBUS_GLIB_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $DBUS_GLIB_LIBS"

WEBKIT_GTK_REQUIRED=1.6.1
AC_ARG_WITH(webkit2,
        [AC_HELP_STRING([--with-webkit2], [build with WebKit2 [default=no]])],
        [],[with_webkit2=no])

if test "x$with_webkit2" != xno; then
   WEBKIT_GTK_PC_NAME=webkit2gtk-3.0
   AC_DEFINE([HAVE_WEBKIT2],[1],[Define if building with WebKit2])
else
   WEBKIT_GTK_PC_NAME=webkitgtk-3.0
fi
AM_CONDITIONAL(WITH_WEBKIT2, test "x$with_webkit2" != xno)

PKG_CHECK_MODULES(WEBKIT_GTK, $WEBKIT_GTK_PC_NAME >= $WEBKIT_GTK_REQUIRED)
KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $WEBKIT_GTK_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $WEBKIT_GTK_LIBS"

LIBNOTIFY_REQUIRED=0.7.0
PKG_CHECK_MODULES(LIBNOTIFY, libnotify >= $LIBNOTIFY_REQUIRED)
KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $LIBNOTIFY_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $LIBNOTIFY_LIBS"

OPENSSL_REQUIRED=0.9.8
PKG_CHECK_MODULES(OPENSSL, openssl >= $OPENSSL_REQUIRED)
KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $OPENSSL_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $OPENSSL_LIBS"

PKG_CHECK_MODULES(SQLITE, sqlite3)
KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $SQLITE_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $SQLITE_LIBS"

PKG_CHECK_MODULES(X11, x11)
KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $X11_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $X11_LIBS"

PKG_CHECK_MODULES(GOB_INTROSPECTION, gobject-introspection-1.0)
KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $GOB_INTROSPECTION_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $GOB_INTROSPECTION_LIBS"

dnl Decide whether to use libsoup or libsoup-gnome (if available)
LIBSOUP_MIN_VERSION=2.36.1

if test "x$platform" = "xGNOME"; then
   dnl For the GNOME platform allow using libsoup-gnome, if available
   AC_ARG_WITH([libsoup],
           [AS_HELP_STRING([--with-libsoup=plain|gnome], [libsoup variant to use (default: gnome)])],
           [case "$with_libsoup" in
           plain|gnome) ;;
           *) AC_MSG_ERROR([Wrong libsoup variant]) ;;
           esac],
           [with_libsoup=gnome])
else
   dnl Other non-GNOME platforms (e.g. MacOSX) to support plain libsoup only
   with_libsoup=plain
fi

if test "x$with_libsoup" = "xgnome"; then
   AC_MSG_CHECKING([libsoup variant to use])
   PKG_CHECK_MODULES(LIBSOUP_GNOME,
                     [libsoup-gnome-2.4 >= $LIBSOUP_MIN_VERSION],
                     [have_libsoup_gnome="yes"], [have_libsoup_gnome="no"])

   if test "x$have_libsoup_gnome" = "xyes"; then
      LIBSOUP_CFLAGS="$LIBSOUP_GNOME_CFLAGS"
      LIBSOUP_LIBS="$LIBSOUP_GNOME_LIBS"

      AC_DEFINE(HAVE_LIBSOUP_GNOME, 1, [Have libsoup-gnome])
   else
      with_libsoup=plain
   fi
fi

if test "x$with_libsoup" = "xplain"; then
   PKG_CHECK_MODULES(LIBSOUP, [libsoup-2.4 >= $LIBSOUP_MIN_VERSION])
fi

KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $LIBSOUP_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $LIBSOUP_LIBS"

dnl Select target version of GTK to use

AC_MSG_CHECKING([GTK+ version to use])

if test "x$platform" = "xGNOME"; then
   dnl For the GNOME platform allow using GTK2 and GTK3 (default for GNOME)
   AC_ARG_WITH([gtk],
           [AS_HELP_STRING([--with-gtk=2.0|3.0], [GTK+ version to use (default: 3.0)])],
           [case "$with_gtk" in
           2.0|3.0) ;;
           *) AC_MSG_ERROR([Wrong GTK+ version]) ;;
           esac],
           [with_gtk=3.0])
else
   dnl Other non-GNOME platforms (e.g. MacOSX) to support GTK2 only
   with_gtk=2.0
fi

case "$with_gtk" in
     2.0) GTK_MIN_VERSION=2.14
          GTK_API_VERSION=2.0
          ;;
     3.0) GTK_MIN_VERSION=3.0
          GTK_API_VERSION=3.0
          ;;
esac

dnl Make GTK API version available to the code
AC_SUBST([GTK_API_VERSION])

if test "x$with_gtk" = "x3.0"; then
   PKG_CHECK_MODULES(GTK3, gtk+-$GTK_API_VERSION >= $GTK_MIN_VERSION,
                     [have_gtk_3="yes"], [have_gtk_3="no"])

   if test "x$have_gtk_3" = "xyes"; then
      GTK_CFLAGS=$GTK3_CFLAGS
      GTK_LIBS=$GTK3_LIBS
   else
      GTK_MIN_VERSION=2.14
      GTK_API_VERSION=2.0
      with_gtk=2.0
   fi
fi

if test "x$with_gtk" = "x2.0"; then
   PKG_CHECK_MODULES(GTK2, gtk+-$GTK_API_VERSION >= $GTK_MIN_VERSION,
                     [have_gtk_2="yes"], [have_gtk_2="no"])
   if test "x$have_gtk_2" = "xyes"; then
      GTK_CFLAGS=$GTK2_CFLAGS
      GTK_LIBS=$GTK2_LIBS
   fi
fi

AM_CONDITIONAL([GTK_API_VERSION_2],[test "$GTK_API_VERSION" = "2.0"])
AM_CONDITIONAL([GTK_API_VERSION_3],[test "$GTK_API_VERSION" = "3.0"])

KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $GTK_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $GTK_LIBS"

dnl Translations
KIXMAIL_LOCALE_DIR='${localedir}'
AM_GLIB_DEFINE_LOCALEDIR([KIXMAIL_LOCALE_DIR])
AM_GLIB_GNU_GETTEXT

dnl Debug mode
AC_MSG_CHECKING([whether to enable debug output])
AC_ARG_ENABLE(debug,
              AC_HELP_STRING([--enable-debug],
                             [turn on debugging [default=no]]),
              [],[enable_debug="no"])
AC_MSG_RESULT([$enable_debug])

if test "x$enable_debug" = "xyes"; then
   AC_DEFINE([DEBUG_ENABLED], [1], [Debug enabled])
   CFLAGS="$CFLAGS -DG_ENABLE_DEBUG -DG_ERRORCHECK_MUTEXES -Wall -Werror -g3 -O0"
else
   CFLAGS="$CFLAGS -DG_DISABLE_ASSERT -DG_DISABLE_CHECKS -DG_DISABLE_CAST_CHECKS"
fi


dnl portability checks
AC_CHECK_FUNCS([localtime_r memchr memmove memset mkdir mkdtemp realpath sqrt strchr strcspn strdup strerror strrchr strtol strtoul strstr])

dnl backtrace()
AC_CHECK_HEADERS([execinfo.h fcntl.h])
AC_C_INLINE
AC_FUNC_MKTIME
AC_FUNC_STRTOD
AC_TYPE_SIZE_T

KIXMAIL_FEATURES=

dnl GObject Introspection
dnl GOBJECT_INTROSPECTION_CHECK([0.9.5])

if test "$with_webkit2" = "no"; then
   if test "$found_introspection" = "yes"; then
      KIXMAIL_FEATURES="$KIXMAIL_FEATURES introspection"
      AC_DEFINE([ENABLE_INTROSPECTION], [1], [Define to enable introspection support])
   fi
else
   found_introspection=no
fi

dnl NSS
AC_MSG_CHECKING([whether NSS support is requested])
AC_ARG_ENABLE([nss],
        [AS_HELP_STRING([--enable-nss], [Enable NSS support (default: enabled)])],
        [], [enable_nss=yes])
AC_MSG_RESULT([$enable_nss])

if test "$enable_nss" = "yes"; then
   PKG_CHECK_MODULES([NSS], [nss])

   AC_DEFINE([ENABLE_NSS], [1], [Define to compile with NSS support])
fi

AM_CONDITIONAL([ENABLE_NSS],[test "$enable_nss" = "yes"])

KIXMAIL_CFLAGS="$KIXMAIL_CFLAGS $NSS_CFLAGS"
KIXMAIL_LIBS="$KIXMAIL_LIBS $NSS_LIBS"

AC_SUBST([KIXMAIL_FEATURES])

dnl Substitute
AC_SUBST(DATA_DIR, "\${datadir}")
AC_SUBST(KIXMAIL_CFLAGS)
AC_SUBST(KIXMAIL_LIBS)

AC_DEFINE([KIXMAIL_COMPILATION],[1],[Compiling kixmail])

dnl Produce output files
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
echo "Compiler               : $CC"
echo "CFLAGS                 : $KIXMAIL_CFLAGS"
echo "LDFLAGS                : $KIXMAIL_LIBS"
echo "Prefix                 : $prefix"
echo "Using GTK+ version     : $with_gtk"
echo "Using libsoup variant  : $with_libsoup"
echo "Enable debug           : $enable_debug"
echo "WebKit2                : $with_webkit2"
echo "WebKit version         : $WEBKIT_GTK_PC_NAME"
echo ""
echo ""
echo "Configure successful. Type 'make' to compile"
echo ""
