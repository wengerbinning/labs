#ifndef __DEVELOP_H__
#define __DEVELOP_H__

// #include <stdarg.h>
#include <stdio.h>

#define devel_error(fmt, ...)       printf(fmt"\n", ## __VA_ARGS__)
#define devel_warning(fmt, ...)     printf(fmt"\n", ## __VA_ARGS__)
#define devel_nitice(fmt, ...)      printf(fmt"\n", ## __VA_ARGS__)
#define devel_info(fmt, ...)        printf(fmt"\n", ## __VA_ARGS__)
#define devel_debug(fmt, ...)       printf(fmt"\n", ## __VA_ARGS__)


#endif /* __DEVELOP_H__ */