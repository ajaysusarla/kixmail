include $(top_srcdir)/Makefile.shared


CPPFLAGS_COMMON =									\
	$(KIXMAIL_CFLAGS)								\
	$(ERROR_CFLAGS)									\
	-I$(top_srcdir)                	\
	-DG_LOG_DOMAIN=\"kixmail\"			\
	-DBIN_DIR=\"$(bindir)\"         \
	$(DISABLE_DEPRECATED)						\
	$(WARN_CFLAGS)									\
	$(NULL)


AM_LDFLAGS = $(LIBM)
AM_CPPFLAGS =								\
	$(CPPFLAGS_COMMON)				\
	$(NULL)


LDADD =																				\
	$(top_builddir)/libkix-gtk/libkix-gtk.la		\
	$(top_builddir)/libkix/libkix.la						\
	$(GCR_LIBS) 																\
	$(KIXMAIL_LIBS)															\
	$(NULL)


noinst_LTLIBRARIES =


bin_PROGRAMS = 	\
	kixmail				\
	$(NULL)

libexec_PROGRAMS = \
	$(NULL)

kixmail_sources = \
	kixmail-folder-sidebar.h kixmail-folder-sidebar.c \
	kixmail-application.h kixmail-application.c \
	kixmail-window.h kixmail-window.c \
	main.c 					\
	$(NULL)

kixmail_SOURCES = 		\
	$(kixmail_sources) 	\
	$(NULL)

kixmail_LDADD =		\
	$(top_builddir)/libkix-gtk/libkix-gtk.la		\
	$(top_builddir)/libkix/libkix.la 						\
	$(KIXMAIL_LIBS)															\
	$(NULL)

check_c_sources = 		\
	$(kixmail_sources) 	\
	$(NULL)

include $(top_srcdir)/tools/check-coding-style.mk
check-local: check-coding-style

uidir = $(datadir)/kixmail/ui
ui_DATA = \
	kixmail-window.ui \
	kixmail.ui \
	$(NULL)

EXTRA_DIST = $(ui_DATA)

dist_man_MANS = 	\
	kixmail.1				\
	$(NULL)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(datadir)/kixmail

