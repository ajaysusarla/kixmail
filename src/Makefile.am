uidir = $(datadir)/kixmail/ui
ui_DATA = kixmail.ui


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(KIXMAIL_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = kixmail

kixmail_SOURCES = \
	main.c \
	kixmail.h \
	kixmail.c	

kixmail_LDFLAGS = \
	-Wl,--export-dynamic

kixmail_LDADD = $(KIXMAIL_LIBS)


EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(datadir)/kixmail

