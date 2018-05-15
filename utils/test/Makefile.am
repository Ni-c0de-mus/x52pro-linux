# Automake for x52test
#
# Copyright (C) 2012-2018 Nirenjan Krishnan (nirenjan@nirenjan.org)
#
# SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0

bin_PROGRAMS += x52test

# Test utility that exercises all the library functions
x52test_SOURCES = \
	%reldir%/x52_test.c \
	%reldir%/x52_test_mfd.c \
	%reldir%/x52_test_led.c \
	%reldir%/x52_test_clock.c

x52test_CFLAGS = @X52_INCLUDE@
x52test_LDADD = libx52.la

# Extra files that need to be in the distribution
EXTRA_DIST += %reldir%/x52_test_common.h
