AM_CPPFLAGS =                                           \
        $(ERROR_CFLAGS)                                 \
        -I$(top_srcdir)                                 \
        -DPKGDATADIR=\""$(pkgdatadir)"\"                \
        -DGCR_API_SUBJECT_TO_CHANGE                     \
        $(KIXMAIL_CFLAGS)                               \
        $(WARN_CFLAGS)                                  \
        $(DISABLE_DEPRECATED)                           \
        -DSRCDIR=\""$(abs_srcdir)"\"                    \
        $(NULL)

LDADD =                                                 \
        $(top_builddir)/libkix-gtk/libkix-gtk.la        \
        $(top_builddir)/libkix/libkix.la                \
        $(KIXMAIL_LIBS)                                 \
        $(NULL)

tests_list = 	\
		kixmail-network-connect-test \
		$(NULL)

noinst_PROGRAMS = $(tests_list)
TESTS = $(tests_list)


kixmail_network_connect_test_SOURCES = \
		kixmail-network-connect-test.h     \
		kixmail-network-connect-test.c		 \
		$(NULL)

test-report: test-report.xml
		gtester-report $(top_builddir)/tests/$@.xml > \
			$(top_builddir)/tests/$@.html

test-report.xml: ${TESTS} test

test: ${TESTS}
		gtester -o test-report.xml -k --verbose ${TESTS}

test-%: kixmail-%-test
		gtester -o $@-report.xml -k --verbose $<

.PHONY: test test-report
