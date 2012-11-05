include $(top_srcdir)/Makefile.shared

SUBDIRS = tools po data libkix libkix-gtk src tests

DISTCHECK_CONFIGURE_FLAGS = \
	--disable-update-mimedb   \
	--enable-gtk-doc					\
	--disable-scrollkeeper		\
	--disable-schemas-install \
	$(NULL)

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

EXTRA_DIST = 			\
        CONTRIBUTORS \
        COPYING-DOCS \
        COPYING.LGPL \
        COPYING.SHARE-ALIKE \
        ChangeLog \
        ChangeLog.old \
        README \
        autogen.sh \
        $(NULL)

DISTCLEANFILES = 		\
        ChangeLog

# Workaround broken scrollkeeper that doesn't remove its files on
# uninstall.
distuninstallcheck_listfiles = find . -type f -print | grep -v '^\./var/scrollkeeper'

distclean-local:
	if test "$(srcdir)" = "."; then :; else \
		rm -f ChangeLog; \
	fi

# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(kixmaildocdir)

ChangeLog:
	@echo Creating $@
	@if test -d "$(srcdir)/.git"; then \
	  (GIT_DIR=$(top_srcdir)/.git ./missing --run git log --stat -M -C --name-status  --date=short --no-color) | fmt --split-only > $@.tmp \
	  && mv -f $@.tmp $@ \
	  || ($(RM) $@.tmp; \
	      echo Failed to generate ChangeLog, your ChangeLog may be outdated >&2; \
	      (test -f $@ || echo git-log is required to generate this file >> $@)); \
	else \
	  test -f $@ || \
	  (echo A git checkout and git-log is required to generate ChangeLog >&2 && \
	  echo A git checkout and git-log is required to generate this file >> $@); \
	fi

.PHONY: ChangeLog
