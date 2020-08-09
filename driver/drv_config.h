/*
 * Saitek X52 driver - configuration management
 *
 * Copyright (C) 2020 Nirenjan Krishnan <nirenjan@nirenjan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0
 */

#ifndef DRV_CONFIG_H
#define DRV_CONFIG_H

#include <stdbool.h>
#include <limits.h>

#include "libx52.h"

struct drv_config {
    bool clock_enabled;
    bool clock_primary_is_local;
    int clock_offset[2];

    struct {
        libx52_led_id id;
        libx52_led_state state;
    } led[11];

    int brightness[2];

    char profiles_dir[NAME_MAX];
    bool clutch_enabled;
    bool clutch_latched;
};

extern struct drv_config drv_config;

void drv_config_apply(const char *line);

void drv_config_load(const char *cfg_file);

void drv_config_load_defaults(void);

#endif // !defined DRV_CONFIG_H
