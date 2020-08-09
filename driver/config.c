/*
 * Saitek X52 driver - configuration management
 *
 * Copyright (C) 2020 Nirenjan Krishnan <nirenjan@nirenjan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0
 */

#include "config.h" // Top level config.h - generated at build time
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "gettext.h"
#include "drv_config.h" // Local file for config management
#include "logging.h"

#define _(x) gettext(x)

#define MAX_LINE_SZ 1024

/* Global config structure */
struct drv_config drv_config;

static bool _drv_cfg_parse_yes_no(const char *val, bool *ptr)
{
    if (strcasecmp(val, "yes") == 0) {
        *ptr = true;
        LOG_DEBUG("Setting %p to true", ptr);
        return true;
    }

    if (strcasecmp(val, "no") == 0) {
        *ptr = false;
        LOG_DEBUG("Setting %p to false", ptr);
        return true;
    }

    /* Neither yes nor no */
    LOG_WARN("Unexpected value '%s', expected either 'yes' or 'no'", val);
    return false;
}

static bool _drv_cfg_parse_int(const char *val, int *ptr)
{
    long int ret;
    const char *nptr = val;
    char *endptr;

    errno = 0;
    ret = strtol(nptr, &endptr, 0);
    if (errno != 0) {
        LOG_WARN("Error encountered when converting integer '%s'", val);
        return false;
    }

    /* val will always have some non-empty string, so it will never be '/0'.
     * According to the man page for strtol, the return value is valid, if
     * *nptr != '\0' && *endptr == '\0'.
     */
    if (*endptr != '\0') {
        /* Result is invalid */
        LOG_WARN("Encountered garbage characters '%s' when converting '%s' to integer",
            endptr, val);
        return false;
    }

    if (ret < INT_MIN || ret > INT_MAX) {
        LOG_WARN("Integer overflow when converting '%s': converted value %ld", val, ret);
        return false;
    }

    LOG_DEBUG("Setting %p to %ld", ptr, ret);
    *ptr = (int)ret;
    return true;
}

static void _drv_cfg_apply_kv(const char *key, const char *val)
{
    LOG_DEBUG("%s: '%s'='%s'", __func__, key, val);

    /* Sizeof a fixed string - this removes the trailing NULL from the size */
    #define SZOF(x) (sizeof(x) - 1)

    #define IF_PREFIX_MATCH() if (strncasecmp(key, KEY_PREFIX, SZOF(KEY_PREFIX)) == 0)
    #define IF_MATCH(skey) if (strcasecmp(&key[SZOF(KEY_PREFIX)], skey) == 0)

    #define PARSE_BOOL_AND_LOG(ptr) do { \
        if (!_drv_cfg_parse_yes_no(val, ptr)) { \
            LOG_INFO("Ignoring malformed line '%s'='%s'", key, val); \
        } \
        return; \
    } while(0)

    #define PARSE_INT_AND_LOG(ptr) do { \
        if (!_drv_cfg_parse_int(val, ptr)) { \
            LOG_INFO("Ignoring malformed line '%s'='%s'", key, val); \
        } \
        return; \
    } while(0)

    /* Handle Clock section - keys begin with 'Clock::' */
    #define KEY_PREFIX "Clock::"
    IF_PREFIX_MATCH() {
        IF_MATCH("Enabled") {
            PARSE_BOOL_AND_LOG(&drv_config.clock_enabled);
        }

        IF_MATCH("PrimaryIsLocal") {
            PARSE_BOOL_AND_LOG(&drv_config.clock_primary_is_local);
        }

        IF_MATCH("Secondary") {
            PARSE_INT_AND_LOG(&drv_config.clock_offset[0]);
        }

        IF_MATCH("Tertiary") {
            PARSE_INT_AND_LOG(&drv_config.clock_offset[1]);
        }

        LOG_INFO("Ignoring unknown key '%s'='%s'", key, val);
        return;
    }
    #undef KEY_PREFIX

    /* Handle brightness section - keys begin with 'Brightness::' */
    #define KEY_PREFIX "Brightness::"
    IF_PREFIX_MATCH() {
        IF_MATCH("LED") {
            PARSE_INT_AND_LOG(&drv_config.brightness[0]);
        }

        IF_MATCH("MFD") {
            PARSE_INT_AND_LOG(&drv_config.brightness[1]);
        }
    }
    #undef KEY_PREFIX

    LOG_INFO("Ignoring unknown section '%s'='%s'", key, val);
}

