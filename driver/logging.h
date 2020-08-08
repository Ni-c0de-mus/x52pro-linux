/*
 * Saitek X52 driver - logging functions
 *
 * Copyright (C) 2020 Nirenjan Krishnan <nirenjan@nirenjan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0
 */

#ifndef DRV_LOGGING_H
#define DRV_LOGGING_H

/* Initialize logger - send output to log_file. If log_file is NULL,
 * then write to stdout. Otherwise, open the given file path for appending
 * and write to that.
 */
void drv_log_init(const char *log_file);

/* Set log level - this will control the level of messages logged */
void drv_log_set_loglevel(int loglevel);

/* Set log flags - this controls the output */
void drv_log_set_flags(int flags);

/* Log a message at the requested log level. This may be suppressed if the
 * requested log level is at a lower priority than the configured log level.
 */
void drv_log_msg(int loglevel, const char *file, int line, const char * fmt, ...);

enum {
    LOG_LEVEL_FATAL = -1,
    LOG_LEVEL_CRITICAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_MAX
};

#define LOG_FATAL(fmt, ...) drv_log_msg(LOG_LEVEL_FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_CRIT(fmt, ...) drv_log_msg(LOG_LEVEL_CRITICAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERR(fmt, ...) drv_log_msg(LOG_LEVEL_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) drv_log_msg(LOG_LEVEL_WARNING, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) drv_log_msg(LOG_LEVEL_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) drv_log_msg(LOG_LEVEL_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_DEFAULT_FILE LOCALSTATEDIR "/log/x52drv.log"

enum {
    LOG_FLAG_DATE = (1 << 0),
    LOG_FLAG_TIME = (1 << 1),
    LOG_FLAG_COLORS = (1 << 2),
    LOG_FLAG_UTC = (1 << 3),
};

#endif // !defined DRV_LOGGING_H
