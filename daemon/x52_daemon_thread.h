/*
 * Saitek X52 Pro Daemon Thread Library
 *
 * Copyright (C) 2017 Nirenjan Krishnan (nirenjan@nirenjan.org)
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, version 2.
 *
 */

/* This file defines standard thread API for internal thread creation */

#ifndef X52_DAEMON_THREAD_H
#define X52_DAEMON_THREAD_H

#include <stdint.h>
#include <pthread.h>

#define THREAD_START    0x1
#define THREAD_STOP     0x0

/**
 * @brief Thread identification enumeration
 *
 * This is used as an index into an array of threads
 */
typedef enum {
    /** Thread ID for the clock thread */
    X52_THREAD_CLOCK,

    /** Maximum number of threads */
    X52_THREAD_MAX
} x52_thread_id;

/**
 * @brief Function pointer type for thread functions
 */
typedef void *(*x52_thread_fn)(void *);
/**
 * @brief Structure to contain thread details
 */
struct x52_thread {
    /** Thread description string */
    const char *        thread_desc;

    /** Thread callback function passed to pthread_create */
    x52_thread_fn       thread_function; 

    /** Pthread structure */
    pthread_t           thread;

    /** Pthread attributes */
    pthread_attr_t *    attr;

    /** Thread context */
    void *              context;

    /** Thread state, running/not running */
    int                 running;

    /** Flag to check if thread has been registered or not */
    int                 registered;
};

/**
 * @brief Control the state of a thread
 *
 * For the given thread ID, enable or disable the thread
 * @param[in]   thread_id   Thread ID, \ref x52_thread_id
 * @param[in]   enable      0 to disable, any other value to enable.
 *
 * @returns 0 on success, EEXIST if the requested thread already exists and
 * enable is true, ENOENT if the requested thread doesn't exist and enable is
 * false, EDOM if \ref thread_id is outside the expected range, and any other
 * errno indicates pthread API failure.
 */
int x52_thread_control(x52_thread_id thread_id, int enable);

/**
 * @brief Register a thread handler for a given thread function
 *
 * @param[in]   thread_id   Thread ID, \ref x52_thread_id
 * @param[in]   desc        Thread description, must be a constant string literal
 * @param[in]   thread_fn   Thread function
 * @param[in]   context     Thread context, may be NULL
 * @param[in]   attr        pthread attributes for thread, may be NULL
 *
 * @returns 0 on success, EDOM if thread_id out of range, EINVAL if parameters
 * are invalid, EEXIST if thread_id is already registered
 */
int x52_thread_register(x52_thread_id thread_id, const char *desc,
                        x52_thread_fn thread_fn, void * context,
                        pthread_attr_t *attr);

/**
 * @brief Update the thread context for a previously registered thread
 *
 * @param[in]   thread_id   Thread ID, \ref x52_thread_id
 * @param[in]   context     Thread context, may be NULL
 *
 * @returns 0 on success, EDOM if thread_id is out of range, ENOENT if thread
 * is not yet registered.
 */
int x52_thread_update_context(x52_thread_id thread_id, void *context);

#endif /* X52_DAEMON_THREAD_H */
