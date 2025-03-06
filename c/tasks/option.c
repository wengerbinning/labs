#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <libgen.h>

#define OPTIONAPI
#include "option.h"

#ifndef CMDLINE_SIZE
#define CMDLINE_SIZE 2048
#endif

enum context_option {
    OPT_HELP,
    OPT_VERSION,
    OPT_FORCE,
    OPT_VERBOSE,
    OPT_MAX
};

static char soptions[] = "fhvV";
static struct option loptions[] = {
    [OPT_HELP]            = { "help",      no_argument, 0, 'h' },
    [OPT_VERSION]         = { "version",   no_argument, 0, 'V' },
    [OPT_FORCE]           = { "force",     no_argument, 0, 'f' },
    [OPT_VERBOSE]         = { "verbose",   no_argument, 0, 'v' },
    [OPT_MAX]             = { 0, 0, 0, 0 },
};

static char *desciption = "\
This is general parase option APIs                                          \n\
                                                                            \n\
 Command Format:                                                            \n\
    %s <OPTIONS> ...                                                        \n\
                                                                            \n\
General Option:                                                             \n\
    -h, --help      Display provision help information.                     \n\
    -V, --version   Display provision version information.                  \n\
    -f, --force     Force doing                                             \n\
    -v, --verbose   Show detail log.                                        \n\
";

static char *version = "\
Compiled on " __DATE__ " " __TIME__ "                                         \
";

static int parse_long_options (context_t *ctx, unsigned int idx) {
    switch (idx) {
        default:
            return 1;
    }
}


int parse_options (context_t *ctx, int argc, char *argv[]) {
    int ret, opt, idx, len;
    char *prog = basename(argv[0]);
    char *p, cmdline[CMDLINE_SIZE + 1] = { 0 };

    /* check */
    // if (argc <= 1) {
    //     printf(desciption, prog);
    //     return 1;
    // }

    /* parse options */
    while ((0 <= (opt = getopt_long(argc, argv, soptions, loptions, &idx)))) {
        switch (opt) {
        case 0:
            if ((ret = parse_long_options(ctx, idx)) != 0) {
                return ret;
            }
            break;
        case 'f':
            break;
        case 'h':
            printf(desciption, prog);
            return 1;
        case 'v':
            break;
        case 'V':
            printf("%s\n", version);
            return 1;
            break;
        case ':':   /* Not parameter */
        case '?':
            /* Note - We cand settting opterr */
            printf("error option -%c\n", optopt);char cmdline[CMDLINE_SIZE + 1] = { 0 };
        default:
            printf(desciption,  prog);
            return -1;
        }


    }
    /* paramaters */
    while (optind < argc) {
        printf("param: %s\n", argv[optind++]);
    }

    /* cmdline */
    p = cmdline; len = CMDLINE_SIZE;
    for (idx = 0; idx < argc; idx++) {
        if (idx)
            snprintf(p, len, " %s", argv[idx]);
        else
            snprintf(p, len, "%s", argv[idx]);
        p = cmdline + strlen(cmdline);
        len = CMDLINE_SIZE - strlen(cmdline);
    }

    return 0;
}
