/*
 * Saitek X52 Pro Daemon Clock Thread
 *
 * Copyright (C) 2017 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, version 2.
 *
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "libx52.h"
#include "x52_daemon_common.h"
#include "x52_daemon_thread.h"

static void * clock_thread(void *context)
{
    int rc;

    /* Get the pointer to the shared structure from the context */
    struct x52_daemon_control *x52d = context;

    /* This thread simply updates the clock display every second.
     * libx52_set_clock returns 0 only if there's a need to update the
     * clock.
     */
    for(;;) {
        rc = libx52_set_clock(x52d->dev, time(NULL),
                x52d->clock_timezone[LIBX52_CLOCK_1]);
        if (!rc) {
            libx52_update(x52d->dev);
        }
        sleep(1);
    }
}

__attribute__((constructor))
static void register_clock_thread(void)
{
    x52_thread_register(X52_THREAD_CLOCK, "Clock thread",
                        clock_thread, NULL, NULL);
}
