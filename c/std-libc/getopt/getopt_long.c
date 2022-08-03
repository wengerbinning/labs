#include <stdio.h>
#include <getopt.h>

#include <stdlib.h>
#include <unistd.h>

#include "develop.h"

struct param_t {
    char verbose;
};

enum options_idx {
    OPT_HELP,
    OPT_VERISON,
    OPT_TIMEOUT,
    OPT_VERBOSE,

    __OPT_MAX
};

/**
    - required argument: prog -t 1000
    - optional argument: prog -vvvvv
 */
static char short_options[] = "ht:v::V";

static struct option long_options[__OPT_MAX] = {
    [OPT_HELP]    = {"help",          no_argument, 0, 'h'},
    [OPT_VERISON] = {"version",       no_argument, 0, 'V'},
    [OPT_TIMEOUT] = {"timeout", required_argument, 0, 't'},
    [OPT_VERBOSE] = {"verbose", optional_argument, 0, 'v'},
};


int parse_long_options (const struct option *opt, int idx, struct param_t *param) {
    switch (idx) {
    case OPT_HELP:
        note("handling long option help");
        break;
    case OPT_VERISON:
        note("handling long option version");
        break;
    case OPT_TIMEOUT:
        note("handling timeout %d", atoi(optarg));
        break;
    case OPT_VERBOSE:
        note("handling verbose %d", atoi(optarg));
        break;
    default:
        warning("Unknown handling this option: --%s", opt[idx].name);
    }
}

int main (int argc, char *argv[]) {
    char chr;
    int idx;
    struct param_t param;

    while ((chr = getopt_long(argc, argv, short_options, long_options, &idx)) != -1) {
        switch (chr) {
        /* Handing long options. */
        case 0:
            parse_long_options(long_options, idx, &param);
            break;

        /* Handling short options. */
        case 'h':
            note("handling short option help");
            break;
        case 'V':
            note("handling short option version");
            break;
        case 't':
            note("handling timeout %s", optarg);
            break;
        case 'v':
            note("handling verbose %s", optarg);
            break;
        /* Handle invalid option. */
        case '?':
        /* Handle unknown valid option. */
        default:
            warning("Unknown handling this option: -%c", chr);
        }
    }

    return 0;
}