/*
 * drv_config_apply takes in a line of the form <SECTION>::<Param>=<Value>
 * and applies it to the local configuration structure.
 */
void drv_config_apply(const char *line)
{
    char cfg_key[MAX_LINE_SZ];
    char cfg_val[MAX_LINE_SZ];
    int pk, pv; // Positions of key and val fields
    int i;
    bool write_value;

    memset(cfg_key, 0, sizeof(cfg_key));
    memset(cfg_val, 0, sizeof(cfg_val));

    /* Remove any comments - comments start with # and proceed to EOL */
    pk = 0;
    pv = 0;
    write_value = false;
    for (i = 0; i < MAX_LINE_SZ && line[i]; i++) {
        // No keys/values have spaces in them, so ignore them
        if (isspace(line[i])) {
            continue;
        }

        // # indicates the start of a comment. Quit parsing.
        if (line[i] == '#') {
            break;
        }

        if (line[i] == '=' && !write_value) {
            write_value = true;
            continue;
        }

        if (!write_value) {
            cfg_key[pk] = line[i];
            pk++;
        } else {
            cfg_val[pv] = line[i];
            pv++;
        }
    }

    LOG_DEBUG("Parsing line '%s'", line);

    if (cfg_key[0] == '\0' && cfg_val[0] == '\0' && !write_value) {
        /* Empty line, return early */
        return;
    }

    /* Handle lines where either the key or the value is empty */
    if (cfg_key[0] == '\0' || cfg_val[0] == '\0') {
        LOG_WARN("Ignoring malformed line '%s'", line);
        return;
    }

    _drv_cfg_apply_kv(cfg_key, cfg_val);
}

void drv_config_load_defaults(void)
{
    return;
}

void drv_config_load(const char *cfg_file)
{
    FILE *cfg;
    char *buffer = NULL;
    size_t bufsiz = 0;
    ssize_t n;
    char *line;

    char section[256];
    char key[MAX_LINE_SZ];

    drv_config_load_defaults();

    cfg = fopen(cfg_file, "r");
    if (cfg == NULL) {
        return;
    }

    errno = 0;
    while ((n = getline(&buffer, &bufsiz, cfg)) != -1) {
        if (n > MAX_LINE_SZ) {
            /* MAX_LINE_SZ is pretty large, but if we're exceeding it, then
             * it's likely that somebody is trying a stack smash attack.
             * Discard this line and log an error message.
             */
            LOG_ERR("Got a line that's too long (%ld bytes). Discarding it.", n);
            continue;
        }

        line = buffer;
        /* Trim off any leading whitespace */
        while (*line && isspace(*line)) {
            line++;
        }

        /* Skip if the current character is the comment character, or if
         * we've hit the end of the line
         */
        if (*line == '#' || *line == '\0') {
            continue;
        }

        /* Check if it matches the section syntax, [<section-name>] */
        if (*line == '[') {
            char *tmp = line + 1;
            while (*tmp && isalpha(*tmp)) {
                tmp++;
            }
            if (*tmp == ']') {
                /* We've got a hit - this is a section name */
                size_t len = (tmp - line - 1) * sizeof(char);
                if (len >= sizeof(section)) {
                    len = sizeof(section) - sizeof(char);
                }

                memset(section, 0, sizeof(section));
                memcpy(section, line + 1, len);
            }
        } else {
            snprintf(key, sizeof(key), "%s::%s", section, line);
            drv_config_apply(key);
        }
    }

    if (errno != 0) {
        LOG_FATAL("Encountered error while reading config file: %s", strerror(errno));
    }

    /* Close the config file pointer, and free any allocated buffers */
    fclose(cfg);
    free(buffer);
}
