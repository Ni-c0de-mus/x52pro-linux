/*
 * Saitek X52 Pro Daemon Common Functionality
 *
 * Copyright (C) 2017 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, version 2.
 *
 */

#ifndef X52_DAEMON_COMMON_H
#define X52_DAEMON_COMMON_H

// #include <mqueue.h>
#include "libx52.h"

struct x52_daemon_control {
    libx52_device *dev;
    int clock_timezone[3]; // Timezones of all 3 clocks
    int clock_format[3];
    int date_format;
};

#define THREAD_ENABLE   1
#define THREAD_DISABLE  0
int x52_clock_thread_control(int state);

#endif /* X52_DAEMON_COMMON_H */
