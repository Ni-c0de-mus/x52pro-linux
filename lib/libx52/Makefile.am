# Automake for libx52
#
# Copyright (C) 2012-2018 Nirenjan Krishnan (nirenjan@nirenjan.org)
#
# SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0

lib_LTLIBRARIES += libx52.la

# Core libx52 library
# This library handles the USB communication between the host and the X52
libx52_la_SOURCES = \
	%reldir%/x52_control.c \
	%reldir%/x52_core.c \
	%reldir%/x52_date_time.c \
	%reldir%/x52_mfd_led.c \
	%reldir%/x52_strerror.c

libx52_la_CFLAGS = @LIBUSB_CFLAGS@
libx52_la_LDFLAGS = -version-info 3:0:1 @LIBUSB_LIBS@

x52include_HEADERS += %reldir%/libx52.h

# Extra files that need to be in the distribution
EXTRA_DIST += %reldir%/libx52.h \
			  %reldir%/x52_commands.h \
			  %reldir%/x52_common.h \
			  %reldir%/README.md
