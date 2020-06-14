# Automake for x52joyd
#
# Copyright (C) 2012-2020 Nirenjan Krishnan (nirenjan@nirenjan.org)
#
# SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0

# Use the pthread compiler variables
LIBS += $(PTHREAD_LIBS)
AM_CFLAGS = $(PTHREAD_CFLAGS)
CC = $(PTHREAD_CC)

bin_PROGRAMS = x52joyd

# Joystick daemon program that monitors and controls the X52 Pro Joystick
x52joyd_SOURCES = x52_daemon_core.c x52_daemon_clock.c x52_daemon_thread.c
x52joyd_CFLAGS = @X52_INCLUDE@
x52joyd_LDADD = ../lib/libx52/libx52.la

EXTRA_DIST = \
	x52_daemon_commands.h \
	x52_daemon_common.h \
	x52_daemon_control.h \
	x52_daemon_thread.h
