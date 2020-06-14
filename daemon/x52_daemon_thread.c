/*
 * Saitek X52 Pro Daemon Thread functions
 *
 * Copyright (C) 2017 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, version 2.
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "x52_daemon_thread.h"

/**
 * Array of threads
 */
static struct x52_thread x52_daemon_threads[X52_THREAD_MAX];

int x52_thread_control(x52_thread_id thread_id, int enable)
{
    struct x52_thread *x52thr;
    int status = 0;

    if (thread_id >= X52_THREAD_MAX) {
        /* Thread ID out of range */
        return EDOM;
    }

    x52thr = &x52_daemon_threads[thread_id];

    /*
     * Make sure the thread has been registered, or we risk calling
     * a NULL pointer for the thread function
     */
    if (!x52thr->registered) {
        return EBADF;
    }

    if (enable) {
        /* Start the thread, if it isn't already running */
        if (x52thr->running) {
            return EEXIST;
        }

        /* Start the thread */
        status = pthread_create(
                    &x52thr->thread,
                    x52thr->attr,
                    x52thr->thread_function,
                    x52thr->context);
        if (status == 0) {
            x52thr->running = enable;
        }
    } else {
        /* Stop the thread if it hasn't already been stopped */
        if (!x52thr->running) {
            return ENOENT;
        }

        /* Cancel the thread */
        status = pthread_cancel(x52thr->thread);
        if (status == 0) {
            x52thr->running = enable;
        }
    }

    return status;
}

int x52_thread_register(x52_thread_id thread_id, const char *desc,
                        x52_thread_fn thread_fn, void * context,
                        pthread_attr_t *attr)
{
    struct x52_thread *x52thr;

    if (thread_id >= X52_THREAD_MAX) {
        return EDOM;
    }

    if (desc == NULL || thread_fn == NULL) {
        return EINVAL;
    }

    x52thr = &x52_daemon_threads[thread_id];
    if (x52thr->registered) {
        return EEXIST;
    }

    x52thr->thread_desc = desc;
    x52thr->thread_function = thread_fn;
    x52thr->context = context;
    x52thr->attr = attr;

    x52thr->registered = true;

    return 0;
}

int x52_thread_update_context(x52_thread_id thread_id, void *context)
{
    struct x52_thread *x52thr;

    if (thread_id >= X52_THREAD_MAX) {
        return EDOM;
    }

    x52thr = &x52_daemon_threads[thread_id];
    if (!x52thr->registered) {
        return ENOENT;
    }

    x52thr->context = context;

    return 0;
}

