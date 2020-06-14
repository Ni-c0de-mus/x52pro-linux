/*
 * Saitek X52 Pro Daemon Control Functionality
 *
 * Copyright (C) 2015 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, version 2.
 *
 */

#ifndef X52_DAEMON_CONTROL_H
#define X52_DAEMON_CONTROL_H

#include <stdint.h>

/* Message queue used by x52d & x52ctl */
#define X52_DAEMON_MSG_QUEUE    "/x52daemon.ctrl"

/* Queue depth */
#define X52_DAEMON_QUEUE_DEPTH  8

/* Maximum message size */
/* TODO: Finalize this once we have the message structures */
#define X52_DAEMON_MSG_SIZE     32

#endif /* X52_DAEMON_CONTROL_H */
