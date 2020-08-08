/*
 * Saitek X52 driver - logging functions
 *
 * Copyright (C) 2020 Nirenjan Krishnan <nirenjan@nirenjan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0
 */

#include "config.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

#include "gettext.h"
#include "logging.h"

static int sys_log_level;

static FILE * sys_log_file;

static int sys_log_flags;

static const char * const log_level_str[] = {
    gettext_noop("FATAL"),
    gettext_noop("CRITICAL"),
    gettext_noop("ERROR"),
    gettext_noop("WARNING"),
    gettext_noop("INFO"),
    gettext_noop("DEBUG"),
};

static const char * const log_level_colors[] = {
    "\033[1;31m",
    "\033[35m",
    "\033[31m",
    "\033[33m",
    "\033[32m",
    "\033[34m",
};

static const bool log_level_show_line[] = {
    true, false, false, false, false, true
};

__attribute__((format(__printf__, 4, 5)))
void drv_log_msg(int loglevel, const char *file, int line, const char * fmt, ...)
{
    if (loglevel <= sys_log_level) {
        va_list ap;
        int level_idx = loglevel + 1;

        if (sys_log_flags & (LOG_FLAG_DATE | LOG_FLAG_TIME)) {
            time_t log_time = time(NULL);
            struct tm tm_s, *ptm;

            if (sys_log_flags & LOG_FLAG_UTC) {
                ptm = gmtime_r(&log_time, &tm_s);
            } else {
                ptm = localtime_r(&log_time, &tm_s);
            }

            if (ptm != NULL) {
                char time_buf[30];

                switch (sys_log_flags & (LOG_FLAG_DATE | LOG_FLAG_TIME)) {
                case LOG_FLAG_DATE:
                    strftime(time_buf, sizeof(time_buf), "%F ", ptm);
                    break;

                case LOG_FLAG_TIME:
                    strftime(time_buf, sizeof(time_buf), "%T%z ", ptm);
                    break;

                case LOG_FLAG_DATE | LOG_FLAG_TIME:
                    strftime(time_buf, sizeof(time_buf), "%F %T%z ", ptm);
                    break;

                default:
                    break;
                }
                fputs(time_buf, sys_log_file);
            }
        }

        if (sys_log_flags & LOG_FLAG_COLORS) {
            fprintf(sys_log_file, "%s%s\033[0m: ",
                log_level_colors[level_idx], gettext(log_level_str[level_idx]));
        } else {
            fprintf(sys_log_file, "%s: ", gettext(log_level_str[level_idx]));
        }

        if (log_level_show_line[level_idx]) {
            fprintf(sys_log_file, "%s:%d: ", file, line);
        }

        va_start(ap, fmt);
        vfprintf(sys_log_file, fmt, ap);
        va_end(ap);
        fputs("\n", sys_log_file);

        if (loglevel == LOG_LEVEL_FATAL) {
            exit(1);
        }
    }
}

void drv_log_init(const char *logfile)
{
    if (logfile == NULL) {
        sys_log_file = stdout;
    } else {
        sys_log_file = fopen(logfile, "a");

        /* Handle the case where we cannot open the file */
        if (sys_log_file == NULL) {
            sys_log_file = stdout;
            LOG_FATAL("Unable to open logfile %s: %s", logfile, strerror(errno));
        }
    }

    /* Set default log level */
    drv_log_set_loglevel(LOG_LEVEL_ERROR);

    /* Set default log flags */
    drv_log_set_flags(LOG_FLAG_DATE | LOG_FLAG_TIME);
}

void drv_log_set_loglevel(int loglevel)
{
    if (loglevel < LOG_LEVEL_CRITICAL) {
        /* LOG_LEVEL_CRITICAL is the lowest value supported, but the user can
         * technically pass in a lower (negative value). Ignore these lower
         * values and clamp it to LOG_LEVEL_CRITICAL
         */
        loglevel = LOG_LEVEL_CRITICAL;
    } else if (loglevel >= LOG_LEVEL_MAX) {
        /* Clamp it to the maximum supported value, which is 1 below MAX */
        loglevel = LOG_LEVEL_MAX - 1;
    }

    sys_log_level = loglevel;
}

void drv_log_set_flags(int flags)
{
    sys_log_flags = flags;
}
