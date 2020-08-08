# Automake for x52drv
#
# Copyright (C) 2012-2020 Nirenjan Krishnan (nirenjan@nirenjan.org)
#
# SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0

ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = x52drv

# X52 driver program
# This program reads and parses the raw events from the joystick, and
# converts them into input events (Linux only)
x52drv_SOURCES = main.c logging.c
x52drv_CFLAGS = \
	-I $(top_srcdir)/lib/libx52io \
	-I $(top_srcdir) \
	-DLOCALEDIR=\"$(localedir)\" \
	-DSYSCONFDIR=\"$(sysconfdir)\" \
	-DRUNSTATEDIR=\"$(runstatedir)\" \
	-DLOCALSTATEDIR=\"$(localstatedir)\" \
	$(WARN_CFLAGS)
x52drv_LDFLAGS = $(WARN_LDFLAGS)
x52drv_LDADD = ../lib/libx52io/libx52io.la

EXTRA_DIST = \
	logging.h
