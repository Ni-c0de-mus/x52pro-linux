/*
 * Saitek X52 Pro Daemon
 *
 * Copyright (C) 2017 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, version 2.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
/* #include <mqueue.h> */
#include <errno.h>

#include "libx52.h"
#include "x52_daemon_common.h"
#include "x52_daemon_control.h"
#include "x52_daemon_thread.h"

/** Common structure shared among threads */
static struct x52_daemon_control x52d;

/* Initialize threads */
static int initialize_threads(void)
{
    /* Set the clock thread to point to the shared structure */
    x52_thread_update_context(X52_THREAD_CLOCK, &x52d);

    return 0;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    /* Ensure that we are running as root */
    if (getuid() != 0) {
        /* Insufficient permissions */
        return EPERM;
    }

    /* Initialize message queue */
    /* ret = setup_message_queue(); */
    if (ret != 0) {
        /* Unable to create message queue */
        return ret;
    }

    /* Initialize threads */
    initialize_threads();
    /* TODO: Daemonize */

    return 0;
}
