include $(top_srcdir)/Makefile.shared

AM_CPPFLAGS =                                     \
	$(ERROR_CFLAGS)																	\
	-I.                                             \
	-I$(top_srcdir)                                 \
	-I$(top_builddir)                               \
	-DDATADIR=\""$(datadir)"\"              				\
	-DPKGDATADIR=\""$(pkgdatadir)"\"								\
	-DLOCALEDIR=\""$(datadir)/locale"\"							\
	-DG_LOG_DOMAIN=\"kixmail\"											\
	-DGCR_API_SUBJECT_TO_CHANGE											\
	$(KIXMAIL_CFLAGS)																\
	$(ENCHANT_CFLAGS)																\
	$(WARN_CFLAGS)																	\
	$(DISABLE_DEPRECATED)                          	\
	$(NULL)

BUILT_SOURCES =		\
	kixmail-gtk-enum-types.h	\
	kixmail-gtk-enum-types.c 	\
	$(NULL)

libkix_gtk_headers = 						 \
	kixmail-ui-utils.h 						 \
	kixmail-initial-setup-widget.h \
	$(NULL)

libkix_gtk_sources = 				     \
	kixmail-ui-utils.c						 \
	kixmail-initial-setup-widget.c \
	$(NULL)


pkglib_LTLIBRARIES = libkix-gtk.la

# libkix-gtk's API is will never be stable, so using -release to make the
# SONAME of the library change with every Kixmail release.
libkix_gtk_la_LDFLAGS = \
	-no-undefined 		\
	-release $(VERSION)
	$(NULL)

libkix_gtk_la_SOURCES = \
	$(libkix_gtk_headers) \
	$(libkix_gtk_sources) \
	$(NULL)

nodist_libkix_gtk_la_SOURCES =\
	$(BUILT_SOURCES)

libkix_gtk_la_LIBADD = 			\
	$(KIXMAIL_LIBS)						\
	$(GTK_LIBS)			 					\
	$(LIBNOTIFY_LIBS)					\
	$(ENCHANT_LIBS)						\
	$(NETWORK_MANAGER_LIBS)		\
	$(GCR_LIBS)								\
	$(CONMAN_LIBS)						\
	$(top_builddir)/libkix/libkix.la \
	$(NULL)

check_c_sources = \
    $(libkix_gtk_sources) 	\
    $(NULL)

include $(top_srcdir)/tools/check-coding-style.mk
check-local: check-coding-style

uidir = $(datadir)/kixmail
ui_DATA =						\
	$(NULL)

kixmail-gtk-enum-types.h: stamp-kixmail-gtk-enum-types.h
	$(AM_V_GEN)true
stamp-kixmail-gtk-enum-types.h: $(libkix_gtk_headers) Makefile
	$(AM_V_GEN) (cd $(srcdir) \
	&& glib-mkenums \
			--fhead "#ifndef __LIBKIX_GTK_ENUM_TYPES_H__\n" \
			--fhead "#define __LIBKIX_GTK_ENUM_TYPES_H__ 1\n\n" \
			--fhead "#include <glib-object.h>\n\n" \
			--fhead "G_BEGIN_DECLS\n\n" \
			--ftail "G_END_DECLS\n\n" \
			--ftail "#endif /* __LIBKIX_GTK_ENUM_TYPES_H__ */\n" \
			--fprod "#include <libkix/@filename@>\n" \
			--eprod "#define KIXMAIL_TYPE_@ENUMSHORT@ @enum_name@_get_type()\n" \
			--eprod "GType @enum_name@_get_type (void);\n" \
		$(libkix_gtk_headers) ) > xgen-gth \
	&& (cmp -s xgen-gth kixmail-gtk-enum-type.h || cp xgen-gth kixmail-gtk-enum-types.h) \
	&& rm -f xgen-gth \
	&& echo timestamp > $(@F)

kixmail-gtk-enum-types.c: $(libkix_gtk_headers) Makefile
	$(AM_V_GEN)(cd $(srcdir) \
	&& glib-mkenums \
			--fhead "#include <config.h>\n" \
			--fhead "#include <glib-object.h>\n" \
			--fhead "#include \"kixmail-gtk-enum-types.h\"\n\n" \
			--fprod "\n/* enumerations from \"@filename@\" */" \
			--vhead "static const G@Type@Value _@enum_name@_values[] = {" \
			--vprod "  { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
			--vtail "  { 0, NULL, NULL }\n};\n\n" \
			--vtail "GType\n@enum_name@_get_type (void)\n{\n" \
			--vtail "  static GType type = 0;\n\n" \
			--vtail "  if (!type)\n" \
			--vtail "    type = g_@type@_register_static (\"@EnumName@\", _@enum_name@_values);\n\n" \
			--vtail "  return type;\n}\n\n" \
		$(libkix_gtk_headers) ) > xgen-gtc \
	&& cp xgen-gtc $(@F) \
	&& rm -f xgen-gtc


CLEANFILES = 									\
	$(BUILT_SOURCES)						\
	stamp-kixmail-gtk-enum-types.h
