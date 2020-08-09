/*
 * Saitek X52 driver
 *
 * Copyright (C) 2020 Nirenjan Krishnan <nirenjan@nirenjan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only WITH Classpath-exception-2.0
 */

#if 0
This is just a series of comments, on how the file is to be structured

Process command line arguments, stash options to be applied after config file
is loaded

Default config file is ${sysconfdir}/x52drv/config
Possibly parse this using python and dump it to another temporary file, or
just pipe it to x52drv, maybe simpler than writing the parser in C.

Profiles in ${sysconfdir}/x52drv/profiles/

Log file in ${localstatedir}/log/x52drv.log. This is the default log file,
and can be modified by command line option.

Logs are written to the log file if running in the background. If running in
the foreground (eg. w/systemd), then logs are written to stdout.

Log messages can have multiple levels of severity - debug, info, warning, error
and critical - critical errors usually terminate the process
#endif

#include "config.h"

#include "gettext.h"
#include "logging.h"
#include "drv_config.h"

int main(int argc, char **argv)
{
    /* Setup i18n */
    #if ENABLE_NLS
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain(LOCALEDIR);
    #endif

    drv_log_init(NULL);
    drv_log_set_loglevel(LOG_LEVEL_MAX);
    drv_log_set_flags(LOG_FLAG_COLORS);

    drv_config_load(argv[1]);
    return 0;
}
