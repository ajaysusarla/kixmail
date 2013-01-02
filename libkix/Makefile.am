include $(top_srcdir)/Makefile.shared

AM_CPPFLAGS =                                     \
	$(ERROR_CFLAGS)																	\
	-I.                                             \
	-I$(top_srcdir)                                 \
	-I$(top_builddir)                               \
	-DDATADIR=\""$(datadir)"\"              				\
	-DLOCALEDIR=\""$(datadir)/locale"\"							\
	-DG_LOG_DOMAIN=\"kixmail\"											\
	-DGCR_API_SUBJECT_TO_CHANGE											\
	$(KIXMAIL_CFLAGS)																\
	$(NETWORK_MANAGER_CFLAGS)												\
	$(CONNMAN_CFLAGS)																\
	$(UOA_CFLAGS)																		\
	$(WARN_CFLAGS)																	\
	$(DISABLE_DEPRECATED)                          	\
	$(NULL)

BUILT_SOURCES =		\
	kixmail-enum-types.h	\
	kixmail-enum-types.c 	\
	$(NULL)

libkix_headers = 				\
	kixmail-gsettings.h   \
	kixmail-debug.h				\
	kixmail-utils.h				\
	kixmail-io.h					\
	kixmail-socket.h			\
	kixmail-filter-input-stream.h \
	$(NULL)

libkix_sources = 				\
	kixmail-debug.c				\
	kixmail-utils.c				\
	kixmail-io.c 					\
	kixmail-socket.c			\
	kixmail-filter-input-stream.c \
	$(NULL)


pkglib_LTLIBRARIES = libkix.la

# libkix's API is will never be stable, so using -release to make the
# SONAME of the library change with every Kixmail release.
libkix_la_LDFLAGS = \
	-no-undefined 		\
	-release $(VERSION)
	$(NULL)

libkix_la_SOURCES = \
	$(libkix_headers) \
	$(libkix_sources) \
	$(NULL)

libkix_la_LIBADD = 					\
	$(GCR_LIBS)			 					\
	$(KIXMAIL_LIBS)						\
	$(NETWORK_MANAGER_LIBS)		\
	$(CONMAN_LIBS)						\
	$(LIBM)										\
	$(NULL)


dtddir = $(datadir)/kixmail
dtd_DATA = 	\
	$(NULL)


CLEANFILES = 									\
	$(BUILT_SOURCES)						\
	stamp-kixmail-enum-types.h

nodist_libkix_la_SOURCES =\
	$(BUILT_SOURCES)

check_c_sources = \
    $(libkix_sources) \
    $(NULL)

include $(top_srcdir)/tools/check-coding-style.mk
check-local: check-coding-style

kixmail-enum-types.h: stamp-kixmail-enum-types.h
	$(AM_V_GEN)true
stamp-kixmail-enum-types.h: $(libkix_headers) Makefile
	$(AM_V_GEN) (cd $(srcdir) \
	&& glib-mkenums \
			--fhead "#ifndef __LIBKIX_ENUM_TYPES_H__\n" \
			--fhead "#define __LIBKIX_ENUM_TYPES_H__ 1\n\n" \
			--fhead "#include <glib-object.h>\n\n" \
			--fhead "G_BEGIN_DECLS\n\n" \
			--ftail "G_END_DECLS\n\n" \
			--ftail "#endif /* __LIBKIX_ENUM_TYPES_H__ */\n" \
			--fprod "#include <libkix/@filename@>\n" \
			--eprod "#define KIXMAIL_TYPE_@ENUMSHORT@ @enum_name@_get_type()\n" \
			--eprod "GType @enum_name@_get_type (void);\n" \
		$(libkix_headers) ) > xgen-gth \
	&& (cmp -s xgen-gth kixmail-enum-type.h || cp xgen-gth kixmail-enum-types.h) \
	&& rm -f xgen-gth \
	&& echo timestamp > $(@F)

kixmail-enum-types.c: $(libkix_headers) Makefile
	$(AM_V_GEN)(cd $(srcdir) \
	&& glib-mkenums \
			--fhead "#include <config.h>\n" \
			--fhead "#include <glib-object.h>\n" \
			--fhead "#include \"kixmail-enum-types.h\"\n\n" \
			--fprod "\n/* enumerations from \"@filename@\" */" \
			--vhead "static const G@Type@Value _@enum_name@_values[] = {" \
			--vprod "  { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
			--vtail "  { 0, NULL, NULL }\n};\n\n" \
			--vtail "GType\n@enum_name@_get_type (void)\n{\n" \
			--vtail "  static GType type = 0;\n\n" \
			--vtail "  if (!type)\n" \
			--vtail "    type = g_@type@_register_static (\"@EnumName@\", _@enum_name@_values);\n\n" \
			--vtail "  return type;\n}\n\n" \
		$(libkix_headers) ) > xgen-gtc \
	&& cp xgen-gtc $(@F) \
	&& rm -f xgen-gtc
