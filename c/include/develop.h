#ifndef __DEVELOP_H__
#define __DEVELOP_H__

#define devel_error(code, fmt, ...)       printf(fmt"\n", ## __VA_ARGS__)
#define devel_warning(code, fmt, ...)     printf(fmt"\n", ## __VA_ARGS__)
#define devel_notice(code, fmt, ...)      printf(fmt"\n", ## __VA_ARGS__)
#define devel_note(code, fmt, ...)        printf(fmt"\n", ## __VA_ARGS__)
#define devel_debug(code, fmt, ...)       heprintf(fmt"\n", ## __VA_ARGS__)

#define error(fmt, ...)     devel_error(0, fmt, ## __VA_ARGS__)
#define warning(fmt, ...)   devel_warning(0, fmt, ## __VA_ARGS__)
#define notice(fmt, ...)    devel_notice(0, fmt, ## __VA_ARGS__)
#define note(fmt, ...)      devel_note(0, fmt, ## __VA_ARGS__)
#define debug(fmt, ...)     devel_debug(0, fmt, ## __VA_ARGS__)

#endif /* __DEVELOP_H__ */
