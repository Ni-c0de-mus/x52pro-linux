# Top level Automake for x52pro-linux
#
# Copyright (C) 2012-2018 Nirenjan Krishnan (nirenjan@nirenjan.org)
#
# SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0

ACLOCAL_AMFLAGS = -I m4

# Initialize Automake variables
lib_LTLIBRARIES =
check_LTLIBRARIES =
bin_PROGRAMS =
check_PROGRAMS =
dist_man1_MANS =
EXTRA_DIST =
CLEANFILES =
TESTS =

# Header files that need to be copied
x52includedir = $(includedir)/x52pro
x52include_HEADERS =

# Include paths for individual Automake fragments
include lib/libx52/Makefile.am
include lib/libx52util/Makefile.am
include lib/libusbx52/Makefile.am

include utils/cli/Makefile.am
include utils/test/Makefile.am

include tests/Makefile.am

# Extra files that need to be in the distribution
EXTRA_DIST += README.md LICENSE
